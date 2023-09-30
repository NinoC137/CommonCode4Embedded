#include "Speech.h"

enum MyEnum
{
  NOTHING = 0,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  HUNDRED,
  THOUSAND,
};

unsigned char speechNumbers[13][8] = {
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xC1, 0xE3, 0xDB}, // ZERO
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xD2, 0xBB, 0x90}, // ONE
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xB6, 0xFE, 0xB1}, // TWO
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xC8, 0xFD, 0xCC}, // THREE
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xCB, 0xC4, 0xF6}, // FOUR
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xCE, 0xE5, 0xD2}, // FIVE
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xC1, 0xF9, 0xC1}, // SIX
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xC6, 0xDF, 0xE0}, // SEVEN
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xB0, 0xCB, 0x82}, // EIGHT
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xBE, 0xC5, 0x82}, // NINE
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xCA, 0xAE, 0x9D}, // TEN
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xB0, 0xD9, 0x90}, // HUNDRED
    {0xFD, 0x00, 0x05, 0x01, 0x00, 0xC7, 0xA7, 0x99}  // THOUSAND
};

void speech()
{
  unsigned char i = 0;
  unsigned char head[20];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x11;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0xCE;
  head[6] = 0xDE;
  head[7] = 0xBC;
  head[8] = 0xAB;
  head[9] = 0xB7;
  head[10] = 0xE7;
  head[11] = 0xBB;
  head[12] = 0xFA;
  head[13] = 0xD2;
  head[14] = 0xD1;
  head[15] = 0xBE;
  head[16] = 0xCD;
  head[17] = 0xD0;
  head[18] = 0xF7;
  head[19] = 0xAC;

  for (i = 0; i < 20; i++)
  {
    Serial1.write(head[i]);
  }
}

void start1()
{
  unsigned char i = 0;
  unsigned char head[16];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x0D;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0xB5;
  head[6] = 0xB1;
  head[7] = 0xC7;
  head[8] = 0xB0;
  head[9] = 0xB9;
  head[10] = 0xA6;
  head[11] = 0xC2;
  head[12] = 0xCA;
  head[13] = 0xCE;
  head[14] = 0xAA;
  head[15] = 0xF1;

  for (i = 0; i < 16; i++)
  {
    Serial1.write(head[i]);
  }
}

void start2()
{
  unsigned char i = 0;
  unsigned char head[12];

  head[0] = 0xFD;
  head[1] = 0x00;
  head[2] = 0x09;
  head[3] = 0x01;
  head[4] = 0x00;
  head[5] = 0xCE;
  head[6] = 0xC2;
  head[7] = 0xB6;
  head[8] = 0xC8;
  head[9] = 0xCE;
  head[10] = 0xAA;
  head[11] = 0xE3;

  for (i = 0; i < 12; i++)
  {
    Serial1.write(head[i]);
  }
}

void PowerSpeech(int power)
{
  int thousands, hundreds, tens, ones; // 各位数值
  thousands = power / 1000;
  power %= 1000;
  hundreds = power / 100;
  power %= 100;
  tens = power / 10;
  ones = power % 10;

  unsigned char *thousandsArray = speechNumbers[thousands];
  unsigned char *hundredsArray = speechNumbers[hundreds];
  unsigned char *tensArray = speechNumbers[tens];
  unsigned char *onesArray = speechNumbers[ones];

  for (int i = 1; i <= 4; i++)
  {
    for (int t = 0; t < 8; t++)
    {
      switch (i)
      {
      case 1:
        Serial1.write(thousandsArray[t]);
        break;
      case 2:
        Serial1.write(hundredsArray[t]);
        break;
      case 3:
        Serial1.write(tensArray[t]);
        break;
      case 4:
        Serial1.write(onesArray[t]);
        break;
      default:
        break;
      }
    }
    delay(550);
    // 单位播报
    // if (thousands != 0)
    // {
    //   for (int m = 0; m < 8; m++)
    //   {
    //     Serial1.write(speechNumbers[13][m]);
    //   }
    //   thousands = 0;
    // }
    // if (hundreds != 0)
    // {
    //   for (int m = 0; m < 8; m++)
    //   {
    //     Serial1.write(speechNumbers[12][m]);
    //   }
    //   hundreds = 0;
    // }
    // if (tens != 0)
    // {
    //   for (int m = 0; m < 8; m++)
    //   {
    //     Serial1.write(speechNumbers[11][m]);
    //   }
    //   tens = 0;
    // }
  }
}

void TemperateSpeech(int temperate)
{
  static int hundreds, tens, ones; // 各位数值
  hundreds = temperate / 100;
  temperate %= 100;
  tens = temperate / 10;
  ones = temperate % 10;

  unsigned char *hundredsArray = speechNumbers[hundreds];
  unsigned char *tensArray = speechNumbers[tens];
  unsigned char *onesArray = speechNumbers[ones];

  for (int i = 1; i <= 3; i++)
  {
    for (int t = 0; t < 8; t++)
    {
      switch (i)
      {
      case 1:
        Serial1.write(hundredsArray[t]);
        break;
      case 2:
        Serial1.write(tensArray[t]);
        break;
      case 3:
        Serial1.write(onesArray[t]);
        break;
      default:
        break;
      }
    }
    delay(550);
    // 单位播报
    // if (hundreds != 0)
    // {
    //   for (int m = 0; m < 8; m++)
    //   {
    //     Serial1.write(speechNumbers[12][m]);
    //   }
    //   hundreds = 0;
    // }
    // if (tens != 0)
    // {
    //   for (int m = 0; m < 8; m++)
    //   {
    //     Serial1.write(speechNumbers[11][m]);
    //   }
    //   tens = 0;
    // }
  }
}