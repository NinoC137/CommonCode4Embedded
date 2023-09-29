//myiic.c��Ҫ����
#include "IIC.h"
 
//��ʼ��IIC
void IIC_Init(void)
{			
    GPIO_setAsOutputPin(SCL_PORT,SCL_PIN); //CLK         
    GPIO_setAsOutputPin(SDA_PORT,SDA_PIN);	//DIN
	IIC_SCL_High();
	IIC_SDA_High();
}
//����IIC��ʼ�ź�
void IIC_Start(void)//SDA 10 SCL 010
{
	xSemaphoreTake(IICMutexHanlde, portMAX_DELAY);
	SDA_OUT();     //sda�����
	IIC_SCL_High();
	IIC_SDA_High();
	delay_us(4);
 	IIC_SDA_Low();//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_Low();//ǯסI2C���ߣ�׼�����ͻ�������� 
	xSemaphoreGive(IICMutexHanlde);
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)//SDA 01 SCL 01
{
	SDA_OUT();//sda�����
	IIC_SCL_Low();//STOP:when CLK is high DATA change form low to high
    IIC_SDA_Low();
 	delay_us(4);
    IIC_SCL_High();
	IIC_SDA_High();//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)//
{
	uint8_t cy;
    SDA_IN();      //SDA����Ϊ����
    IIC_SCL_High();delay_us(10);
	IIC_SDA_High();delay_us(10);
    if(READ_SDA)
    {
        cy=1;
        IIC_SCL_Low();
        return cy; 
    }      
    else
    {
        cy=0;
    }             
	IIC_SCL_Low();//ʱ�����0
	return cy;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
    IIC_SCL_Low();
	SDA_OUT();
    IIC_SDA_Low();
	delay_us(2);
    IIC_SCL_High();
    delay_us(2);
	IIC_SCL_Low();
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL_Low();
	SDA_OUT();
    IIC_SDA_High();
	delay_us(2);
    IIC_SCL_High();
    delay_us(2);
	IIC_SCL_Low();
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{
	xSemaphoreTake(IICMutexHanlde, portMAX_DELAY);
    uint8_t t;   
	SDA_OUT(); 
    IIC_SCL_Low();//����ʱ�ӿ�ʼ���ݴ���
    delay_us(2);
    for(t=0;t<8;t++)
    {       
        if(txd&0x80)
        {
            IIC_SDA_High();delay_us(2); 
        }  
        else
        {
            IIC_SDA_Low();delay_us(2);  
        }
        txd<<=1;
		IIC_SCL_High();
		delay_us(4); 
        IIC_SCL_Low();
        delay_us(2); 
    }
        delay_us(2);
	xSemaphoreGive(IICMutexHanlde);
    
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	xSemaphoreTake(IICMutexHanlde, portMAX_DELAY);
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
		IIC_SCL_Low();
        delay_us(2);
        IIC_SCL_High();
        receive<<=1;
        if(READ_SDA)
            receive++;
        delay_us(2);
    }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
	xSemaphoreGive(IICMutexHanlde);
    return receive;
}

void delay_us(uint32_t us)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD;
	
	portENTER_CRITICAL();
	ticks = us* 48;	//48M��Ƶ�´�����ʱ
	told = SysTick->VAL;
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow != told)
		{
			if(tnow < told) tcnt += told - tnow;
			else tcnt += reload - tnow + told;
			told = tnow;
			if(tcnt >= ticks) break;
		}
	};
	portEXIT_CRITICAL();
}

uint8_t IIC_Write_Byte(uint8_t device_addr,uint8_t register_addr,uint8_t data)
{
    xSemaphoreTake(IICMutexHanlde, portMAX_DELAY);
    uint8_t ack;
	
	ack = IIC_Wait_Ack();
	
    IIC_Start();
	ack = IIC_Wait_Ack();
    IIC_Send_Byte(device_addr+0);
	ack = IIC_Wait_Ack();
    if (ack == 1)return 0;
    IIC_Send_Byte(register_addr);
	ack = IIC_Wait_Ack();
    if (ack == 1)return 0;
    IIC_Send_Byte(data);
	ack = IIC_Wait_Ack();
    if (ack == 1)return 0;
    IIC_Stop();
	
	xSemaphoreGive(IICMutexHanlde);
    return 1;
}

uint8_t I2C_BeginTransmission(uint8_t address)
{
	IIC_Start(); 
	IIC_Send_Byte((address << 1) | 0); 
	
	 if (IIC_Wait_Ack() == 0) {
         // Ӧ��ɹ�
		 IIC_Stop();
		 return 0;
     } else {
         // Ӧ��ʧ��
		 IIC_Stop();
		 return 1;
     }
}
