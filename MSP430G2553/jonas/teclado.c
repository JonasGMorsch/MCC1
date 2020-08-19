#include "teclado.h"

 void init_matrixkb()
 {
     KB_PORT_DIR = 0b00001111;
 }



uint8_t matrixkb()
{
    uint8_t c = 1;
    for(uint8_t i=0; i<4; i++)
    {
        KB_PORT_OUT = ~(1 << i);
        for(uint8_t j=0; j<3; j++)
        {
            if (TST_BIT(KB_PORT_IN, (1 << (j+4))))
                c++;
            else
            {
                if(c < 13)
                {
                    if(c==11)
                        return 0;
                    else
                        return c;
                }
                else
                    return 255;
            }
        }
    }
    return 255;
}
