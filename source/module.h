#ifndef AWS_CRT_NODEJS_MODULE_H
#define AWS_CRT_NODEJS_MODULE_H
/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#define WIN32_LEAN_AND_MEAN

/* Suppress compiler warnings from node_api.h.
 * See: https://github.com/nodejs/node/pull/49103 */
#if defined(__clang__) || defined(__GNUC__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wstrict-prototypes"
#endif

#define NAPI_VERSION 4
#include <node_api.h>

#if defined(__clang__) || defined(__GNUC__)
#    pragma GCC diagnostic pop
#endif

#define AWS_CRT_NODEJS_PACKAGE_ID 11


napi_value create_addon(napi_env env);

static napi_value DoSomethingUseful(napi_env env, napi_callback_info info);

#endif  // AWS_CRT_NODEJS_MODULE_H