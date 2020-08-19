/*
 * simple_display_mux.h
 *
 *  Created on: Feb 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#ifndef TIMER_DISPLAY_MUX_H
#define TIMER_DISPLAY_MUX_H

#include <stdint.h>

#define NUN_OF_DISPLAYS 4

#define COM_ANODO
//#define COM_CATODO

#define DISPLAYS_MUX_PORT_DIR P1DIR
#define DISPLAYS_MUX_PORT_OUT P1OUT

#define DISPLAY_PORT_DIR P3DIR
#define DISPLAY_PORT_OUT P3OUT

void timer_display_mux_init();

void __inline timer_display_mux_write(uint16_t data);

#endif /* TIMER_DISPLAY_MUX_H */
