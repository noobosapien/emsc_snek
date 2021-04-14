#ifndef TEXT_H
#define TEXT_H

class Text{
    public:
        Text(class Game* game, glm::vec2 position, class Font* font, const std::string& string, glm::vec2 pointScale, glm::vec3 color);
        ~Text();

        void drawText(class Shader* shader);
        void setColor(const glm::vec3& color);
        void setPointScale(const glm::vec2& pointScale);

    private:
        std::string mString;
        class Font* mFont;
        class Game* mGame;
        glm::vec2 mPosition;
        glm::vec2 mPointScale;
        glm::vec3 mColor;
        std::vector<struct Character*> mCharacters;
};

#endif