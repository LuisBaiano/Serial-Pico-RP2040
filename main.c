#include <stdio.h>
#include "pico/stdlib.h"
#include "src/debouncer.h"
#include "src/hardwareFiles/Leds.h"
#include "src/hardwareFiles/buttons.h"
#include "src/hardwareFiles/Led_Matrix.h"
#include "hardware/i2c.h"
#include "src/inc/ssd1306.h"
#include "src/inc/font.h"
#include "hardware/uart.h"
#include "led_matrix.pio.h"
#include "hardware/clocks.h"

/** @brief Configurações de UART e I2C, pinos de LED, botões e outros periféricos. */

#define UART_ID uart0
#define BAUD_RATE 9600

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define MATRIX_WS2812_PIN     7     // Pino de controle da matriz 5x5

#define LED_GREEN   11    // LED RGB: componente verde
#define LED_BLUE    12    // LED RGB: componente azul

#define BUTTON_A    5     // Botão A
#define BUTTON_B    6     // Botão B

#define DEBOUNCE_TIME 200000  // Tempo de debouncing em microsegundos (200ms)

// Variáveis para armazenar o tempo da última interrupção de cada botão
static uint32_t last_interrupt_time_A = 0;
static uint32_t last_interrupt_time_B = 0;

ssd1306_t ssd; // Inicializa a estrutura do display

/**
 * @brief Atualiza o display com o estado dos LEDs.
 *
 * @param ssd Ponteiro para a estrutura do display.
 */

void led_status_display(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false); // Limpa o display

    // Verifica o estado do LED verde
    bool estado_verde = gpio_get(LED_GREEN);
    char *msg_verde = estado_verde ? "Verde ON" : "Verde OFF";
    ssd1306_draw_string(ssd, msg_verde, 0, 0);

    // Verifica o estado do LED azul
    bool estado_azul = gpio_get(LED_BLUE);
    char *msg_azul = estado_azul ? "Azul ON" : "Azul OFF";
    ssd1306_draw_string(ssd, msg_azul, 0, 15);

    ssd1306_send_data(ssd); // Atualiza o display
}

/**
 * @brief Callback de interrupção para tratamento dos botões.
 *
 * Alterna o estado dos LEDs correspondentes aos botões pressionados,
 * utilizando debounce para evitar múltiplos acionamentos.
 *
 * @param gpio Pino que gerou a interrupção.
 * @param events Tipo de evento ocorrido.
 */

void gpio_callback(uint gpio, uint32_t events) {
    // Botão A: alterna o LED Verde
    if (gpio == BUTTON_A) {
        if (check_debounce(&last_interrupt_time_A, DEBOUNCE_TIME)) {
            toggle_led(LED_GREEN);
            bool state = get_led_state(LED_GREEN);
            printf("LED Verde %s\n", state ? "Ligado" : "Desligado");
        }
    }
    // Botão B: alterna o LED Azul
    else if (gpio == BUTTON_B) {
        if (check_debounce(&last_interrupt_time_B, DEBOUNCE_TIME)) {
            toggle_led(LED_BLUE);
            bool state = get_led_state(LED_BLUE);
            printf("LED Azul %s\n", state ? "Ligado" : "Desligado");
        }
    }
}

/**
 * @brief Atualiza a matriz de LED e o display com base no caractere recebido via UART.
 *
 * Se o caractere for dígito (0-9), atualiza a LED matrix; caso contrário, limpa a matriz.
 * Também exibe o caractere recebido no display OLED.
 *
 * @param c_uart Caractere recebido via UART.
 * @param pio Objeto PIO utilizado para controle dos LEDs.
 * @param sm Máquina de estado do PIO.
 */

void display(char c_uart, PIO pio, uint sm){
                // Se for um dígito (0-9), atualiza a LED matrix
            if (c_uart >= '0' && c_uart <= '9') {
                uint8_t num = c_uart - '0';
                update_led_matrix(num, pio, sm);
            }
            else{
                clear_led_matrix(pio, sm);
            }
            // Atualiza o display
            ssd1306_fill(&ssd, false); // Limpa o buffer do display

            // Se houver caractere c_uart via UART, exibe na linha 3
            if (c_uart != '\0') {
                ssd1306_fill(&ssd, 0);
                ssd1306_send_data(&ssd); // Atualiza o display com o novo conteúdo

                char str[2] = {c_uart, '\0'};
                ssd1306_draw_string(&ssd, "Caractere ", 0, 30); // Desenha uma string
                ssd1306_draw_string(&ssd, str, 80, 30); // Linha 3 (y=16)
                ssd1306_send_data(&ssd);
                c_uart = '\0'; // Reseta após exibir
            }
}

/**
 * @brief Inicializa a UART com os parâmetros definidos.
 */

void uart_init_function(){
    // Configura UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(0, GPIO_FUNC_UART); // UART0 TX
    gpio_set_function(1, GPIO_FUNC_UART); // UART0 RX
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
}


/**
 * @brief Função principal.
 *
 * Inicializa os periféricos, configura a comunicação (UART, I2C, PIO) e define as interrupções.
 * No loop principal, lê dados das interfaces USB e UART, atualizando o display e os LEDs conforme necessário.
 *
 * @return int Retorna 0 ao término (nunca alcançado).
 */

int main() {
    // Inicializa o STDIO para comunicação via USB (Serial Monitor)
    stdio_init_all();
    uart_init_function();

    // Inicializa o PIO para controlar os LEDs WS2812
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    pio_matrix_program_init(pio, sm, offset, MATRIX_WS2812_PIN);

    sleep_ms(2000);  // Aguarda a conexão USB
    Led_init();
    init_buttons();

    // Inicialização do I2C (400 kHz) para o display SSD1306
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); 
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); 
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Configura as interrupções para os botões, acionando na borda de descida (FALLING)
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    while (true) {
        // Lê da interface USB CDC (a entrada padrão, se USB estiver conectada)
        int c_usb = getchar_timeout_us(0);
        if (c_usb != PICO_ERROR_TIMEOUT) {
            sleep_ms(500);
            display(c_usb, pio, sm);
            sleep_ms(500);
        }

        // Atualiza o display com o status dos LEDs
        led_status_display(&ssd);

        // Verifica se há dados disponíveis na UART
        if (uart_is_readable(uart0)) {
            char c_uart = uart_getc(uart0);
            sleep_ms(500);
            display(c_uart, pio, sm);
            sleep_ms(500);
        }

        sleep_ms(200);
    }
    return 0;
    }
