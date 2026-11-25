# T-Embed AI

Firmware modular para transformar a LILYGO T-Embed CC1101 Plus em um assistente de produtividade portátil. A inteligência artificial é sempre remota: este firmware nunca tenta executar um modelo de linguagem no ESP32-S3.

O repositório começou pela Fase 0 e pela fundação verificável da Fase 1. A imagem atual inicializa a placa, protege o barramento SPI compartilhado, inicializa o display e a iluminação AW9364, lê encoder e botões, monta o MicroSD, cria a estrutura inicial de dados, sonda o I2C e apresenta esses estados reais em uma interface navegável. Recursos não implementados não aparecem como funcionais.

## Estado atual

| Área | Estado |
| --- | --- |
| Plataforma específica T-Embed | Baseada no mapa de pinos oficial, pendente de confirmação da revisão física |
| Display ST7789 em 320 x 170 | Implementado na fundação |
| Encoder e botão de usuário | Implementado por eventos, com clique e pressionamento longo |
| LEDs WS2812 | Serviço centralizado de estado implementado |
| MicroSD e diretórios | Implementado; informa ausência ou erro de montagem |
| Descoberta PN532 / BQ27220 / BQ25896 | Implementada por I2C; não altera o carregador |
| Wi-Fi / IA / voz / USB HID | Não implementados; sem credenciais e sem respostas simuladas |
| CC1101 / nRF24 / IR | Não inicializados nesta etapa; nenhuma transmissão é feita |

## Compilar

Instale PlatformIO Core e execute:

```powershell
pio run -e t_embed_ai
pio run -e t_embed_ai -t upload
pio device monitor -b 115200
```

Leia [docs/BUILD.md](docs/BUILD.md) antes da primeira gravação e [docs/HARDWARE_AUDIT.md](docs/HARDWARE_AUDIT.md) antes de conectar um nRF24L01 ou testar rádios.

## Navegação da imagem atual

- Gire o encoder: muda a seleção no launcher.
- Pressione o encoder: abre o painel selecionado.
- Botão de usuário: retorna ao launcher.
- Pressionamento longo do encoder ou do botão de usuário: retorna ao launcher.

Os painéis `Sistema`, `Arquivos`, `Rede` e `Hardware` são diagnósticos reais da fundação; eles não anunciam IA, rádio ou editor como prontos.

## Documentação

- [Auditoria de hardware](docs/HARDWARE_AUDIT.md)
- [Arquitetura](docs/ARCHITECTURE.md)
- [Integração da IA](docs/API_INTEGRATION.md)
- [Compilação e gravação](docs/BUILD.md)
- [Plano de validação](docs/VALIDATION.md)
- [Registro de mudanças](CHANGELOG.md)

## Licença

MIT. Consulte [LICENSE](LICENSE).
