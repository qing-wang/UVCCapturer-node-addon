#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <thread>
#include "apifunctions.h"

Napi::Boolean UVCCapturer::InitWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, true);
}

Napi::Object UVCCapturer::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("init", Napi::Function::New(env, UVCCapturer::InitWrapped));
    return exports;
}

