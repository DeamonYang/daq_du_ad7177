#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "motor.h"
#include "spi.h"
#include "ad7177.h"
#include "exti.h" 
#include "can.h"


int main(void)
{ 
 
	u8 t;
	u8 len = 'H';	
	u16 times=0;  
    u8 led_toogle = 0;
    u16 dev_id = 0;
    uint16_t speed_code;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    
    
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	LED_Init();		  		//初始化与LED连接的硬件接口 
	SPI1_Init();
    SPI2_Init();
    ad7177_gpio_init();
    nvic_init(0);
    nvic_init(1);

    ad7177_reset_part1();
    ad7177_reset_part2();
    printf("init\n");
    
    
    dev_id = read_adc_dev_id_part1();
    if((dev_id&0xFFF0) != 0x4FD0)
    {
        printf("adc0 init error\n");
        LED2 = 0;
    }

     dev_id = read_adc_dev_id_part2();
    if((dev_id&0xFFF0) != 0x4FD0)
    {
        printf("adc1 init error\n");
        LED2 = 0;
    }     

    
    adc7177_init();
    //read_adc_val();
    dready_dout_sw_ch1(0);
    dready_dout_sw_ch2(0);
    //read_adc_val();
    //CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal); //CAN_Mode_Normal  /CAN_Mode_LoopBack
	while(1)
	{
        
        led_toogle = ~led_toogle;
        delay_ms(10);
        LED2 = led_toogle;
        read_adc_reg_part2(0x28);
        if((times  ++)%10 == 0){
            speed_code = read_adc_reg_part2(0x28);
            //printf("id %x\r\n",speed_code);
        }
        
        
//        //printf("id %x\n",dev_id);
// 
//		delay_ms(10); 
//		LED0 = 0;
//		LED1 = 0;
//		//LED2 = 0;
//		
//		USART_SendData(USART2,len);
//
//		delay_ms(10);
//		LED0 = 1;
//		LED1 = 1;
		//LED2 = 1;
		

	}
}

