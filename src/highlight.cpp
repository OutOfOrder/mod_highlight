/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Colorer Library
 *
 * The Initial Developer of the Original Code is
 * Cail Lomecb <cail@nm.ru>.
 * Portions created by the Initial Developer are Copyright (C) 1999-2003
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */



// TODO: make this work :)

genOutput() {
    TextLinesStore textLinesStore;
    // parsers factory
    ParserFactory pf(catalogPath);
    // HRC loading
    HRCParser *hrcParser = pf.getHRCParser();
    // HRD RegionMapper creation
    bool useMarkup = false;
    RegionMapper *mapper = null;
    if (hrdName == null) hrdName = new DString("default");
    if (!useTokens){
      try{
        mapper = pf.createStyledMapper(&DString("rgb"), hrdName);
      }catch(ParserFactoryException &e){
        useMarkup = true;
        mapper = pf.createTextMapper(hrdName);
      };
    };
    // Base editor to make primary parse
    BaseEditor baseEditor(&pf, &textLinesStore);
    // Using compact regions
    baseEditor.setRegionCompact(true);
    baseEditor.setRegionMapper(mapper);
    baseEditor.lineCountEvent(textLinesStore.getLineCount());
    // Choosing file type
    FileType *type = selectType(hrcParser, textLinesStore.getLine(0));
    baseEditor.setFileType(type);
    // parsing all the text
    baseEditor.validate(-1);
    //  writing result into HTML colored stream...
    const RegionDefine *rd = null;
    if (mapper != null) rd = baseEditor.rd_def_Text;

    Writer *escapedWriter = null;
    Writer *commonWriter = null;
    try{
      if (outputFileName != null) commonWriter = new FileWriter(outputFileName, outputEncodingIndex, bomOutput);
      else commonWriter = new StreamWriter(stdout, outputEncodingIndex, bomOutput);
      if (htmlEscaping) escapedWriter = new HtmlEscapesWriter(commonWriter);
      else escapedWriter = commonWriter;
    }catch(Exception &e){
      fprintf(stderr, "can't open file '%s' for writing:\n", outputFileName->getChars());
      fprintf(stderr, e.getMessage()->getChars());
      return;
    };

    if (htmlWrapping && useTokens){
      commonWriter->write(DString("<html>\n<head>\n<style></style>\n</head>\n<body><pre>\n"));
    }else if (htmlWrapping && rd != null){
      if (useMarkup){
        commonWriter->write(TextRegion::cast(rd)->stext);
      }else{
        commonWriter->write(DString("<html><body style='"));
        ParsedLineWriter::writeStyle(commonWriter, StyledRegion::cast(rd));
        commonWriter->write(DString("'><pre>\n"));
      };
    };

    if (copyrightHeader){
      commonWriter->write(DString("Created with colorer-take5 library. Type '"));
      commonWriter->write(type->getName());
      commonWriter->write(DString("'\n\n"));
    };

    for(int i = 0; i < textLinesStore.getLineCount(); i++){
      if (useTokens){
        ParsedLineWriter::tokenWrite(commonWriter, escapedWriter, docLinkHash, textLinesStore.getLine(i), baseEditor.getLineRegions(i));
      }else if (useMarkup){
        ParsedLineWriter::markupWrite(commonWriter, escapedWriter, docLinkHash, textLinesStore.getLine(i), baseEditor.getLineRegions(i));
      }else{
        ParsedLineWriter::htmlRGBWrite(commonWriter, escapedWriter, docLinkHash, textLinesStore.getLine(i), baseEditor.getLineRegions(i));
      };
      commonWriter->write(DString("\n"));
    };

    if (htmlWrapping && useTokens){
      commonWriter->write(DString("</pre></body></html>\n"));
    }else if (htmlWrapping && rd != null){
      if (useMarkup){
        commonWriter->write(TextRegion::cast(rd)->etext);
      }else{
        commonWriter->write(DString("</pre></body></html>\n"));
      };
    };

    if (htmlEscaping) delete commonWriter;
    delete escapedWriter;

    delete mapper;
  }catch(Exception &e){
    fprintf(stderr, "%s\n", e.getMessage()->getChars());
  }catch(...){
    fprintf(stderr, "unknown exception ...\n");
  };
}



}
