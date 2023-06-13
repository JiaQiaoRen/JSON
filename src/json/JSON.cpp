//
// Created by 汪明 on 2023/6/8.
//

#include <sstream>
#include "JSON.h"
#include "JSONSerialization.h"

static  JSON * const nullJSON = new JSON();

JSON::JSON(): m_type(NullType){}

JSON::~JSON() {
}

JSON::JSON(int value) :m_type(IntType){
    m_value.m_int = value;
}
JSON::JSON(double value) {
    m_type = DoubleType;
    m_value.m_double = value;
}

JSON::JSON(const char *value) {
    m_type = StringType;
    m_value.m_string = new string(value);
}

JSON::JSON(const string &value) {
    m_type = StringType;
    m_value.m_string = new string(value);
}
JSON::JSON(const JSON &other) {
    copy(other);
}

JSON::Type JSON::type() const {
    return m_type;
}

int JSON::intValue() const {
    switch (m_type) {
        case NullType:
            return 0;
        case BoolType:
            return m_value.m_bool ? 1 : 0;
        case IntType:
            return m_value.m_int;
        case DoubleType:
            return (int)m_value.m_double;
        case StringType:
            return atoi(m_value.m_string->c_str());
        default:
            return 0;
    }
}
bool JSON::boolValue() const {
    switch (m_type) {
        case NullType:
            return false;
        case BoolType:
            return m_value.m_bool;
        case IntType:
            return m_value.m_int > 0;
        case DoubleType:
            return m_value.m_double > 0;
        case StringType:
            if (m_value.m_string->compare("true") == 0) {
                return true;
            } else if (m_value.m_string->compare("YES") == 0) {
                return true;
            }
            return false;
        default:
            return false;
    }
}
double JSON::doubleValue() const {
    switch (m_type) {
        case IntType:
            return (double)m_value.m_int;
        case DoubleType:
            return m_value.m_double;
        case StringType:
            return atof(m_value.m_string->c_str());
        default:
            return 0;
    }
}

string JSON::stringValue() const {
    switch (m_type) {
        case IntType:
            return to_string(m_value.m_int);
        case DoubleType:
            return to_string(m_value.m_double);
        case StringType:
            return *m_value.m_string;
        default:
            return "";
    }
}
JSON::operator int() const {
    return intValue();
}
JSON::operator bool() const {
    return boolValue();
}
JSON::operator double() const {
    return doubleValue();
}
JSON::operator string() const {
    return stringValue();
}

JSON &JSON::operator[](int index) const{
    if (index < 0) {
        return *nullJSON;
    }
    if (m_type != ArrayType) {
        return *nullJSON;
    }
    auto size = m_value.m_array->size();
    if (index >= size) {
        return *nullJSON;
    }
    auto it = m_value.m_array->begin();
    for (int i = 0; i < index; ++i) {
        it++;
    }
    return *it;
}
JSON &JSON::operator[](const char *key) {
    string name = key;
    return (*this)[name];
}
JSON &JSON::operator[](const string &key)  {
    if (m_type != ObjectType) {
        clear();
        m_type = ObjectType;
        m_value.m_object = new map<string,JSON>();
    }
    return (*m_value.m_object)[key];
}

void JSON::clear() {
    switch (m_type) {
        case NullType:
        case BoolType:
        case IntType:
        case DoubleType:
            break;
        case StringType:
            delete m_value.m_string;
            break;
        case ArrayType:
        {
            for (auto it = m_value.m_array->begin();it != m_value.m_array->end();it++) {
                it->clear();
            }
            delete m_value.m_array;
        }
            break;
        case ObjectType:
        {
            for (auto it = m_value.m_object->begin();it != m_value.m_object->end();it++) {
                it->second.clear();
            }
            delete m_value.m_object;
        }
            break;
    }
}

string JSON::jsonString() const {
    stringstream ss;
    switch (m_type) {
        case NullType:
            ss << "null";
            break;
        case BoolType:
            ss << (m_value.m_bool ? "true" : "false");
            break;
        case IntType:
            ss << m_value.m_int;
            break;
        case DoubleType:
            ss << m_value.m_double;
            break;
        case StringType:
            ss << "\"" << *m_value.m_string << "\"";
            break;
        case ArrayType:
        {
            ss << "[";
            for(auto it = m_value.m_array->begin();it != m_value.m_array->end();it++){
                if (it != m_value.m_array->begin()) {
                    ss << ",";
                }
                ss << it->jsonString();
            }
            ss << "]";
        }
            break;
        case ObjectType:
        {
            ss << "{";
            for (auto it=m_value.m_object->begin();it != m_value.m_object->end();it++) {
                if (it != m_value.m_object->begin()) {
                    ss << ",";
                }
                ss << "\"" << it->first << "\":" << it->second.jsonString();
            }
            ss << "}";
        }
            break;
        default:
            break;
    }
    return ss.str();
}

void JSON::copy(const JSON &other) {
    m_type = other.m_type;
    m_value = other.m_value;
}

JSON &JSON::operator=(const JSON &other) {
    clear();
    copy(other);
    return *this;
}

JSON::JSON(JSON::Type type) {
    switch (m_type) {
        case NullType:
            break;
        case BoolType:
            m_value.m_bool = false;
            break;
        case IntType:
            m_value.m_int = 0;
            break;
        case DoubleType:
            m_value.m_double = 0.0;
            break;
        case StringType:
            m_value.m_string = new std::string("");
            break;
        case ArrayType:
            m_value.m_array = new std::list<JSON>();
            break;
        case ObjectType:
            m_value.m_object = new std::map<std::string, JSON>();
            break;
        default:
            break;
    }
}

void JSON::append(const JSON &value) {
    if (m_type != ArrayType) {
        clear();
        m_type = ArrayType;
        m_value.m_array = new list<JSON>();
    }
    m_value.m_array->push_back(value);
}

JSON JSON::parse(const char *json) {
    JSONSerialization serialization(json);
    return serialization.parse();
}

JSON JSON::parse(const string &json) {
    JSONSerialization serialization(json);
    return serialization.parse();
}



