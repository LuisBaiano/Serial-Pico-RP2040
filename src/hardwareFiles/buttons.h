#ifndef BUTTONS_H
#define BUTTONS_H

#include "pico/stdlib.h"

// Define os pinos dos botões
#define BUTTON_A 5
#define BUTTON_B 6

// Função para inicializar os botões com resistor de pull-up
void init_buttons(void);

#endif // BUTTONS_H
