#include "uiscreen.h"

//UISCREEN

UIScreen::UIScreen(Game* game): mGame(game),
mTitle(nullptr), mBackground(nullptr), mTitlePos(0.0f, 300.0f),
mNextButtonPos(0.0f, 200.0f), mBGPos(0.0f, 250.0f), mState(EActive){

mGame->pushUI(this);
// mFont = mGame->getFont("fonts/normal.ttf");
mButtonOn = mGame->getTexture("textures/buttonOn.png");
mButtonOff = mGame->getTexture("textures/buttonOff.png");

}

UIScreen::~UIScreen(){
    if(mTitle){
        mTitle->unload();
        delete mTitle;
    }

    for(auto b: mButtons){
        delete b;
    }
    mButtons.clear();
}

void UIScreen::update(float delta){

}

void UIScreen::draw(class Shader* shader){
    if(mBackground){
        drawTexture(shader, mBackground, mBGPos);
    }

    if(mTitle){
        drawTexture(shader, mTitle, mTitlePos);
    }

    for(auto b: mButtons){
        Texture* tex = b->getHighlighted() ? mButtonOn : mButtonOff;
        drawTexture(shader, tex, b->getPosition());
        drawTexture(shader, b->getNameTex(), b->getPosition());
    }
}

void UIScreen::processInput(const uint8_t* keys){
    if(!mButtons.empty()){
        int x, y;
        SDL_GetMouseState(&x, &y);
        
        glm::vec2 mousePos(static_cast<float>(x), static_cast<float>(y));

        mousePos.x -= Game::WIN_WIDTH * 0.5f;
        mousePos.y = Game::WIN_HEIGHT * 0.5f - mousePos.y;

        for(auto b: mButtons){
            if(b->containsPoint(mousePos)){
                b->setHighlighted(true);
            }else{
                b->setHighlighted(false);
            }
        }
    }
}

void UIScreen::handleKeyPress(int key){
    switch (key){
        case SDL_BUTTON_LEFT:
            if(!mButtons.empty()){
                for(auto b: mButtons){
                    if(b->getHighlighted()){
                        b->onClick();
                        break;
                    }
                }
            }
            break;

        default:
            break;
    }
}

void UIScreen::close(){
    mState = EClosing;
}

UIScreen::UIState UIScreen::getState(){
    return mState;
}

void UIScreen::setTitle(const std::string& text, const glm::vec3& color, int pointSize){
    if(mTitle){
        mTitle->unload();
        delete mTitle;
        mTitle = nullptr;
    }

    // mTitle = mFont->renderText(text, color, pointSize);
}

void UIScreen::addButton(const std::string& name, std::function<void()> onClick){
    glm::vec2 dims (static_cast<float>(mButtonOn->getWidth()), static_cast<float>(mButtonOn->getHeight()));
    Button* b = new Button(name, mFont, onClick, mNextButtonPos, dims);
    mButtons.emplace_back(b);

    mNextButtonPos.y -= mButtonOff->getHeight() + 20.0f;
}


void UIScreen::drawTexture(class Shader* shader, class Texture* texture, const glm::vec2& offset, float scale){

    glm::mat4 scaleMat(1.f);    //create a scale matrix
    glm::mat4 transMatrix(1.f); //create a translate matrix from offset
    glm::mat4 world = scaleMat * transMatrix;
    shader->setMatrixUniform("uWorldTransform", world);

    texture->setActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}

void UIScreen::setRelativeMouseMode(bool relative){
    if(relative){
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_GetRelativeMouseState(nullptr, nullptr);
    }else{
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}

//Button

Button::Button(const std::string& name, class Font* font,std::function<void()> onClick, const glm::vec2& pos, const glm::vec2& dims): 
mOnClick(onClick), 
mNameTex(nullptr),
mFont(font), 
mPosition(pos), 
mDimensions(dims), 
mHighlighted(false){
    setName(name);
}

Button::~Button(){
    if(mNameTex){
        mNameTex->unload();
        delete mNameTex;
    }
}


void Button::setName(const std::string& name){
    mName = name;

    if(mNameTex){
        mNameTex->unload();
        delete mNameTex;
        mNameTex = nullptr;
    }

    // mNameTex = mFont->renderText(mName);
}


class Texture* Button::getNameTex(){
    return mNameTex;
}

const glm::vec2& Button::getPosition() const{
    return mPosition;
}

void Button::setHighlighted(bool sel){
    mHighlighted = sel;
}

bool Button::getHighlighted(){
    return mHighlighted;
}

bool Button::containsPoint(const glm::vec2& pt) const{
    bool no = pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
    pt.x > (mPosition.x + mDimensions.x / 2.0f) ||
    pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
    pt.y > (mPosition.y + mDimensions.y / 2.0f);

    return !no;
}

void Button::onClick(){
    if(mOnClick){
        mOnClick();
    }
}
