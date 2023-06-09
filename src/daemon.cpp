#include <stdio.h>
#include <iostream>
#include <thread>
#include <queue>
#include <napi.h>
#include "apifunctions.h"

struct TsfnContext
{
    TsfnContext(Napi::Env env) : deferred(Napi::Promise::Deferred::New(env))
    {
    };
    Napi::Promise::Deferred deferred;
    std::thread nativeThread;
    Napi::ThreadSafeFunction tsfn;
};

void callbackDaemonThreadEntry(TsfnContext* context);

void FinalizerCallback(Napi::Env env, void* finalizeData, TsfnContext* context);

Napi::Value CreateCallbackDaemon(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    //
    auto testData = new TsfnContext(env);
    testData->tsfn = Napi::ThreadSafeFunction::New(
        env,                           // Environment
        info[0].As<Napi::Function>(),  // JS function from caller
        "TSFN",                        // Resource name
        0,                             // Max queue size (0 = unlimited).
        1,                             // Initial thread count
        testData,                      // Context,
        FinalizerCallback,             // Finalizer
        (void*)nullptr                 // Finalizer data
    );
    testData->nativeThread = std::thread(callbackDaemonThreadEntry, testData);
    return testData->deferred.Promise();
}

void callbackDaemonThreadEntry(TsfnContext* context)
{
    auto callback = [](Napi::Env env, Napi::Function jsCallback, FrameUpdateQueueItem* item) {
        Napi::ArrayBuffer frame = Napi::ArrayBuffer::New(env, item->width * item->height * 4);
        memcpy(frame.Data(), item->argb, item->width * item->height * 4);
        //
        Napi::Object obj = Napi::Object::New(env);
        obj.Set("width", item->width);
        obj.Set("height", item->height);
        obj.Set("frame", frame);
        //
        jsCallback.Call({ obj });
        //
        UVCCapturer_DeleteObject(item->argb);
        delete item;
    };
    //
    while (true)
    {
        FrameUpdateQueueItem* item = UVCCapturer::PopFrameUpdateQueueItem();
        if (item != NULL)
        {
            napi_status status = context->tsfn.BlockingCall(item, callback);
            //
            if (status != napi_ok)
            {
                Napi::Error::Fatal(
                    "ThreadEntry",
                    "Napi::ThreadSafeNapi::Function.BlockingCall() failed");
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    context->tsfn.Release();
}

void FinalizerCallback(Napi::Env env, void* finalizeData, TsfnContext* context)
{
    context->nativeThread.join();
    context->deferred.Resolve(Napi::Boolean::New(env, true));
    delete context;
}
