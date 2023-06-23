#define USARTBUFFER 100	
    static uint8_t uartBuffer[USARTBUFFER];
   
    int i;
    int array_empty_flag = 1;

    for (;;) {
        HAL_UART_Receive_DMA(&huart3, &uartBuffer[0], 50);
        for (i = 0; i < USARTBUFFER; i++) {
            if (uartBuffer[i] != 0) {
                array_empty_flag = 0;
            }
        }
        if (array_empty_flag == 0) {
            array_empty_flag = 1;
            printf("User String:    %s\r\n", uartBuffer);
            //HAL_UART_Transmit(&huart3,uartBuffer,(sizeof(uartBuffer) / sizeof(uartBuffer[0])),100);

            //Motor Control Part
            if(uartBuffer[0] == '1')
            {
                motor_enable = ~motor_enable;
                xQueueSend(QueueMotorEnableHandle,&motor_enable,0);
                //memset(uartBuffer, 0, sizeof(uartBuffer));
            }

            memset(uartBuffer, 0, USARTBUFFER);

            HAL_UART_DMAStop(&huart3);
        }
        osDelay(500);
    }
