#ifndef FONT_H
#define FONT_H

#include "game.h"

class Font{
    public:
        Font(class Game* game);
        ~Font();

        bool load(const std::string& fileName);
        void unLoad();

        class Texture* renderText(const std::string& textKey, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), int pointSize = 30);

    private:
        std::unordered_map<int, TTF_Font*> mFontData;
        class Game* mGame;
};

#endif