#include <iostream>
#include "file_handler.h"
#include "utils.h"

using namespace std;

class FileHandlerImpl : public FileHandler {
  public:
    FileHandlerImpl(const string &filename, bool truncate) {
      ios_base::openmode flags = ios::in | ios::out | ios::binary | ios::ate;
      if (truncate) {
        flags = flags | ios::trunc;
      }
      file_.open(filename, flags);
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
      return readNumber<int_id>(file_);
    }

    virtual void writeInt(int_id v) {
      writeNumber<int_id>(file_, v);
    }

    virtual ifstream::pos_type readFilePointer() {
      return readNumber<ifstream::pos_type>(file_);
    }
    virtual void writeFilePointer(ifstream::pos_type v) {
      writeNumber<ifstream::pos_type>(file_, v); 
    }

    virtual string readString() {
      if (file_.eof()) {
        throw IoException("end of file");
      }
      string output;
      for (char byte; true;) {
        file_.read(&byte, 1);
        if (byte == '\0') {
          break;
        } else {
          output.append(1, byte);
        }
      }
      return output;
    }

    virtual void writeString(const string &input) {
      file_.write(input.c_str(), input.length() + 1);
    }

    virtual ifstream::pos_type getSeekPos() {
      return file_.tellg();
    }

  private:
    fstream file_;
    ifstream::pos_type size_;
    virtual ~FileHandlerImpl() {
      file_.close();
    };
};

FileHandler* createFile(const string &name) {
  return new FileHandlerImpl(name, true);
}

FileHandler* openFile(const string &name) {
  return new FileHandlerImpl(name, false);
}
