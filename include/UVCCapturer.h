#pragma once
#include <stdio.h>
#include <windows.h>
#include <stdint.h>

typedef void (*UVCCapturer_FrameUpdateCallback)(uint8_t *frame, int width, int height);

extern "C" __declspec(dllexport) int UVCCapturer_Init(void);
extern "C" __declspec(dllexport) int UVCCapturer_Start(void);
extern "C" __declspec(dllexport) int UVCCapturer_Stop(void);
extern "C" __declspec(dllexport) int UVCCapturer_SetFrameUpdateCallback(UVCCapturer_FrameUpdateCallback _frameUpdateCallback);
extern "C" __declspec(dllexport) int UVCCapturer_DeleteObject(void *object);