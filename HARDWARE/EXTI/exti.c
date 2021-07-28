#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"
#include "spi.h"
#include "sys.h"
#include "stm32f4xx.h" 
#include "ad7177.h"
#include "usart.h"
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





extern struct adc_dev_str adc_dev ;


void nvic_init(u8 ch_id)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
    if(ch_id == 0){
        
        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure); 

    }else{
        NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);     
    
    }



    
}




void nvic_deinit()
{
    NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);   
}

void nvic_deinit_ch1()
{
    NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);   
}


/***************************************************************************/
void dready_dout_sw_ch1(char is_miso)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    if(is_miso)  
    {
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, DISABLE);
        nvic_deinit();
        SPI1_Init();
        
        
    }else{
        
        SPI_Cmd(SPI1, DISABLE);
           
        /* config the extiline clock and AFIO clock */
        RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //KEY0 KEY1 KEY2��Ӧ����
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
        GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
        
        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);
        
        
        EXTI_InitStructure.EXTI_Line = EXTI_Line6;//LINE0
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�����ش��� 
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
        EXTI_Init(&EXTI_InitStructure);//����
        
        nvic_init(0); 
    }
}


/******************************************sync******************************************/
void sync_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
 

    /* config the extiline clock and AFIO clock */
    RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //KEY0 KEY1 KEY2��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
    
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
    
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;//LINE0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
    EXTI_Init(&EXTI_InitStructure);//����
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);        
    
}







//CS CLK DIN DOUT

void EXTI9_5_IRQHandler(void)
{
    uint32_t adc_val;
    static int data_idx = 0;
    static int grep_idx = 0;
    float temp_val;
	if(EXTI_GetITStatus(EXTI_Line6) == SET)
	{
        
		//EXTI_ClearITPendingBit(EXTI_Line14);
		//spi_simu_init( );
        //send_pulse( );
        
        
        dready_dout_sw_ch1(1);					// switch to MISO mode
        adc_val = read_adc_val(0);//read_adc_val_continue();
        dready_dout_sw_ch1(0);			// switch back to extern INT mode
        
        append_data(&adc_dev,adc_val);
        temp_val = get_voltage(adc_val);
        printf("adc1 %x %f\r\n",adc_val,temp_val);
        
		//adc_inter_call_back(); 	// receive the data
        EXTI_ClearITPendingBit(EXTI_Line6);
	}
    
    
    
    
}


void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
        if(PAin(1) == SET){
            adc_dev.sync_input = 1;
        }else{
            adc_dev.sync_input = 0;
        }

        EXTI_ClearITPendingBit(EXTI_Line1);
	}
    
}


////////////////////////////////////////////////////



void dready_dout_sw_ch2(char is_miso)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    if(is_miso)  
    {
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, DISABLE);
        nvic_deinit_ch1();
        SPI2_Init();
        
    }else{
        
        SPI_Cmd(SPI2, DISABLE);
           
        /* config the extiline clock and AFIO clock */
        RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //KEY0 KEY1 KEY2��Ӧ����
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
        GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
        
        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14);
        
        
        EXTI_InitStructure.EXTI_Line = EXTI_Line14;//LINE0
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�����ش��� 
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
        EXTI_Init(&EXTI_InitStructure);//����
        
        nvic_init(1); 
    }
}




void EXTI15_10_IRQHandler(void)
{
    uint32_t adc_val;
    static int data_idx = 0;
    static int grep_idx = 0;
    float temp_val;
    uint16_t speed_code;
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
        
		
        //printf("EXTI15_10_IRQHandler\r\n");
		//spi_simu_init( );
        //send_pulse( );
///        
///        
        dready_dout_sw_ch2(1);					// switch to MISO mode
        //adc_val = read_adc_val(1);
        //speed_code = read_adc_reg_part2(0x28);
        //printf("EXTI15_10_IRQHandler %x\r\n",speed_code);
        
        
        adc_val = read_adc_val(1);//read_adc_val_continue();
        dready_dout_sw_ch2(0);			// switch back to extern INT mode
        
        //##/TODO//append_data(&adc_dev,adc_val);
        temp_val = get_voltage(adc_val);
        printf("adc2 %x %f\r\n",adc_val,temp_val);
        
///		//adc_inter_call_back(); 	// receive the data
///        //EXTI_ClearITPendingBit(EXTI_Line14);
        EXTI_ClearITPendingBit(EXTI_Line14);
	}
}






