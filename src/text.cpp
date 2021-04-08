#include "text.h"

Text::Text(class Game* game, class Font* font, const std::string& string, int pointSize, glm::vec3 color):
mGame(game), mFont(font), mString(string), mPointSize(pointSize), mColor(color){

    for(auto c = mString.begin(); c != mString.end(); c++){
        Character* ch = mFont->renderCharacter(*c);
        mCharacters.emplace_back(ch);
    }

}


Text::~Text(){
    //font clears the characters
    mCharacters.clear();
}



void Text::drawText(class Shader* shader, const glm::vec2& position){
    glm::mat4 projection = glm::ortho(0.f, 800.f, 0.f, 600.f);

    unsigned int s = shader->setActive();
    shader->setVec3Uniform("u_textColor", mColor);
    shader->setMatrixUniform("u_projection", mGame->getCamera()->getViewProj());

    float x = position.x;
    float y = position.y;

    for(auto ch: mCharacters){
        float xPos = x + ch->bearing.x * mPointSize;
        float yPos = y - (ch->size.y - ch->bearing.y) * mPointSize;

        float w = ch->size.x * mPointSize;
        float h = ch->size.y * mPointSize;

        float verts[] = {
            xPos, yPos + h, 0.f, 0.f,
            xPos, yPos, 0.f, 1.f,
            xPos + w, yPos, 1.f, 1.f,
            xPos + w, yPos + h, 1.f, 0.f,
        };
        
        ch->texture->setActive();

        shader->setBufferSubData(verts, 4, 4);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        x += (ch->advance >> 6) * mPointSize;
    }
}


void Text::setColor(const glm::vec3& color){
    mColor = color;
}


void Text::setPointSize(int pointSize){
    mPointSize = pointSize;
}

