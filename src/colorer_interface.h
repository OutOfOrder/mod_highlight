/* ====================================================================
 *  Copyright 2004 Edward Rudd and Paul Querna
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#ifndef _COLORER_COLOER_INTERFCAE_H_
#define _COLORER_COLOER_INTERFACE_H_

#include "http_core.h"
#include "http_protocol.h"
#include "http_log.h"
#include "util_filter.h"
#include "apr_buckets.h"
#include "apr_strings.h"
#include "apr_uri.h"
#include "apr_tables.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* to C, all we see is a void* ... keeps it simple. */
    int colorer_init(ap_filter_t * f);
    int colorer_parse_chunk(void *colorer, const char *buf, int len);
    int colorer_send_output(void *colorer);
    int colorer_cleanup(void *colorer);

#ifdef __cplusplus
}
#endif

#endif
