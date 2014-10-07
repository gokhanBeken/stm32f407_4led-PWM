#include "stm32f4xx_conf.h"

#define LED1_STEP   1
#define LED2_STEP   10
#define LED3_STEP   20
#define LED4_STEP   40

volatile short MyDelay;

/* For 50 Hz PWM signal Prescaler and Period Values */
uint16_t Prescaler = 84;
uint32_t Period = 20000;

/* Pulse Values for Channel 1,2,3,4 */
uint32_t PulseValue1 = 0;     // Channel1 Duty Cycle %0  = (0/20000)*100
uint32_t PulseValue2 = 50000; // Channel2 Duty Cycle %25 = (5000/20000)*100
uint32_t PulseValue3 = 10000; // Channel1 Duty Cycle %50 = (10000/20000)*100
uint32_t PulseValue4 = 15000; // Channel1 Duty Cycle %75 = (15000/20000)*100

void Pin3_Config(void);
void Timer3_PWM_Output_Config(uint32_t PeriodValue, uint16_t PrescalerValue);

void Pin4_Config(void);
void Timer4_PWM_Output_Config(uint32_t PeriodValue, uint16_t PrescalerValue);

void DelayMs(unsigned int ms);

int main(void)
{
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000); //1milisaniyelik system clock elde edelim

    Pin3_Config();
    Timer3_PWM_Output_Config(Period, Prescaler);

    Pin4_Config();
    Timer4_PWM_Output_Config(Period, Prescaler);

    unsigned int sayac1=0,sayac2=0,sayac3=0,sayac4=0;
    char durum1=1,durum2=1,durum3=1,durum4=1; //1=artma, 0=eksilme
    while (1)
    {
        if(durum1)
        {
            sayac1+=LED1_STEP;
            if(sayac1>=15000)durum1=0;
        }
        else
        {
            sayac1-=LED1_STEP;
            if(sayac1==0)durum1=1;
        }


        if(durum2)
        {
            sayac2+=LED2_STEP;
            if(sayac2>=15000)durum2=0;
        }
        else
        {
            sayac2-=LED2_STEP;
            if(sayac2==0)durum2=1;
        }



        if(durum3)
        {
            sayac3+=LED3_STEP;
            if(sayac3>=15000)durum3=0;
        }
        else
        {
            sayac3-=LED3_STEP;
            if(sayac3==0)durum3=1;
        }



        if(durum4)
        {
            sayac4+=LED4_STEP;
            if(sayac4>=15000)durum4=0;
        }
        else
        {
            sayac4-=LED4_STEP;
            if(sayac4==0)durum4=1;
        }


        TIM_SetCompare1(TIM4,sayac1);
        TIM_SetCompare2(TIM4,sayac2);
        TIM_SetCompare3(TIM4,sayac3);
        TIM_SetCompare4(TIM4,sayac4);

        TIM_SetCompare1(TIM3,sayac1);
        TIM_SetCompare2(TIM3,sayac2);
        TIM_SetCompare3(TIM3,sayac3);
        TIM_SetCompare4(TIM3,sayac4);

        DelayMs(1);
    }
}

void Pin3_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* GPIOC and GPIOB clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);

    /* GPIOC Configuration: TIM3 CH1 (PC6) and TIM3 CH2 (PC7) TIM3 CH3 (PC8) and TIM3 CH4 (PC9)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Connect TIM3 pins to AF2 */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);

    /******************************************************************/
}

void Timer3_PWM_Output_Config(uint32_t PeriodValue, uint16_t PrescalerValue)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = PeriodValue - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PulseValue1;

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PulseValue2;

    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PulseValue3;

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PulseValue4;

    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);

}




void Pin4_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable clock for TIM4 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* GPIOD and GPIOB clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOB, ENABLE);

    /* GPIOC Configuration: TIM4 CH1 (PD12) and TIM4 CH2 (PD13) TIM3 CH4 (PD14) and TIM4 CH4 (PD15)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


    /* Connect TIM4 pins to AF2 */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

    /******************************************************************/
}


void Timer4_PWM_Output_Config(uint32_t PeriodValue, uint16_t PrescalerValue)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = PeriodValue - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PulseValue1;

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PulseValue2;

    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PulseValue3;

    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = PulseValue4;

    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);

    /* TIM4 enable counter */
    TIM_Cmd(TIM4, ENABLE);

}

void SysTick_Handler(void)
{
    if (MyDelay)
    {
        MyDelay--;
    }
}

void DelayMs(unsigned int ms)
{
    MyDelay=ms;
    while(MyDelay);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif
