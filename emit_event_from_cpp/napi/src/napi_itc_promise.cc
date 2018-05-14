#include "napi_itc_queue.h"
#include "napi_itc_promise.h"

struct NapiItcPromiseHandle {
  napi_itc_handle napi_itc;
  napi_deferred deferred;
  napi_itc_promise_resolver resolver;
  void* userdata;
};

typedef NapiItcPromiseHandle napi_itc_promise_handle_t;

struct PromiseHelper {
  bool is_success;
  void* data;
};

static void exe(napi_env env, napi_itc_handle h, void* data, void* event) {
  auto hdl = (napi_itc_promise_handle_t*)data;
  auto p = (PromiseHelper*)event;
  napi_value resolution;
  hdl->resolver(env, hdl->userdata, p->data, p->is_success, &resolution);
  if (p->is_success) {
    napi_resolve_deferred(env, hdl->deferred, resolution);
  } else {
    napi_reject_deferred(env, hdl->deferred, resolution);
  }
  delete p;
  delete hdl;
}

EXTERN_C_START

napi_status napi_itc_promise_init(
  napi_env env,
  napi_deferred deferred,
  napi_itc_promise_resolver resolver,
  void* userdata,
  napi_itc_promise_handle* handle)
{
  napi_itc_promise_handle_t* hdl = new napi_itc_promise_handle_t;
  hdl->deferred = deferred;
  hdl->resolver = resolver;
  hdl->userdata = userdata;
  *handle = (napi_itc_promise_handle)hdl;
  return napi_itc_init(env, exe, nullptr, (void*)hdl, &hdl->napi_itc);
}

void napi_itc_promise_resolve(napi_itc_promise_handle handle, void* data) {
  auto x = (napi_itc_promise_handle_t*)handle;
  auto y = new PromiseHelper;
  y->data = data;
  y->is_success = true;
  napi_itc_send_and_complete(x->napi_itc, y);
}

void napi_itc_promise_reject(napi_itc_promise_handle handle, void* data) {
  auto x = (napi_itc_promise_handle_t*)handle;
  auto y = new PromiseHelper;
  y->data = data;
  y->is_success = false;
  napi_itc_send_and_complete(x->napi_itc, y);
}

EXTERN_C_END
