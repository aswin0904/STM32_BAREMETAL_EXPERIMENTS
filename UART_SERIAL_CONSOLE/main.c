#include <stdio.h>
#include "uart.h"
int main(void)
{
	int a=0;
/*Initialize debug UART*/
uart_init();
while(a!=1)
{
printf("Hello Scholar\r\n");
a++;
}
}
