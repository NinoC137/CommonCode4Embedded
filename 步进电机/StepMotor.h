#ifndef __STEP_H
#define __STEP_H

#include "stm32f1xx_hal.h"
#include "main.h"
#include "cmsis_os.h"

//motor enable
#define ENABLE 1
#define DISABLE 0

//motor direct
#define FORWARD 1
#define BACK	0

typedef struct StepMotor_OOP{
    //elements
    char* name;
    TIM_HandleTypeDef* TIMER;
    uint32_t Channel;

    uint8_t Current_steps;
    uint8_t Target_steps;

    //API functions
    char* (*Reset)(struct StepMotor_OOP *);                             //Reset to default place
    char* (*Step2PointPlace)(struct StepMotor_OOP *, char* PlaceName);  //Step to NamedPlace
}StepMotor;

//Create an Oriented
StepMotor StepMotor_Create(void);

//OOP function define
char* prReset(StepMotor * SM);
char* prStep2PointPlace(StepMotor * SM, char* PlaceName);

//Application functions
void StepMotor_init(void);
char* Step_OnePeriod(const StepMotor * aStepMotor);
char* Step_KeepMoving(const StepMotor * aStepMotor, uint8_t direct, uint8_t Enable);

#endif


