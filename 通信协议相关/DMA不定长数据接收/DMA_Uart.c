#define USARTBUFFER 100	
uint8_t uartBuffer[USARTBUFFER];

/***********************************************************************
更好的处理方案:
    在串口接收到数据之后触发接收中断, 中断回调函数中开启DMA传输
    不要着急清除串口接收中断标志位, 在DMA传输完毕中断事件中, 清空串口接收中断标志位
    DMA传输完毕中断事件回调函数中,将缓冲区数据发送出去, 并清空缓冲区,关闭DMA

    这种处理方案可以省略掉"检查串口缓冲区是否为空"的步骤,
    因为DMA传输结束后,一定不为空,并且已将数据接收完毕,可以直接发送
***********************************************************************/

osMutexDef(printfMutex);
osMutexId printfMutex;
void UARTTask(void const *argument) {
    int i;
    int array_empty_flag = 1;

    printfMutex = osMutexCreate(osMutex(printfMutex));

    uart_printf("Uart Task Start.\r\n");
    for (;;) {
        HAL_UART_Receive_DMA(&huart1, &uartBuffer[0], UARTBUFFER);
        for (i = 0; i < UARTBUFFER; i++) {
            if (uartBuffer[i] != 0) {
                array_empty_flag = 0;
                break;
            }
        }
        if (array_empty_flag == 0) {
            array_empty_flag = 1;
            /*
             * example:
             * UartInput: x:+003,
             * PWMValue[1] = 3;
             * UartInput: x:+012,
             * PWMValue[1] = 12;
             * */
            ReformatBuffer(uartBuffer, PWMValue);
            uart_printf("PWM Value: %0.2f.\r\n", PWMValue[0]);

            memset(uartBuffer, 0, UARTBUFFER);

            HAL_UART_DMAStop(&huart1);
        }
        osDelay(100);
    }

}