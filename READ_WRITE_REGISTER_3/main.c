#include <stdio.h>
#include <stdlib.h>
#include "uart.h"

// UART read function
int uart_read(void)
{
    while (!(USART1->SR & (1U << 5))); // RXNE
    return (USART1->DR & 0xFF);
}

// Flush leftover characters (\r, \n)
void uart_flush(void)
{
    char temp;

    while (USART1->SR & (1U << 5)) // while data available
    {
        temp = USART1->DR;

        if (temp != '\r' && temp != '\n')
            break;
    }
}

int main(void)
{
    char ch;
    char buffer[20];
    int i;
    unsigned int address;
    unsigned int value;

    uart_init();

    while (1)
    {
        // Menu
        printf("\r\n------ MENU ------\r\n");
        printf("1. Read Register\r\n");
        printf("2. Write Register\r\n");
        printf("Enter choice: ");

        // Read valid choice
        while (1)
        {
            ch = uart_read();

            if (ch == '\r' || ch == '\n')
                continue;

            printf("%c", ch); // echo
            break;
        }

        // Flush leftover Enter keys
        uart_flush();

        // ================= READ =================
        if (ch == '1')
        {
            printf("\r\nEnter address (hex): ");
            fflush(stdout);

            i = 0;
            while (1)
            {
                ch = uart_read();

                if (ch == '\r' || ch == '\n')
                {
                    if (i == 0)
                        continue;
                    else
                        break;
                }

                buffer[i++] = ch;
                printf("%c", ch);

                if (i >= 19)
                    break;
            }

            buffer[i] = '\0';

            // Convert string → number
            address = strtol(buffer, NULL, 16);

            // Read register
            value = *(volatile unsigned int *)address;

            printf("\r\nValue at 0x%X = 0x%X\r\n", address, value);
        }

        // ================= WRITE =================
        else if (ch == '2')
        {
            printf("\r\nEnter address (hex): ");
            fflush(stdout);

            i = 0;
            while (1)
            {
                ch = uart_read();

                if (ch == '\r' || ch == '\n')
                {
                    if (i == 0)
                        continue;
                    else
                        break;
                }

                buffer[i++] = ch;
                printf("%c", ch);

                if (i >= 19)
                    break;
            }

            buffer[i] = '\0';
            address = strtol(buffer, NULL, 16);

            printf("\r\nEnter value (hex): ");
            fflush(stdout);

            i = 0;
            while (1)
            {
                ch = uart_read();

                if (ch == '\r' || ch == '\n')
                {
                    if (i == 0)
                        continue;
                    else
                        break;
                }

                buffer[i++] = ch;
                printf("%c", ch);

                if (i >= 19)
                    break;
            }

            buffer[i] = '\0';
            value = strtol(buffer, NULL, 16);

            // Write register
            *(volatile unsigned int *)address = value;

            printf("\r\nWritten 0x%X to address 0x%X\r\n", value, address);
        }
        else
        {
            printf("\r\nInvalid choice!\r\n");
        }
    }
}
