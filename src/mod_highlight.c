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

/** 
 * mod_highlight
 *  - Syntax Highlight Source code
 */

#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_request.h"
#include "http_log.h"
#include "apr_tables.h"
#include "apr_hash.h"
#include "apr_buckets.h"
#include "apr_strings.h"
#include "ap_config.h"
#include "util_filter.h"

#include "mod_highligh.h"

module AP_MODULE_DECLARE_DATA highlight_module;

typedef struct highlight_struct {
    apr_bucket_brigade *bb;
} highlight_struct;

static int highlight_filter(ap_filter_t *f, apr_bucket_brigade *bb) {


}

static void highlight_register_hook(apr_pool_t *p) {
    ap_register_output_filter("HIGHLIGHT", highlight_filter, AP_FTYPE_CONTENT);
}

module AP_MODULE_DECLARE_DATA highlight_module = 
{
    STANDARD20_MODULE_STUFF,
    NULL,			/* create per-directory config structure */
    NULL,        		/* merge per-directory config structures */
    NULL,			/* create per-server config structure */
    NULL,			/* merge per-server config structures */
    NULL,			/* command apr_table_t */
    highlight_register_hook	/* register hooks */
};
