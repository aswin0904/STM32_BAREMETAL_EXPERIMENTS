#include <stdint.h>
#include "stm32f1xx.h"

/* ================= FUNCTION DECLARATIONS ================= */

void ADC1_Init(void);
uint16_t ADC1_Read(void);
void GPIO_Init(void);

/* ================= MAIN ================= */

int main(void)
{
    uint16_t adc_val;
    uint16_t threshold = 2000;   // Adjust based on testing

    // Enable GPIOA and ADC1 clocks
    RCC->APB2ENR |= (1 << 2);   // IOPAEN
    RCC->APB2ENR |= (1 << 9);   // ADC1EN

    ADC1_Init();
    GPIO_Init();

    while (1)
    {
        adc_val = ADC1_Read();

        if (adc_val < threshold)
        {
            // DARK → LED ON
            GPIOA->BSRR = (1 << 1);   // PA1 HIGH
        }
        else
        {
            // BRIGHT → LED OFF
            GPIOA->BRR = (1 << 1);    // PA1 LOW
        }
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
    // Select channel 0 (PA0)
    ADC1->SQR3 = 0;

    // Start conversion
    ADC1->CR2 |= (1 << 0);

    // Wait for conversion complete
    while (!(ADC1->SR & (1 << 1)));

    return (uint16_t)ADC1->DR;
}

/* ================= GPIO INIT ================= */

void GPIO_Init(void)
{
    // PA1 as Output Push-Pull (LED)
    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |= (0x1 << 4);   // MODE1 = 01 (10 MHz), CNF1 = 00
}
