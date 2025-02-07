#include "Led_Matrix.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "led_matrix.pio.h"
#include "hardware/clocks.h"

// Definições de pinos
#define MATRIX_WS2812_PIN     7     // Pino de controle da matriz 5x5


volatile uint8_t current_number = 0;  // Número atual exibido
volatile bool update_num_matrix = false; // Flag utilizada para atualizar a matriz

/**
 * @brief Rotina para definição da intensidade de cores do LED
 * @param b Intensidade do azul (0.0 a 1.0)
 * @param r Intensidade do vermelho (0.0 a 1.0)
 * @param g Intensidade do verde (0.0 a 1.0)
 * @return Valor de cor formatado em uint32_t
 */

uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}


/**
 * @brief Retorna a cor associada a um número (0-9) utilizando a função matrix_rgb.
 * @param number Número (0-9)
 * @return Cor calculada em formato uint32_t
 */

uint32_t get_number_color(uint8_t number) {
    switch (number) {
        case 0:
            return matrix_rgb(0.0, 0.0, 0.4); // 
        case 1:
            return matrix_rgb(0.0, 0.4, 0.0); // 
        case 2:
            return matrix_rgb(0.0, 0.4, 0.4); //
        case 3:
            return matrix_rgb(0.4, 0.0, 0.0); // 
        case 4:
            return matrix_rgb(0.4, 0.0, 0.4); // 
        case 5:
            return matrix_rgb(0.4, 0.4, 0.0); // 
        case 6:
            return matrix_rgb(0.4, 0.4, 0.4); // 
        case 7:
            return matrix_rgb(0.1, 0.4, 0.25); // 
        case 8:
            return matrix_rgb(0.0, 0.2, 0.4); // 
        case 9:
            return matrix_rgb(0.5, 0.25, 0.1); // 
        default:
            return matrix_rgb(0.3, 0.2, 0.15); // 
    }
}

/**
 * @brief Matriz de representação numérica para display 5x5
 * @details Cada número (0-9) é representado por 25 valores binários (5x5)
 *          Valor 1 = LED ligado, 0 = LED desligado
 */

const uint32_t numbers[10][25] = {
    // Número 0
    {1,1,1,1,1,  1,0,0,0,1,  1,0,0,0,1,  1,0,0,0,1,  1,1,1,1,1},
    // Número 1
    {1,1,1,1,1,  0,0,1,0,0,  0,0,1,0,1,  0,1,1,0,0,  0,0,1,0,0},
    // Número 2
    {1,1,1,1,1,  1,0,0,0,0,  1,1,1,1,1,  0,0,0,0,1,  1,1,1,1,1},
    // Número 3
    {1,1,1,1,1,  0,0,0,0,1,  1,1,1,1,1,  0,0,0,0,1,  1,1,1,1,1},
    // Número 4
    {1,0,0,0,0,  0,0,0,0,1,  1,1,1,1,1,  1,0,0,0,1,  1,0,0,0,1},
    // Número 5
    {1,1,1,1,1,  0,0,0,0,1,  1,1,1,1,1,  1,0,0,0,0,  1,1,1,1,1},
    // Número 6
    {1,1,1,1,1,  1,0,0,0,1,  1,1,1,1,1,  1,0,0,0,0,  1,1,1,1,1},
    // Número 7
    {0,0,0,0,1,  0,1,0,0,0,  0,0,1,0,0,  0,0,0,1,0,  1,1,1,1,1},
    // Número 8
    {1,1,1,1,1,  1,0,0,0,1,  1,1,1,1,1,  1,0,0,0,1,  1,1,1,1,1},
    // Número 9
    {1,1,1,1,1,  0,0,0,0,1,  1,1,1,1,1,  1,0,0,0,1,  1,1,1,1,1},
};



/**
 * @brief Atualiza a matriz de LEDs com o número especificado
 * @param number Número a ser exibido (0-9)
 * @details Envia os dados para a matriz LED WS2812 usando PIO
 */

void update_led_matrix(uint8_t number, PIO pio, uint sm) {
    // Calcula a cor utilizando a função auxiliar baseada em matrix_rgb
    uint32_t color = get_number_color(number);

    for (int i = 0; i < 25; i++) {
        if (numbers[number][i]) {
            pio_sm_put_blocking(pio, sm, color); // Envia a cor para o LED
        } else {
            pio_sm_put_blocking(pio, sm, 0); // Desliga o LED
        }
    }
    sleep_us(50);
}


/**
 * @brief Apaga todos os LEDs da matriz.
 * @param pio Instância do PIO utilizada.
 * @param sm Número da state machine.
 * @details Envia o valor 0 para cada um dos 25 LEDs da matriz, desligando-os.
 */
void clear_led_matrix(PIO pio, uint sm) {
    for (int i = 0; i < 25; i++) {
        pio_sm_put_blocking(pio, sm, 0); // Envia 0 para desligar o LED
    }
    sleep_us(50);
}



