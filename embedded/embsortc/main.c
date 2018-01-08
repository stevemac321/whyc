#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

const int NUM_ELEMS = 100000;

static inline void dummy() {volatile int i = 9;}

static inline int cmp(const void *x, const void *y)
{
	return *(int *)x - *(int *)y;
}
int main()
{
        GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	int a[NUM_ELEMS];
        clock_t start, stop;

	srand(time(NULL));
	for (int i = 0; i < NUM_ELEMS; i++)
		a[i] = rand() % NUM_ELEMS - 100;

        start = clock();
	qsort(&a[0], NUM_ELEMS, sizeof(int), cmp);

	stop = clock();
        printf("%.4lf\n", (stop - start) / ((double) CLOCKS_PER_SEC));

        dummy();
        while(1)
                ;
}
/* override _write for semihosting*/
void PrintByte(char c)
{
  USART_SendData(USART2, c);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
          ; /* do nothing until RESET state*/
}

size_t __write(int handle, const unsigned char* data, size_t byteCount)
{
    size_t nChars = 0;

    // handle == -1 means "flush all streams". Not implemented here.
    if (handle == -1 || byteCount == 0)
        return 0;

    // 1 - stdout; 2 - stderr. 
    if (handle != 1 && handle != 2)
        return -1;

    while (byteCount-- > 0) {
        char ch = (char)(*data);
        PrintByte(ch);
        if (ch == '\n') {
            PrintByte('\r');
        }
       ++data;
       ++nChars;
    }
    return nChars;

} // __write

