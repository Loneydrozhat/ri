#include <iostream>
#include "temp_file.h"
#include "utils.h"

class TempFileImpl : public TempFile {
  public:
    TempFileImpl() {
      file_.open ("buffer.tmp", ios::in | ios::out | ios::binary | ios::trunc | ios::ate);
      if (!file_.is_open()) {
        throw IoException("could not open temp file");
      }
      size_ = file_.tellg();
      setSeekPos(0);
    }
    virtual void setSeekPos(const ifstream::pos_type pos) {
      file_.seekg (pos);
      file_.seekp (pos);
    }
    virtual int_id readInt() {
      if (file_.eof()) {
        throw IoException("end of file");
      }
      char bytes[sizeof(int_id)];
      file_.read(bytes, sizeof(int_id));
      int_id* value = (int_id*) bytes;
      return *value;
    }
    virtual void writeInt(int_id v) {
      char* bytes = (char*) &v;
      file_.write(bytes, sizeof(int_id));
    }
    virtual ifstream::pos_type getSeekPos() {
      return file_.tellg();
    }

  private:
    fstream file_;
    ifstream::pos_type size_;
    virtual ~TempFileImpl() {
      file_.close();
    };
};

TempFile* tempFile() {
  return new TempFileImpl();
}
