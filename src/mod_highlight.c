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
    apr_bucket *b;
    apr_size_t len;
    apr_bucket *eos = NULL;
    apr_bucket_brigade *bb_tmp;
    const char *data;

    for (b = APR_BRIGADE_FIRST(bb);
         b != APR_BRIGADE_SENTINEL(bb);
         b = APR_BUCKET_NEXT(b))
    {
        if (APR_BUCKET_IS_EOS(b)) {
            eos = b;
            break;
        }
        rv = apr_bucket_read(b, &data, &len, APR_BLOCK_READ);
        if (rv != APR_SUCCESS) {
            ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, r, "apr_bucket_read()");
            return rv;
        }

        // TODO: write pass_data_to_highlight(...)
        if (len > 0 &&
            (rv = pass_data_to_highlight(f, data, (apr_size_t)len, bb_tmp))
                != APR_SUCCESS) {
            return rv;
        }
    }
    apr_brigade_cleanup(bb);
    APR_BRIGADE_CONCAT(bb, bb_tmp);
    apr_brigade_destroy(bb_tmp);

    if (eos) {
        /**
         * No more content coming, generate the HTML...
         */
        b = apr_bucket_eos_create(c->bucket_alloc);
        APR_BRIGADE_INSERT_TAIL(bb, b);
    }

    return APR_SUCCESS;
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
