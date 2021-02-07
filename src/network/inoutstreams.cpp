#include "inoutstreams.h"

InputStream::InputStream(uint8_t* inBuffer, uint32_t bitCount):
mBuffer(inBuffer), mBitCapacity(bitCount), mBitHead(0), mIsBufferOwner(false)
{

}


InputStream::InputStream(InputStream& other):
mBitCapacity(other.mBitCapacity), mBitHead(other.mBitHead),
mIsBufferOwner(true)
{
    int byteCount = mBitCapacity / 8;

    mBuffer = static_cast<uint8_t*> (malloc(byteCount));

    memcpy(mBuffer, other.mBuffer, byteCount);
}

InputStream::~InputStream(){
    if(mIsBufferOwner)
        free(mBuffer);
}

const uint8_t* InputStream::getBufferPtr() const{
    return mBuffer;
}

uint32_t InputStream::getRemainingBitCount() const {
    return mBitCapacity - mBitHead;
}

void InputStream::readBits(uint8_t& outData, uint32_t bitCount){
    
    uint32_t byteOffset = mBitHead >> 3;
    uint32_t bitOffset = mBitHead & 0x7;
    // printf("bitOffset: %u  byteOffset: %u\n", bitOffset, byteOffset);
    // printf("bitHead: %u\n", mBitHead);
    outData = static_cast<uint8_t>(mBuffer[byteOffset]) >> bitOffset;
    uint32_t bitsFreeThisByte = 8 - bitOffset;

    if(bitsFreeThisByte < bitCount){
        outData |= static_cast<uint8_t>(mBuffer[byteOffset + 1]) << bitsFreeThisByte;
    }

    outData &= (~(0x00ff << bitCount));
    mBitHead += bitCount;

}

void InputStream::readBits(void* outData, uint32_t bitCount){
    uint8_t* destByte = reinterpret_cast<uint8_t*>(outData);

    while(bitCount > 8){
        readBits(*destByte, 8);
        ++destByte;
        bitCount -= 8;
    }

    if(bitCount > 0){
        readBits(*destByte, bitCount);
    }
}

void InputStream::readBytes(void* outData, uint32_t byteCount){
    readBits(outData, byteCount << 3);
}

template<typename T>
void InputStream::read(T& inData, uint32_t bitCount){
    static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic read supports primitive data types only.");
    readBits(&inData, bitCount);
}

void InputStream::read(uint32_t& outData, uint32_t bitCount){
    readBits(&outData, bitCount);
}

void InputStream::read(int& outData, uint32_t bitCount){
    readBits(&outData, bitCount);
}

void InputStream::read(float& outData){
    readBits(&outData, 32);
}

void InputStream::read(uint16_t& outData, uint32_t bitCount){
    readBits(&outData, bitCount);
}

void InputStream::read(int16_t& outData, uint32_t bitCount){
    readBits(&outData, bitCount);
}

void InputStream::read(uint8_t& outData, uint32_t bitCount){
    readBits(&outData, bitCount);
}

void InputStream::read(bool& outData){
    readBits(&outData, 1);
}


void InputStream::read(std::string& inString){
    uint32_t elementCount;
    read(elementCount);

    inString.resize(elementCount);
    for(auto& element : inString){
        read(element);
    }
}

void InputStream::resetToCapacity(uint32_t byteCapacity){
    mBitCapacity = byteCapacity << 3; 
    mBitHead = 0;
}


//OUTPUT STREAM

OutputStream::OutputStream(): mBitHead(0), mBuffer(nullptr)
{
    reallocBuffer(1500 * 8);
}

OutputStream::~OutputStream(){
    std::free(mBuffer);
}

void OutputStream::writeBits(uint8_t inData, uint32_t bitCount){
    uint32_t nextBithead = mBitHead + static_cast<uint32_t>(bitCount);

    if(nextBithead > mBitCapacity){
        reallocBuffer(std::max(mBitCapacity*2, nextBithead));
    }

    uint32_t byteOffset = mBitHead >> 3;
    uint32_t bitOffset = mBitHead & 0x7;

    uint8_t currentMask = ~(0xff << bitOffset);
    mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);


    uint32_t bitsFreeThisByte = 8 - bitOffset;

    if(bitsFreeThisByte < bitCount){
        mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
    }

    mBitHead = nextBithead;

}

void OutputStream::writeBits(const void* inData, uint32_t bitCount){
    const char* srcByte = static_cast<const char*>(inData);

    while(bitCount > 8){
        writeBits(*srcByte, 8);
        ++srcByte;
        bitCount -= 8;
    }
    
    if(bitCount > 0){
        writeBits(*srcByte, bitCount);
    }
}

const char* OutputStream::getBufferPtr() const {
    return mBuffer;
}

uint32_t OutputStream::getBitLength() const {
    return mBitHead;
}

uint32_t OutputStream::getByteLength() const {
    return (mBitHead + 7) >> 3;
}

void OutputStream::writeBytes(const void* inData, uint32_t byteCount){
    writeBits(inData, byteCount << 3);
}

template<typename T>
void OutputStream::write(T inData, uint32_t bitCount){
    static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Generic read supports primitive data types only.");
    writeBits(&inData, bitCount);
}

void OutputStream::write(bool inData){
    writeBits(&inData, 1);
}

void OutputStream::write(const std::string& inString){
    uint32_t elementCount = static_cast<uint32_t> (inString.size());
    write(elementCount);

    for(const auto& element : inString){
        write(element);
    }
}

void OutputStream::reallocBuffer(uint32_t inNewBitCapacity){
    
    if(mBuffer == nullptr){
        mBuffer = static_cast<char*>(std::malloc(inNewBitCapacity >> 3));
        memset(mBuffer, 0, inNewBitCapacity >>3);
    }else{
        char* tempBuffer = static_cast<char*>(std::malloc(inNewBitCapacity >> 3));
        memset(tempBuffer, 0, inNewBitCapacity >> 3);
        memcpy(tempBuffer, mBuffer, mBitCapacity >> 3);
        std::free(mBuffer);
        mBuffer = tempBuffer;
    }
    mBitCapacity = inNewBitCapacity;
}