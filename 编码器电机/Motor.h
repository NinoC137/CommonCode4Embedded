#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

#define FORWARD 1
#define BACK 0

#define RR 317u

#define TIM4_RELOADVALUE __HAL_TIM_GET_AUTORELOAD(&htim4)
#define TIM4_COUNTERNUM __HAL_TIM_GET_COUNTER(&htim4)

#define TIM1_RELOADVALUE __HAL_TIM_GET_AUTORELOAD(&htim1)
#define TIM1_COUNTERNUM __HAL_TIM_GET_COUNTER(&htim1)

typedef struct Direct_Control_GPIO{
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
}Direct_GPIO;

//Motor OOP struct define
typedef struct MOTOR{
    char* name;
    TIM_HandleTypeDef* TIMER;
    uint32_t TIM_Channel;

    Direct_GPIO GPIO_1;
    Direct_GPIO GPIO_2;

    uint8_t loopCounter;    //计数器溢出周期
    uint8_t prDirect;       //运转方向
    uint8_t prTargetSpeed;
    uint8_t prRealSpeed;

    uint8_t (*GetSpeed)(const struct MOTOR *aMotor);
    uint8_t (*SetSpeed)(struct MOTOR *aMotor, uint8_t TargetSpeed);

    char* (*MotorEnable)(const struct MOTOR *aMotor, uint8_t Pulse, uint8_t Enable);

}Motor;

//OOP prAPI Functions
uint8_t prGetSpeed(const struct MOTOR *aMotor);
uint8_t prSetSpeed(struct MOTOR *aMotor, uint8_t TargetSpeed);

char* prMotorEnable(const struct MOTOR *aMotor, uint8_t Pulse, uint8_t Enable);

//OOP functions define
Motor Motor_Create();
void Motor_Init();

//private oriented
extern Motor Motor_Left;
extern Motor Motor_Carrier_1;
extern Motor Motor_Carrier_2;

#endif