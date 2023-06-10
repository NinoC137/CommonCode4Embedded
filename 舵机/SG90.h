#ifndef __SG90_H
#define __SG90_H

//include
#include "stm32f1xx_hal.h"
#include "main.h"
#include "cmsis_os.h"


//extern

//SG90 OOP_By_C
typedef struct SG90_OOC {
    //elements
    char *name;
    uint8_t prTarget_Angle;
    uint8_t prHighLevelTimes_Ms;

    TIM_HandleTypeDef* TIMER;
    uint32_t TIM_Channel;

    //functions
    int (*GetAngle2Pulse)(struct SG90_OOC *);
    void (*SetAngle)(struct SG90_OOC *, int target_Angle);
    void (*Move2TargetAngle)(struct SG90_OOC *);
} SG90;

//Create an Oriented
SG90 SG90_Create();

//OOP function define
int getAngle2Pulse(struct SG90_OOC * SG90);
void setAngle(struct SG90_OOC * SG90, int target_Angle);
void move2TargetAngle(struct SG90_OOC* SG90);

//Application functions
void SG90_init();
char* Click_Once(struct SG90_OOC * aSG90);
char* Click_Num(struct SG90_OOC * aSG90, uint8_t Num);

extern SG90 clickBox_1;
extern SG90 clickBox_2;
extern SG90 clickBox_3;
#endif