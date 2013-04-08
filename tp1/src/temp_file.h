#ifndef __TEMP_FILE_H__
#define __TEMP_FILE_H__

#include <fstream>
#include "utils.h"

using namespace std;

class TempFile {
  public:
    virtual ~TempFile() {};
    virtual int_id readInt() = 0;
    virtual void writeInt(int_id v) = 0;
    virtual void setSeekPos(const ifstream::pos_type pos) = 0;
    virtual ifstream::pos_type getSeekPos() = 0;
};

TempFile* tempFile();

#endif
