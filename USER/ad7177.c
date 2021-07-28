#include "ad7177.h"
#include "sys.h"
#include "spi.h"
#include "delay.h"
#include "spi.h"
#include "usart.h"

struct adc_dev_str adc_dev;

void adc_struct_init(struct adc_dev_str *s)
{
    s->rdata_idx = 0;
    s->rgrp_idx =0;
    s->wdata_idx = 0;
    s->wgrp_idx = 0;
    s->void_grp_len = BUG_GRP;
}

float get_voltage(unsigned int adc_code)
{
    float val;
    val = adc_code;//(adc_code - 0x80000000);
    val = 5.0*adc_code/0x80000000 - 5.0;
    return val;
}


/**/
char append_data(struct adc_dev_str *s,unsigned int data)
{
    float temp_val;
    if(s->void_grp_len > 0){
        s->buf[s->wgrp_idx][s->wdata_idx] = data;
        if(s->wdata_idx < BUF_LEN - 1){
            s->wdata_idx ++;
        }else{
            if(s->void_grp_len > 1){
                s->void_grp_len --;
                s->wgrp_idx = (s->wgrp_idx + 1)%BUG_GRP;
                s->wdata_idx = 0;
            }else{
                //temp_val = get_voltage(data);
                //printf("buffer overflow %f\n",temp_val);
                s->wdata_idx = 0;
            }
        }
    }
}

  
void ad7177_gpio_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /****************************AD7177_PART1**************************************/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOF时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化	
	GPIO_SetBits(GPIOA,GPIO_Pin_3);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    /****************************AD7177_PART2**************************************/
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOF时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化	
	GPIO_SetBits(GPIOB,GPIO_Pin_11);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);    



    adc_struct_init(&adc_dev);
}


void ad7177_reset_part1()
{
    char i;
    
    delay_ms(1);
    ADC1_CS_L;
    delay_us(500);
    for(i = 0;i < 10;i ++)
    {
        SPI1_ReadWriteByte(0xFF);
    }
    ADC1_CS_H;
    delay_ms(500);  
}

void ad7177_reset_part2()
{
    char i;
    
    delay_ms(1);
    ADC2_CS_L;
    delay_us(500);
    for(i = 0;i < 10;i ++)
    {
        SPI2_ReadWriteByte(0xFF);
    }
    ADC2_CS_H;
    delay_ms(500);  
}


unsigned short read_adc_dev_id_part1()
{
    unsigned short dev_id = 0;
    
    
    /*send command*/
    ADC1_CS_L;
    SPI1_ReadWriteByte(0x47);
    dev_id = SPI1_ReadWriteByte(0x00);
    dev_id = dev_id << 8 | SPI1_ReadWriteByte(0x00);
    ADC1_CS_H;
    return dev_id;
}

unsigned short read_adc_dev_id_part2()
{
    unsigned short dev_id = 0;
    
    
    /*send command*/
    ADC2_CS_L;
    SPI2_ReadWriteByte(0x47);
    dev_id = SPI2_ReadWriteByte(0x00);
    dev_id = dev_id << 8 | SPI2_ReadWriteByte(0x00);
    ADC2_CS_H;
    return dev_id;
}




uint16_t read_adc_reg(unsigned char addr)
{
    uint16_t reg;
    ADC1_CS_L;
    SPI1_ReadWriteByte(addr);
    reg = SPI1_ReadWriteByte(0x00);
    reg = (reg << 8) | SPI1_ReadWriteByte(0x00);
    ADC1_CS_H;
    return reg;
}

uint16_t read_adc_reg_part2(unsigned char addr)
{
    uint16_t reg;
    ADC2_CS_L;
    SPI2_ReadWriteByte(addr);
    reg = SPI2_ReadWriteByte(0x00);
    reg = (reg << 8) | SPI2_ReadWriteByte(0x00);
    ADC2_CS_H;
    return reg;
}

void readl_all_reg( char id )
{
    unsigned char i;
    unsigned char idx;
    uint16_t reg_val;
    for(i = 0;i < 0x3B;i++)
    {
        idx = i | 0x40;
        if(id == 0){
            reg_val = read_adc_reg(idx);
        }else{
            reg_val = read_adc_reg_part2(idx);
        }
        printf("reg[%x] is %04X \r\n",i,reg_val);
        delay_ms(1);
    }
    
    //printf("id is %x\n",read_adc_dev_id_part1());
    
}


void write_adc_reg_ch1(unsigned char addr,uint16_t value)
{
    unsigned char regval = 0;
    ADC1_CS_L;
    SPI1_ReadWriteByte(addr);
    regval = (value>>8)&0xFF;
    SPI1_ReadWriteByte(regval);
    regval = value&0xFF;
    SPI1_ReadWriteByte(regval);
    ADC1_CS_H;
}

void write_adc_reg_ch2(unsigned char addr,uint16_t value)
{
    unsigned char regval = 0;
    ADC2_CS_L;
    SPI2_ReadWriteByte(addr);
    regval = (value>>8)&0xFF;
    SPI2_ReadWriteByte(regval);
    regval = value&0xFF;
    SPI2_ReadWriteByte(regval);
    ADC2_CS_H;
}


#define ADC_50SPS 0x10
#define ADC_20SPS 0x11
#define ADC_10SPS 0x13
#define ADC_05SPS 0x14
#define ADC_1000SPS 0x0A
#define ADC_500SPS 0x0B
#define AIN_CH0 0
#define AIN_CH1 1
#define AIN_CH2 2
#define AIN_CH3 3
#define AIN_CH4 4
#define AIN_REFP 0x15
#define AIN_REFN 0x16

#define INTER_REF 0x8000
#define EXTER_REF 0x0000
#define SING_CYC    0x2000
#define REF_SEL_EXT 0x00
#define REF_SEL_INT 0x20

void adc7177_init()
{
    //
    
    //readl_all_reg();
    //printf("___________________________\r\n\n");
    
//    ADC1_CS_L;

    /************************************adc init fot ch1*******************************************/
    write_adc_reg_ch1(0x01,EXTER_REF  );//1100 0000  internal buffer en | 
    /////
    write_adc_reg_ch1(0x10,0x8000 | (AIN_CH2 << 5) | AIN_CH1);//ch0 enable   8036
    //
    /* configure the reference selection, input buffers, and output coding of the ADC*/
    write_adc_reg_ch1(0x20,0x1f00 | REF_SEL_EXT); 
    
    write_adc_reg_ch1(0x28,0x0500|ADC_05SPS);
    
    write_adc_reg_ch1(0x02,0x0002);
    
    delay_ms(500);
    ADC1_CS_L; 
    
    /************************************adc init fot ch1*******************************************/
    write_adc_reg_ch2(0x01,EXTER_REF  );//1100 0000  internal buffer en | 
    /////
    write_adc_reg_ch2(0x10,0x8000 | (AIN_CH2 << 5) | AIN_CH1);//ch0 enable   8036
    //
    /* configure the reference selection, input buffers, and output coding of the ADC*/
    write_adc_reg_ch2(0x20,0x1f00 | REF_SEL_EXT); 
    
    write_adc_reg_ch2(0x28,0x0500|ADC_05SPS);
    
    write_adc_reg_ch2(0x02,0x0002);
    
    delay_ms(500);
    ADC2_CS_L;    
    
    readl_all_reg(1);
    
    //while(1);
    
    //read_adc_val();
    
    //readl_all_reg();
    
//    ADC1_CS_H;
}

uint32_t read_adc_val(u8 ch_id)
{
    uint32_t read_data = 0;
    
    if(ch_id == 0)
    {
        ADC1_CS_L; 
        SPI1_ReadWriteByte(0x44);
        read_data = SPI1_ReadWriteByte(0x00);
        read_data = (read_data<<8) | SPI1_ReadWriteByte(0x00);
        read_data = (read_data<<8) | SPI1_ReadWriteByte(0x00);
        read_data = (read_data<<8) | SPI1_ReadWriteByte(0x00);
    }else{
        ADC2_CS_L; 
        SPI2_ReadWriteByte(0x44);
        read_data = SPI2_ReadWriteByte(0x00);
        read_data = (read_data<<8) | SPI2_ReadWriteByte(0x00);
        read_data = (read_data<<8) | SPI2_ReadWriteByte(0x00);
        read_data = (read_data<<8) | SPI2_ReadWriteByte(0x00);    
    }
    //ADC1_CS_H;
    return read_data;
}

 
uint32_t read_adc_val_continue()
{
//    uint32_t read_data = 0;
//    //ADC1_CS_L;
//    //SPI1_ReadWriteByte(0x44);
//    read_data = SPI1_ReadWriteByte(0x00);
//    read_data = (read_data<<8) | SPI1_ReadWriteByte(0x00);
//    read_data = (read_data<<8) | SPI1_ReadWriteByte(0x00);
//    //SPI1_ReadWriteByte(0x00);
//    read_data = (read_data<<8) | SPI1_ReadWriteByte(0x00);
//    
//    
//    
//    return read_data;
}











