/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include "module.h"
#include <uv.h>


#define AWS_DEFINE_ERROR_INFO_CRT_NODEJS(CODE, STR) AWS_DEFINE_ERROR_INFO(CODE, STR, "aws-crt-nodejs")

/* TODO:
 * Hardcoded enum value for `napi_no_external_buffers_allowed`.
 * The enum `napi_no_external_buffers_allowed` is introduced in node21 and backport
 * to node 14.21.2, 16.19.0, 18.13.0.
 * Use `napi_no_external_buffers_allowed` for external buffer related changes after bump to node 21 */
#define NAPI_NO_EXTERNAL_BUFFER_ENUM_VALUE 22

#if defined(__clang__) || defined(__GNUC__)
/* Suppress compiler warnings about NAPI_MODULE_INIT().
 * See: https://github.com/nodejs/node/pull/49103 */
#    pragma GCC diagnostic ignored "-Wstrict-prototypes"
#endif


#define NODE_API_CALL(env, call)                                  \
  do {                                                            \
    napi_status status = (call);                                  \
    if (status != napi_ok) {                                      \
      const napi_extended_error_info* error_info = NULL;          \
      napi_get_last_error_info((env), &error_info);               \
      const char* err_message = error_info->error_message;        \
      bool is_pending;                                            \
      napi_is_exception_pending((env), &is_pending);              \
      /* If an exception is already pending, don't rethrow it */  \
      if (!is_pending) {                                          \
        const char* message = (err_message == NULL)               \
            ? "empty error message"                               \
            : err_message;                                        \
        napi_throw_error((env), NULL, message);                   \
      }                                                           \
      return NULL;                                                \
    }                                                             \
  } while(0)

static napi_value
DoSomethingUseful(napi_env env, napi_callback_info info) {
  // Do something useful.
  return 0;
}

napi_value create_addon(napi_env env) {
  napi_value result;
  NODE_API_CALL(env, napi_create_object(env, &result));

  napi_value exported_function;
  NODE_API_CALL(env, napi_create_function(env,
                                          "doSomethingUseful",
                                          NAPI_AUTO_LENGTH,
                                          DoSomethingUseful,
                                          NULL,
                                          &exported_function));

  NODE_API_CALL(env, napi_set_named_property(env,
                                             result,
                                             "doSomethingUseful",
                                             exported_function));

  return result;
}

NAPI_MODULE_INIT(/* napi_env env, napi_value exports */) {
  // This function body is expected to return a `napi_value`.
  // The variables `napi_env env` and `napi_value exports` may be used within
  // the body, as they are provided by the definition of `NAPI_MODULE_INIT()`.
  return create_addon(env);
}