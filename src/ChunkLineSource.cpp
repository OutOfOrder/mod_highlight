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

ChunkLineSource::ChunkLineSource(ap_filter_t* pFilter) {
    kFilter = pFilter;
}

ChunkLineSource::~ChunkLineSource() {
    freeLines();
}

void ChunkLineSource::freeLines(){
  for(int i = 0; i < lines.size(); i++)
    delete lines.elementAt(i);
  lines.setSize(0);
};

int ChunkLineSource::ParseChunk(const char* chunk, int len) {

  char *strtok_state;
  char *tokit;
  char *key;
  ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ChunkLineSource::ParseChunk() bytes: %d", len);
  if(len < 1) {
      return 0;
  }

  tokit = apr_pstrdup(kFilter->r->pool, chunk);
     
  key = apr_strtok(tokit, "\n", &strtok_state);
  do {
    lines.addElement(new SString(key,-1));
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ChunkLineSource::ParseChunk() Line: %s", key);
    key = apr_strtok(NULL, "\n", &strtok_state);
  } while(key);

/*
  int ei = -1; // todo: fix input encoding.
  DString file(chunk, len, ei);
  int length = file.length();
  ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ChunkLineSource::ParseChunk() strlen: %d", length);
  lines.ensureCapacity(length/30); // estimate number of lines

  int i = 0;
  int filepos = 0;
  int prevpos = 0;
  if (length && file[0] == 0xFEFF) filepos = prevpos = 1;
  while(filepos < length+1){
    if (filepos == length || file[filepos] == '\r' || file[filepos] == '\n'){
      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ChunkLineSource::ParseChunk() Adding Line");
      lines.addElement(new SString(&file, prevpos, filepos-prevpos));
      if (filepos+1 < length && file[filepos] == '\r' && file[filepos+1] == '\n')
        filepos++;
      else if (filepos+1 < length && file[filepos] == '\n' && file[filepos+1] == '\r')
        filepos++;
      prevpos = filepos+1;
      i++;
    };
    filepos++;
  };
*/
  ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ChunkLineSource::ParseChunk() LineCount: %d", lines.size());
  return 0;
}


String *ChunkLineSource::getLine(int lno){
  if (lines.size() <= lno) return null;
  return lines.elementAt(lno);
};
int ChunkLineSource::getLineCount(){
  return lines.size();
};


