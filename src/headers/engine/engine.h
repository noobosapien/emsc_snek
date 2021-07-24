#ifndef ENGINE_H
#define ENGINE_H

class Engine{
    public:
        friend class Game;

        Engine();
        virtual ~Engine();

        void initialize(int width, int height);
        void shutDown();

        SDL_Renderer* getRenderer(){return renderer;}
        void setDebug(bool value) {mDebug = value;}


    protected:
        virtual void updateGame();
        virtual void generateOutput();

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_GLContext gl_context;

        bool mDebug;

        Uint32 mTicksCount;
        float mDeltaTime;
    
    private:
        

};

#endif