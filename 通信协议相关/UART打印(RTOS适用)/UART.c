void uart_printf(const char* format, ...) {
    char buffer[256];  // ���������ڴ洢��ʽ������ַ���
    va_list args;
    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);  // ��ʽ���ַ�����������
    va_end(args);

    for (size_t i = 0; buffer[i] != '\0'; ++i) {
        UART_transmitData(EUSCI_A0_BASE, buffer[i]); //����Ϊ��Ӧ����
    }
}

//������STM32
void uart_printf(const char* format, ...) {
    xSemaphoreTake(printMutex, portMAX_DELAY);
    char buffer[128];  // ���������ڴ洢��ʽ������ַ���
    va_list args;
    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);  // ��ʽ���ַ�����������
    va_end(args);

    for (size_t i = 0; buffer[i] != '\0'; ++i) {
        HAL_UART_Transmit(&huart1, (uint8_t *)&buffer[i], 1, HAL_MAX_DELAY);
    }
    xSemaphoreGive(printMutex);
}

