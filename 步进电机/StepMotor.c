#include "StepMotor.h"

StepMotor SM_For_SubBox;
StepMotor SM_For_littleCup;
int SubBox_Buffer[4]={ID_BOX1, ID_BOX2, ID_BOX3, ID_OUT };

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

StepMotor StepMotor_Create(void) {
    StepMotor SM_temp;

    SM_temp.CurrentPlace = 0;
    SM_temp.TargetPlace = 0;
    SM_temp.Reset = prReset;
    SM_temp.Step2PointPlace = prStep2PointPlace;

    return SM_temp;
}

void StepMotor_init(void) {
    SM_For_SubBox = StepMotor_Create();
    SM_For_littleCup = StepMotor_Create();

    SM_For_SubBox.name = "SM_SubBox";
    SM_For_SubBox.TIMER = &htim4;
    SM_For_SubBox.Channel = TIM_CHANNEL_2;
    SM_For_SubBox.CurrentPlace = ID_OUT;

    SM_For_littleCup.name = "SM_LittleCup";
    SM_For_littleCup.TIMER = &htim4;
    SM_For_littleCup.Channel = TIM_CHANNEL_1;
    SM_For_littleCup.CurrentPlace = Lit_INT;

    //输出50%占空比方波
            __HAL_TIM_SetCompare(SM_For_SubBox.TIMER, SM_For_SubBox.Channel, 250);
            __HAL_TIM_SetCompare(SM_For_littleCup.TIMER, SM_For_littleCup.Channel,250);
}

char *Step_OnePeriod(const StepMotor *aStepMotor, uint8_t direct) {
    HAL_TIM_PWM_Start_IT(aStepMotor->TIMER,aStepMotor->Channel);
    HAL_TIM_PWM_Start(aStepMotor->TIMER, aStepMotor->Channel);

    if (direct == FORWARD) {
        HAL_GPIO_WritePin(Direct_GPIO_Port, Direct_Pin, 1);
    } else {
        HAL_GPIO_WritePin(Direct_GPIO_Port, Direct_Pin, 0);
    }

    while(!tim4_it_flag);
    tim4_it_flag = 0;
    HAL_TIM_PWM_Stop(aStepMotor->TIMER, aStepMotor->Channel);
    HAL_TIM_PWM_Stop_IT(aStepMotor->TIMER,aStepMotor->Channel);
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


