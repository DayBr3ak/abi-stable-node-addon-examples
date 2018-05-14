#ifndef __NAPI_ITC_PROMISE_H
#define __NAPI_ITC_PROMISE_H

#include <node_api.h>

#ifdef __cplusplus
  #define EXTERN_C_START extern "C" {
  #define EXTERN_C_END }
#else
  #define EXTERN_C_START
  #define EXTERN_C_END

  #ifndef true
    typedef int bool;
    #define true 1
    #define false 0
  #endif
#endif

EXTERN_C_START
typedef napi_status (*napi_itc_promise_resolver) (napi_env env, void* userdata, void* data, bool is_success, napi_value* out);
typedef struct napi_itc_promise_handle__ *napi_itc_promise_handle;

napi_status napi_itc_promise_init(
  napi_env env,
  napi_deferred deferred,
  napi_itc_promise_resolver resolver,
  void* userdata,
  napi_itc_promise_handle* handle);
void napi_itc_promise_resolve(napi_itc_promise_handle handle, void* data);
void napi_itc_promise_reject(napi_itc_promise_handle handle, void* data);

EXTERN_C_END

#endif
