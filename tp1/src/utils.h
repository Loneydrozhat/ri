#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <stdexcept>
#include <string>

typedef unsigned int int_id;

using namespace std;

string stripHttpHeaders(const string &text);

void lowerString(string &input);

string stripSpecialChars(const string &input);

struct IoException : public runtime_error {
    explicit IoException(const string &msg):runtime_error(msg){};
};

const string ERASE = "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";

#endif
