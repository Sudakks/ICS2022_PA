#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
	uint32_t a = inl(KBD_ADDR);
	if(a != AM_KEY_NONE)
	{

	uptime->us =  (uint64_t)inl(RTC_ADDR + 4) << 32;
	uint32_t lo = inl(RTC_ADDR);
	uptime->us = uptime->us | lo;
	}
	else
		putch('n'),putch('o');
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
