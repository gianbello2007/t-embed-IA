# Arquitetura

## Princípios

- A IA está fora do dispositivo. O ESP32 coleta entrada, mantém o histórico local, faz a requisição e apresenta a resposta.
- Aplicações nunca acessam GPIO ou SPI diretamente. Elas recebem serviços do núcleo.
- Estado real e indisponibilidade explícita são preferidos a telas simuladas.
- Operações de rede, SD, áudio e rádio devem rodar fora da tarefa de UI, sincronizadas por filas e mutexes.

## Estrutura atual

```text
src/
  core/       ciclo do sistema, filas de eventos e, nas próximas fases, AppManager
  hardware/   placa, SPI, display, entrada, LEDs, MicroSD e sondagem I2C
  services/   Wi-Fi agora; IA, áudio, storage e USB nas próximas fases
  ui/         tema inicial e tela de início sem dependência de touch
  main.cpp    ponto de entrada Arduino mínimo
include/      contratos públicos dos módulos
```

## Fluxo da Fase 1

```text
boot
  -> Board: CS SPI inativos, SPI/I2C iniciados, domínio GPIO15 ligado
  -> Input + fila de eventos
  -> RGB + Display ST7789 + backlight AW9364
  -> MicroSD + diretórios /T-EMBED-AI
  -> I2C passivo (PN532 / BQ27220 / BQ25896)
  -> Wi-Fi desligado até provisionamento seguro
  -> HomeScreen

encoder/botões -> Queue<InputEvent> -> System::tick -> HomeScreen
```

## Concorrência planejada

| Tarefa | Núcleo sugerido | Comunicação | Requisito |
| --- | ---: | --- | --- |
| UI/LVGL | 1 | fila de UI | nunca bloquear em rede, SD ou áudio |
| eventos de entrada | 1 | `Queue<InputEvent>` | debounce e repetição contextual |
| rede/IA | 0 | filas de requisição/resposta | HTTPS, timeouts e cancelamento |
| armazenamento | 0 | fila de jobs + `SpiBus` | leitura por streaming, escrita atômica |
| captura/reprodução de áudio | 0 | buffer PSRAM + fila | sem competir com renderização |
| USB | 0 | fila de frames/comandos | confirmação do usuário para ações |

O detalhe de stacks, prioridades e afinidade será fechado depois da medição de uso de RAM na placa. A criação prematura de tarefas não é aceitável.

## Dados persistentes

- MicroSD: conversas JSONL, documentos, projetos, scripts, cache, logs, temporários e backups dentro de `/T-EMBED-AI`.
- NVS: rede e configurações não secretas; credenciais/API só depois de definir a estratégia de proteção e, preferencialmente, um proxy remoto.
- Flash interna: apenas NVS e armazenamento técnico do firmware; não contém chats nem chaves em texto simples.

## Próximo marco

Encapsular a tela inicial atrás do futuro `AppManager`, introduzir uma camada de arquivo com operações atômicas e criar o fluxo de provisionamento Wi-Fi para NVS. LVGL será avaliado como substituto do renderer da Fase 1 após confirmação da taxa de atualização e do consumo de PSRAM do ST7789.
