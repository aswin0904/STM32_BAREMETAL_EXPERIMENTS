#include "stm32f1xx.h"

// -------- UART CONFIG --------
void USART1_Init(void)
{
    // Enable clocks: GPIOA + USART1
    RCC->APB2ENR |= (1U << 2) | (1U << 14);

    // PA9 (TX) → AF Push Pull
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |= (0xB << 4);

    // PA10 (RX) → Floating input
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |= (0x4 << 8);

    // Baud rate = 9600 (for 8 MHz clock)
    USART1->BRR = 0x341;

    // Enable TX, RX, USART
    USART1->CR1 |= (1 << 3) | (1 << 2) | (1 << 13);
}

// -------- UART SEND --------
void USART1_WriteChar(char c)
{
    while (!(USART1->SR & (1 << 7))); // TXE
    USART1->DR = c;
}

void USART1_WriteString(char *str)
{
    while (*str)
    {
        USART1_WriteChar(*str++);
    }
}

// -------- UART RECEIVE --------
char USART1_ReadChar(void)
{
    while (!(USART1->SR & (1 << 5))); // RXNE
    return USART1->DR;
}

// -------- GPIO CONFIG --------
void GPIO_Config(void)
{
    RCC->APB2ENR |= (1U << 2);   // GPIOA clock
    RCC->APB1ENR |= (1U << 0);   // TIM2 clock

    GPIOA->CRL &= ~(0xFFF);

    // PA0 → AF Push-Pull (PWM)
    GPIOA->CRL |= (0xB << (0 * 4));

    // PA1, PA2 → Output (motor direction)
    GPIOA->CRL |= (0x3 << (1 * 4));
    GPIOA->CRL |= (0x3 << (2 * 4));

    // Set direction (Forward)
    GPIOA->ODR |= (1 << 1);
    GPIOA->ODR &= ~(1 << 2);
}

// -------- TIM2 PWM CONFIG --------
void TIM2_PWM_Config(void)
{
    TIM2->PSC = 800 - 1;   // 8MHz → 10kHz
    TIM2->ARR = 100 - 1;   // 100 steps → % control

    TIM2->CNT = 0;

    // PWM Mode 1 + preload
    TIM2->CCMR1 &= ~(0xFF);
    TIM2->CCMR1 |= (6 << 4);   // PWM mode 1
    TIM2->CCMR1 |= (1 << 3);   // preload enable

    TIM2->CCER |= (1 << 0);    // enable CH1
    TIM2->CR1 |= (1 << 7);     // auto-reload preload

    TIM2->CCR1 = 0;           // start at 50%

    TIM2->CR1 |= (1 << 0);     // start timer
}

// -------- MAIN --------
int main(void)
{
    GPIO_Config();
    TIM2_PWM_Config();
    USART1_Init();

    char input;

    while (1)
    {
        USART1_WriteString("\n\r=== PWM CONTROL MENU ===\n\r");
        USART1_WriteString("1 -> 25%\n\r");
        USART1_WriteString("2 -> 50%\n\r");
        USART1_WriteString("3 -> 75%\n\r");
        USART1_WriteString("4 -> 100%\n\r");
        USART1_WriteString("Enter choice: ");

        input = USART1_ReadChar();   // Read key
        USART1_WriteChar(input);     // Echo back

        switch(input)
        {

            case '1':
                TIM2->CCR1 = 25;
                USART1_WriteString("\n\rSet to 25%\n\r");
                break;

            case '2':
                TIM2->CCR1 = 50;
                USART1_WriteString("\n\rSet to 50%\n\r");
                break;

            case '3':
                TIM2->CCR1 = 75;
                USART1_WriteString("\n\rSet to 75%\n\r");
                break;

            case '4':
                TIM2->CCR1 = 99;   // ~100%
                USART1_WriteString("\n\rSet to 100%\n\r");
                break;

            default:
                USART1_WriteString("\n\rInvalid choice!\n\r");
                break;
        }
    }
}
