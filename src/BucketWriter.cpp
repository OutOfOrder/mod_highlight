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

#include "BucketWriter.h"

BucketWriter::BucketWriter(ap_filter_t* pFilter) {
    kFilter = pFilter;
    bb = apr_brigade_create(kFilter->r->pool, apr_bucket_alloc_create(kFilter->r->pool));
    encodingIndex = Encodings::getDefaultEncodingIndex();
}

BucketWriter::~BucketWriter() {
    apr_brigade_destroy(bb);
}

void BucketWriter::write(wchar c) {
    /* TODO: This extremely bad? */
    /* TODO: Is this really Wide Character Safe? */
#if GOD_THIS_IS_EVIL
    wctomb(db, c);
    ap_fprintf(kFilter->next, bb, "%.1s", db);    
#else /* GOD_THIS_IS_SLIGHTLY_LESS_EVIL */
    byte buf[8];
    int bufLen = Encodings::toBytes(encodingIndex, c, buf);
    for(int pos = 0; pos < bufLen; pos++) {
        ap_fputc(kFilter->next, bb, buf[pos]);
    }
    /* ap_fwrite(kFilter->next, bb, &buf, bufLen); */
#endif
}

void BucketWriter::SendBrigade() {
    apr_bucket *b = apr_bucket_eos_create(bb->bucket_alloc);
    APR_BRIGADE_INSERT_TAIL(bb, b);
    ap_pass_brigade(kFilter->next, bb);
}
