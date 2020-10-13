#ifndef BATERIA_H
#define BATERIA_H

#include <stdint.h>

void init_adc();

uint16_t get_vbat_top();

uint16_t get_vbat_bottom();

#endif /* BATERIA_H */

