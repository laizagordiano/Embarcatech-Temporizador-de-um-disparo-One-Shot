# Temporizador de um Disparo (One Shot) para Controle de LEDs

Este projeto foi desenvolvido para o microcontrolador **Raspberry Pi Pico W** com o objetivo de controlar LEDs de forma sequencial, utilizando um temporizador com a função `add_alarm_in_ms()` do Pico SDK. O acionamento dos LEDs depende do pressionamento de um botão e segue uma sequência de tempos definidos.

## Descrição do Funcionamento

Este sistema realiza o controle de LEDs de forma sequencial, com base no clique de um botão. Após o pressionamento do botão, os LEDs são acionados e desligados de acordo com um temporizador. O fluxo de controle é descrito abaixo:

### 1. Estado Inicial
- Nenhum LED está aceso.

### 2. Acionamento dos LEDs
- **Pressionar o botão**: Todos os LEDs (azul, vermelho e verde) acendem imediatamente.
- Após **3 segundos**, o LED azul é apagado.
- Após **mais 3 segundos**, o LED vermelho é apagado.
- Após **mais 3 segundos**, o LED verde é apagado e todos os LEDs ficam apagados.

### 3. Controle do Botão
- O botão só pode iniciar a sequência de LEDs se todos os LEDs estiverem apagados. Durante a execução da sequência, o botão é ignorado para evitar reiniciar o processo enquanto os LEDs ainda estiverem ligados ou em processo de desligamento.

### 4. Temporização
- O temporizador é configurado utilizando a função `add_alarm_in_ms()`, que chama uma função de callback após cada intervalo de **3 segundos**. Cada callback altera o estado de um LED, apagando-os na sequência: primeiro o azul, depois o vermelho e, finalmente, o verde.

## Fluxo de Operação

1. **Botão pressionado**: Todos os LEDs são acesos.
2. **Após 3 segundos**: O LED azul é apagado.
3. **Após mais 3 segundos**: O LED vermelho é apagado.
4. **Após mais 3 segundos**: O LED verde é apagado.
5. **Fim do processo**: Todos os LEDs estão apagados e o sistema está pronto para reiniciar o ciclo quando o botão for pressionado novamente.
