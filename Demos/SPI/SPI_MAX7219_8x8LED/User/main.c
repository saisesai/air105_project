// Copyright 2022 IOsetting <iosetting(at)outlook.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * Demo: MAX7219 8x8 LED Matrix
 * Board: Air105 EVB
 * 
 * 3.3V            -> VCC
 * GND             -> GND
 * PC12 SPI0_CLK   -> CLK
 * PC13 CS         -> CS
 * PC14 SPI0_MOSI  -> DIN
*/

#include <stdio.h>
#include "air105.h"
#include "delay.h"
#include "sysc.h"
#include "usart.h"
#include "max7219.h"


const uint8_t numbers[]={
0x00,0x00,0x7C,0xC6,0xC6,0xCE,0xD6,0xD6,  // -0-.  
0xE6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x38,0x78,0x18,0x18,0x18,  // -1-  
0x18,0x18,0x18,0x7E,0x00,0x00,0x00,0x00,  
0x00,0x00,0x7C,0xC6,0x06,0x0C,0x18,0x30,  // -2-  
0x60,0xC0,0xC6,0xFE,0x00,0x00,0x00,0x00,  
0x00,0x00,0x7C,0xC6,0x06,0x06,0x3C,0x06,  // -3-  
0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00,  
0x00,0x00,0x0C,0x1C,0x3C,0x6C,0xCC,0xFE,  // -4-  
0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00,  
0x00,0x00,0xFE,0xC0,0xC0,0xC0,0xFC,0x0E,  // -5-  
0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00,  
0x00,0x00,0x38,0x60,0xC0,0xC0,0xFC,0xC6,  // -6-  
0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,  
0x00,0x00,0xFE,0xC6,0x06,0x06,0x0C,0x18,  // -7-  
0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,  
0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7C,0xC6,  // -8-  
0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00, 
0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7E,0x06,  // -9-  
0x06,0x06,0x0C,0x78,0x00,0x00,0x00,0x00};


int main(void)
{
    uint8_t pos = 0, size = sizeof(numbers), i, j;

    SystemClock_Config_HSE();
    Delay_Init();
    USART_Init();
    printf("USART Init\r\n");

    MAX7219_SPI_Init();
    MAX7219_GPIO_Init();
    MAX7219_Init();
    printf("MAX7219 Init\r\n");

    while (1)
    {
        for (i = 0; i < 8; i++)
        {
            j = (pos + i) % size;
            MAX7219_Write(i + 1, numbers[j]);
        }
        pos = (pos + 1) % size;
        Delay_ms(100);
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

     /* Infinite loop */
    while (1)
    {
    }
}
#endif