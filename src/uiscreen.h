#ifndef UISCREEN_H
#define UISCREEN_H

#include "game.h"
#include "text.h"
#include <functional>

class Button{
    public:
        Button(Game* game, const std::string& name, class Font* font,
        std::function<void()> onClick, const glm::vec2& pos, const glm::vec2& dims);

        ~Button();

        void setName(const std::string& name);

        void setBackground(class Texture* tex);
        void drawBackground(class Shader* shader);
        void drawText(class Shader* shader);
        
        const glm::vec2& getPosition() const;
        void setHighlighted(bool sel);
        bool getHighlighted();
        bool containsPoint(const glm::vec2& pt) const;
        void onClick();

    private:
        class Game* mGame;
        std::function<void()> mOnClick;
        std::string mName;
        class Text* mNameText;
        class Font* mFont;
        class Texture* mBackground;
        glm::vec2 mPosition;
        glm::vec2 mDimensions;
        bool mHighlighted;
};

class UIScreen{
    public:
        UIScreen(class Game* game);
        virtual ~UIScreen();

        virtual void update(float delta);
        virtual void draw(class Shader* textShader, class Shader* spriteShader);
        virtual void processInput(const SDL_Event& evet);
        virtual void handleKeyPress(int key);

        enum UIState{
            EActive,
            EClosing
        };

        void close();
        UIState getState();
        void setTitle(const std::string& text, const glm::vec2& position, const glm::vec3& color = glm::vec3(0.0f), const glm::vec2& pointScale = glm::vec2(1.0));
        void addButton(const std::string& name, std::function<void()> onClick);
    
    protected:
        class Text* mTitle;
        void drawTexture(class Shader* shader, class Texture* texture, const glm::vec2& offset = glm::vec2(0.0f), float scale = 1.0f);
        void setRelativeMouseMode(bool relative);

        class Game* mGame;
        class Font* mFont;

        class Texture* mBackground;
        class Texture* mSelected;
        class Texture* mUnSelected;

        glm::vec2 mTitlePos;
        glm::vec2 mNextButtonPos;
        glm::vec2 mBGPos;
        UIState mState;
        std::vector<class Button*> mButtons;
};

#endif