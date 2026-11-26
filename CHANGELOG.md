# Changelog

## 0.1.0 — 2026-11-25

### Adicionado

- Auditoria de hardware, matriz de viabilidade e plano de validação para a T-Embed CC1101 Plus.
- Projeto PlatformIO fixado em `espressif32@6.13.0`, Arduino e perfil ESP32-S3 com 16 MB de flash e OPI PSRAM.
- Particionamento OTA de duas imagens de 7 MB e armazenamento local de 2 MB; dados de usuário permanecem no MicroSD.
- Mapa de pinos centralizado, inicialização segura dos CS SPI e controle centralizado da alimentação compartilhada GPIO15.
- Tela inicial escura em 320 x 170, navegação por encoder/botões, serviço de LEDs e painel de diagnóstico.
- Montagem do MicroSD e criação da árvore `/T-EMBED-AI`.
- Sondagem I2C de PN532, BQ27220 e BQ25896, sem alteração dos registros de carga.
- Web Flasher estático com Web Serial, validação ESP32-S3, manifesto de release com SHA-256 e seleção manual de binários.
- Workflow GitHub Actions para compilar o ESP32-S3, montar o pacote do Web Flasher, publicar artefatos e implantar no GitHub Pages.

### Conhecidas / pendentes

- Nenhuma compilação foi executada neste ponto sem PlatformIO Core instalado no ambiente de desenvolvimento. A tentativa de instalar um toolchain isolado foi bloqueada pela credencial TLS do ambiente (`curl: SEC_E_NO_CREDENTIALS`); nenhuma instalação global foi feita.
- A presença do nRF24L01 não é assumida: fontes oficiais recentes o descrevem como módulo externo opcional.
- Leituras do BQ27220, configuração segura do BQ25896, Wi-Fi provisionado, IA, áudio, USB e rádio são fases posteriores.
