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
void npInit(uint pin) {

 // Cria programa PIO.
 uint offset = pio_add_program(pio0, &ws2818b_program);
 np_pio = pio0;

 // Toma posse de uma máquina PIO.
 sm = pio_claim_unused_sm(np_pio, false);
 if (sm < 0) {
   np_pio = pio1;
   sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
 }

 // Inicia programa na máquina PIO obtida.
 ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

 // Limpa buffer de pixels.
 for (uint i = 0; i < NUM_LEDS; ++i) {
   leds[i].R = 0;
   leds[i].G = 0;
   leds[i].B = 0;
 }
}

/**
* Atribui uma cor RGB a um LED.
*/
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
 leds[index].R = r;
 leds[index].G = g;
 leds[index].B = b;
}
//Em ordem crescente: 0.8; 0.1; 0.9; 0.4; 0.6; 0.3; 0.2; 0.7; 1.0
void definir_intensidade(const uint index, const double r, const double g, const double b){
 leds[index].R =(uint8_t) round(r*255.0);
 leds[index].G =(uint8_t) round(g*255.0);
 leds[index].B =(uint8_t) round(b*255.0);
 //if(index==0 || index==5)
 //  if(r==0.5)
 //   printf("b = %.2lf\n(index %d) leds[index].B = %d\n",b,index,leds[index].B);
}

/**
* Limpa o buffer de pixels.
*/
void npClear() {
 for (uint i = 0; i < NUM_LEDS; ++i)
   npSetLED(i, 0, 0, 0);
}

/**
* Escreve os dados do buffer nos LEDs.
*/
void npWrite() {
 // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
 for (uint i = 0; i < NUM_LEDS; ++i) {
   pio_sm_put_blocking(np_pio, sm, leds[i].G);
   pio_sm_put_blocking(np_pio, sm, leds[i].R);
   pio_sm_put_blocking(np_pio, sm, leds[i].B);
 }
}

int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return y * 5 + x; // Linha par (esquerda para direita).
    } else {
        return y * 5 + (4 - x); // Linha ímpar (direita para esquerda).
    }
}

//Corrige o Index pra que o LED certo seja acendido
uint correcao_index(int index){
     //Caso esteja numa linha ímpar
     if((index>=5 && index<10) || (index>=15 && index<20))
     return index<10 ? index+10:index-10;
     else
     return NUM_LEDS-index-1;
    }