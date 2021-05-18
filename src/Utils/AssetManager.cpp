#include "Utils/AssetManager.hpp"

AssetManager::AssetManager(){

    map<ThemeAssets,string> pacman_theme;
    pacman_theme.insert({ThemeAssets::FOLDER_NAME,"assets/pngs/pacman"});
    pacman_theme.insert({ThemeAssets::PACMAN_SPRITE,"pac_sprite.png"});
    pacman_theme.insert({ThemeAssets::ENEMY_SPRITE,"enemy_sprite.png"});
    pacman_theme.insert({ThemeAssets::VENT_PNG,"vent.png"});
    pacman_theme.insert({ThemeAssets::COIN_PNG,"coin.png"});
    pacman_theme.insert({ThemeAssets::COIN_SOUND,"coin_collect.mp3"});
    pacman_theme.insert({ThemeAssets::CHERRY_PNG,"cherry.png"});
    pacman_theme.insert({ThemeAssets::FREEZEBULLET_PNG,"freeze_bullet.png"});
    pacman_theme.insert({ThemeAssets::FREEZEBULLET_SOUND,"freezeBullet.wav"});

    data.insert({Themes::MASTER,pacman_theme});
    data.insert({Themes::PACMAN,pacman_theme});

    cout <<"YEP: " <<  data[selected_theme][PACMAN_SPRITE];
    init(Themes::MASTER);
}

string AssetManager::getFolderNameAppended(string fol,string val){
    return fol +"/"+val;
}

// Never call this twice
void AssetManager::init(Themes startTheme){
    selected_theme = startTheme;
    cout << "Initing" << endl;

    // First iterate over each theme avaialble in map
    // Note first theme shud be master, and it SHUD contain all keys
    // now iterate over ThemeAssets keys, if any is missing, pick from master else simply append the folder to the name and we are done
    // Now after closing all loops iterate over Themes and if any is missing, simply pick from master
    for (auto const& x : data)
    {
        auto theme_data = x.second;
        if(x.first==Themes::MASTER){
            cout << "Initialising theme " << "Master" << endl;
        }else{
            cout << "Initialising theme " << "Something Else, if it comes before Master, report issue" << endl;
        }
        for (ThemeAssets ta = ThemeAssets(ThemeAssets::FOLDER_NAME+1); ta < ThemeAssets::E_Last; ta = ThemeAssets(ta + 1)){
            if(theme_data.find(ta)==theme_data.end()){
                theme_data[ta] = data[Themes::MASTER][ta];
            }else{
                theme_data[ta] = getFolderNameAppended(theme_data[ThemeAssets::FOLDER_NAME],theme_data[ta]);
            }
        }
        data[x.first] = theme_data;
    }

    cout << "See this freeze bullet " << data[Themes::MASTER][ThemeAssets::FREEZEBULLET_PNG] << endl;
    cout << "See this cherry " << data[Themes::MASTER][ThemeAssets::CHERRY_PNG] << endl;

    for(Themes th = Themes::MASTER; th <= Themes::T_Last; th = Themes(th+1)){
        if(data.find(th)==data.end()){
            data[th] = data[Themes::MASTER];
            cout << "Initialising theme to master" << endl;
        }
    }

    // All Set!!!
}

void AssetManager::set_theme(Themes theme){
    this->selected_theme = theme;
}

Themes AssetManager::get_theme(){
    return selected_theme;
}

string AssetManager::get_asset(ThemeAssets key){
    cout << "See this asset " << data[selected_theme][key] << endl;
    cout << "See this freeze bullet " << data[selected_theme][ThemeAssets::FREEZEBULLET_PNG] << endl;
    cout << "See this cherry " << data[Themes::MASTER][ThemeAssets::CHERRY_PNG] << endl;

    return data[selected_theme][key];
}
