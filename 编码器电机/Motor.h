#ifndef __MOTOR_H
#define __MOTOR_H

#include "cmsis_os.h"
#include "timA.h"
#include <driverlib.h>
#include "msp.h"

#define FORWARD 1
#define BACK 0

#define RR 317

#define LEFT_TIMER	(PERIPH_BASE +0x00000400)
#define RIGHT_TIMER	(PERIPH_BASE +0x00000000)

typedef struct Direct_Control_GPIO{
    uint_fast8_t GPIOx;
    uint16_t GPIO_Pin;
}Direct_GPIO;

//Motor OOP struct define
typedef struct MOTOR{
    char* name;
    uint32_t TIMER;
    uint32_t TIM_Channel;

    Direct_GPIO GPIO_1;
    Direct_GPIO GPIO_2;

    uint8_t prDirect;       //��ת����
    float prTargetSpeed;
    float prRealSpeed;

    float (*GetSpeed)(const struct MOTOR *aMotor);
    float (*SetSpeed)(struct MOTOR *aMotor, uint8_t TargetSpeed);

    char* (*MotorEnable)(struct MOTOR *aMotor, int8_t Pulse, uint8_t Enable);

}Motor;

//OOP prAPI Functions
float prGetSpeed(const struct MOTOR *aMotor);
float prSetSpeed(struct MOTOR *aMotor, uint8_t TargetSpeed);

char* prMotorEnable(struct MOTOR *aMotor, int8_t Pulse, uint8_t Enable);

//OOP functions define
Motor Motor_Create(void);
void Motor_Init(void);

//private oriented
extern Motor Motor_Left;
extern Motor Motor_Right;

#endif

