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

#define HIGHLIGHT_FILTER_NAME "HIGHLIGHT"

#include "http_core.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_log.h"
#include "util_filter.h"
#include "apr_buckets.h"
#include "apr_strings.h"
#include "apr_uri.h"
#include "apr_tables.h"

#include "colorer_interface.h"

module AP_MODULE_DECLARE_DATA highlight_module;

static apr_status_t highlight_filter(ap_filter_t * f, apr_bucket_brigade * bb)
{
    apr_bucket *b;
    int rv = -1;
    const char *buf = 0;
    apr_size_t bytes = 0;
    apr_status_t ret = APR_SUCCESS;

    if (!f->ctx) {
        if ((f->r->proto_num >= 1001) && !f->r->main && !f->r->prev) {
            f->r->chunked = 1;
        }
    }

    for (b = APR_BRIGADE_FIRST(bb);
         b != APR_BRIGADE_SENTINEL(bb); b = APR_BUCKET_NEXT(b)) {
        if (APR_BUCKET_IS_EOS(b)) {
            if (f->ctx) {       /* end of file */
                rv = colorer_send_output(f->ctx);
                colorer_cleanup(f->ctx);
                f->ctx = 0;
            }
        }
        else if (apr_bucket_read(b, &buf, &bytes, APR_BLOCK_READ)
                 == APR_SUCCESS) {
            if (f->ctx) {
                /* append data */
                //ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, f->r, "Adding: %d bytes", bytes);
                rv = colorer_parse_chunk(f->ctx, buf, bytes);
            }
            else {
                /* starting up */
                colorer_init(f);
                //rv = colorer_parse_chunk(f->ctx, buf, bytes);
                ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, f->r, "Adding: %d bytes", bytes);
            }
        }
    }
    apr_brigade_destroy(bb);
    return rv;
}

static const command_rec highlight_cmds[] = {

    {NULL}
};

static void highlight_hooks(apr_pool_t * p)
{
    ap_register_output_filter(HIGHLIGHT_FILTER_NAME, highlight_filter, NULL,
                              AP_FTYPE_RESOURCE);

};

module AP_MODULE_DECLARE_DATA highlight_module = {
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    highlight_cmds,
    highlight_hooks
};
