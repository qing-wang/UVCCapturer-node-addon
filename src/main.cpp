#include <napi.h>
#include "apifunctions.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return UVCCapturer::Init(env, exports);
}

NODE_API_MODULE(mcsdk_api, InitAll)