//
// Created by 汪明 on 2023/6/8.
//

#ifndef WANGMING_JSONSERIALIZATION_H
#define WANGMING_JSONSERIALIZATION_H
#include <string>
#include "JSON.h"
using namespace std;
class JSONSerialization {
public:
    JSONSerialization(const string & json);
    JSONSerialization(const char * json);
    JSON parse();
private:
    string m_json;
};


#endif //WANGMING_JSONSERIALIZATION_H
