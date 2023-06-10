void uart_printf(const char* format, ...) {
    char buffer[256];  // 缓冲区用于存储格式化后的字符串
    va_list args;
    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);  // 格式化字符串到缓冲区
    va_end(args);

    for (size_t i = 0; buffer[i] != '\0'; ++i) {
        UART_transmitData(EUSCI_A0_BASE, buffer[i]); //更新为对应外设
    }
}