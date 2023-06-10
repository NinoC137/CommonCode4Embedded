#include "Motor.h"

extern TIM_HandleTypeDef htim4; //马达PWM
extern TIM_HandleTypeDef htim1; //编码器计数

Motor Motor_Left;

Motor Motor_Carrier_1;
Motor Motor_Carrier_2;

uint8_t prGetSpeed(const struct MOTOR *aMotor){
    
    return aMotor->prRealSpeed;
}

uint8_t prSetSpeed(struct MOTOR *aMotor, uint8_t TargetSpeed){
    aMotor->prTargetSpeed = TargetSpeed;

    return aMotor->prTargetSpeed;
}

char* prMotorEnable(const struct MOTOR *aMotor, uint8_t Pulse, uint8_t Enable){
    static uint32_t pulse2compare;

    pulse2compare = (TIM4_RELOADVALUE/100) * Pulse;

    if(Enable == 1)
    {
        HAL_TIM_PWM_Start(aMotor->TIMER,aMotor->TIM_Channel);
        __HAL_TIM_SetCompare(aMotor->TIMER, aMotor->TIM_Channel, pulse2compare);
    }
    else
    {
        HAL_TIM_PWM_Stop(aMotor->TIMER,aMotor->TIM_Channel);
    }

    switch (aMotor->prDirect) {
        case FORWARD:
            HAL_GPIO_WritePin(aMotor->GPIO_1.GPIOx,aMotor->GPIO_1.GPIO_Pin,1);
            HAL_GPIO_WritePin(aMotor->GPIO_2.GPIOx,aMotor->GPIO_2.GPIO_Pin,0);
            break;
        case BACK:
            HAL_GPIO_WritePin(aMotor->GPIO_1.GPIOx,aMotor->GPIO_1.GPIO_Pin,0);
            HAL_GPIO_WritePin(aMotor->GPIO_2.GPIOx,aMotor->GPIO_2.GPIO_Pin,1);
            break;
        default:
            printf("ERROR MOTOR DIRECT!\r\n");
            HAL_GPIO_WritePin(aMotor->GPIO_1.GPIOx,aMotor->GPIO_1.GPIO_Pin,0);
            HAL_GPIO_WritePin(aMotor->GPIO_2.GPIOx,aMotor->GPIO_2.GPIO_Pin,0);
            break;
    }
}

Motor Motor_Create(){
    Motor tMotor;

    tMotor.GetSpeed = prGetSpeed;
    tMotor.SetSpeed = prSetSpeed;
    tMotor.MotorEnable = prMotorEnable;

    tMotor.prDirect = FORWARD;
    tMotor.prTargetSpeed = 0;
    tMotor.prRealSpeed = 0;
    tMotor.loopCounter = 0;

    return tMotor;
}

void Motor_Init(){
    Motor_Left = Motor_Create();

    Motor_Carrier_1 = Motor_Create();
    Motor_Carrier_2 = Motor_Create();

    Motor_Left.name = "Left";

    Motor_Left.TIMER = &htim4;
    Motor_Carrier_1.TIMER = &htim4;
    Motor_Carrier_2.TIMER = &htim4;

    Motor_Left.TIM_Channel = TIM_CHANNEL_1;
    Motor_Carrier_1.TIM_Channel = TIM_CHANNEL_2;
    Motor_Carrier_2.TIM_Channel = TIM_CHANNEL_3;

    Motor_Carrier_1.GPIO_1.GPIOx = IN3_GPIO_Port;
    Motor_Carrier_1.GPIO_2.GPIOx = IN4_GPIO_Port;
    Motor_Carrier_1.GPIO_1.GPIO_Pin = IN3_Pin;
    Motor_Carrier_1.GPIO_2.GPIO_Pin = IN4_Pin;

    Motor_Carrier_2.GPIO_1.GPIOx = IN1_GPIO_Port;
    Motor_Carrier_2.GPIO_2.GPIOx = IN2_GPIO_Port;
    Motor_Carrier_2.GPIO_1.GPIO_Pin = IN1_Pin;
    Motor_Carrier_2.GPIO_2.GPIO_Pin = IN2_Pin;
    //镜像两电机的基本配置
    Motor_Left.GPIO_1 = Motor_Carrier_2.GPIO_1;
    Motor_Left.GPIO_2 = Motor_Carrier_2.GPIO_2;

    Motor_Carrier_1.name = "First_Carrier";
    Motor_Carrier_2.name = "Second_Carrier";

    __HAL_TIM_SET_COUNTER(&htim1, 10000);
}