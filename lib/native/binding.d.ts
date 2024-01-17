/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

/**
 * This module is internal-only and not exported.
 *
 * @packageDocumentation
 * @module binding
 */

/** @internal */
export function doSomethingUseful(): number;

export const cRuntime: string;
export const CRuntimeType: {
    NON_LINUX: string,
    MUSL: string,
    GLIBC: string
};
