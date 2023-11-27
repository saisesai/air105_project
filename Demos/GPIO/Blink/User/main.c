#include <stdio.h>
#include "air105.h"
#include "sysc.h"
#include "delay.h"
#include "usart.h"


int main(void)
{
    SystemClock_Config_HSE();
    Delay_Init();
    USART_Init();
    printf("USART Init\r\n");

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Remap = GPIO_Remap_1;
    GPIO_Init(GPIOD, &gpio);
    printf("GPIO Init\r\n");

    while (1)
    {
        GPIO_ResetBits(GPIOD, GPIO_Pin_14|GPIO_Pin_15);
        printf("RESET ");
        Delay_ms(500);
        GPIO_SetBits(GPIOD, GPIO_Pin_14|GPIO_Pin_15);
        printf("SET\r\n");
        Delay_ms(500);
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
