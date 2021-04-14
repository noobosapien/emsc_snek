#ifndef FONT_H
#define FONT_H

struct Character{
    class Texture* texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class Font{
    public:
        Font(class Game* game);
        ~Font();

        bool loadCharacters(const std::string& pathName, int pointSize);
        void unLoad();
        
        Character* renderCharacter(char c);
    
    private:
        class Game* mGame;
        std::unordered_map<char, Character*> mCharacters;
};


#endif