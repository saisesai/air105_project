#include <stdio.h>

#include "air105.h"
#include "delay.h"
#include "sysc.h"
#include "usart.h"
#include "usb_cdc_acm.h"
#include "usbd_core.h"

void usb_dc_low_level_init(void) {
	SYSCTRL_AHBPeriphResetCmd(SYSCTRL_AHBPeriph_USB, DISABLE); // srst_usb require software clear
	SYSCTRL_AHBPeriphClockCmd(SYSCTRL_AHBPeriph_USB, DISABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USB_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	SYSCTRL_AHBPeriphClockCmd(SYSCTRL_AHBPeriph_USB, ENABLE);
}

int main(void) {
  SystemClock_Config_HSE();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_0);
  Delay_Init();
  USART_Init();
  printf("USART Init\r\n");

  GPIO_InitTypeDef gpio;
  gpio.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio.GPIO_Remap = GPIO_Remap_1;
  GPIO_Init(GPIOD, &gpio);
  printf("GPIO Init\r\n");

  cdc_acm_init();
  while (!usb_device_is_configured()) {
  }
  printf("USB Init\r\n");

  while (1) {
    GPIO_ResetBits(GPIOD, GPIO_Pin_14 | GPIO_Pin_15);
    Delay_ms(500);
    GPIO_SetBits(GPIOD, GPIO_Pin_14 | GPIO_Pin_15);
    cdc_acm_data_send_test();
    Delay_ms(500);
  }
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
  /* User can add his own implementation to report the file name and line
   number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
   line) */

  /* Infinite loop */
  while (1) {
  }
}
#endif
