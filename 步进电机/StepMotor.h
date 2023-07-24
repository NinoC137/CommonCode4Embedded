#ifndef __STEP_H
#define __STEP_H

#include "main.h"
#include "cmsis_os.h"
#include "stdlib.h"

//motor enable
#define ENABLE 1
#define DISABLE 0
//motor direct
#define FORWARD 1
#define BACK	0

//Sub_Box Position ID
//这部分的数值改为各个位置相对于原点的圈数
//如: ID_BOX1 20 代表从原点走到BOX1需要20圈
#define ID_INIT 46
#define ID_BOX1 0
#define ID_BOX2 9
#define ID_BOX3 18
#define ID_OUT  46
//SM_For_littleCup Position ID
#define Lit_INT 0
#define Lit_OUT 8
typedef struct StepMotor_OOP{
    //elements
    char* name;
    TIM_HandleTypeDef* TIMER;
    uint32_t Channel;

    uint32_t CurrentPlace;
    uint32_t TargetPlace;

    //API functions
    char* (*Reset)(struct StepMotor_OOP *);                             //Reset to default place
    char* (*Step2PointPlace)(struct StepMotor_OOP *, uint32_t PlaceName);  //Step to NamedPlace
}StepMotor;

//Create an Oriented
StepMotor StepMotor_Create(void);

//OOP function define
char* prReset(StepMotor * SM);
char* prStep2PointPlace(StepMotor * SM, uint32_t PlaceName);

//Application functions
void StepMotor_init(void);
char* Step_OnePeriod(const StepMotor * aStepMotor, uint8_t direct);
char* Step_KeepMoving(const StepMotor * aStepMotor, uint8_t direct, uint8_t Enable);

extern StepMotor SM_For_SubBox;
extern StepMotor SM_For_littleCup;
extern int SubBox_Buffer[4];


#endif


