#include "platform.h"
#include "main.h"
#include "string.h"
#include "math.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "mbcrc.h"
#include "stdlib.h"
#include "math.h"
#include "74HC595.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

static uint32_t wwdg_time = 0;
static uint16_t ADC_Value[800] = {0};

static unsigned short usRegHoldingBuf[48]={0};

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

#define huart485  huart3
#define USART485  USART3
//485
static uint32_t lastTick = 0;         //时间检测
static uint32_t buf_size = 0;         //数据包位置
static uint8_t  aucRtuBuf[256];       //透传Modbus rtu buf
static uint8_t  temp_value = 0;

static u8 DA_OUT(u8 ch,u8 rng,u8 data);

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 10);
  return ch;
}

void RS485_Send(uint8_t * send_buf, uint16_t size)
{
	HAL_UART_Transmit(&huart485, send_buf, size, 500);
}

void RS485_Recive(uint8_t ** recive_buf, uint8_t *size)
{
	if(((HAL_GetTick() - lastTick) >= 4) && buf_size >= 2)
	{
		*size = buf_size;
		*recive_buf = &aucRtuBuf[0];
		buf_size = 0;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART485)
	{
		if(((HAL_GetTick() - lastTick) > 5 && buf_size >= 2) || buf_size > 255)
		{
			buf_size = 0;
		}
		lastTick = HAL_GetTick();
		aucRtuBuf[buf_size++] = temp_value;
		
		HAL_UART_Receive_IT(&huart485, &temp_value, 1);
	}
}


//更新输出状态 0.00精度  24个
static void updateRegHoldingCB()
{
	int index = 24;
	HAL_GPIO_WritePin(D_OUT1_GPIO_Port,  D_OUT1_Pin, (GPIO_PinState)(usRegHoldingBuf[index++] & 0x01));
	HAL_GPIO_WritePin(D_OUT2_GPIO_Port,  D_OUT2_Pin, (GPIO_PinState)(usRegHoldingBuf[index++] & 0x01));
	HAL_GPIO_WritePin(D_OUT3_GPIO_Port,  D_OUT3_Pin, (GPIO_PinState)(usRegHoldingBuf[index++] & 0x01));
	HAL_GPIO_WritePin(D_OUT4_GPIO_Port,  D_OUT4_Pin, (GPIO_PinState)(usRegHoldingBuf[index++] & 0x01));
	
	HAL_GPIO_WritePin(D_OUT5_GPIO_Port,  D_OUT5_Pin, (GPIO_PinState)(usRegHoldingBuf[index++] & 0x01));
	HAL_GPIO_WritePin(D_OUT6_GPIO_Port,  D_OUT6_Pin, (GPIO_PinState)(usRegHoldingBuf[index++] & 0x01));
	HAL_GPIO_WritePin(D_OUT7_GPIO_Port,  D_OUT7_Pin, (GPIO_PinState)(usRegHoldingBuf[index++] & 0x01));
	HAL_GPIO_WritePin(D_OUT8_GPIO_Port,  D_OUT8_Pin, (GPIO_PinState)(usRegHoldingBuf[index++] & 0x01));
	
	for(; index < 36; index++)
	{
		DA_OUT(index - 32 , 2, usRegHoldingBuf[index] / 100.0 * 25);
	}
}
//更新输入状态 0.00精度  24个
static void updateRegInputCB()
{
	int index = 0;
	usRegHoldingBuf[index++]= HAL_GPIO_ReadPin(DI_IN0_GPIO_Port, DI_IN0_Pin) ? 0 : 1;
	usRegHoldingBuf[index++]= HAL_GPIO_ReadPin(DI_IN1_GPIO_Port, DI_IN1_Pin) ? 0 : 1;
	usRegHoldingBuf[index++]= HAL_GPIO_ReadPin(DI_IN2_GPIO_Port, DI_IN2_Pin) ? 0 : 1;
	usRegHoldingBuf[index++]= HAL_GPIO_ReadPin(DI_IN3_GPIO_Port, DI_IN3_Pin) ? 0 : 1;
	usRegHoldingBuf[index++]= HAL_GPIO_ReadPin(DI_IN4_GPIO_Port, DI_IN4_Pin) ? 0 : 1;
	usRegHoldingBuf[index++]= HAL_GPIO_ReadPin(DI_IN5_GPIO_Port, DI_IN5_Pin) ? 0 : 1;
	usRegHoldingBuf[index++]= HAL_GPIO_ReadPin(DI_IN6_GPIO_Port, DI_IN6_Pin) ? 0 : 1;
	usRegHoldingBuf[index++]= HAL_GPIO_ReadPin(DI_IN7_GPIO_Port, DI_IN7_Pin) ? 0 : 1;
	
	volatile int adc_value = 0;
	int i, size;
	for(i = index; i < 16; i++)
	{
		adc_value = 0;
		for(size = 0; size < 100; size++)
		{
			adc_value = adc_value + ADC_Value[size * 8 + i - 8];
		}
		
		if(i == 15) {
			usRegHoldingBuf[i] = (-40 + 186.20 * (adc_value * 3.3 / 4096) / 100.0) * 100;
			continue;
		}
		
		usRegHoldingBuf[i] = (adc_value * 3.3 / 4096)* 3.0769;
	}

	//0->开关量， 1->电压， 2->电流 3->
	for(i = index; i < 16; i++)
	{
		switch(usRegHoldingBuf[40 + i - index])
		{
			case 0:
				usRegHoldingBuf[i] = usRegHoldingBuf[i] > 500 ? 0 : 1;
				break;
			case 1:
				usRegHoldingBuf[i] = usRegHoldingBuf[i];
				break;
			case 2:
				usRegHoldingBuf[i] = usRegHoldingBuf[i] * 2;
				break;
			case 3:
				usRegHoldingBuf[i] = usRegHoldingBuf[i] > 900 ? 0 : 1000 * 20 * usRegHoldingBuf[i] / 100 / (29.2 - 3 * usRegHoldingBuf[i] / 100);    //   100 * (100 * usRegHoldingBuf[i] / 100 - 144.5) / (14.45 - 20 * usRegHoldingBuf[i] / 100); //;usRegHoldingBuf[i] > 500 ? 0 : 10.0 * usRegHoldingBuf[i] / (14.45 - usRegHoldingBuf[i] / 100.0);
				break;
		}
	}
}


void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
	if(wwdg_time <= 200)	{
		HAL_WWDG_Refresh(hwwdg);
		wwdg_time++;
	}
}

void WWDG_Refresh(void)
{
	wwdg_time = 0;
}

void portConfig(void )
{
	static uint16_t switchInModel[20] = {0};
	//端口初始化
	int index = 0;
	
	usRegHoldingBuf[45] = 2;
	//usRegHoldingBuf[46] = 1;
	usRegHoldingBuf[47] = 1;
	
	for(int i = 40; i < 48; i++)
	{
		switchInModel[index * 2] = 0;
		switchInModel[index * 2 + 1] = 0;
		
		if(usRegHoldingBuf[i] == 0 || usRegHoldingBuf[i] == 3) 
			switchInModel[index * 2] = 1;
		if(usRegHoldingBuf[i] == 2) 
			switchInModel[index * 2 + 1] = 1;
		
		index++;
	}
	HC595Send(switchInModel, 16);
}

void PLATFORM_Init(void )
{
	lastTick = HAL_GetTick();
	HAL_UART_Receive_IT(&huart485, &temp_value, 1);        //开启485
	
	portConfig();   //根据配置文件初始化采集类型
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&ADC_Value, 800);
	
	//set port 35 = 1000
	usRegHoldingBuf[35] = 1000; 
	updateRegHoldingCB();
}

void uartPoll(void)
{
		static uint8_t *frame;
		static uint8_t  length;
	
		length = 0;
		
		RS485_Recive(&frame, &length);
		//read
		if(length >= 2 && frame[0] == 0x03 && frame[1] <= 48) {
				updateRegInputCB();
				RS485_Send((uint8_t *)usRegHoldingBuf, frame[1] * 2);
		}		
		//write
		if(length >= 4 && frame[0] == 0x04 && frame[1] < 48) {
			usRegHoldingBuf[frame[1]] = *(uint16_t *)&frame[2];
			
			updateRegHoldingCB();
		}
}

#define TLC5620_SCLK(SET) HAL_GPIO_WritePin(DAC_CLK_GPIO_Port, DAC_CLK_Pin, (GPIO_PinState)(SET))	  // TLC5620时钟管脚
#define TLC5620_SDAA(SET) HAL_GPIO_WritePin(DAC_DATA_GPIO_Port, DAC_DATA_Pin, (GPIO_PinState)(SET))		// TLC5620数据管教
#define TLC5620_LOAD(SET) HAL_GPIO_WritePin(DAC_LOAD_GPIO_Port, DAC_LOAD_Pin, (GPIO_PinState)(SET))	  // TLC5620 LOAD管教
#define TLC5620_LDAC(SET) HAL_GPIO_WritePin(DAC_LD_GPIO_Port, DAC_LD_Pin, (GPIO_PinState)(SET))	// TLC5620 LDAC管教

#define TLC5620_OUTA 0x0000
#define TLC5620_OUTB 0x4000
#define TLC5620_OUTC 0x8000
#define TLC5620_OUTD 0xc000

#define TLC5620_OUT_one    0x0000
#define TLC5620_OUT_double 0x2000

static void InitTLC5620(void)
{
	TLC5620_SCLK(0);
	TLC5620_SDAA(0);
	TLC5620_LOAD(1);
	TLC5620_LOAD(1);
}
//TLC 5620电压输出
//
static void  TLC5620_Conversion (u16 tlc5620_data)
{
    u8   m=0;
    u16  n;
    for(; m<0x0b; m++)
    {
        TLC5620_SCLK(1);
        n=tlc5620_data;
        n=n&0x8000;
        if(n==0x8000)
            TLC5620_SDAA(1);
        else
            TLC5620_SDAA(0);
        TLC5620_SCLK(0);
        tlc5620_data<<=1;
    }
    TLC5620_LOAD(0);
    TLC5620_LOAD(1);
    TLC5620_LDAC(0);
    TLC5620_LDAC(1);
}
//ch = 0 TLC5620_OUTA;1 TLC5620_OUTB;2 TLC5620_OUTC;3 TLC5620_OUTD;other err;
//rng= 0 TLC5620_OUT_one;other TLC5620_OUT_double;
//VO(DACA|B|C|D)+REF*(CODE/256)*(1+RNG)
static u8 DA_OUT(u8 ch,u8 rng,u8 data)
{
    u16 out_data=0;
    
    out_data |= data;
    out_data <<= 5;
    if(ch == 0){
        out_data |= TLC5620_OUTA;
    }else if(ch == 1){
        out_data |= TLC5620_OUTB;
    }else if(ch == 2){
        out_data |= TLC5620_OUTC;
    }else if(ch == 3){
        out_data |= TLC5620_OUTD;
    }else{
        return 0;
    }
    
    if(rng == 1){
        out_data |= TLC5620_OUT_one;
    }else if(rng == 2){
        out_data |= TLC5620_OUT_double;
    }else{
        out_data |= TLC5620_OUT_one;
    }
    
    TLC5620_Conversion(out_data);
    return 1;
}















