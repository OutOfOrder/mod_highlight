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


#include "ColorerFilter.h"
#include <colorer/ParserFactory.h>
#include <colorer/editor/BaseEditor.h>
#include <colorer/viewer/TextLinesStore.h>
#include <colorer/viewer/ParsedLineWriter.h>
#include <colorer/viewer/TextConsoleViewer.h>


ColorerFilter::ColorerFilter(ap_filter_t* pFilter) {
    kFilter = pFilter;
    pSource = new ChunkLineSource(kFilter);
    hrdName = new SString("default");
    inputFileName = new SString(kFilter->r->filename);
    typeDescription = NULL;
    //ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ColorerFilter::ColorerFilter()");
    kFilter->ctx = this;
}

ColorerFilter::~ColorerFilter() {
    // Free Anything else?
    delete pSource;
    delete hrdName;
    delete inputFileName;
    delete typeDescription;
}

int ColorerFilter::SendOutput() {
    //ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ColorerFilter::SendOutput()");

    ap_set_content_type(kFilter->r,"text/html; charset=utf-8");
    try {
    // parsers factory
    ParserFactory pf(NULL);
    // HRC loading
    HRCParser *hrcParser = pf.getHRCParser();

    RegionMapper *mapper = null;
    bool useMarkup = false;

    try {
        mapper = pf.createStyledMapper(&DString("rgb"), hrdName);
    }
    catch(ParserFactoryException &e) {
        useMarkup = true;
        mapper = pf.createTextMapper(hrdName);
    };

    // Base editor to make primary parse
    BaseEditor baseEditor(&pf, pSource);
    // Using compact regions
    baseEditor.setRegionCompact(true);
    baseEditor.setRegionMapper(mapper);
    baseEditor.lineCountEvent(pSource->getLineCount());
    // Choosing file type
    FileType *type = selectType(hrcParser, pSource->getLine(0));
    baseEditor.setFileType(type);
    // parsing all the text
    baseEditor.validate(-1);

    Hashtable<String*> *docLinkHash = new Hashtable<String*>;
    BucketWriter* myWriter = new BucketWriter(kFilter);
    //ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ColorerFilter::SendOutput(): Line Count: %d", pSource->getLineCount());    

    Writer* escapedWriter = new HtmlEscapesWriter(myWriter);
//    ((Writer*)myWriter)->write(DString("<html>\n<head>\n<style></style>\n</head>\n<body><pre>\n"));
    ((Writer*)myWriter)->write(DString("<pre>\n"));
    for(int i = 0; i < pSource->getLineCount(); i++){
//        tokenWrite(myWriter, myWriter, docLinkHash, pSource->getLine(i), baseEditor.getLineRegions(i));
//        tokenWrite(myWriter, myWriter, docLinkHash, pSource->getLine(i), baseEditor.getLineRegions(i));
        htmlRGBWrite(myWriter, escapedWriter, docLinkHash, pSource->getLine(i), baseEditor.getLineRegions(i));
    }
//    ((Writer*)myWriter)->write(DString("</pre></body></html>\n"));
    ((Writer*)myWriter)->write(DString("</pre>\n"));

    myWriter->SendBrigade();
    delete docLinkHash;
    delete escapedWriter;
    delete myWriter;
  }catch(Exception &e){
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ColorerFilter::SendOutput(): uncaught exception: %s",  e.getMessage()->getChars());    
    return 500;
  }catch(...){
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, kFilter->r, "ColorerFilter::SendOutput(): unknown exception!");    
    return 500;
  };
  return OK;
}

FileType *ColorerFilter::selectType(HRCParser *hrcParser, String *fline){
  FileType *type = null;
  if (typeDescription != null){
    type = hrcParser->getFileType(typeDescription);
    if (type == null){
      for(int idx = 0;; idx++){
        type = hrcParser->enumerateFileTypes(idx);
        if (type == null) break;
        if (type->getDescription() != null &&
            type->getDescription()->length() >= typeDescription->length() &&
            DString(type->getDescription(), 0, typeDescription->length()).equalsIgnoreCase(typeDescription))
          break;
        if (type->getName()->length() >= typeDescription->length() &&
            DString(type->getName(), 0, typeDescription->length()).equalsIgnoreCase(typeDescription))
          break;
        type = null;
      };
    };
  };
  if (typeDescription == null || type == null)
    type = hrcParser->chooseFileType(inputFileName, fline, 0);
  return type;
}

