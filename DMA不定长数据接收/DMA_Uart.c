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