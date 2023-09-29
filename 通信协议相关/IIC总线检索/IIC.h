#ifndef __IIC_H
#define __IIC_H
#include "driverlib.h"
#include "cmsis_os.h"
//#include "cmsis_os2.h"
#include "common.h"

#define SDA_PORT	GPIO_PORT_P6
#define SCL_PORT	GPIO_PORT_P6
#define SDA_PIN		GPIO_PIN4
#define SCL_PIN		GPIO_PIN5
	   		   
//IO方向设置
#define SDA_IN()  GPIO_setAsInputPin(GPIO_PORT_P6,GPIO_PIN4)	//SDA输入模式
#define SDA_OUT() GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN4) //SDA输出模式
//IO操作函数	 
#define IIC_SCL_High()  GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN5) //SCL_High
#define IIC_SCL_Low()   GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN5) //SCL_Low
#define IIC_SDA_High()  GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN4) //SDA_High
#define IIC_SDA_Low()   GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN4) //SDA_Low
#define READ_SDA        GPIO_getInputPinValue(GPIO_PORT_P6,GPIO_PIN4)  //输入SDA 


//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
 
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 

void delay_us(uint32_t us);

uint8_t IIC_Write_Byte(uint8_t device_addr,uint8_t register_addr,uint8_t data);
uint8_t I2C_BeginTransmission(uint8_t address); 

#endif
















