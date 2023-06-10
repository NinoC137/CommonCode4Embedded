#include "StepMotor.h"

StepMotor SM_For_SubBox;

//Reset Target_Steps, it will make Motor back to default place.
char* prReset(StepMotor * SM){
    SM->Target_steps = 0;
    return SM->name;
}

//Step to NamedPlace
char* prStep2PointPlace(StepMotor * SM, char* PlaceName){
	return SM -> name;
}

StepMotor StepMotor_Create(void){
    StepMotor SM_temp;

    SM_temp.Current_steps = 0;
    SM_temp.Target_steps = 0;
    SM_temp.Reset = prReset;
    SM_temp.Step2PointPlace = prStep2PointPlace;

    return SM_temp;
}

void StepMotor_init(void){
//    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,25);

    SM_For_SubBox = StepMotor_Create();

    SM_For_SubBox.name = "SM_SubBox";
    SM_For_SubBox.Channel = 0;

}

char* Step_OnePeriod(const StepMotor * aStepMotor){
    HAL_TIM_PWM_Start_IT(&htim4, aStepMotor->Channel);
    HAL_TIM_PWM_Start(&htim4, aStepMotor->Channel);

    if (tim4_it_flag == 1)
    {
        HAL_TIM_PWM_Stop(&htim4, aStepMotor->Channel);
    }

    return aStepMotor->name;
}

/*
API Function: Control Motor State(Run / Pause)
argument:
	1. aStepMotor -- Oriented	//not used in HT32 script
	2. direct -- Motor moving direction
	3. Enable -- enable Motor
				Enable == 1 -- Run
				Enable == 0 -- Pause
 	
*/
char* Step_KeepMoving(const StepMotor * aStepMotor, uint8_t direct, uint8_t Enable){
    if(direct == 1)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
    } else{
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
    }
	
	if(Enable == 1)
        HAL_TIM_PWM_Start(&htim4, aStepMotor->Channel);
	else
        HAL_TIM_PWM_Stop(&htim4, aStepMotor->Channel);
	
    return aStepMotor->name;
}


