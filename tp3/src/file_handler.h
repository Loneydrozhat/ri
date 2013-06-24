#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

#include <fstream>
#include "utils.h"

using namespace std;

class FileHandler {
  public:
    virtual ~FileHandler() {};
    virtual int_id readInt() = 0;
    virtual void writeInt(int_id v) = 0;
    virtual ifstream::pos_type readFilePointer() = 0;
    virtual void writeFilePointer(ifstream::pos_type v) = 0;
    virtual string readString() = 0;
    virtual void writeString(const string &input) = 0;
    virtual void setSeekPos(const ifstream::pos_type pos) = 0;
    virtual ifstream::pos_type getSeekPos() = 0;
    virtual void flush() = 0;
    virtual void close() = 0;
};

FileHandler* createFile(const string &name);

FileHandler* openFile(const string &name);

template<class T>
T readNumber(fstream &file) {
  if (file.eof()) {
    throw IoException("end of file");
  }
  const unsigned int sizeOfT = sizeof(T);
  char bytes[sizeOfT];
  file.read(bytes, sizeOfT);
  T* value = (T*) bytes;
  return *value;
}

template<class T>
void writeNumber(fstream &file, T v) {
  char* bytes = (char*) &v;
  file.write(bytes, sizeof(T));
}

#endif
