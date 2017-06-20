#include "bsp_io.h"

//���������ų�ʼ��
void BUZZER_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC,  ENABLE );
	
	//����������Beep���������Ĭ����0
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_ResetBits(GPIOC,GPIO_Pin_0);	//��0���رշ��������
}

void Switch_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,  ENABLE );
	/***************************************************************************
	                      ��������״̬����ʼ��
	****************************************************************************/
	//��������״̬��⣬��������Ĭ����1
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14); //��1��Ĭ����������
	GPIO_SetBits(GPIOC,GPIO_Pin_1); //��1��Ĭ����������
}
