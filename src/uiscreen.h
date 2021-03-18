#ifndef UISCREEN_H
#define UISCREEN_H

#include "game.h"
#include <functional>

class Button{
    public:
        Button(const std::string& name, class Font* font,
        std::function<void()> onClick, const glm::vec2& pos, 
        const glm::vec2& dims);

        ~Button();

        void setName(const std::string& name);

        class Texture* getNameTex();
        const glm::vec2& getPosition() const;
        void setHighlighted(bool sel);
        bool getHighlighted();
        bool containsPoint(const glm::vec2& pt) const;
        void onClick();

    private:
        std::function<void()> mOnClick;
        std::string mName;
        class Texture* mNameTex;
        class Font* mFont;
        glm::vec2 mPosition;
        glm::vec2 mDimensions;
        bool mHighlighted;

};

class UIScreen{
    public:
        UIScreen(class Game* game);
        virtual ~UIScreen();

        virtual void update(float delta);
        virtual void draw(class Shader* shader);
        virtual void processInput(const SDL_Event& evet);
        virtual void handleKeyPress(int key);

        enum UIState{
            EActive,
            EClosing
        };

        void close();
        UIState getState();
        void setTitle(const std::string& text, const glm::vec3& color = glm::vec3(0.0f), int pointSize = 40);
        void addButton(const std::string& name, std::function<void()> onClick);
    
    protected:
        void drawTexture(class Shader* shader, class Texture* texture, const glm::vec2& offset = glm::vec2(0.0f), float scale = 1.0f);
        void setRelativeMouseMode(bool relative);

        class Game* mGame;
        class Font* mFont;

        class Texture* mTitle;
        class Texture* mBackground;
        class Texture* mButtonOn;
        class Texture* mButtonOff;

        glm::vec2 mTitlePos;
        glm::vec2 mNextButtonPos;
        glm::vec2 mBGPos;
        UIState mState;
        std::vector<class Button*> mButtons;

};

#endif