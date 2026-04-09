#include <stdio.h>
#include "stm32f1xx.h"
#include "uart.h"

// ================= UART READ =================
int uart_read(void)
{
    while (!(USART1->SR & (1U << 5)));  // Wait RXNE
    return (USART1->DR & 0xFF);
}

// ================= GPIO INIT =================
void relay_gpio_init(void)
{
    RCC->APB2ENR |= (1U << 3); // Enable GPIOB clock

    // PB0 → Output Push-Pull, 50MHz (STRONG DRIVE)
    GPIOB->CRL &= ~(0xF << 0);
    GPIOB->CRL |=  (0x3 << 0);  // 0011
}

// ================= RELAY CONTROL =================
// ACTIVE LOW (most relay modules)

void relay_on(void)
{
    GPIOB->ODR &= ~(1U << 0);   // STRONG LOW → Relay ON
}

void relay_off(void)
{
    GPIOB->ODR |= (1U << 0);    // HIGH → Relay OFF
}

// ================= MAIN =================
int main(void)
{
    char ch;

    uart_init();
    relay_gpio_init();

    // Start with relay OFF
    relay_off();

    while (1)
    {
        printf("\r\n====== RELAY CONTROL MENU ======\r\n");
        printf("1. Turn Relay ON\r\n");
        printf("2. Turn Relay OFF\r\n");
        printf("Enter choice: ");

        ch = uart_read();
        printf("%c", ch);  // Echo input

        if (ch == '1')
        {
            relay_on();
            printf("\r\nRelay ON\r\n");
        }
        else if (ch == '2')
        {
            relay_off();
            printf("\r\nRelay OFF\r\n");
        }
        else
        {
            printf("\r\nInvalid Choice\r\n");
        }
    }
}
