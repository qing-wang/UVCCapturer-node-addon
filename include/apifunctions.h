#include <stdio.h>
#include <napi.h>
#include "UVCCapturer.h"

class FrameUpdateQueueItem
{
public:
	uint8_t* argb;
	int width;
	int height;
public:
	FrameUpdateQueueItem()
	{
		argb = NULL;
		width = 0;
		height = 0;
	}
};

namespace UVCCapturer
{
	void PushFrameUpdateQueueItem(FrameUpdateQueueItem* item);
	FrameUpdateQueueItem* PopFrameUpdateQueueItem(void);
	//
	Napi::Boolean InitWrapped(const Napi::CallbackInfo& info);
	Napi::Boolean Start(const Napi::CallbackInfo& info);
	Napi::Boolean Stop(const Napi::CallbackInfo& info);
	//
	Napi::Object Init(Napi::Env env, Napi::Object exports);
}