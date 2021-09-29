/*
 * Copyright (c) 2018-2021 Qinglong<sysu.zqlong@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SYSUTILS_OS_LOG_H__
#define __SYSUTILS_OS_LOG_H__

#include <stdio.h>
#include <stdlib.h>
#include "osal_namespace.h"
#include "os_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void os_fatal(const char *tag, const char *format, ...);

void os_error(const char *tag, const char *format, ...);

void os_warning(const char *tag, const char *format, ...);

void os_info(const char *tag, const char *format, ...);

void os_debug(const char *tag, const char *format, ...);

void os_verbose(const char *tag, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* __SYSUTILS_OS_LOG_H__ */
