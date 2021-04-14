#include "headers/gamepch.h"

Font::Font(Game* game): mGame(game){

}

Font::~Font(){
    unLoad();
}

bool Font::loadCharacters(const std::string& pathName, int pointSize){
    FT_Face face;
    if(FT_New_Face(mGame->getFtLib(), pathName.c_str(), 0, &face)){
        printf("ERROR::FREETYPE could not load the font\n");
        return false;
    }

    if(FT_Set_Pixel_Sizes(face, 0, pointSize)){
        printf("ERROR::FREETYPE could not load the pointsize of the font\n");
    }

    for(unsigned char c = 0; c < 128; c++){
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
                printf("ERROR::FREETYPE failed to load the glyph\n");
                return false;
        }

        Texture* tex = new Texture();

        if(!tex->loadFromGlyph(face)){
            printf("Failed to load texture from glyph\n");
            return false;
        }

        Character* ch = new Character{
            tex,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };

        mCharacters.emplace(c, ch);

    }

    FT_Done_Face(face);
    return true;
}

void Font::unLoad(){
    for(auto ch: mCharacters){
        delete ch.second;
    }
    mCharacters.clear();
}

Character* Font::renderCharacter(char c){
    auto ch = mCharacters.find(c);
    if(ch != mCharacters.end())
        return ch->second;
    return nullptr;
}

