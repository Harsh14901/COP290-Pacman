#include "UI/ImageButton.hpp"

ImageButton::ImageButton(){}
ImageButton::ImageButton(int x,int y,int w, int h,string asset_clicked,string asset_unclicked){
    init(x,y,w,h,asset_clicked,asset_unclicked);
}

void ImageButton::init(int x,int y,int w, int h,string asset_clicked,string asset_unclicked){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->asset_clicked = asset_clicked;
    this->asset_unclicked = asset_unclicked;

}

void ImageButton::setRenderer(SDL_Renderer* _gRenderer){
    // this->_gRenderer = _gRenderer;

    texture_selected.setRenderer(_gRenderer);
    texture_unselected.setRenderer(_gRenderer);
    cout << "Loading from:" << asset_clicked << endl;
    texture_unselected.loadFromFile(asset_unclicked);
    texture_selected.loadFromFile(asset_clicked);

    texture_selected.set_image_dimenstions(w,h);
    texture_unselected.set_image_dimenstions(w,h);

}

void ImageButton::toggle_click(){
    is_clicked = !is_clicked;
}

void ImageButton::set_clicked(bool clicked){
    is_clicked = clicked;
}


void ImageButton::render() {
    if(is_clicked)   texture_selected.render(x,y);
    else texture_unselected.render(x,y);
}