#define USARTBUFFER 100	
uint8_t uartBuffer[USARTBUFFER];

/***********************************************************************
���õĴ�����:
    �ڴ��ڽ��յ�����֮�󴥷������ж�, �жϻص������п���DMA����
    ��Ҫ�ż�������ڽ����жϱ�־λ, ��DMA��������ж��¼���, ��մ��ڽ����жϱ�־λ
    DMA��������ж��¼��ص�������,�����������ݷ��ͳ�ȥ, ����ջ�����,�ر�DMA

    ���ִ���������ʡ�Ե�"��鴮�ڻ������Ƿ�Ϊ��"�Ĳ���,
    ��ΪDMA���������,һ����Ϊ��,�����ѽ����ݽ������,����ֱ�ӷ���
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