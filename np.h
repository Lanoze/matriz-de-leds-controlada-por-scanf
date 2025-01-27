#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "ws2818b.pio.h"

//Definição de pinos, variáveis e número de LED
#define NUM_LEDS 25
#define MATRIZ_PIN 7
#define NUM_COLUNAS 5

// Definição de pixel GRB
struct pixel_t {
 uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[NUM_LEDS];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
* Inicializa a máquina PIO para controle da matriz de LEDs.
*/
void npInit(uint pin);

/**
* Atribui uma cor RGB a um LED.
*/
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);

//Em ordem crescente: 0.8; 0.1; 0.9; 0.4; 0.6; 0.3; 0.2; 0.7; 1.0
void definir_intensidade(const uint index, const double r, const double g, const double b);

/**
* Limpa o buffer de pixels.
*/
void npClear();

/**
* Escreve os dados do buffer nos LEDs.
*/
void npWrite();

int getIndex(int x, int y);

//Corrige o Index pra que o LED certo seja acendido
uint correcao_index(int index);