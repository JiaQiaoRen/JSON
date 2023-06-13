//
// Created by 汪明 on 2023/6/8.
//

#include "JSONSerialization.h"
#include <iostream>

#define InRange(x,lower,upper) (x >= lower && x <= upper)
JSON JSONSerializationParse(const string & json, int &index);
void JSONSerializationSkipWhiteSpace(const string & json, int &index) {
    while (json[index] == ' ' || json[index] == '\n' || json[index] == '\r' || json[index] == '\t') {
        index += 1;
    }
    if (json[index] == '/' && json[index + 1] == '/') {
        index += 2;
        while (json[index] != '\n'){
            index += 1;
        }
        JSONSerializationSkipWhiteSpace(json,index);
    }
}
JSON JSONSerializationParseNumber(const string & json, int &index) {
    size_t pos = index;
    if (json[index] == '-') {
        index += 1;
    }
    while (InRange(json[index],'0','9') || json[index] == 'e' || json[index] == 'E'){
        index += 1;
    }
    if (json[index] == '.') {
        index += 1;
        while (InRange(json[index],'0','9') || json[index] == 'e' || json[index] == 'E'){
            index += 1;
        }
        auto v = atof(json.c_str() + pos);
        return JSON(v);
    }
    return JSON(atoi(json.c_str() + pos));
}

JSON JSONSerializationParseNull(const string & json, int &index){
    if (json.compare(index,4,"null") == 0) {
        index += 4;
        return JSON();
    }
    throw std::logic_error("JSON解析Null出错");
}

JSON JSONSerializationParseBool(const string & json, int &index){
    if (json.compare(index,4,"true") == 0) {
        index += 4;
        return JSON(true);
    }else if (json.compare(index,5,"false") == 0) {
        index += 5;
        return JSON(false);
    }
    throw std::logic_error("JSON解析Bool出错");
}

JSON JSONSerializationParseString(const string & json, int &index){
    auto pos = index + 1;
    auto size = json.size();
    while (true) {
        index += 1;
        if (index >= size) {
            throw std::logic_error("JSON解析String出错");
        }
        if (json[index] == '"') {
            break;
        }
    }
    auto str = json.substr(pos,index - pos);
    index += 1;
    return JSON(str);
}
JSON JSONSerializationParseObject(const string & json, int &index){
    JSONSerializationSkipWhiteSpace(json,index);
    JSON object(JSON::ObjectType);
    index += 1;
    JSONSerializationSkipWhiteSpace(json,index);
    if (json[index] == '}') {
        index += 1;
        return object;
    }
    if (json[index] != '"') {
        throw std::logic_error("JSON解析Object出错: \" ");
    }
    while (true) {
        auto key = JSONSerializationParseString(json,index);
        JSONSerializationSkipWhiteSpace(json,index);
        auto ch = json[index];
        if (ch != ':') {
            throw std::logic_error("JSON解析Object出错: :");
        }
        index += 1;
        JSONSerializationSkipWhiteSpace(json,index);
        auto value = JSONSerializationParse(json,index);
        object[key.stringValue()] = value;
        JSONSerializationSkipWhiteSpace(json,index);
        ch = json[index];
        if (ch == '}') {
            break;
        }
        if (ch != ',') {
            throw std::logic_error("JSON解析Object出错: ,");
        }
        index += 1;
        JSONSerializationSkipWhiteSpace(json,index);
    }
    index += 1;
    return object;
}
JSON JSONSerializationParseArray(const string & json, int &index) {
    JSONSerializationSkipWhiteSpace(json,index);
    JSON array(JSON::ArrayType);
    index += 1;
    JSONSerializationSkipWhiteSpace(json,index);
    if (json[index] == ']') {
        index += 1;
        return array;
    }
    while (true) {
       auto value = JSONSerializationParse(json,index);
       array.append(value);
       JSONSerializationSkipWhiteSpace(json,index);
        auto ch = json[index];
        if (ch == ']') {
            break;
        }
        if (ch != ',') {
            throw std::logic_error("解析JSON出错7");
        }
        index += 1;
        JSONSerializationSkipWhiteSpace(json,index);
    }
    index += 1;
    return array;
}
JSON JSONSerializationParse(const string & json, int &index) {
    JSONSerializationSkipWhiteSpace(json,index);
    auto token = json[index];
    switch (token) {
        case 'n':
            return JSONSerializationParseNull(json,index);
        case 't':
        case 'f':
            return JSONSerializationParseBool(json,index);
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return JSONSerializationParseNumber(json,index);
        case '"':
            return JSONSerializationParseString(json,index);
        case '[':
            return JSONSerializationParseArray(json,index);
        case '{':
            return JSONSerializationParseObject(json,index);
    }
    return JSON();
}


JSONSerialization::JSONSerialization(const string &json) {
    m_json = json;
}

JSONSerialization::JSONSerialization(const char *json) {
    m_json = string (json);
}

JSON JSONSerialization::parse() {
    auto size = m_json.size();
    if (size == 0) {
        return JSON();
    }
    int index = 0;
    return JSONSerializationParse(m_json,index);
}



