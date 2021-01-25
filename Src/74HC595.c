
#include "74HC595.h"

static void delay()
{
	__NOP();
	__NOP();
  __NOP();
	__NOP();
}

static void HC595Load(void)
{
	HC595_CS_L();
  delay();
	HC595_CS_H();
}
	

void HC595Init(void)
{
	HC595_CLK_H();
	HC595_DATA_H();
	HC595_CS_H();
	
}
void HC595Send(uint16_t data[], int length)
{
  while(length--) {
    if(data[length] == 0) {
			HC595_DATA_L();
		} else {
			HC595_DATA_H();
		}

		HC595_CLK_L();
    delay();
    HC595_CLK_H();
    delay();
  }

	HC595Load();
}


