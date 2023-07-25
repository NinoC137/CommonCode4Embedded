#include "Motor.h"

//extern TIM_HandleTypeDef htim4; //����PWM
//extern TIM_HandleTypeDef htim1; //����������

Motor Motor_Left;
Motor Motor_Right;

float prGetSpeed(const struct MOTOR *aMotor){
    
    return aMotor->prRealSpeed;
}

float prSetSpeed(struct MOTOR *aMotor, uint8_t TargetSpeed){
    aMotor->prTargetSpeed = TargetSpeed;

    return aMotor->prTargetSpeed;
}

char* prMotorEnable(struct MOTOR *aMotor, int8_t Pulse, uint8_t Enable){
    static int16_t pulse2compare;

    pulse2compare = (200/100) * Pulse;
	
	  pulse2compare = ((pulse2compare > 0) ? pulse2compare : (-pulse2compare));
		aMotor->prDirect = ((Pulse > 0) ? FORWARD : BACK);

    if(Enable == 1)
    {
//        HAL_TIM_PWM_Start(aMotor->TIMER,aMotor->TIM_Channel);
//        __HAL_TIM_SetCompare(aMotor->TIMER, aMotor->TIM_Channel, pulse2compare);
		
		//���ָ��ռ�ձȵ�ֵ
		MAP_Timer_A_setCompareValue(aMotor->TIMER, aMotor->TIM_Channel, pulse2compare);
    }
    else
    {
//        HAL_TIM_PWM_Stop(aMotor->TIMER,aMotor->TIM_Channel);
		
		//ֹͣPWM���
		MAP_Timer_A_setCompareValue(aMotor->TIMER, aMotor->TIM_Channel, 0);
    }

    switch (aMotor->prDirect) {
        case FORWARD:
//            HAL_GPIO_WritePin(aMotor->GPIO_1.GPIOx,aMotor->GPIO_1.GPIO_Pin,1);
//            HAL_GPIO_WritePin(aMotor->GPIO_2.GPIOx,aMotor->GPIO_2.GPIO_Pin,0);
					  GPIO_setOutputHighOnPin(aMotor->GPIO_1.GPIOx, aMotor->GPIO_1.GPIO_Pin);
				    GPIO_setOutputLowOnPin(aMotor->GPIO_2.GPIOx, aMotor->GPIO_2.GPIO_Pin);
				
            break;
        case BACK:
//            HAL_GPIO_WritePin(aMotor->GPIO_1.GPIOx,aMotor->GPIO_1.GPIO_Pin,0);
//            HAL_GPIO_WritePin(aMotor->GPIO_2.GPIOx,aMotor->GPIO_2.GPIO_Pin,1);
						GPIO_setOutputLowOnPin(aMotor->GPIO_1.GPIOx, aMotor->GPIO_1.GPIO_Pin);
				    GPIO_setOutputHighOnPin(aMotor->GPIO_2.GPIOx, aMotor->GPIO_2.GPIO_Pin);
            break;
        default:
//            printf("ERROR MOTOR DIRECT!\r\n");
//            HAL_GPIO_WritePin(aMotor->GPIO_1.GPIOx,aMotor->GPIO_1.GPIO_Pin,0);
//            HAL_GPIO_WritePin(aMotor->GPIO_2.GPIOx,aMotor->GPIO_2.GPIO_Pin,0);
						GPIO_setOutputLowOnPin(aMotor->GPIO_1.GPIOx, aMotor->GPIO_1.GPIO_Pin);
				    GPIO_setOutputLowOnPin(aMotor->GPIO_2.GPIOx, aMotor->GPIO_2.GPIO_Pin);
            break;
    }
	
	return aMotor->name;
}

Motor Motor_Create(){
    Motor tMotor;

    tMotor.GetSpeed = prGetSpeed;
    tMotor.SetSpeed = prSetSpeed;
    tMotor.MotorEnable = prMotorEnable;

    tMotor.prDirect = FORWARD;
    tMotor.prTargetSpeed = 0;
    tMotor.prRealSpeed = 0;

    return tMotor;
}

void Motor_Init(){
    Motor_Left = Motor_Create();
    Motor_Right = Motor_Create();

    Motor_Left.TIMER =  LEFT_TIMER;
    Motor_Right.TIMER = RIGHT_TIMER;

	  Motor_Left.TIM_Channel = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    Motor_Right.TIM_Channel = TIMER_A_CAPTURECOMPARE_REGISTER_1;

    Motor_Left.GPIO_1.GPIOx = GPIO_PORT_P8;
    Motor_Left.GPIO_2.GPIOx = GPIO_PORT_P8;
    Motor_Left.GPIO_1.GPIO_Pin = GPIO_PIN5;
    Motor_Left.GPIO_2.GPIO_Pin = GPIO_PIN6;

    Motor_Right.GPIO_1.GPIOx = GPIO_PORT_P8;
    Motor_Right.GPIO_2.GPIOx = GPIO_PORT_P9;
    Motor_Right.GPIO_1.GPIO_Pin = GPIO_PIN7;
    Motor_Right.GPIO_2.GPIO_Pin = GPIO_PIN0;

    //����������Ļ�������
//    Motor_Left.GPIO_1 = Motor_Right.GPIO_1;
//    Motor_Left.GPIO_2 = Motor_Right.GPIO_2;

    Motor_Left.name = "Left_Motor";
    Motor_Right.name = "Right_Carrier";

    //__HAL_TIM_SET_COUNTER(&htim1, 10000);
}


