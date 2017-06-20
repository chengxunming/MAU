#include "bsp_can.h"
#include "global.h"
#include "bsp.h"

static Frame_DefType receive_Frame;

static void CAN_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	CAN1_GPIO_CLK_ENABLE();//ʹ��CAN1_GPIOʱ��	                   											

    GPIO_InitStructure.GPIO_Pin = CAN1_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(CAN1_GPIO, &GPIO_InitStructure);			//��ʼ��IO
   
    GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(CAN1_GPIO, &GPIO_InitStructure);		 //��ʼ��IO	
}
/**************************************************************************************************************
�������� : CAN��ʼ��
***************************************************************************************************************/
void CAN_Configuration(void)
{
	CAN_InitTypeDef  CAN_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	CAN_DeInit(CAN1);/* CAN register init */
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_GPIO_Init();
	CAN1_CLK_ENABLE();//ʹ��CAN1ʱ��
	
	CAN_InitStructure.CAN_TTCM = DISABLE; /*��ֹʱ�䴥��ͨѶģʽ*/
//	CAN_InitStructure.CAN_ABOM = ENABLE; /*�Զ��˳�����״̬��ʽ��0-�������ֶ����ߣ�1-�Զ��˳�����״̬*/
	CAN_InitStructure.CAN_ABOM = DISABLE; /*�Զ��˳�����״̬��ʽ��0-�������ֶ����ߣ�1-�Զ��˳�����״̬*/
	CAN_InitStructure.CAN_AWUM = DISABLE; /*0-�����ͨ����0���ѣ�1-��⵽����ʱ���Զ�����*/
	CAN_InitStructure.CAN_NART = DISABLE; /*0-һֱ�ظ�����ֱ���ɹ���1-���۳ɹ��Է�ֻ����һ��*///�Զ��ش�											
	CAN_InitStructure.CAN_RFLM = DISABLE; /*0-���ʱFIFOδ�������±��ĸǵ��ɱ��ģ�1-������������±���ֱ�Ӷ�ʧ*/
	CAN_InitStructure.CAN_TXFP = DISABLE; /*0-���ķ������ȼ��ɱ�־��������1-���ķ������ȼ��������Ⱥ�˳�����*/
//	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;/*ģʽ-����ģʽ-����ģʽ*/
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
/*-----------------------------------------------
//BaudRate = APB1 / ((BS1 + BS2 + 1) * Prescaler)
//--------------------------------------------------------
//   ����   |  SJW  |  BS1  |  BS2  | prescaler | Sample |
//--------------------------------------------------------
// 1000kbps |  1tq  | 2tq   |  1tq  |     9     |  75.0% |
//--------------------------------------------------------
//  800kbps |  1tq  | 3tq   |  1tq  |     9     |  80.0% |
//--------------------------------------------------------
//  500kbps |  1tq  | 6tq   |  1tq  |     9     |  87.5% |
//--------------------------------------------------------
//  250kbps |  1tq  | 6tq   |  1tq  |     18    |  87.5% |
//--------------------------------------------------------
//  125kbps |  1tq  | 13tq  |  2tq  |     18    |  87.5% |
//--------------------------------------------------------
//  100kbps |  1tq  | 6tq   |  1tq  |     45    |  87.5% |  
//--------------------------------------------------------*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//��ͬ��ʱ����
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;//CAN_BS1_5tq;//ʱ��1���
	CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;//ʱ��2���
	CAN_InitStructure.CAN_Prescaler = 9;//��Ƶֵ(ʱ�䵥Ԫ����)
	CAN_Init(CAN1, &CAN_InitStructure);
	
	//CAN_Filter_Reconfig(BOARD);
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	/* Enable the CAN Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_Filter_Config();
}
/*
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
void CAN_Filter_Config(void)
{
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN��������ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//��������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ�б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	/* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */

    CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(MAU_CAN_ID<<21)&0xffff0000)>>16;				//Ҫ���˵�ID��λ 
    CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //Ҫ���˵�ID��λ 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xEFFF;//0x7FFF;			//��������16λÿλ����ƥ��
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//��������16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//��������������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

//����CAN����֡--PDU����--��֡
uint8_t Send_CAN_DataFrame(uint8_t *ptr,uint8_t len) //ptr: ����ָ��. len: ���ݳ��� 
{
	uint16_t i=0;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;

	
	SENDID = (0x01<<7)|MAU_CAN_ID;
	
	TxMessage.StdId = SENDID;	
	TxMessage.IDE = CAN_ID_STD;//��׼֡     //CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA;//����֡
    
	if((len<=8)&&(len>0))
	{
		TxMessage.DLC = len;
		
		for(i=0; i<len; i++)
		{
			TxMessage.Data[i] = *ptr++;
		}	
	}
	else
	{
		TxMessage.DLC = 0;
	}
	
	mailbox_num = CAN_Transmit(CAN1, &TxMessage);
	
	while((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<1000)) //����״̬,�ȵ��������  
	{
		i++;
	}
	if(i==1000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);//��ʱ�رշ���
		can_tx_fail = 1;
	}
	return can_tx_fail;
}

//����CAN����֡��������ʶ��
void Receive_CAN_DataFrame(CanRxMsg* canRx,uint8_t num)
{
	uint8_t i;
	uint8_t *prt_rx;
	if(num!=5)
	{
		ev_CanRespone=CAN_EV_ResFail;
		return;
	}
	prt_rx=(uint8_t *)(&receive_Frame);
	for(i=0; i<num; i++)
	{
		*(prt_rx+i)= canRx->Data[i];
	}
	if(DataCheckSum((uint8_t *)(&receive_Frame),4)!=receive_Frame.check_sum)
	{
		ev_CanRespone=CAN_EV_ResFail;
		return;
	}
	switch(receive_Frame.fun_code)
	{
		case FUN_CAN:
			MAU_FunCan_Respone_Deal(&receive_Frame);
			break;
		case FUN_RESULT:
			
			break;
		case FUN_END:
			
			break;
		default:
			ev_CanRespone=CAN_EV_ResFail;
			break;
	}

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USB_LP_CAN1_RX0_IRQHandler(void)       //CAN�����жϺ���
{
	uint8_t i;
	CanRxMsg RxMessage;
 	

	RxMessage.StdId = 0x00;  //��׼��ʶ����ȡֵ 0~0x7FF��
	RxMessage.ExtId = 0x00;  //��չ��ʶ����ȡֵ 0~0x3FFFF��
	RxMessage.IDE = 0x00;    //��ʶ��������
	RxMessage.DLC = 0x00;    //��Ϣ��֡���ȣ�ȡֵ 0~0x8��
	RxMessage.FMI = 0x00;    //Ҫͨ���Ĺ�������������Щ��Ϣ�洢�������У�ȡֵΪ0~0xFF��
	for(i=0; i<8; i++)
	{
		RxMessage.Data[i] = 0x00;//���������ݣ�ȡֵΪ0~0xFF��
	}
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//����
	
	if(RxMessage.IDE==CAN_ID_STD)
	{
		if(RxMessage.DLC>0)
		{
			Receive_CAN_DataFrame(&RxMessage,RxMessage.DLC);//����CAN����,ִ����Ӧ����	
		}
		if(RxMessage.DLC==0)
		{

		}
		
	}
}

