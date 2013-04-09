#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

typedef unsigned int int_id;

const string ERASE = "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";

string stripHttpHeaders(const string &text);

void lowerString(string &input);

string stripSpecialChars(const string &input);

struct IoException : public runtime_error {
    explicit IoException(const string &msg):runtime_error(msg){};
};


#endif
