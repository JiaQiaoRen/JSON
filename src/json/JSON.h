//
// Created by 汪明 on 2023/6/8.
//

#ifndef WANGMING_JSON_H
#define WANGMING_JSON_H

#include <string>
#include <list>
#include <map>
using namespace std;

class JSON {
public:
    enum Type {
        NullType = 0,
        BoolType,
        IntType,
        DoubleType,
        StringType,
        ObjectType,
        ArrayType
    };
    JSON();
    JSON(Type type);
    JSON(int value);
    JSON(double value);
    JSON(const char * value);
    JSON(const string & value);
    JSON(const JSON &other);
    static JSON parse(const char * json);
    static JSON parse(const string & json);
    ~JSON();
    Type type() const;
    int intValue() const;
    double doubleValue() const;
    string stringValue() const;
    bool boolValue() const;
    operator int() const;
    operator double() const;
    operator bool() const;
    operator string() const;

    JSON & operator = (const JSON &other);

    JSON& operator[] (int index) const;
    JSON& operator[] (const char *key);
    JSON& operator[] (const string & key) ;
    /*销毁json对象内部*/
    void clear();

    string jsonString() const;

    std::list<JSON>::iterator begin() {
        return m_value.m_array->begin();
    }
    std::list<JSON>::iterator end() {
        return m_value.m_array->end();
    }
    void append(const JSON &value);
private:
    void copy(const JSON & other);
    union Value {
        bool m_bool;
        int m_int;
        double m_double;
        string * m_string;
        list<JSON> * m_array;
        map<string,JSON> * m_object;
    };
    Value m_value;
    Type m_type;
};


#endif //WANGMING_JSON_H
