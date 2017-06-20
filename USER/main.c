#include "stm32f10x.h"
#include "global.h"
#include "bsp.h"
#include "bsp_can.h"
#include "bsp_time.h"
#include "bsp_io.h"
#include "bsp_led.h"
#include "delay.h"
/* Extern variables-----------------------------------------------------------*/
uint8_t Time_CanRespone_Count=0;
bool fault_canTest=false;
CAN_EV_TYPE ev_CanRespone=CAN_EV_None;

int main(void)
{	
	SystemInit();			 //ϵͳ��ʼ��	 	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init(72);	    	 //��ʱ������ʼ��
	
	Switch_GPIO_Init();
	BUZZER_Init();
	LED_Init();
	CAN_Configuration();
	Time_Init();
	
	delay_ms(1000);			//�ϵ��ӳ�1s������Ƭ���ȶ�
	
	MAU_CanTest();
	MAU_SwitchTest();
	MAU_BuzzerTest();
	LED_ALL_ON();
	MAU_TestEnd();
	while(1)
	{
		BEEPON;
		delay_ms(1000);
		BEEPOFF;
		delay_ms(1000);
	}
}

