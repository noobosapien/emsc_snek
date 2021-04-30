#ifndef ENGINE_H
#define ENGINE_H

class Engine{
    public:
        friend class Game;

        Engine();
        virtual ~Engine();

        void initialize(int width, int height);
        void shutDown();

    
    protected:
        virtual void updateGame();
        virtual void generateOutput();

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_GLContext gl_context;

        FT_Library mFtLib;

        Uint32 mTicksCount;
        float mDeltaTime;
    
    private:
        

};

#endif