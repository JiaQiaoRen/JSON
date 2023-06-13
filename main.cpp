//
// Created by 汪明 on 2023/6/8.
//
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "src/json/JSON.h"


using namespace std;

void test1(){
    ifstream file("../test/key_word_search_v212.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }
    std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    file.close();
    JSON json = JSON::parse(content);
    cout << json.jsonString();
}
int main() {
    test1();
    return 0;
}
