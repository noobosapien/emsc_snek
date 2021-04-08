#ifndef TEXT_H
#define TEXT_H

#include "game.h"
#include "font.h"

class Text{
    public:
        Text(class Game* game, class Font* font, const std::string& string, int pointSize, glm::vec3 color);
        ~Text();

        void drawText(class Shader* shader, const glm::vec2& position);
        void setColor(const glm::vec3& color);
        void setPointSize(int pointSize);

    private:
        std::string mString;
        class Font* mFont;
        class Game* mGame;
        int mPointSize;
        glm::vec3 mColor;
        std::vector<struct Character*> mCharacters;
};

#endif