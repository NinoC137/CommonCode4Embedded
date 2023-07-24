#ifndef __SG90_H
#define __SG90_H

//include
#include "main.h"
#include "cmsis_os.h"

#define SG90_FINISH 1
#define SG90_RUNNING 0
//extern

//SG90 OOP_By_C
typedef struct SG90_OOC {
    //elements
    char *name;
    float prTarget_Angle;
    float prHighLevelTimes_Ms;

    TIM_HandleTypeDef* TIMER;
    uint32_t TIM_Channel;

    //functions
    float (*GetAngle2Pulse)(struct SG90_OOC *);
    void (*SetAngle)(struct SG90_OOC *, float target_Angle);
    void (*Move2TargetAngle)(struct SG90_OOC *);
} SG90;

//Create an Oriented
SG90 SG90_Create();

//OOP function define
float getAngle2Pulse(struct SG90_OOC * SG90);
void setAngle(struct SG90_OOC * SG90, float target_Angle);
void move2TargetAngle(struct SG90_OOC* SG90);

//Application functions
void SG90_init();
char* Click_Once(struct SG90_OOC * aSG90);
char* Click_Num(struct SG90_OOC * aSG90, uint8_t Num);

extern SG90 clickBox_1;
extern SG90 clickBox_2;
extern SG90 clickBox_3;
#endif