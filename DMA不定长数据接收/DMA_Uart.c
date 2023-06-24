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

{   
    int i;
    int array_empty_flag = 1;

    for (;;) {
        HAL_UART_Receive_DMA(&huart3, &uartBuffer[0], USARTBUFFER);
        for (i = 0; i < USARTBUFFER; i++) {
            if (uartBuffer[i] != 0) {
                array_empty_flag = 0;
            }
        }
        if (array_empty_flag == 0) {
            array_empty_flag = 1;
            printf("User String:    %s\r\n", uartBuffer);
            
            //User Functions Start

            //User Functions End

            memset(uartBuffer, 0, USARTBUFFER);

            HAL_UART_DMAStop(&huart3);
        }
        osDelay(500);
    }
}