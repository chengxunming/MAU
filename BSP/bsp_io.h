#ifndef __BSP_IO_H
#define __BSP_IO_H 			   

#include "sys.h"


#define  BEEPON           GPIO_SetBits(GPIOC,GPIO_Pin_0)//��������
#define  BEEPOFF          GPIO_ResetBits(GPIOC,GPIO_Pin_0)//�������ر�

//��������״̬��ȡ���У��ϣ�
#define SIGNAL1	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//��ƿ���
#define SIGNAL2	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//�ֿؿ��أ�0Ϊ�ر�
#define SIGNAL3	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//���ƿ��أ�1Ϊ��
#define SIGNAL4	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//ң�ؿ��أ�Ĭ��Ϊ��

void BUZZER_Init(void);
void Switch_GPIO_Init(void);

#endif

