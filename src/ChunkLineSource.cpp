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

#include "ChunkLineSource.h"

ChunkLineSource::ChunkLineSource(ap_filter_t* pFilter) 
{
    kFilter = pFilter;
}

ChunkLineSource::~ChunkLineSource() 
{
    freeLines();
}

void ChunkLineSource::freeLines()
{
    for(int i = 0; i < lines.size(); i++) {
        delete lines.elementAt(i);
    }
    lines.setSize(0);
};

int ChunkLineSource::ParseChunk(const char* chunk, int len) 
{
    char *strtok_state;
    char *tokit;
    char *key;

    if(len < 1) {
        return 0;
    }

    tokit = apr_pstrdup(kFilter->r->pool, chunk);
     
    key = apr_strtok(tokit, "\n", &strtok_state);
    do {
        lines.addElement(new SString(key,-1));
        key = apr_strtok(NULL, "\n", &strtok_state);
    } while(key);

    return 0;
}


String *ChunkLineSource::getLine(int lno){
  if (lines.size() <= lno) return null;
  return lines.elementAt(lno);
};
int ChunkLineSource::getLineCount(){
  return lines.size();
};


