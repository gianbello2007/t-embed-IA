# Plano de validação

## Executado no ambiente de desenvolvimento

| Verificação | Resultado |
| --- | --- |
| Estrutura nova do projeto | Criada |
| Auditoria de fontes oficiais | Concluída |
| Versão do PlatformIO/Core | Ausente inicialmente; build pendente de instalação |
| Instalação isolada do toolchain | Não concluída: o `curl.exe` do ambiente falhou ao adquirir credenciais TLS (`SEC_E_NO_CREDENTIALS`) ao buscar Python oficial. Nenhuma instalação global foi feita. |
| Compilação cruzada | Pendente até o ambiente ser instalado |
| Checagem estrutural | Aprovada em 25/11/2026 com `node tools/check-project.mjs` |
| Teste físico | Pendente: não há placa conectada a esta sessão |

Uma compilação sem erros não é considerada prova de funcionamento no dispositivo.

## Protocolo de teste físico da Fase 1

| Teste | Procedimento | Critério de aprovação |
| --- | --- | --- |
| Boot | Energize pela bateria e USB | UI abre sem reinicializações repetidas |
| Display | Observe splash e quatro painéis | imagem horizontal sem artefatos |
| Backlight | Ligue/desligue e observe 12/16 de brilho | brilho discreto e sem PWM visível |
| Encoder | Gire lentamente e rápido | seleção previsível, sem saltos excessivos |
| Botões | clique e pressione por >700 ms | retorno ao launcher conforme rodapé |
| LEDs | Observe após boot com/sem MicroSD | verde se SD montou, âmbar se não montou |
| MicroSD | Insira cartão e reinicie | diretórios criados e capacidade exibida |
| I2C | Abra Hardware e acompanhe Serial | endereços instalados são reportados corretamente |
| Segurança de energia | Meça/observe carga por 5 min | sem escrita de registradores BQ nesta versão |

## Bloqueadores antes da próxima fase

- Confirmar revisão física e a presença ou ausência de nRF24L01.
- Executar a compilação e corrigir qualquer incompatibilidade do pacote Arduino instalado.
- Executar o protocolo físico acima e registrar resultados reais.
- Definir o serviço de IA externo antes de criar o cliente de chat.
