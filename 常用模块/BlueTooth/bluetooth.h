#ifndef __BLUETOOTH_H_
#define __BLUETOOTH_H_

#include <stdint.h>

#define USART_TX_LEN 55
#define USART_RX_LEN 55


typedef struct{
    short sint_1;
    short sint_2;
    short sint_3;
    short sint_4;
    float float_1;
    float float_2;
    float float_3;
    float float_4;
    float float_5;
    float float_6;
    float float_7;
    float float_8;
    float float_9;
    float float_10;
    float float_11;

}BlueTooth;

extern void Buletooth_Debugger(void);

void Blue_Transmit(BlueTooth *Tr_inter);
void Blue_Receive(BlueTooth *Re_inter);

void Int_to_Byte(int i,unsigned char *byte);
void Float_to_Byte(float f,unsigned char *byte);
void Short_to_Byte(short s,unsigned char *byte);

float hex_float(uint8_t *a);
short int hex_sint(uint8_t *a);

#endif
