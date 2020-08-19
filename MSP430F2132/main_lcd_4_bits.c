
#include <msp430.h>
#include "lib/gpio.h"
#include "lib/bits.h"
#include "displays/lcd.h"
//definição para acessar a memória flash
char mensagem[] = " DADOS DE 4BITS!\0"; //mensagem armazenada na memória flash
//-----------------------------------------------------------------------------------
int main()
{
    WDTCTL = WDTPW + WDTHOLD;          // Stop WDT
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;



    P2DIR = 0xFF; //PORTD como saída

 
 inic_LCD_4bits(); //inicializa o LCD
 escreve_LCD(" INTERFACE DE"); //string armazenada na RAM
 cmd_LCD(0xC0,0); //desloca cursor para a segunda linha
 escreve_LCD_Flash(mensagem); //string armazenada na flash
 for(;;){} //laço infinito, aqui vai o código principal
}
//===================================================================================

