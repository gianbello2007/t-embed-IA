# Contrato de integração de IA

## Limite de responsabilidade

O T-Embed AI não hospeda nem executa modelos de linguagem. Ele será um cliente HTTPS configurável para um agente remoto. A Fase 1 não faz chamadas e não armazena credenciais.

## Interface planejada

```text
AIService
  configure(endpoint, authentication, model, custom_headers, timeout)
  sendConversation(messages, attachments, streaming)
  cancel(request_id)
  receive token / complete / error
```

O adaptador de cada fornecedor fica atrás de uma interface comum. Isso permite trocar entre um servidor próprio, um gateway e um provedor de IA sem reescrever a UI ou o histórico.

## Dados enviados

- Texto confirmado pelo usuário.
- Metadados de conversa necessários à continuidade.
- Anexos selecionados explicitamente, transmitidos em streaming e com limite de tamanho.
- Áudio somente depois de o usuário iniciar e encerrar a gravação; a transcrição será serviço remoto.

Nada é enviado automaticamente por causa da simples presença no MicroSD.

## Segurança

- Não compile chaves de API no firmware.
- Não grave chaves em texto simples no MicroSD nem as imprima em logs.
- Prefira um proxy próprio que preserve a chave no servidor.
- Se a credencial ficar no dispositivo, use NVS e documente que acesso físico ainda pode permitir extração.
- Valide certificado TLS, use timeout, limite de resposta e cancelamento.
- Scripts gerados pela IA exigem visualização e confirmação antes de serem executados.

## Informações necessárias para implementar a Fase 3

1. URL HTTPS do endpoint ou do proxy.
2. Esquema de autenticação (por exemplo, bearer, header proprietário ou mTLS).
3. Formato de mensagens e de anexos.
4. Modelo/versão de agente e suporte a streaming.
5. Limites de requisição, timeout e política de retenção de dados.

Sem esses dados, a Fase 3 deve exibir configuração ausente — nunca uma resposta de IA simulada.
