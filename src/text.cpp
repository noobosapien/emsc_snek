#include "headers/gamepch.h"

Text::Text(class Game* game, glm::vec2 position, class Font* font, const std::string& string, glm::vec2 pointScale, glm::vec3 color):
mGame(game), mPosition(position), mFont(font), mString(string), mPointScale(pointScale), mColor(color){

    for(auto c = mString.begin(); c != mString.end(); c++){
        Character* ch = mFont->renderCharacter(*c);
        if(ch)
            mCharacters.emplace_back(ch);
    }

}


Text::~Text(){
    //font clears the characters
    mCharacters.clear();
}



void Text::drawText(class Shader* shader){

    unsigned int s = shader->setActive();
    shader->setVec3Uniform("u_textColor", mColor);
    shader->setMatrixUniform("u_projection", mGame->getCamera()->getUIProjection());

    float x = mPosition.x;
    float y = mPosition.y;

    for(auto ch: mCharacters){

        float xPos = (x + (ch->bearing.x * mPointScale.x) / Game::WIN_WIDTH) * Game::WIN_RES.x;
        float yPos = (y - ((ch->size.y - ch->bearing.y) * mPointScale.y) / Game::WIN_HEIGHT) * Game::WIN_RES.y;

        float w = ((float)ch->size.x * mPointScale.x / Game::WIN_WIDTH) * Game::WIN_RES.x;
        float h = ((float)ch->size.y * mPointScale.y / Game::WIN_HEIGHT) * Game::WIN_RES.y;


        float verts[] = {
            xPos, yPos + h, 0.f, 0.f,
            xPos, yPos, 0.f, 1.f,
            xPos + w, yPos, 1.f, 1.f,
            xPos + w, yPos + h, 1.f, 0.f,
        };

        ch->texture->setActive();

        shader->setBufferSubData(verts, 4, 4);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        x += ((float)(ch->advance >> 6) * mPointScale.x / Game::WIN_WIDTH) * Game::WIN_RES.x;
    }
}


void Text::setColor(const glm::vec3& color){
    mColor = color;
}


void Text::setPointScale(const glm::vec2& pointScale){
    mPointScale = pointScale;
}

