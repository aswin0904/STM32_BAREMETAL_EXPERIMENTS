#include <stdint.h>
#include "stm32f1xx.h"

/* ================= FUNCTION DECLARATIONS ================= */

void ADC1_Init(void);
uint16_t ADC1_Read(void);
void PWM_Init(void);

/* ================= MAIN ================= */

int main(void)
{
    uint16_t adc_val;

    // Enable GPIOA and ADC1 clocks
    RCC->APB2ENR |= (1 << 2);   // IOPAEN
    RCC->APB2ENR |= (1 << 9);   // ADC1EN

    // Enable TIM2 clock
    RCC->APB1ENR |= (1 << 0);   // TIM2EN

    ADC1_Init();
    PWM_Init();

    while (1)
    {
        adc_val = ADC1_Read();

        // Scale ADC (0–4095) to PWM (0–1000)
        TIM2->CCR2 = (adc_val * 1000) / 4095;
    }
}

/* ================= ADC INIT ================= */

void ADC1_Init(void)
{
    // PA0 as analog input
    GPIOA->CRL &= ~(0xF << 0);

    // ADC prescaler = PCLK2 / 6
    RCC->CFGR |= (2 << 14);

    // Turn ON ADC
    ADC1->CR2 |= (1 << 0);

    // Small delay
    for (int i = 0; i < 1000; i++);

    // Reset calibration
    ADC1->CR2 |= (1 << 3);
    while (ADC1->CR2 & (1 << 3));

    // Start calibration
    ADC1->CR2 |= (1 << 2);
    while (ADC1->CR2 & (1 << 2));
}

/* ================= ADC READ ================= */

uint16_t ADC1_Read(void)
{
    // Select channel 0
    ADC1->SQR3 = 0;

    // Start conversion
    ADC1->CR2 |= (1 << 0);

    // Wait for conversion complete
    while (!(ADC1->SR & (1 << 1)));

    return (uint16_t)ADC1->DR;
}

/* ================= PWM INIT ================= */

void PWM_Init(void)
{
    // PA1 as Alternate Function Push-Pull
    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |= (0xB << 4);  // MODE1=11, CNF1=10

    // Timer settings
    TIM2->PSC = 8 - 1;      // 1 MHz timer clock
    TIM2->ARR = 1000 - 1;   // 1 kHz PWM

    // PWM Mode 1 (CH2)
    TIM2->CCMR1 &= ~(0x7 << 12);
    TIM2->CCMR1 |= (6 << 12);   // OC2M = 110
    TIM2->CCMR1 |= (1 << 11);   // Preload enable

    // Enable CH2 output
    TIM2->CCER |= (1 << 4);

    // Enable auto-reload preload
    TIM2->CR1 |= (1 << 7);

    // Start timer
    TIM2->CR1 |= (1 << 0);
}
