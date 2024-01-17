/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

import { crt } from "aws-crt";


async function main(){
    var result :number = crt.doSomething();
    console.log(`value : ${{result}}`);
}

main();
