/*
 * lcd.c
 *
 *  Created on: Aug 20, 2018
 *      Author: Renan Augusto Starke
 *      
 *      Adaptado de AVR e Arduino: Técnicas de Projeto, 2a ed. - 2012.
 *      Instituto Federal de Santa Catarina
 */

#include "lcdi2c.h"

uint8_t i2c_port = 0x0;

static inline void enable_pulse()
{
	__delay_cycles(1000);
	SET_BIT(i2c_port, E_PIN);
	i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);

	__delay_cycles(1000);
	CLR_BIT(i2c_port, E_PIN);
	i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);

	__delay_cycles(1000);
}

/**
 * @brief  Configura hardware: verificar lcd.h para mapa de pinos e nible de dados.
 * @param  Nenhum
 *
 * @retval Nenhum.
 */
void lcd_init_4bits()
{
	/* Configura pinos de controle */
	SET_BIT(i2c_port, RS_PIN | E_PIN);
	i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);

	CLR_BIT(i2c_port, RS_PIN | E_PIN);
	i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);

	__delay_cycles(100000);

	/* Interface de 8 bits */
#if (NIBBLE_DADOS)
	i2c_port = (i2c_port & 0x0F) | 0x30;
#else
	i2c_port = (i2c_port & 0xF0) | 0x03;
#endif
	i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);


	enable_pulse();
	__delay_cycles(10000);
	enable_pulse();
	__delay_cycles(100000);
	enable_pulse();

#if (NIBBLE_DADOS)
	i2c_port = (i2c_port & 0x0F) | 0x20;
#else
	i2c_port = (i2c_port & 0xF0) | 0x02;
#endif
	i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);

	enable_pulse();

	/* Interface de 4 bits 2 linhas
	 * Mudar comando para displays maiores */
	lcd_send_data(0x28, LCD_CMD);
	lcd_send_data(LCD_TURN_OFF, LCD_CMD);
	lcd_send_data(LCD_CLEAR, LCD_CMD);

	/* Mensagem aparente e cursor inativo não piscante
	 * Outros modos podem ser consultados no datasheet */
	lcd_send_data(0x0C, LCD_CMD);
	lcd_send_data(LCD_LINE_0, LCD_CMD);
}

/**
 * @brief Envia um dado estático para o display: caractere ou comando.
 * @param data: valor do comando.
 * @param data_type: LCD_CMD para comando. LCD_DATA para caractere.
 *
 * @retval Nenhum
 */
void lcd_send_data(uint8_t data, lcd_data_t data_type)
{
	if (data_type == LCD_CMD)
		CLR_BIT(i2c_port, RS_PIN);
	else
		SET_BIT(i2c_port, RS_PIN);

	i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);

	/* Envia 4 MSB primeiramente */
#if (NIBBLE_DADOS)
	i2c_port = (i2c_port & 0x0F) | (0xF0 & data);
#else
	i2c_port = (i2c_port & 0xF0) | (data >> 4);
#endif
	i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);

	enable_pulse();

	/* Envia 4 LSB restantes do byte */
#if (NIBBLE_DADOS)
	i2c_port = (i2c_port & 0x0F) | (0xF0 & (data << 4));
#else
	i2c_port = (i2c_port & 0xF0) | (0x0F & data);
#endif
	i2c_write_single_byte(DEVICE_I2C_ADDR, i2c_port);

	enable_pulse();

	/* Delay adicional em caso de instruções lentas: limpeza, etc */
	if ((data == 0) && (data_type < 4))
		__delay_cycles(10000); // _delay_ms(2);
}

/**
 * @brief Escreve um string estática no LCD.
 * @param c: ponteiro para a string em RAM
 *
 * @retval Nenhum
 */
void lcd_write_string(char *c)
{
	for (; *c != '\0'; c++)
		lcd_send_data(*c, LCD_DATA);
}

