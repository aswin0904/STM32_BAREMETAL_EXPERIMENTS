#include <stdio.h>
#include "uart.h"



void keypad_init(void)
{
    // 1. Enable GPIOA clock
    RCC->APB2ENR |= (1U << 2);

    // ---------------------------
    // 2. Configure PA0–PA3 (ROWS) as Output Push-Pull
    // MODE = 11 (50 MHz), CNF = 00 → 0x3
    // ---------------------------
    GPIOA->CRL &= ~(0xFFFF);        // Clear bits for PA0–PA3 (16 bits)
    GPIOA->CRL |=  (0x3333);        // Set PA0–PA3 as output

    // ---------------------------
    // 3. Configure PA5–PA7 (COLUMNS) as Input Pull-up/down
    // MODE = 00, CNF = 10 → 0x8
    // Pins:PA4, PA5, PA6, PA7 → bits 20–31
    // ---------------------------
    GPIOA->CRL &= ~(0xFFFF << 16);   // Clear bits for PA5–PA7
    GPIOA->CRL |=  (0x8888 << 16);   // Set PA5–PA7 as input pull-up/down

    // ---------------------------
    // 5. Enable Pull-up (IMPORTANT)
    // Set ODR bits HIGH for PA4–PA7
    // ---------------------------
    GPIOA->ODR |= (1 << 4);  // PA4 pull-up
    GPIOA->ODR |= (1 << 5);  // PA5 pull-up
    GPIOA->ODR |= (1 << 6);  // PA6 pull-up
    GPIOA->ODR |= (1 << 7);  // PA7 pull-up
}




// Simple delay for debounce
void delay(volatile int time)
{
    while (time--);
}





char keypad_scan(void)
{
    char keys[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };

    for (int row = 0; row < 4; row++)
    {
        // Set all rows HIGH
        GPIOA->ODR |= (0x0F << 0);

        // Set current row LOW
        GPIOA->ODR &= ~(1 << row);

        // Check columns
        for (int col = 0; col < 4; col++)
        {
            if (!(GPIOA->IDR & (1 << (col + 4)))) // PA4–PA7
            {
                delay(100000); // debounce

                // Wait until key release
                while (!(GPIOA->IDR & (1 << (col + 4))));

                return keys[row][col];
            }
        }
    }

    return 0; // no key pressed
}

int main(void)
{
    keypad_init();
    uart_init();

    while (1)
    {
        char key = keypad_scan();

        if (key)
        {
            printf("Key: %c\r\n", key);
        }
    }
}

