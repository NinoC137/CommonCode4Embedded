void delay_us(uint32_t us)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD;
	
	portENTER_CRITICAL();
	ticks = us* 48;	//48M主频下处理延时
	told = SysTick->VAL;
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow != told)
		{
			if(tnow < told) tcnt += told - tnow;
			else tcnt += reload - tnow + told;
			told = tnow;
			if(tcnt >= ticks) break;
		}
	};
	portEXIT_CRITICAL();
}
