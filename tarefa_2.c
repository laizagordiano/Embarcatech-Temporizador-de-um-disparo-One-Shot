#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include <stdio.h>

// Definição dos pinos para os LEDs e o botão
#define LED_RED 12
#define LED_GREEN 13
#define LED_BLUE 11
#define BUTTON 5

// Variável global para controlar o estado do sistema
volatile bool is_running = false;

// Declaração das funções de callback para desligar os LEDs
bool turn_off_blue_callback(struct repeating_timer *t);
bool turn_off_red_callback(struct repeating_timer *t);
bool turn_off_green_callback(struct repeating_timer *t);

// Função de interrupção para o botão
void button_callback(uint gpio, uint32_t events) {
    static uint32_t last_press_time = 0;  // Variável para controlar o debounce
    uint32_t current_time = to_ms_since_boot(get_absolute_time());  // Obtém o tempo atual em milissegundos

    // Evita múltiplas leituras do botão em um curto espaço de tempo (debounce)
    if (current_time - last_press_time < 200) return; // Debounce de 200ms
    last_press_time = current_time;

    // Se o sistema não está rodando, inicia o processo de ligar os LEDs
    if (!is_running) {
        is_running = true;
        printf("Botão pressionado, ligando LEDs\n");
        gpio_put(LED_RED, 1);    // Liga o LED vermelho
        gpio_put(LED_GREEN, 1);  // Liga o LED verde
        gpio_put(LED_BLUE, 1);   // Liga o LED azul
        
        // Alarma para desligar o LED azul após 3 segundos
        add_alarm_in_ms(3000, (alarm_callback_t)turn_off_blue_callback, NULL, false);
    }
}

// Função de callback para desligar o LED azul
bool turn_off_blue_callback(struct repeating_timer *t) {
    printf("Desligando Azul\n");
    gpio_put(LED_BLUE, 0);  // Desliga o LED azul
    
    // Alarma para desligar o LED vermelho após 3 segundos
    add_alarm_in_ms(3000, (alarm_callback_t)turn_off_red_callback, NULL, false);
    return false;
}

// Função de callback para desligar o LED vermelho
bool turn_off_red_callback(struct repeating_timer *t) {
    printf("Desligando Vermelho\n");
    gpio_put(LED_RED, 0);  // Desliga o LED vermelho
    
    // Alarma para desligar o LED verde após 3 segundos
    add_alarm_in_ms(3000, (alarm_callback_t)turn_off_green_callback, NULL, false);
    return false;
}

// Função de callback para desligar o LED verde e finalizar o ciclo
bool turn_off_green_callback(struct repeating_timer *t) {
    printf("Desligando Verde\n");
    gpio_put(LED_GREEN, 0);  // Desliga o LED verde
    is_running = false;      // Atualiza o estado para indicar que o ciclo foi completado
    return false;
}

int main() {
    stdio_init_all();  // Inicializa a comunicação serial para printar mensagens no terminal
    
    // Inicializa os pinos dos LEDs e do botão
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_init(BUTTON);

    // Define a direção dos pinos (saída para LEDs e entrada para o botão)
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_set_dir(BUTTON, GPIO_IN);
    
    // Ativa o pull-up no pino do botão para garantir que o valor seja 1 quando não pressionado
    gpio_pull_up(BUTTON);

    // Ativa a interrupção para o botão, chamada quando ocorre uma borda de descida (botão pressionado)
    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    
    // Loop principal vazio, o programa continua executando enquanto o microcontrolador estiver ligado
    while (1) {
        tight_loop_contents();  // Um loop de espera ativa para economizar energia
    }
}
