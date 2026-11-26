# Fase 0 — auditoria de hardware

Data da auditoria: 25 de novembro de 2026. A base foi verificada contra o repositório e a documentação pública da LilyGO e contra a documentação do ESP32-S3. Isto valida a configuração de software de referência, não substitui a inspeção da revisão física da unidade.

## Conclusão de compatibilidade

A configuração alvo é compatível com o ESP32-S3-WROOM-1, flash de 16 MB, PSRAM OPI de 8 MB, display ST7789 170 x 320 em rotação horizontal e os periféricos descritos para a T-Embed CC1101. A escolha de desenvolvimento é PlatformIO + Arduino-ESP32, inicialmente sem LVGL. Ela reproduz o ambiente recomendado pela LilyGO, fornece FreeRTOS e TinyUSB por baixo do Arduino e permite introduzir ESP-IDF quando uma API de baixo nível for indispensável.

Referências principais:

- [Repositório oficial T-Embed CC1101](https://github.com/Xinyuan-LilyGO/T-Embed-CC1101) — versão de hardware publicada V1.0-241103, dependências e orientação PlatformIO.
- [Definição T-Embed-Plus da LilyGoLib](https://github.com/Xinyuan-LilyGO/LilyGoLib/blob/master/docs/hardware/lilygo-t-embed-plus.md) — mapa de pinos, barramentos compartilhados e energia.
- [Documentação ESP-IDF para ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/) — I2S/PDM, FreeRTOS e USB Device/TinyUSB.

## Descoberta que exige confirmação física

O pedido descreve um nRF24L01 incluído na versão Plus. As fontes oficiais mais recentes divergem: o repositório da T-Embed lista uma expansão nRF24 e a LilyGoLib define o nRF24 como **módulo externo opcional**, não populado na placa principal, usando CS GPIO44 e CE GPIO43. Esses mesmos pinos são compartilhados com a UART externa.

Consequência: `nRF24` permanece desabilitado por padrão. Antes de ativá-lo, confirme visualmente o módulo e a revisão da PCB, fotografe a face onde o módulo deveria estar e execute uma sonda SPI passiva. Não utilize GPIO43/44 simultaneamente para UART externa e nRF24.

## Mapa de GPIO adotado

| Grupo | Sinal | GPIO | Regra aplicada |
| --- | --- | ---: | --- |
| Alimentação | domínio comum | 15 | Alto liga CC1101, WS2812 e MAX98357A juntos; nunca é usado como chave individual |
| I2C | SDA / SCL | 8 / 18 | PN532, BQ27220 e BQ25896 em um único barramento |
| SPI | MOSI / MISO / SCK | 9 / 10 / 11 | Display, MicroSD, CC1101 e nRF24 opcional compartilham o barramento |
| Display | CS / DC / BL | 41 / 16 / 21 | Sem reset dedicado; BL é AW9364 pulsado, não PWM |
| MicroSD | CS | 13 | Mantido inativo antes de `SPI.begin()` |
| Entrada | encoder A / B / botão | 4 / 5 / 0 | GPIO0 é strap de boot; não deve estar pressionado durante reset/gravação |
| Entrada | botão usuário | 6 | Retorno contextual e ação de cancelamento |
| LEDs | WS2812 | 14 | Oito LEDs, sob serviço centralizado |
| Áudio | PDM data / clock | 42 / 39 | Microfone SPM1423HM4H-B |
| Áudio | I2S BCLK / LRCLK / data | 46 / 40 / 7 | MAX98357A no domínio de alimentação compartilhado |
| NFC | reset / IRQ | 45 / 17 | PN532 I2C, endereço 0x24 |
| CC1101 | CS / GDO0 / GDO2 | 12 / 3 / 38 | Rádio Sub-GHz, não LoRa |
| CC1101 | SW1 / SW0 | 47 / 48 | Caminhos: 315 MHz = 1/0; 434 = 1/1; 868/915 = 0/1 |
| nRF24 opcional | CS / CE | 44 / 43 | Compartilha SPI e conflita com UART externa |
| IR | TX / RX | 2 / 1 | Só controles autorizados |
| USB nativo | D- / D+ | 19 / 20 | USB CDC/HID via TinyUSB em fase posterior |

## Barramentos e energia

1. Todos os CS SPI são configurados em nível alto antes do barramento ser iniciado. Isto evita seleção simultânea de display, SD, CC1101 e nRF24 opcional.
2. `SpiBus` fornece o mutex obrigatório para as tarefas futuras de SD e rádio. Nesta imagem ainda não há concorrência de SPI: a UI e a montagem ocorrem durante a inicialização.
3. O I2C é sondado sem escrever em registros. A tela só indica a presença física dos endereços 0x24, 0x55 e 0x6B.
4. O firmware não reconfigura o carregador nesta versão. Quando o driver BQ25896 entrar, a corrente rápida será limitada a no máximo 600 mA, conforme a recomendação da LilyGO para a bateria de 1300 mAh.
5. O backlight AW9364 recebe pulsos discretos. A implementação não usa PWM.

## Matriz de viabilidade

| Recurso | Classificação | Decisão desta etapa |
| --- | --- | --- |
| UI, display, encoder e botões | Viável diretamente | Fundamentos implementados sem touchscreen |
| MicroSD, diretórios e histórico | Viável diretamente | Montagem e árvore de dados implementadas; histórico virá com Chat |
| Wi-Fi e HTTPS | Viável diretamente | Rádio permanece desligado até haver configuração protegida por NVS |
| Chat / transcrição / TTS | Viável com serviço externo | Sem LLM, STT ou TTS local; contrato de API preparado para Fase 3 |
| PDM e I2S | Viável mediante otimização | Requer tarefas, buffers PSRAM e teste na placa; ainda não inicializado |
| USB HID / CDC | Viável diretamente | ESP32-S3 possui USB nativo; entra na Fase 6 |
| CC1101 | Viável diretamente, uso regulado | Driver e diagnóstico posteriores; não há transmissão nesta base |
| PN532 | Viável diretamente | Diagnóstico I2C agora; leitura pública autorizada depois |
| nRF24L01 | Dependente de componente adicional | Só ativar após confirmação física do módulo externo |
| IR | Viável diretamente | Módulo posterior, somente sinais próprios/autorizados |
| Python local | Viável mediante otimização | MicroPython embutido precisa estudo de tamanho, isolamento e integração |
| JavaScript local | Viável mediante otimização | Duktape/QuickJS exige POC de RAM, tamanho e licença |
| C++ local | Não será implementado | Tratado apenas como arquivo de código-fonte |

## Checklist para a unidade física

- [ ] Fotografe a etiqueta/revisão da PCB e compare com o esquema V1.0-241103.
- [ ] Confirme o nRF24L01 físico; se ausente, mantenha a opção desabilitada.
- [ ] Insira MicroSD compatível e confirme leitura/escrita com esta imagem.
- [ ] Confirme que o encoder central está solto antes de reset ou upload.
- [ ] Confirme o painel em 320 x 170, o sentido do encoder e o botão usuário.
- [ ] Compare os três endereços I2C exibidos com a tela Hardware.
- [ ] Somente após a confirmação acima, avance para leituras de bateria e drivers de rádio.
