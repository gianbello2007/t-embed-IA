# T-Embed AI Web Flasher

Flasher estático para Chrome ou Edge que usa [esptool-js](https://github.com/espressif/esptool-js) e Web Serial. O navegador pede a porta explicitamente; o site não recebe os dados do dispositivo.

## Recursos

- Confere se o chip detectado é um ESP32-S3 antes de liberar a gravação.
- Grava uma release declarada em `firmware/manifest.json` apenas se seus SHA-256 forem válidos.
- Permite selecionar manualmente binários PlatformIO nos offsets corretos.
- Mantém apagamento completo de flash como opção explícita e desmarcada.
- Usa a velocidade de conexão ROM de 115200 e parâmetros de flash da placa: QIO, 80 MHz, 16 MB.

## Hospedagem

Publique a pasta `webflasher/` em um host HTTPS. Para desenvolvimento, siga o passo de localhost em [`firmware/README.md`](firmware/README.md).

## Publicação de release

Consulte [`firmware/README.md`](firmware/README.md). O manifesto inicial está desativado propositalmente porque nenhum binário foi compilado e testado nesta sessão.
