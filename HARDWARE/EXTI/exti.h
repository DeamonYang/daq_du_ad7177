#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"  	
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//void dready_dout_sw(char is_miso);
void dready_dout_sw_ch1(char is_miso);
void dready_dout_sw_ch2(char is_miso);
void nvic_init(u8 ch_id);
//void EXTIX_Init(void);	//�ⲿ�жϳ�ʼ��		 					    
#endif

























