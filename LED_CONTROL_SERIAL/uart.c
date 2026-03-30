#include "stm32f1xx.h"
#include "uart.h"
#include <stdint.h>

#define SYS_CLK   8000000   // IMPORTANT: 72 MHz
#define BAUDRATE  115200

#define CR1_TE (1U << 3)
#define CR1_RE (1U << 2)
#define CR1_UE (1U << 13)

#define SR_TXE (1U << 7)

// Function prototypes
static void uart_set_baudrate(uint32_t clk, uint32_t baudrate);
static void uart_write(int ch);

// Initialize UART1 on PB6 (TX), PB7 (RX)
void uart_init(void)
{
    // 1. Enable clocks: GPIOB, USART1, AFIO
    RCC->APB2ENR |= (1U << 3);   // GPIOB clock
    RCC->APB2ENR |= (1U << 14);  // USART1 clock
    RCC->APB2ENR |= (1U << 0);   // AFIO clock

    // 2. Remap USART1 to PB6/PB7
    AFIO->MAPR |= (1U << 2);

    // 3. Configure PB6 (TX) → AF Push-Pull, 50MHz
    GPIOB->CRL &= ~(0xF << 24);   // clear
    GPIOB->CRL |=  (0xB << 24);   // 1011

    // 4. Configure PB7 (RX) → Input floating
    GPIOB->CRL &= ~(0xF << 28);   // clear
    GPIOB->CRL |=  (0x4 << 28);   // 0100

    // 5. Set baud rate
    uart_set_baudrate(SYS_CLK, BAUDRATE);

    // 6. Enable TX, RX and USART
    USART1->CR1 = CR1_TE | CR1_RE | CR1_UE;
}

// Set baud rate
static void uart_set_baudrate(uint32_t clk, uint32_t baudrate)
{
    USART1->BRR = 0x45;
}

// Write one character
static void uart_write(int ch)
{
    while (!(USART1->SR & SR_TXE)); // wait until TX empty
    USART1->DR = (ch & 0xFF);
}

// Redirect printf to UART
int __io_putchar(int ch)
{
    uart_write(ch);
    return ch;
}
