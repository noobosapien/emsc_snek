#ifndef INOUTSTREAMS_H
#define INOUTSTREAMS_H

#include <cstdint>
#include <string>

class InputStream{
    
public:
    InputStream(char* buffer, uint32_t bitCount);
    InputStream(InputStream& inputStream);
    ~InputStream();

    const char* getBufferPtr() const;
    uint32_t getRemainingBitCount() const;

    void readBits(uint8_t& outData, uint32_t bitCount);
    void readBits(void* outData, uint32_t bitCount);
    
    void readBytes(void* outData, uint32_t byteCount);

    template< typename T >
    void read(T& inData, uint32_t bitCount = sizeof(T) * 8);

    void read(uint32_t& outData, uint32_t bitCount = 32);
    void read(int& outData, uint32_t bitCount = 32);
    void read(float& outData);


    void read(uint16_t& outData, uint32_t bitCount = 16);
    void read(int16_t& outData, uint32_t bitCount = 16);

    void read(uint8_t& outData, uint32_t bitCount = 8);
    void read(bool& outData);

    void read(std::string& inString);

    void resetToCapacity(uint32_t byteCapacity);


private:
    char* mBuffer;
    uint32_t mBitHead;
    uint32_t mBitCapacity;
    bool mIsBufferOwner;
};





class OutputStream{

public:
    OutputStream();
    ~OutputStream();

    void writeBits(uint8_t inData, uint32_t bitCount);
    void writeBits(const void* inData, uint32_t bitCount);

    const char* getBufferPtr() const;
    uint32_t getBitLength() const;
    uint32_t getByteLength() const;

    void writeBytes(const void* inData, uint32_t byteCount);

    template<typename T>
    void write(T inData, uint32_t bitCount = sizeof(T) * 8);

    void write(bool inData);
    void write(const std::string& inString);


private:
    void reallocBuffer(uint32_t inNewBitCapacity);
    char* mBuffer;
    uint32_t mBitHead;
    uint32_t mBitCapacity;
};

#endif