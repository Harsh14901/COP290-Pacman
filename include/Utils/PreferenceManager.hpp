#pragma once


#include <iostream>
#include <map>
using namespace std;



class PreferenceManager{
public:
    PreferenceManager(bool init_defaults);
    void addSetting(string key,string default_val);
    string getSettingValue(string key);
    string setSettingValue(string key,string value);
    void init_standard_settings();

private:
    map<string,string> keySet;
    bool keyExists(string key);
};