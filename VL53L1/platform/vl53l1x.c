#include "vl53l1.h"
int32_t distance;
VL53L1_Dev_t VL53;
VL53L1_RangingMeasurementData_t result_data;
mode_data Mode_data[]=
{
	{(FixPoint1616_t)(16384), //its uint32_t, uint16_.uint16_t, 0.25*65536
	 (FixPoint1616_t)(1179648),	 //18*65536
	 33000,
	 14,
	 10},//default
		
	{(FixPoint1616_t)(16384),	//0.25*65536
	 (FixPoint1616_t)(1179648),		//18*65536
	 200000, 
	 14,
	 10},//high accuracy
		
  {(FixPoint1616_t)(6554),		//0.1*65536
	 (FixPoint1616_t)(3932160),		//60*65536
	 33000,
	 18,
	 14},//long distance
	
  {(FixPoint1616_t)(16384),	//0.25*65536
	 (FixPoint1616_t)(2097152),		//32*65536
	 20000,
	 14,
	 10},//high speed
};

VL53L1_Error VL53L1Init(VL53L1_Dev_t* pDev)
{
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    pDev->I2cDevAddr=0x52;//Ĭ�ϵ�ַ
    pDev->comms_type=1;//Ĭ��ͨ��ģʽ
    pDev->comms_speed_khz = 400;//ͨ�����ʣ��ɵ�400hz��
    Status = VL53L1_WaitDeviceBooted(pDev);
    if(Status!=VL53L1_ERROR_NONE)
	{
		printf("Wait device Boot failed!\r\n");
		return Status;
	}
    HAL_Delay(2);

	Status = VL53L1_DataInit(pDev);//device init
	if(Status!=VL53L1_ERROR_NONE) 
	{
		printf("datainit failed!\r\n");
		return Status;
	}

	HAL_Delay(2);
	Status = VL53L1_StaticInit(pDev);
	if(Status!=VL53L1_ERROR_NONE) 
	{
		printf("static init failed!\r\n");
		return Status;
	}
	HAL_Delay(2);
	Status = VL53L1_SetDistanceMode(pDev, VL53L1_DISTANCEMODE_LONG);	//short,medium,long
	if(Status!=VL53L1_ERROR_NONE) 
	{
		printf("set discance mode failed!\r\n");
		return Status;
	}
	HAL_Delay(2);
	return Status;
}

/** 
* @brief  ����������һ������У׼
* @param [in] pDev  ָ��������
* @param [in] save  ����洢��ַ
* @retval  VL53L1_Error���� 
* @par ��־ 
*
*/
VL53L1_Error VL53Cali(VL53L1_Dev_t* pDev,void * save)
{
    VL53L1_Error Status = VL53L1_ERROR_NONE;
    Status = VL53L1_StopMeasurement(pDev);
	if(Status!=VL53L1_ERROR_NONE) 
		return Status;
    Status = VL53L1_PerformRefSpadManagement(pDev);//perform ref SPAD management
	if(Status!=VL53L1_ERROR_NONE) 
		return Status;
    /*
    Status = VL53L1_PerformOffsetSimpleCalibration(pDev,140);//14cm�ĳ���У��ֵ
	if(Status!=VL53L1_ERROR_NONE) 
		return Status;
    */
    Status = VL53L1_GetCalibrationData(pDev,save);
	if(Status!=VL53L1_ERROR_NONE) 
		return Status;
    //ȫ����� ���´򿪲���
    Status = VL53L1_StartMeasurement(pDev);
    return Status;
}

VL53L1_Error VL53InitParam(VL53L1_Dev_t* pDev,uint8_t mode)
{
    VL53L1_Error status = VL53L1_ERROR_NONE;
    //4������
    status = VL53L1_SetLimitCheckEnable(pDev,VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE,1);//sigma--standard deviation, enable SIGMA limit check
	if(status!=VL53L1_ERROR_NONE) 
		return status;
    HAL_Delay(2);
	status = VL53L1_SetLimitCheckEnable(pDev,VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,1);//signal--amplitude of the signal-
																																												//-reflected. enable signal rate limit check
	if(status!=VL53L1_ERROR_NONE) 
		return status;
	
	
	HAL_Delay(2);
	status = VL53L1_SetLimitCheckValue(pDev,VL53L1_CHECKENABLE_SIGMA_FINAL_RANGE,Mode_data[mode].sigmaLimit);//set SIGMA limit
	if(status!=VL53L1_ERROR_NONE) 
		return status;
	
	
	HAL_Delay(2);
	status = VL53L1_SetLimitCheckValue(pDev,VL53L1_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,Mode_data[mode].signalLimit);//set signal rate limit
	if(status!=VL53L1_ERROR_NONE) 
		return status;
    //4������ֵ�������
    
    
    status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(pDev,Mode_data[mode].timingBudget);//set the max interval for a whole diatance test
	if(status!=VL53L1_ERROR_NONE) 
		return status;
	HAL_Delay(2);
	status = VL53L1_SetInterMeasurementPeriodMilliSeconds(pDev, 300);
	if(status!=VL53L1_ERROR_NONE) 
	{
		printf("SetInterMeasurementPeriodMilliSeconds failed!\r\n");
		return status;
	}
	HAL_Delay(2);
	status = VL53L1_StartMeasurement(pDev);
	if(status!=VL53L1_ERROR_NONE) 
	{
		printf("start measurement failed!\r\n");
		return status;
	}
    return status;
}
int32_t flag=0;
VL53L1_Error getDistance(VL53L1_Dev_t* pDev)
{
	VL53L1_Error status = VL53L1_ERROR_NONE;
    uint8_t isDataReady=1;
    status = VL53L1_WaitMeasurementDataReady(pDev);
    //status = VL53L1_GetMeasurementDataReady(pDev,&isDataReady);
    if(status!=VL53L1_ERROR_NONE) 
	{
		printf("Wait too long!\r\n");
		return status;
	}
    if(!isDataReady)
    {
        flag++;
        return -7;
    }
    status = VL53L1_GetRangingMeasurementData(pDev, &result_data);
    distance = result_data.RangeMilliMeter;
    status = VL53L1_ClearInterruptAndStartMeasurement(pDev);
    return status;
}
