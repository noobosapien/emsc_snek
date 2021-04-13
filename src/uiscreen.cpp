#include "uiscreen.h"

//UISCREEN

UIScreen::UIScreen(Game* game): mGame(game),
mTitle(nullptr), mBackground(nullptr), mTitlePos(0.0f, 300.0f), 
mSelected(nullptr), mUnSelected(nullptr),
mNextButtonPos(0.0f, 0.0f), mBGPos(0.0f, 250.0f), mState(EActive){
    mGame->pushUI(this);
    mFont = mGame->getFont("src/fonts/normal.ttf");
    mSelected = mGame->getTexture("src/textures/ButtonYellow.png");
    mUnSelected = mGame->getTexture("src/textures/ButtonBlue.png");
}

UIScreen::~UIScreen(){
    if(mTitle){
        delete mTitle;
    }

    for(auto b: mButtons){
        delete b;
    }

    mButtons.clear();
}

void UIScreen::update(float delta){

}

void UIScreen::draw(class Shader* textShader, class Shader* spriteShader){
    if(mBackground){
        drawTexture(spriteShader, mBackground, mBGPos);
    }

    if(mTitle){
        mTitle->drawText(textShader);
    }

    for(auto b: mButtons){
        if(b->getHighlighted()){
            b->setBackground(mSelected);
        }else{
            b->setBackground(mUnSelected);
        }

        b->drawBackground(spriteShader);
        b->drawText(textShader);
    }
}

void UIScreen::processInput(const SDL_Event& event){
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

void UIScreen::setTitle(const std::string& text, const glm::vec2& position, const glm::vec3& color, const glm::vec2& pointScale){
    if(mTitle){
        delete mTitle;
        mTitle = nullptr;
    }

    mTitle = new Text(mGame, position, mFont, text, pointScale, color);
}

void UIScreen::addButton(const std::string& name, std::function<void()> onClick){

    // glm::vec2 dims (static_cast<float>(mButtonOn->getWidth()), static_cast<float>(mButtonOn->getHeight()));
    glm::vec2 dims(10.f);
    Button* b = new Button(mGame, name, mFont, onClick, mNextButtonPos, dims);

    //call button setcolor and setPointSize

    mButtons.emplace_back(b);

    mNextButtonPos.y -= .08f; //mSelected->getHeight() + 
}


void UIScreen::drawTexture(class Shader* shader, class Texture* texture, const glm::vec2& offset, float scale){
    //todo
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}

void UIScreen::setRelativeMouseMode(bool relative){
    relative = false;
    if(relative){
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_GetRelativeMouseState(nullptr, nullptr);
    }else{
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}

//Button

Button::Button(Game* game, const std::string& name, class Font* font,std::function<void()> onClick, const glm::vec2& pos,
const glm::vec2& dims): 
mGame(game),
mOnClick(onClick), 
mNameText(nullptr),
mFont(font), 
mPosition(pos), 
mDimensions(dims),
mHighlighted(false),
mBackground(nullptr){
    setName(name);
}

Button::~Button(){
    if(mNameText){
        delete mNameText;
    }
}


void Button::setName(const std::string& name){
    mName = name;

    if(mNameText){
        delete mNameText;
        mNameText = nullptr;
    }

    //edit this later
    mNameText = new Text(mGame, glm::vec2(-0.01, 0.0), mFont, name, glm::vec2(.1, .1), glm::vec3(1.f, 0.f, 0.f));
}

void Button::setBackground(class Texture* tex){
    mBackground = tex;
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

    bool inside = ((pt.x < (mPosition.x + (mDimensions.x / 2))) && (pt.x > (mPosition.x - (mDimensions.x / 2))))
    &&
    ((pt.y < (mPosition.y + (mDimensions.y / 2))) && (pt.y > (mPosition.y - (mDimensions.y / 2))));

    return inside;
}

void Button::onClick(){
    if(mOnClick){
        mOnClick();
    }
}


void Button::drawBackground(class Shader* shader){
    shader->setMatrixUniform("u_projection", mGame->getCamera()->getUIProjection());
    mBackground->setActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}


void Button::drawText(class Shader* shader){
    mNameText->drawText(shader);
}

