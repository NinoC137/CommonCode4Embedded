#include "Motor.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim1;

Motor Motor_Left;
Motor Motor_Right;

float prGetSpeed(const struct MOTOR *aMotor) {
    return aMotor->prRealSpeed;
}

float prSetSpeed(struct MOTOR *aMotor, uint8_t TargetSpeed) {
    aMotor->prTargetSpeed = TargetSpeed;

    aMotor->PID_param.Target = TargetSpeed;
    aMotor->PID_param.Actual = aMotor->prRealSpeed;
    Position_Pid_Calculate(aMotor->PID_param);

    aMotor->MotorEnable(aMotor, aMotor->PID_param.Output, ENABLE);

    return aMotor->prTargetSpeed;
}

char *prMotorEnable(struct MOTOR *aMotor, int8_t Pulse, uint8_t Enable) {
    static int16_t pulse2compare;

    //此处参数应根据实际计数值进行修改,这里计数值上限为20000
    pulse2compare = (20000 / 100) * Pulse;

    pulse2compare = ((pulse2compare > 0) ? pulse2compare : (-pulse2compare));
    aMotor->prDirect = ((Pulse > 0) ? FORWARD : BACK);

    if (Enable == 1) {
        HAL_TIM_PWM_Start(aMotor->TIMER, aMotor->TIM_Channel);
        __HAL_TIM_SetCompare(aMotor->TIMER, aMotor->TIM_Channel, pulse2compare);
    } else {
        HAL_TIM_PWM_Stop(aMotor->TIMER, aMotor->TIM_Channel);
//		MAP_Timer_A_setCompareValue(aMotor->TIMER, aMotor->TIM_Channel, 0);
    }

    switch (aMotor->prDirect) {
        case FORWARD:
            HAL_GPIO_WritePin(aMotor->DirectIO_1.GPIOx, aMotor->DirectIO_1.GPIO_Pin, 1);
            HAL_GPIO_WritePin(aMotor->DirectIO_2.GPIOx, aMotor->DirectIO_2.GPIO_Pin, 0);
//            GPIO_setOutputHighOnPin(aMotor->DirectIO_1.GPIOx, aMotor->DirectIO_1.GPIO_Pin);
//            GPIO_setOutputLowOnPin(aMotor->DirectIO_2.GPIOx, aMotor->DirectIO_2.GPIO_Pin);

            break;
        case BACK:
            HAL_GPIO_WritePin(aMotor->DirectIO_1.GPIOx, aMotor->DirectIO_1.GPIO_Pin, 0);
            HAL_GPIO_WritePin(aMotor->DirectIO_2.GPIOx, aMotor->DirectIO_2.GPIO_Pin, 1);
//            GPIO_setOutputLowOnPin(aMotor->DirectIO_1.GPIOx, aMotor->DirectIO_1.GPIO_Pin);
//            GPIO_setOutputHighOnPin(aMotor->DirectIO_2.GPIOx, aMotor->DirectIO_2.GPIO_Pin);
            break;
        default:
//            printf("ERROR MOTOR DIRECT!\r\n");
            HAL_GPIO_WritePin(aMotor->DirectIO_1.GPIOx, aMotor->DirectIO_1.GPIO_Pin, 0);
            HAL_GPIO_WritePin(aMotor->DirectIO_2.GPIOx, aMotor->DirectIO_2.GPIO_Pin, 0);
//            GPIO_setOutputLowOnPin(aMotor->DirectIO_1.GPIOx, aMotor->DirectIO_1.GPIO_Pin);
//            GPIO_setOutputLowOnPin(aMotor->DirectIO_2.GPIOx, aMotor->DirectIO_2.GPIO_Pin);
            break;
    }

    return aMotor->name;
}

Motor Motor_Create(char *name, TIM_HandleTypeDef *TIMER, uint32_t Channel, Direct_GPIO DirectIO, PID *PID_param) {
    Motor tMotor;

    tMotor.name = name;
    tMotor.TIMER = TIMER;
    tMotor.Channel = Channel;

    tMotor.PID_param = PID_param;

    tMotor.GetSpeed = prGetSpeed;
    tMotor.SetSpeed = prSetSpeed;
    tMotor.MotorEnable = prMotorEnable;

    tMotor.prDirect = FORWARD;
    tMotor.prTargetSpeed = 0;
    tMotor.prRealSpeed = 0;

    return tMotor;
}

void Motor_Init() {
    Direct_GPIO Motor_Left_IO;
    Direct_GPIO Motor_Right_IO;

    Motor_Left_IO.GPIOx_1 = GPIOA;
    Motor_Left_IO.GPIO_Pin_1 = GPIO_PIN_5;
    Motor_Left_IO.GPIOx_2 = GPIOA;
    Motor_Left_IO.GPIO_Pin_2 = GPIO_PIN_6;

    Motor_Right_IO.GPIOx_1 = GPIOA;
    Motor_Right_IO.GPIO_Pin_1 = GPIO_PIN_7;
    Motor_Right_IO.GPIOx_2 = GPIOA;
    Motor_Right_IO.GPIO_Pin_2 = GPIO_PIN_8;

    Motor_Left = Motor_Create("Left_Motor", htim1, TIM_CHANNEL_1, Motor_Left_IO, &PID_Motor_Left);
    Motor_Right = Motor_Create("Right_Motor", htim4, TIM_CHANNEL_2, Motor_Right_IO, &PID_Motor_Right);
}
