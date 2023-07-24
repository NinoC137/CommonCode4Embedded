/* Includes ------------------------------------------------------------------*/
#include "GT911.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void GT911_INT_EXIT(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(INT_GPIO_Port, &GPIO_InitStruct);
}

void GT911_INT_Input(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(INT_GPIO_Port, &GPIO_InitStruct);
}

void GT911_INT_Output(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(INT_GPIO_Port, &GPIO_InitStruct);
}

void GT911_RST_Control(bool high_or_low) {
    switch (high_or_low) {
        case true:
            HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_SET);
            break;
        case false:
            HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET);
            break;
    }
}

void GT911_INT_Control(bool high_or_low) {
    switch (high_or_low) {
        case true:
            HAL_GPIO_WritePin(INT_GPIO_Port, INT_Pin, GPIO_PIN_SET);
            break;
        case false:
            HAL_GPIO_WritePin(INT_GPIO_Port, INT_Pin, GPIO_PIN_RESET);
            break;
    }
}


void GT911_Delay(uint16_t ms) {
    HAL_Delay(ms);
}

GT911_Status_t GT911_I2C_Init(void) {
    return GT911_OK;
}

GT911_Status_t GT911_I2C_Write(uint8_t Addr, uint8_t *write_data, uint16_t write_length) {
    uint8_t Address = Addr << 1;
    switch (HAL_I2C_Master_Transmit(&hi2c1, Address, write_data, write_length, HAL_MAX_DELAY)) {
        case HAL_OK:
            return GT911_OK;
            break;
        case HAL_ERROR:
            return GT911_Error;
            break;
        case HAL_BUSY:
        case HAL_TIMEOUT:
            return GT911_NotResponse;
            break;
    }
    return GT911_Error;
}

GT911_Status_t GT911_I2C_Read(uint8_t Addr, uint8_t *read_data, uint16_t read_length) {
    uint8_t Address = Addr << 1;
    switch (HAL_I2C_Master_Receive(&hi2c1, Address, read_data, read_length, HAL_MAX_DELAY)) {
        case HAL_OK:
            return GT911_OK;
            break;
        case HAL_ERROR:
            return GT911_Error;
            break;
        case HAL_BUSY:
        case HAL_TIMEOUT:
            return GT911_NotResponse;
            break;
    }
    return HAL_ERROR;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
