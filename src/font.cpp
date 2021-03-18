#include "font.h"

Font::Font(Game* game): mGame(game){

}

Font::~Font(){

}

bool Font::load(const std::string& fileName){
    std::vector<int> fontSizes = {
        8, 9, 10, 11, 12, 14, 16, 18,
        20, 22, 24, 26, 28, 30, 32, 34,
        36, 38, 40, 42, 44, 46, 48, 52, 
        56, 60, 64, 68, 72
    };

    for(auto& size : fontSizes){
        TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
        if(font == nullptr){
            printf("Failed to load the font %s in size %d\n", fileName.c_str(), size);
            return false;
        }

        mFontData.emplace(size, font);
    }

    return true;
}

void Font::unLoad(){
    for(auto& font: mFontData){
        TTF_CloseFont(font.second);
    }
}

Texture* Font::renderText(const std::string& textKey, const glm::vec3& color, int pointSize){
    Texture* texture = nullptr;

    SDL_Color sdlColor;
    sdlColor.r = static_cast<uint8_t>(color.x * 255);
    sdlColor.g = static_cast<uint8_t>(color.y * 255);
    sdlColor.b = static_cast<uint8_t>(color.z * 255);
    sdlColor.a = 255;

    auto iter = mFontData.find(pointSize);

    if(iter != mFontData.end()){
        TTF_Font* font = iter->second;
        const std::string& actualText = mGame->getText(textKey);

        SDL_Surface* surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);

        if(surf != nullptr){
            texture = new Texture();
            texture->createFromSurface(surf);
            SDL_FreeSurface(surf);
        }
    }else{
        printf("Point size %d is unsupported", pointSize);
    }

    return texture;

}