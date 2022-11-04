#include <am.h>
#include <nemu.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)

void __am_audio_init() {
/*	SDL_AudioSpec s = {};
	s.format = AUDIO_S16SYS;  // 假设系统中音频数据的格式总是使用16位有符号数来表示
	s.userdata = NULL;        // 不使用
	//TODO
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	SDL_OpenAudio(&s, NULL);
	SDL_PauseAudio(0);*/
}

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
	//PA2.3
	/*
	cfg->bufsize = inl(AUDIO_SBUF_SIZE_ADDR);
  cfg->present = true;//change into true
	*/
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
	//PA2.3
	/*
	ctrl->freq = inl(AUDIO_FREQ_ADDR);
	ctrl->channels = inl(AUDIO_CHANNELS_ADDR);
	ctrl->samples = inl(AUDIO_SAMPLES_ADDR);
	*/
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
	//PA2.3
	/*
  stat->count = inl(AUDIO_COUNT_ADDR);
	*/
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
	//将[buf.start, buf.end)区间的内容作为音频数据写入流缓冲区
}
