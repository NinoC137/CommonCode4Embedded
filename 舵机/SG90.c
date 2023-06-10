#include "SG90.h"
#include "stdlib.h"

SG90 clickBox_1;
SG90 clickBox_2;
SG90 clickBox_3;

int getAngle2Pulse(struct SG90_OOC * SG90) {
    return SG90->prHighLevelTimes_Ms;
}

void setAngle(struct SG90_OOC * SG90, int target_Angle) {
    SG90->prTarget_Angle = target_Angle;
    SG90->prHighLevelTimes_Ms = (5 + (SG90->prTarget_Angle / 180) * 20);
}

void move2TargetAngle(struct SG90_OOC* SG90){
    __HAL_TIM_SetCompare(SG90->TIMER, SG90->TIM_Channel, SG90->prHighLevelTimes_Ms);
}

SG90 SG90_Create()
{
    SG90 sg90_temple;
    //default arguments
    sg90_temple.prTarget_Angle = 0;
    sg90_temple.prHighLevelTimes_Ms = 5;
    sg90_temple.TIMER = NULL;
    sg90_temple.TIM_Channel = 0;
    //functions define
    sg90_temple.GetAngle2Pulse = getAngle2Pulse;
    sg90_temple.SetAngle = setAngle;
    sg90_temple.Move2TargetAngle = move2TargetAngle;

    return sg90_temple;
}

void SG90_init()
{
    clickBox_1 = SG90_Create();
    clickBox_2 = SG90_Create();
    clickBox_3 = SG90_Create();

    clickBox_1.name = "First";
    clickBox_2.name = "Second";
    clickBox_3.name = "Third";

    clickBox_1.TIMER = &htim1;
    clickBox_2.TIMER = &htim2;
    clickBox_3.TIMER = &htim3;

    clickBox_1.TIM_Channel = TIM_CHANNEL_1;
    clickBox_2.TIM_Channel = TIM_CHANNEL_2;
    clickBox_3.TIM_Channel = TIM_CHANNEL_1;
}

char* Click_Once(struct SG90_OOC * aSG90)
{
    static uint32_t time_old,time_now;
    //click out two pills
    aSG90->SetAngle(aSG90,180);
    aSG90->Move2TargetAngle(aSG90);
    if(time_now == 0) {
        time_old = HAL_GetTick();
    }
    time_now = HAL_GetTick();
    if((time_now - time_old) >= (2 / portTICK_PERIOD_MS))
    {
        time_now = 0;
        //return to default
        aSG90->SetAngle(aSG90,0);
        aSG90->Move2TargetAngle(aSG90);
    }

    return aSG90->name;
}

/*the number must >= 2, because SG90 must reset, and reset will also click out one pill,
 * It is meaning we at least click two pill at once*/
char* Click_Num(struct SG90_OOC * aSG90, uint8_t Num)
{
    for(uint8_t i = 0; i < (Num / 2); i++) {
        static uint32_t time_old,time_now;
        //click out o pieces
        aSG90->SetAngle(aSG90,180);
        aSG90->Move2TargetAngle(aSG90);
        if(time_now == 0) {
            time_old = HAL_GetTick();
        }
        time_now = HAL_GetTick();
        if((time_now - time_old) >= (2 / portTICK_PERIOD_MS))
        {
            time_now = 0;
            //return to default
            aSG90->SetAngle(aSG90,0);
            aSG90->Move2TargetAngle(aSG90);
        }
    }

    return aSG90->name;
}