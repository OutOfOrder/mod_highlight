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

#include "colorer_interface.h"
#include "ColorerFilter.h"

int colorer_init(ap_filter_t * f, const char* cpath) {
    ColorerFilter* cf = new ColorerFilter(f, cpath);
    return 0;
}

int colorer_parse_chunk(void *colorer, const char *buf, int len) {
    ColorerFilter* cf = (ColorerFilter* )colorer;
    return cf->pSource->ParseChunk(buf, len);
}

int colorer_send_output(void *colorer) {
    ColorerFilter* cf = (ColorerFilter* )colorer;
    return cf->SendOutput();
}

int colorer_cleanup(void *colorer) {
    ColorerFilter* cf = (ColorerFilter* )colorer;
    delete cf;
    return 0;
}

