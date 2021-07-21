#include "headers/gamepch.h"

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


    glm::vec2 dims(static_cast<float>(mSelected->getWidth()), static_cast<float>(mSelected->getHeight()));
    printf("button width = %d\n", mSelected->getWidth());
    printf("button Height = %d\n", mSelected->getHeight());
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
    std::cout << pt.x << ", " << pt.y << "     " << mPosition.x - (mDimensions.x / 2) << ", " << mPosition.y - (mDimensions.y / 2) << std::endl;
    
    glm::vec2 normPt = glm::vec2((float)(pt.x * Game::WIN_RES.x)/Game::WIN_WIDTH, (float)(pt.y * Game::WIN_RES.y)/Game::WIN_HEIGHT);
    glm::vec2 normPos = glm::vec2((float)(mPosition.x * Game::WIN_RES.x)/Game::WIN_WIDTH, (float)(mPosition.y * Game::WIN_RES.y)/Game::WIN_HEIGHT);
    glm::vec2 normDims = glm::vec2((float)(mDimensions.x)/Game::WIN_WIDTH, (float)(mDimensions.y)/Game::WIN_HEIGHT);
    
    // bool inside = ((pt.x < (mPosition.x + (mDimensions.x / 2))) && (pt.x > (mPosition.x - (mDimensions.x / 2))))
    // &&
    // ((pt.y < (mPosition.y + (mDimensions.y / 2))) && (pt.y > (mPosition.y - (mDimensions.y / 2))));

    // std::cout << pt.x << ", " << pt.y << "     " << normPos.x - (normDims.x) << ", " << normPos.y - (normDims.y) << "     "
    // << Game::WIN_RES.x << ", " << Game::WIN_RES.y 
    // << std::endl;

    bool inside = ((normPt.x < (normPos.x + (normDims.x))) && (normPt.x > (normPos.x - (normDims.x))))
    &&
    ((normPt.y < (normPos.y + (normDims.y))) && (normPt.y > (normPos.y - (normDims.y))));

    return inside;
}

void Button::onClick(){
    if(mOnClick){
        mOnClick();
    }
}


void Button::drawBackground(class Shader* shader){
    shader->setActive();

    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));

    shader->setMatrixUniform("u_model", model);
    shader->setMatrixUniform("u_viewproj", mGame->getCamera()->getUIViewProj());
    // shader->setMatrixUniform("u_viewproj", mGame->getCamera()->getUIProjection());

    // shader->setMatrixUniform("u_projection", mGame->getCamera()->getUIProjection());
    mBackground->setActive();
    // // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    // float x = ((float)mDimensions.x / Game::WIN_WIDTH) * Game::WIN_RES.x;
    // float y = ((float)mDimensions.y / Game::WIN_HEIGHT) * Game::WIN_RES.y;

    // // std::cout << x * Game::WIN_WIDTH / Game::WIN_RES.x << ", " << y * Game::WIN_HEIGHT / Game::WIN_RES.y << std::endl;

    // float verts[] = {
    //         -x, -y, 0.f, 0.f,
    //         -x, y, 0.f, 1.f,
    //         x, y, 1.f, 1.f,
    //         x, -y, 1.f, 0.f,
    //     };

    // shader->setBufferSubData(verts, 4, 4);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}


void Button::drawText(class Shader* shader){
    mNameText->drawText(shader);
}

