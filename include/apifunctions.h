#include <stdio.h>
#include <napi.h>

namespace UVCCapturer
{
	Napi::Boolean InitWrapped(const Napi::CallbackInfo& info);
	//
	Napi::Object Init(Napi::Env env, Napi::Object exports);
}