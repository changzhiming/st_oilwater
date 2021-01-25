#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdint.h>
#include "mb.h"
#include "mbutils.h"
#ifdef __cplusplus
 extern "C" {
#endif

void RS485_Send(uint8_t * send_buf, uint16_t size);
void RS485_Recive(uint8_t ** recive_buf, uint8_t *size);

void Soft_Reset(void);

void WWDG_Refresh(void);
void PLATFORM_Init(void );   //系统初始化

//uart
void uartPoll(void);

#ifdef __cplusplus
}
#endif

#endif /* __PLATFORM_H__ */
