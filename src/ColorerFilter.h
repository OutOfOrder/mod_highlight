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

#ifndef _COLORER_COLOERFILTER_H_
#define _COLORER_COLOERFILTER_H_

#include <httpd.h>
#include <http_config.h>
#include <http_protocol.h>
#include <http_request.h>
#include <http_connection.h>
#include <http_log.h>
#include <apr_strings.h>
#include <apr_hash.h>

#include "ChunkLineSource.h"
#include "BucketWriter.h"

#include <common/io/Writer.h>
#include <colorer/handlers/LineRegion.h>


class ColorerFilter {
public:
    ColorerFilter(ap_filter_t* pFilter, const char* cpath);
    ~ColorerFilter();

    /* who cares about being 'proper' OOP */
    ChunkLineSource* pSource;
    int SendOutput();
    FileType *selectType(HRCParser *hrcParser, String *fline);
private:
     String* hrdName;
     String* catalogPath;
     String* typeDescription;
     String* inputFileName;
     ap_filter_t* kFilter;

  static void tokenWrite(Writer *markupWriter, Writer *textWriter, Hashtable<String*> *docLinkHash, String *line, LineRegion *lineRegions){
    int pos = 0;
    for(LineRegion *l1 = lineRegions; l1; l1 = l1->next){
      if (l1->special || l1->region == null) continue;
      if (l1->start == l1->end) continue;
      int end = l1->end;
      if (end == -1) end = line->length();
      if (l1->start > pos){
        textWriter->write(line, pos, l1->start - pos);
        pos = l1->start;
      };
      String *token = l1->region->getName()->replace(DString(":"),DString("_"));
      markupWriter->write(DString("<span class='"));
      markupWriter->write(token);
      markupWriter->write(DString("'>"));
      textWriter->write(line, pos, end - l1->start);
      markupWriter->write(DString("</span>\n"));
      delete token;
      pos += end - l1->start;
    };
  };
  /** Write specified line of text using list of LineRegion's.
      This method uses integer fields of LineRegion class
      to enwrap each line region with generated HTML markup.
      Each region is
      @param markupWriter Writer, used for markup output
      @param textWriter Writer, used for text output
      @param line Line of text
      @param lineRegions Linked list of LineRegion structures
  */
  static void htmlRGBWrite(Writer *markupWriter, Writer *textWriter, Hashtable<String*> *docLinkHash, String *line, LineRegion *lineRegions){
    int pos = 0;
    for(LineRegion *l1 = lineRegions; l1; l1 = l1->next){
      if (l1->special || l1->rdef == null) continue;
      if (l1->start == l1->end) continue;
      int end = l1->end;
      if (end == -1) end = line->length();
      if (l1->start > pos){
        textWriter->write(line, pos, l1->start - pos);
        pos = l1->start;
      };
      if (docLinkHash->size() > 0)
        writeHref(markupWriter, docLinkHash, l1->scheme, DString(line, pos, end - l1->start), true);
      writeStart(markupWriter, l1->styled());
      textWriter->write(line, pos, end - l1->start);
      writeEnd(markupWriter, l1->styled());
      if (docLinkHash->size() > 0)
        writeHref(markupWriter, docLinkHash, l1->scheme, DString(line, pos, end - l1->start), false);
      pos += end - l1->start;
    };
    if (pos < line->length()){
      textWriter->write(line, pos, line->length() - pos);
    };
    markupWriter->write(DString("\n"));
  };
  /** Puts into stream starting HTML \<span> tag with requested style specification
  */
  static void writeStart(Writer *writer, const StyledRegion *lr){
    if (!lr->bfore && !lr->bback) return;
    writer->write(DString("<span style='"));
    writeStyle(writer, lr);
    writer->write(DString("'>"));
  };
  /** Puts into stream ending HTML \</span> tag
  */
  static void writeEnd(Writer *writer, const StyledRegion *lr){
    if (!lr->bfore && !lr->bback) return;
    writer->write(DString("</span>"));
  };

  static void writeHref(Writer *writer, Hashtable<String*> *docLinkHash, const Scheme *scheme, const String &token, bool start){
    String *url = null;
    if (scheme != null){
      url = docLinkHash->get(&(StringBuffer(token).append(DString("--")).append(scheme->getName())));
    };
    if (url == null){
      url = docLinkHash->get(&token);
    };
    if (url != null){
      if (start) writer->write(StringBuffer("<a href='")+url+DString("'>"));
      else writer->write(DString("</a>"));
    };
  };
  /** Puts into stream style attributes from RegionDefine object.
  */
  static void writeStyle(Writer *writer, const StyledRegion *lr){
    static char span[256];
    int cp = 0;
    if (lr->bfore) cp += sprintf(span, "color:#%.6x; ", lr->fore);
    if (lr->bback) cp += sprintf(span+cp, "background:#%.6x; ", lr->back);
    if (lr->style&StyledRegion::RD_BOLD) cp += sprintf(span+cp, "font-weight:bold; ");
    if (lr->style&StyledRegion::RD_ITALIC) cp += sprintf(span+cp, "font-style:italic; ");
    if (lr->style&StyledRegion::RD_UNDERLINE) cp += sprintf(span+cp, "text-decoration:underline; ");
    if (lr->style&StyledRegion::RD_STRIKEOUT) cp += sprintf(span+cp, "text-decoration:strikeout; ");
    if (cp > 0) writer->write(DString(span));
  };

};

class HtmlEscapesWriter : public Writer {
public:
  HtmlEscapesWriter(Writer *writer){
    this->writer = writer;
  };
  void write(wchar c){
    if (c == '&'){
      writer->write(DString("&amp;"));
    }
    else if (c == '<') {
      writer->write(DString("&lt;"));
    }
    else {
      writer->write(c);
    }
  };
protected:
  Writer *writer;
};


#endif


