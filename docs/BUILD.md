# Compilação e gravação

## Ambiente fixado

| Item | Versão / escolha |
| --- | --- |
| Plataforma | `espressif32@6.13.0` |
| Framework | Arduino-ESP32 fornecido pela plataforma |
| Placa PlatformIO | `t_embed_ai`, definição local do projeto |
| MCU | ESP32-S3, 240 MHz |
| Flash / PSRAM | 16 MB QIO / 8 MB OPI |
| USB CDC | habilitado no boot |
| Partições | duas imagens OTA de 7 MB + 2 MB SPIFFS técnico |

O MicroSD é o armazenamento de usuário. A partição `storage` não substitui o cartão e não deve ser usada para segredos ou chats.

## Pré-requisitos

1. Instale Python 3.10 ou mais recente.
2. Instale PlatformIO Core: `python -m pip install --upgrade platformio`.
3. Conecte a placa por USB-C usando um cabo de dados.
4. Não mantenha o botão central (GPIO0/BOOT) pressionado ao resetar.

## Comandos

```powershell
# No diretório do projeto
pio run -e t_embed_ai
pio run -e t_embed_ai -t upload
pio device monitor -b 115200
```

O primeiro build baixa a toolchain e as bibliotecas fixadas. Não grave a imagem até a compilação terminar sem erros.

## Publicar para o Web Flasher

Após compilar e validar na placa, empacote os artefatos para [`webflasher/`](../webflasher/README.md):

```powershell
node tools/package-webflasher.mjs --version 0.1.0 --boot-app0 "C:\caminho\para\boot_app0.bin"
```

O empacotador copia `bootloader.bin`, `partitions.bin`, `boot_app0.bin` e `firmware.bin`, calcula SHA-256 e habilita a release no manifesto. A publicação de uma imagem não substitui o teste físico; não há release incluída neste repositório enquanto o build não for confirmado.

## Primeira validação na placa

1. Grave o firmware e abra o monitor em 115200 baud.
2. Verifique no log: PSRAM encontrada, estado do MicroSD e os dispositivos I2C.
3. Confirme que a tela está horizontal (320 x 170) e legível.
4. Gire o encoder; o destaque do menu deve mudar uma posição por detent aproximado.
5. Teste o botão do encoder e o botão de usuário.
6. Com MicroSD inserido, abra `Arquivos` e confirme a criação de `/T-EMBED-AI`.

## Recuperação

Se o upload não detectar a placa, solte o botão do encoder, mantenha BOOT pressionado apenas para entrar no modo de download e pressione RESET. Solte BOOT ao iniciar o upload. Isto decorre de GPIO0 ser, ao mesmo tempo, o botão do encoder e o pino de strapping do ESP32-S3.

Não altere parâmetros do BQ25896 nesta versão. O firmware somente detecta o endereço I2C do carregador e não escreve em seus registros.
