#ifndef __MOTOR_H
#define __MOTOR_H

#include "cmsis_os.h"
#include "my_PID.h"
//#include "timA.h"
//#include <driverlib.h>
//#include "msp.h"

#define FORWARD 1
#define BACK 0

#define RR 317

typedef struct Direct_Control_GPIO{
    GPIO_TypeDef* GPIOx_1;
    uint32_t GPIO_Pin_1;
    GPIO_TypeDef* GPIOx_2;
    uint32_t GPIO_Pin_2;
}Direct_GPIO;

//Motor OOP struct define
typedef struct MOTOR{
    char* name;
    TIM_HandleTypeDef* TIMER;
    uint32_t Channel;

    Direct_GPIO DirectIO_1;
    Direct_GPIO DirectIO_2;

    uint8_t prDirect;
    float prTargetSpeed;
    float prRealSpeed;

    PID PID_param;

    float (*GetSpeed)(const struct MOTOR *aMotor);
    float (*SetSpeed)(struct MOTOR *aMotor, uint8_t TargetSpeed);

    char* (*MotorEnable)(struct MOTOR *aMotor, int8_t Pulse, uint8_t Enable);

}Motor;

//OOP prAPI Functions
float prGetSpeed(const struct MOTOR *aMotor);
float prSetSpeed(struct MOTOR *aMotor, uint8_t TargetSpeed);

char* prMotorEnable(struct MOTOR *aMotor, int8_t Pulse, uint8_t Enable);

//OOP functions define
Motor Motor_Create(char* name, TIM_HandleTypeDef* TIMER, uint32_t Channel, Direct_GPIO DirectIO);
void Motor_Init(void);

//private oriented
extern Motor Motor_Left;
extern Motor Motor_Right;

#endif

