
#ifndef _BUCKETLINESTORE_H_
#define _BUCKETLINESTORE_H_

#include "common/Vector.h"
#include "colorer/LineSource.h"

class BucketLinesStore : public LineSource
{
public:
    BucketLinesStore();
    ~BucketLinesStore();
    void readBucket(const String *fileName, const String *inputEncoding, bool tab2spaces);
    const String *getFileName();
    int getLineCount();
    String *getLine(int lno);
protected:
    void freeFile();
private:
    Vector<String*> lines;
    SString *fileName;
    void replaceTabs(int lno);

};

};

#endif
