#include "StepMotor.h"

/**
 *  注意:
 *  1. 需要开启PWM中断触发
 *  2. 需要定义步进电机的方向控制IO脚为"Direct", 定时器上限数为500(可定义为其他数,但是要修改init函数中输出50%占空比方波部分) 
 *  3. 参考Init文件中的对象定义方案, 提前配置好对应外设.
 *  4. 在外设初始化完毕后,再调用步进电机的init函数
 *  5. 请勿调用pr开头的函数, 这些函数都有在结构体中通过函数指针的方式进行调用
 */

StepMotor SM_For_SubBox;
int SubBox_Buffer[4]={ID_BOX1, ID_BOX2, ID_BOX3, ID_OUT };

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
    if(htim == SM_For_SubBox.TIMER){
        if(HAL_GPIO_ReadPin(Direct_GPIO_Port, Direct_Pin) == FORWARD){
            SM_For_SubBox.PulseCounter++;   //增加正向运动脉冲计数
        }else{
            SM_For_SubBox.PulseCounter--;   //减少正向运动脉冲计数
        }

        if(SM_For_SubBox.PulseCounter == ONEPERIOD){
            SM_For_SubBox.MotorState = 1;   //完成一圈转动的标志位
            SM_For_SubBox.CurrentPlace++;   //转动圈数+1
        }
    }
}

void StepMotor_init(void) {
    SM_For_SubBox = StepMotor_Create("SM_SubBox", &htim4, TIM_CHANNEL_2);

    //输出50%占空比方波
            __HAL_TIM_SetCompare(SM_For_SubBox.TIMER, SM_For_SubBox.Channel, 250);
}

//Reset TargetPlace, it will make Motor back to default place.
char *prReset(StepMotor *SM) {
    return SM->Step2PointPlace(SM, ID_INIT);
}

//Step to NamedPlace
char *prStep2PointPlace(StepMotor *SM, uint32_t PlaceName) {
    uint8_t directFlag;
    SM->TargetPlace = PlaceName;

    if (SM->TargetPlace > SM->CurrentPlace) {
        directFlag = FORWARD;
    } else {
        directFlag = BACK;
    }
    //SM->TargetPlace = PlaceName;
    for (uint32_t i = 0; i < abs((int)SM->TargetPlace - (int)SM->CurrentPlace); i++) {
        Step_OnePeriod(SM, directFlag);
    }
    SM->CurrentPlace = SM->TargetPlace;
    HAL_TIM_PWM_Stop_IT(SM->TIMER,SM->Channel);

    return SM->name;
}

StepMotor StepMotor_Create(char* name, TIM_HandleTypeDef* TIMER, uint32_t Channel){
    StepMotor SM_temp;

    SM_temp.name = name;
    SM_temp.TIMER = TIMER;
    SM_temp.Channel = Channel;

    SM_temp.CurrentPlace = 0;
    SM_temp.TargetPlace = 0;
    SM_temp.Reset = prReset;
    SM_temp.Step2PointPlace = prStep2PointPlace;

    return SM_temp;
}

char *Step_OnePeriod(const StepMotor *aStepMotor, uint8_t direct) {
    HAL_TIM_PWM_Start_IT(aStepMotor->TIMER,aStepMotor->Channel);
    HAL_TIM_PWM_Start(aStepMotor->TIMER, aStepMotor->Channel);

    if (direct == FORWARD) {
        HAL_GPIO_WritePin(Direct_GPIO_Port, Direct_Pin, 1);
    } else {
        HAL_GPIO_WritePin(Direct_GPIO_Port, Direct_Pin, 0);
    }

    //在定时器PWM中断中,发送转动一圈的PWM脉冲后将该标志位置1
    while(!aStepMotor->MotorState);
    aStepMotor->MotorState = 0;

    HAL_TIM_PWM_Stop_IT(aStepMotor->TIMER,aStepMotor->Channel);
    HAL_TIM_PWM_Stop(aStepMotor->TIMER, aStepMotor->Channel);
    printf("One period.\r\n");

    return aStepMotor->name;
}

/*
API Function: Control Motor State(Run / Pause)
argument:
	1. aStepMotor -- Oriented
	2. direct -- Motor moving direction
                direct == 1 -- Forward
                direct == 0 -- Back
	3. Enable -- enable Motor
				Enable == 1 -- Run
				Enable == 0 -- Pause
 	
*/
char *Step_KeepMoving(const StepMotor *aStepMotor, uint8_t direct, uint8_t Enable) {
    if (direct == FORWARD) {
        HAL_GPIO_WritePin(Direct_GPIO_Port, Direct_Pin, 1);
    } else {
        HAL_GPIO_WritePin(Direct_GPIO_Port, Direct_Pin, 0);
    }

    if (Enable == ENABLE)
        HAL_TIM_PWM_Start(aStepMotor->TIMER, aStepMotor->Channel);
    else
        HAL_TIM_PWM_Stop(aStepMotor->TIMER, aStepMotor->Channel);

    return aStepMotor->name;
}
