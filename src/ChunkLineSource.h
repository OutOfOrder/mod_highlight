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

#ifndef _COLORER_CHUNKLINESSOURCE_H_
#define _COLORER_CHUNKLINESSOURCE_H_

#include <common/Vector.h>
#include <common/io/InputSource.h>
#include <colorer/LineSource.h>

#include <httpd.h>
#include <http_config.h>
#include <http_protocol.h>
#include <http_request.h>
#include <http_connection.h>
#include <http_log.h>
#include <apr_strings.h>
#include <apr_hash.h>
#include <apr_buckets.h>

class ChunkLineSource : public LineSource
{
public:

  ChunkLineSource(ap_filter_t* pFilter);
  ~ChunkLineSource();

  int ParseChunk(const char* chunk, int len);
  String *getLine(int lno);
  int getLineCount();
protected:
  void freeLines();
private:
  ap_filter_t* kFilter;
  Vector<String*> lines;
};
#endif


