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

typedef struct { 
     const char *path;
} highlight_conf; 

#include "colorer_interface.h"

module AP_MODULE_DECLARE_DATA highlight_module;

static apr_status_t highlight_filter(ap_filter_t * f, apr_bucket_brigade * bb)
{
    apr_bucket *b;
    highlight_conf *c;
    int rv = OK;
    const char *buf = 0;
    apr_size_t bytes = 0;

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
                rv = colorer_parse_chunk(f->ctx, buf, bytes);
            }
            else {
                c = ap_get_module_config(f->r->server->module_config,
                                                     &highlight_module);
                /* starting up */
                colorer_init(f, c->path);
                rv = colorer_parse_chunk(f->ctx, buf, bytes);
            }
        }
    }
    apr_brigade_destroy(bb);
    return rv;
}

static void *create_highlight_config(apr_pool_t *p, server_rec *s) 
{ 
     highlight_conf *c = (highlight_conf *) apr_pcalloc(p, sizeof(highlight_conf)); 

     c->path = ap_server_root_relative(p, "share/colorer/catalog.xml");
     return c; 
} 

static const char *set_catalog_path(cmd_parms *cmd, void *dummy,
                                         const char *arg)
{
     highlight_conf *c = ap_get_module_config(cmd->server->module_config,
                                                     &highlight_module);

     c->path = ap_server_root_relative(cmd->pool, arg);
 
     return NULL;
}

static const command_rec highlight_cmds[] = {
     AP_INIT_TAKE1("HighlightCatalogPath", set_catalog_path, NULL,
                   RSRC_CONF, "Path to the Colorer Catalog for mod_highlight"),
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
    create_highlight_config,
    NULL,
    highlight_cmds,
    highlight_hooks
};
