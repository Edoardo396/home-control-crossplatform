#pragma once

#include <vector>
#include <algorithm>
#include <unordered_map>

#define OUT
const int port = 8080;
#define ABSTRACT

typedef std::unordered_map<std::string, std::string> Dictionary;


/* FOREGROUND */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define DEVXMLTEXTOF(X) device->getNodeByPath(#X)->innerText()
#define XMLTEXTOF(X, Y) Y->getNodeByPath(#X)->innerText()

#define BOOLTOSTR(X) (X ? std::string("True") : std::string("False"))

#define FLOATTOSTR(X) boost::str(boost::format("%d") % X)