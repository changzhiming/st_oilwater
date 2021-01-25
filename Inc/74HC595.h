#ifndef __74HC595_H__
#define __74HC595_H__
#include "main.h"

#define HC595_CLK_H()           HAL_GPIO_WritePin(R595_SCK_GPIO_Port, R595_SCK_Pin, GPIO_PIN_SET);
#define HC595_CLK_L()		        HAL_GPIO_WritePin(R595_SCK_GPIO_Port, R595_SCK_Pin, GPIO_PIN_RESET);

#define HC595_CS_H()            HAL_GPIO_WritePin(R595_RCK_GPIO_Port, R595_RCK_Pin, GPIO_PIN_SET);
#define HC595_CS_L()		        HAL_GPIO_WritePin(R595_RCK_GPIO_Port, R595_RCK_Pin, GPIO_PIN_RESET);

#define HC595_DATA_H()          HAL_GPIO_WritePin(R595_DATA_GPIO_Port, R595_DATA_Pin, GPIO_PIN_SET);
#define HC595_DATA_L()		      HAL_GPIO_WritePin(R595_DATA_GPIO_Port, R595_DATA_Pin, GPIO_PIN_RESET);

void HC595Send(uint16_t data[], int length);
void HC595Init(void);
#endif
