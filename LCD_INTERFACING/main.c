#include "stm32f1xx.h"

// ================= PIN DEFINITIONS =================
#define RS 0
#define EN 1
#define D4 2
#define D5 3
#define D6 4
#define D7 5

#define SET(pin)   (GPIOA->BSRR = (1 << pin))
#define CLR(pin)   (GPIOA->BSRR = (1 << (pin + 16)))

// ================= DELAY =================
void delay(volatile uint32_t t) {
    while(t--);
}

// ================= LCD LOW LEVEL =================
void lcd_pulse(void) {
    SET(EN);
    delay(3000);
    CLR(EN);
    delay(3000);
}

void lcd_write4(uint8_t data) {

    // Clear data pins
    CLR(D4); CLR(D5); CLR(D6); CLR(D7);

    if(data & 0x01) SET(D4);
    if(data & 0x02) SET(D5);
    if(data & 0x04) SET(D6);
    if(data & 0x08) SET(D7);

    lcd_pulse();
}

// ================= LCD CORE =================
void lcd_send(uint8_t data, uint8_t rs) {

    if(rs) SET(RS);
    else   CLR(RS);

    lcd_write4(data >> 4);   // HIGH nibble
    delay(5000);

    lcd_write4(data & 0x0F); // LOW nibble
    delay(50000);
}

void lcd_cmd(uint8_t cmd) {
    lcd_send(cmd, 0);
}

void lcd_data(uint8_t data) {
    lcd_send(data, 1);
}

// ================= INIT =================
void lcd_init(void) {

    delay(1000000); // wait after power

    CLR(RS);

    lcd_write4(0x03);
    delay(200000);

    lcd_write4(0x03);
    delay(200000);

    lcd_write4(0x03);
    delay(200000);

    lcd_write4(0x02); // 4-bit mode
    delay(200000);

    lcd_cmd(0x28); // 2 line
    lcd_cmd(0x0C); // display ON
    lcd_cmd(0x06); // entry mode
    lcd_cmd(0x01); // clear

    delay(500000);
}

// ================= HIGH LEVEL =================
void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x80 : 0xC0;
    lcd_cmd(addr + col);
}

void lcd_print(char *str) {
    while(*str) lcd_data(*str++);
}

// ================= MAIN =================
int main(void) {

    // Enable GPIOA
    RCC->APB2ENR |= (1<<2);

    // PA0–PA5 output
    GPIOA->CRL &= ~0x00FFFFFF;
    GPIOA->CRL |=  0x00222222;

    delay(500000);

    lcd_init();

    lcd_set_cursor(0,0);
    lcd_print("ASWIN KUMAR A");

    lcd_set_cursor(1,0);
    lcd_print("SSN COLLEGE");

    while(1);
}
