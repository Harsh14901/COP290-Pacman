#include "Utils/PreferenceManager.hpp"


PreferenceManager::PreferenceManager(bool init_defaults){
    if(init_defaults){
        init_standard_settings();
    }
}


bool PreferenceManager::keyExists(string key){
    return false;//keySet.find(key) == keySet.end();
}

void PreferenceManager::addSetting(string key,string default_val){
    if(keyExists(key)){
        clog << "Key Already Exists" << endl;
        return;
    }
    keySet.insert(pair<string,string>(key,default_val));
}

string PreferenceManager::getSettingValue(string key){
    if(!keyExists(key)){
        clog << "Key Doesnt Exists" << endl;
        return "";
    }
    return keySet[key];
    return "";
}

string PreferenceManager::setSettingValue(string key,string value){
    if(!keyExists(key)){
        clog << "Key Doesnt Exists" << endl;
        return "";
    }
    keySet[key] = value;
    return "";
}

void PreferenceManager::init_standard_settings(){
    addSetting("music","true");
    addSetting("sfx","true");
}
