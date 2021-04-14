#ifndef TEXTURE_H
#define TEXTURE_H

class Texture{
public:
    Texture();
    ~Texture();

    bool load(const std::string& filename);
    bool loadFromGlyph(FT_Face& face);
    void unload();

    void setActive();

    int getWidth(){return mWidth;};
    int getHeight(){return mHeight;};
    unsigned int getId(){return mTextureID;}

    void createFromSurface(SDL_Surface* surface);

private:
    unsigned int mTextureID;
    int mWidth;
    int mHeight;

};

void flipTexMid32(SDL_Surface* surface);

#endif