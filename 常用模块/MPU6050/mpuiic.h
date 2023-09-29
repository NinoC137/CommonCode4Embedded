#ifndef _SOFTWAREIIC_H_
#define _SOFTWAREIIC_H_

#include "cmsis_os.h"
#include "main.h"

typedef enum{
    NACK = 0,
    ACK  = 1
}ACK_STATUS;

#define IIC_DELAY_TIME 10

// #define IIC_SDA_H() GPIOB->BSRR = 0x010
// #define IIC_SDA_L() GPIOB->BRR = 0x010
// #define IIC_SCL_H() GPIOB->BSRR = 0x100
// #define IIC_SCL_L() GPIOB->BRR = 0x100

#define IIC_SCL_High()     HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_SET)
#define IIC_SCL_Low()     HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_RESET)
#define IIC_SDA_High()     HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_SET)
#define IIC_SDA_Low()     HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_RESET)
#define READ_SDA    HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin)

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_NAck(void);
uint8_t IIC_Wait_Ack(void);
void IIC_Send_Byte(uint8_t Byte);
uint8_t IIC_Read_Byte(unsigned char ack);

uint8_t I2C_BeginTransmission(uint8_t address);

void delay_us(uint32_t us);

#endif

