#ifndef AD7177_H_
#define AD7177_H_ 1
#include "sys.h"



#define BUF_LEN 1024
#define BUG_GRP 2
struct adc_dev_str{
    unsigned int buf[BUG_GRP][BUF_LEN];
    int void_grp_len;
    int wgrp_idx;
    int wdata_idx;
    int rgrp_idx;
    int rdata_idx;
    char sync_input;
    int sync_delay;
    char sync_af_delay;
};






#define ADC1_CS PAout(3)

#define ADC1_CS_H PAout(3) = 1
#define ADC1_CS_L PAout(3) = 0

#define ADC2_CS PBout(11)
#define ADC2_CS_H PBout(11) = 1
#define ADC2_CS_L PBout(11) = 0


void ad7177_gpio_init();
void ad7177_reset_part1();
unsigned short read_adc_dev_id_part1();
unsigned char ad7177_read_byte(unsigned char addr);
void adc7177_init();
uint16_t read_adc_reg(unsigned char addr);
char append_data(struct adc_dev_str *s,unsigned int data);
float get_voltage(unsigned int adc_code);
uint32_t read_adc_val(u8 ch_id);

void ad7177_reset_part2();
unsigned short read_adc_dev_id_part2();
uint16_t read_adc_reg_part2(unsigned char addr);


#endif