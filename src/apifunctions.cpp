#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <thread>
#include <queue>
#include "UVCCapturer.h"
#include "apifunctions.h"

Napi::Value CreateCallbackDaemon(const Napi::CallbackInfo& info);

namespace UVCCapturer
{
    Napi::FunctionReference frameUpdateCallback;
    std::queue<FrameUpdateQueueItem*> frameUpdateQueue;
    CRITICAL_SECTION csFrameUpdateQueue;
}

void capturedFrameUpdateCallback(uint8_t* frame, int width, int height)
{
    printf("capturedFrameUpdateCallback: %d, %d\n", width, height);
    FrameUpdateQueueItem* item = new FrameUpdateQueueItem();
    item->argb = frame;
    item->width = width;
    item->height = height;
    UVCCapturer::PushFrameUpdateQueueItem(item);
}

void UVCCapturer::PushFrameUpdateQueueItem(FrameUpdateQueueItem* item)
{
    EnterCriticalSection(&csFrameUpdateQueue);
    frameUpdateQueue.push(item);
    LeaveCriticalSection(&csFrameUpdateQueue);
}

FrameUpdateQueueItem* UVCCapturer::PopFrameUpdateQueueItem(void)
{
    EnterCriticalSection(&csFrameUpdateQueue);
    if (UVCCapturer::frameUpdateQueue.empty())
    {
        LeaveCriticalSection(&csFrameUpdateQueue);
        return NULL;
    }
    FrameUpdateQueueItem* item = UVCCapturer::frameUpdateQueue.front();
    if (item == NULL)
    {
        LeaveCriticalSection(&csFrameUpdateQueue);
        return NULL;
    }
    UVCCapturer::frameUpdateQueue.pop();
    LeaveCriticalSection(&csFrameUpdateQueue);
    return item;
}

Napi::Boolean UVCCapturer::InitWrapped(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    //
    InitializeCriticalSection(&csFrameUpdateQueue);
    //
    UVCCapturer_SetFrameUpdateCallback(capturedFrameUpdateCallback);
    UVCCapturer_Init();
    //
    return Napi::Boolean::New(env, true);
}

Napi::Boolean UVCCapturer::Start(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if( UVCCapturer_Start() == 0 )
        return Napi::Boolean::New(env, true);
    return Napi::Boolean::New(env, false);
}

Napi::Boolean UVCCapturer::Stop(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (UVCCapturer_Stop() == 0)
        return Napi::Boolean::New(env, true);
    return Napi::Boolean::New(env, false);
}

Napi::Object UVCCapturer::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("init", Napi::Function::New(env, UVCCapturer::InitWrapped));
    exports.Set("start", Napi::Function::New(env, UVCCapturer::Start));
    exports.Set("stop", Napi::Function::New(env, UVCCapturer::Stop));
    //
    exports.Set("createCallbackDaemon", Napi::Function::New(env, CreateCallbackDaemon));
    return exports;
}

