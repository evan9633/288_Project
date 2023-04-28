#include "adc.h"


void adc_init(void){

    SYSCTL_RCGCADC_R |= 0x01;
    SYSCTL_RCGCGPIO_R |= 0x02;

    while((SYSCTL_PRGPIO_R & 0x02) != 0x02);
    //while((SYSCTL_PRADC_R & 0x0001) != 0x0001) {};
    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    ADC0_CC_R = 0x1;


    while((SYSCTL_PRADC_R & 0x0001) != 0x0001) {};
    ADC0_PC_R = 0x1;


    ADC0_SSPRI_R &= 0x3210;

    //averaging
    ADC0_SAC_R = 0x4;



    ADC0_SSPRI_R |= 0x3210;
    //ADC0_SSPRI_R = 0x3210;
    ADC0_ACTSS_R &= ~0x0001;
    ADC0_EMUX_R &= ~0x000F;


    ADC0_SSMUX0_R &= ~0x000F;
    //ADC0_SSMUX0_R &= 0xFFF0;
    //ADC0_SSMUX0_R |= 0xFFFA;
    ADC0_SSMUX0_R += 10;
    ADC0_SSCTL0_R &= 0x0006;
    ADC0_SSCTL0_R |= 0x0006;
    ADC0_ACTSS_R |= 0x0001;




}

uint16_t adc_read(void){

    ADC0_PSSI_R &= 0x0001;
    ADC0_PSSI_R |= 0x0001;
    while((ADC0_RIS_R & 0x01) == 0) {};
    int result = ADC0_SSFIFO0_R & 0xFFF;
    ADC0_ISC_R = 0x0001;
    return result;

}
