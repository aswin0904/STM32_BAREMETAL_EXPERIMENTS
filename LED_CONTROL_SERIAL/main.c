#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
  // change based on your MCU

#define LED_PORT GPIOA

// UART read
int uart_read(void)
{
    while (!(USART1->SR & (1U << 5)));
    return (USART1->DR & 0xFF);
}

// Flush extra chars
void uart_flush(void)
{
    char temp;
    while (USART1->SR & (1U << 5))
    {
        temp = USART1->DR;
        if (temp != '\r' && temp != '\n')
            break;
    }
}

// GPIO init for PA0–PA7 (Blue Pill - STM32F103)
void led_init(void)
{
    RCC->APB2ENR |= (1 << 2); // Enable GPIOA clock

    // Configure PA0–PA7 as Output Push-Pull (2 MHz)

    for (int i = 0; i < 8; i++)
    {
        GPIOA->CRL &= ~(0xF << (i * 4));   // Clear CNF + MODE
        GPIOA->CRL |=  (0x2 << (i * 4));   // MODE = 10 (2 MHz), CNF = 00 (Push-Pull)
    }
}


// Delay (rough)
void delay(void)
{
    for (volatile int i = 0; i < 300000; i++);
}

int main(void)
{
    char ch;
    int led;

    uart_init();
    led_init();

    while (1)
    {
        printf("\r\n------ LED MENU ------\r\n");
        printf("1. Turn ON LED\r\n");
        printf("2. Turn OFF LED\r\n");
        printf("3. Read LED Status\r\n");
        printf("4. Left to Right\r\n");
        printf("5. Right to Left\r\n");
        printf("Enter choice: ");

        ch = uart_read();
        printf("%c", ch);
        uart_flush();

        // ========= TURN ON =========
        if (ch == '1')
        {
            printf("\r\nEnter LED (0-7): ");
            led = uart_read() - '0';
            printf("%d", led);

            LED_PORT->ODR |= (1 << led);   // OR operation

            printf("\r\nLED %d ON\r\n", led);
        }

        // ========= TURN OFF =========
        else if (ch == '2')
        {
            printf("\r\nEnter LED (0-7): ");
            led = uart_read() - '0';
            printf("%d", led);

            LED_PORT->ODR &= ~(1 << led);  // AND with mask

            printf("\r\nLED %d OFF\r\n", led);
        }

        // ========= STATUS =========
        else if (ch == '3')
        {
            printf("\r\nEnter LED (0-7): ");
            led = uart_read() - '0';
            printf("%d", led);

            if (LED_PORT->ODR & (1 << led))
                printf("\r\nLED %d is ON\r\n", led);
            else
                printf("\r\nLED %d is OFF\r\n", led);
        }

        // ========= LEFT TO RIGHT =========
        else if (ch == '4')
        {
            for (int i = 0; i < 8; i++)
            {
                LED_PORT->ODR = (1 << i);   // Left shift
                delay();
            }
        }

        // ========= RIGHT TO LEFT =========
        else if (ch == '5')
        {
            for (int i = 7; i >= 0; i--)
            {
                LED_PORT->ODR = (1 << i);   // Right shift
                delay();
            }
        }

        else
        {
            printf("\r\nInvalid choice!\r\n");
        }
    }
}
