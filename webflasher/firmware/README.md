# Pacotes para o Web Flasher

Esta pasta é a única origem de imagens que o botão **Gravar imagem oficial** aceita. A página exige que cada arquivo tenha hash SHA-256 no manifesto antes de habilitar esse botão.

## Publicar uma release

1. Compile e valide fisicamente o firmware.
2. Execute o empacotador a partir da raiz do projeto. Passe o `boot_app0.bin` da instalação PlatformIO/Arduino-ESP32:

```powershell
node tools/package-webflasher.mjs --version 0.1.0 --boot-app0 "C:\caminho\para\boot_app0.bin"
```

3. O comando copia os binários para `webflasher/firmware/releases/0.1.0/`, calcula SHA-256 e substitui `manifest.json`.
4. Sirva o diretório `webflasher/` por HTTPS ou `localhost` e teste no Chrome/Edge com uma placa real.

Os artefatos esperados do PlatformIO são:

| Arquivo | Origem | Offset |
| --- | --- | ---: |
| `bootloader.bin` | `.pio/build/t_embed_ai/bootloader.bin` | `0x0000` |
| `partitions.bin` | `.pio/build/t_embed_ai/partitions.bin` | `0x8000` |
| `boot_app0.bin` | Framework Arduino-ESP32 | `0xe000` |
| `t-embed-ai-<versão>.bin` | `.pio/build/t_embed_ai/firmware.bin` | `0x10000` |

Não altere os offsets. A opção de apagar toda a flash fica desmarcada por padrão, para preservar NVS e armazenamento interno. Publique uma release somente depois de confirmar compilação, upload e teste físico.

## Uso local para desenvolvimento

Web Serial precisa de contexto seguro. Com Node.js instalado:

```powershell
npx --yes serve webflasher
```

Abra a URL `localhost` mostrada pelo comando em Chrome ou Edge. Não abra `index.html` diretamente como `file://`, porque o navegador poderá bloquear Web Serial e o carregamento do manifesto.
