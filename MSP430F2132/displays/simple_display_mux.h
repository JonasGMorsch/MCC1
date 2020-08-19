/*
 * simple_display_mux.h
 *
 *  Created on: Feb 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#ifndef DISPLAY_LEDDISPLAY_MUX_H_
#define DISPLAY_LEDDISPLAY_MUX_H_

#include <stdint.h>

#define COM_ANODO
//#define COM_CATODO

#define NUN_OF_DISPLAYS 3

#define DISPLAYS_MUX_PORT_DIR P1DIR
#define DISPLAYS_MUX_PORT_OUT P1OUT

/**
  * @brief  Configura hardware.
  * @param  Nenhum
  *
  * @retval Nenhum.
  */
void display_mux_init();

/**
  * @brief  Escrevre nos displays de 7 segmentos.
  * @param  data: valor sem decimal sem conversão. Dados
  *             são convertidos internamente. data
  *             deve ser maior caso mais de dois displays.
  *
  * @retval Nenhum.
  */
void display_mux_write(uint16_t data);

#endif /* DISPLAY_LEDDISPLAY_H_ */
