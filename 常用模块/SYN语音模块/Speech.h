#ifndef __SPEECH_H
#define __SPEECH_H

#include "Arduino.h"

extern uint16_t Temp1;

void speech(); // �޼�����Ѿ���
void start1(); // ��ǰ����Ϊ
void start2(); // ��ǰ�¶�Ϊ
void PowerSpeech(int power);
void TemperateSpeech(int temperate);

// unsigned char one[8];

// one[0] = 0xFD;
// one[1] = 0x00;
// one[2] = 0x05;
// one[3] = 0x01;
// one[4] = 0x00;
// one[5] = 0xD2;
// one[6] = 0xBB;
// one[7] = 0x90;

// unsigned char two[8];

// two[0] = 0xFD;
// two[1] = 0x00;
// two[2] = 0x05;
// two[3] = 0x01;
// two[4] = 0x00;
// two[5] = 0xB6;
// two[6] = 0xFE;
// two[7] = 0xB1;

// unsigned char three[8];

// three[0] = 0xFD;
// three[1] = 0x00;
// three[2] = 0x05;
// three[3] = 0x01;
// three[4] = 0x00;
// three[5] = 0xC8;
// three[6] = 0xFD;
// three[7] = 0xCC;

// unsigned char four[8];

// four[0] = 0xFD;
// four[1] = 0x00;
// four[2] = 0x05;
// four[3] = 0x01;
// four[4] = 0x00;
// four[5] = 0xCB;
// four[6] = 0xC4;
// four[7] = 0xF6;

// unsigned char five[8];

// five[0] = 0xFD;
// five[1] = 0x00;
// five[2] = 0x05;
// five[3] = 0x01;
// five[4] = 0x00;
// five[5] = 0xCE;
// five[6] = 0xE5;
// five[7] = 0xD2;

// unsigned char six[8];

// six[0] = 0xFD;
// six[1] = 0x00;
// six[2] = 0x05;
// six[3] = 0x01;
// six[4] = 0x00;
// six[5] = 0xC1;
// six[6] = 0xF9;
// six[7] = 0xC1;

// unsigned char seven[8];

// seven[0] = 0xFD;
// seven[1] = 0x00;
// seven[2] = 0x05;
// seven[3] = 0x01;
// seven[4] = 0x00;
// seven[5] = 0xC6;
// seven[6] = 0xDF;
// seven[7] = 0xE0;

// unsigned char eight[8];

// eight[0] = 0xFD;
// eight[1] = 0x00;
// eight[2] = 0x05;
// eight[3] = 0x01;
// eight[4] = 0x00;
// eight[5] = 0xB0;
// eight[6] = 0xCB;
// eight[7] = 0x82;

// unsigned char nine[8];

// nine[0] = 0xFD;
// nine[1] = 0x00;
// nine[2] = 0x05;
// nine[3] = 0x01;
// nine[4] = 0x00;
// nine[5] = 0xBE;
// nine[6] = 0xC5;
// nine[7] = 0x82;

// unsigned char ten[8];

// ten[0] = 0xFD;
// ten[1] = 0x00;
// ten[2] = 0x05;
// ten[3] = 0x01;
// ten[4] = 0x00;
// ten[5] = 0xCA;
// ten[6] = 0xAE;
// ten[7] = 0x9D;

// unsigned char hundred[8];

// hundred[0] = 0xFD;
// hundred[1] = 0x00;
// hundred[2] = 0x05;
// hundred[3] = 0x01;
// hundred[4] = 0x00;
// hundred[5] = 0xB0;
// hundred[6] = 0xD9;
// hundred[7] = 0x90;

// unsigned char thousand[8];

// thousand[0] = 0xFD;
// thousand[1] = 0x00;
// thousand[2] = 0x05;
// thousand[3] = 0x01;
// thousand[4] = 0x00;
// thousand[5] = 0xC7;
// thousand[6] = 0xA7;
// thousand[7] = 0x99;


#endif