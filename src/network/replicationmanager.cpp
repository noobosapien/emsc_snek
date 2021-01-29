#include "replicationmanager.h"

ReplicationManager::ReplicationManager(Game* game)
{
    mGame = game;
}

void ReplicationManager::read(InputStream& inStream){
    
    while(inStream.getRemainingBitCount() > 32){

        uint8_t action;
        inStream.read(action, 2);

        switch(action){
            case RA_CREATE:
                readAndCreate(inStream);
                break;
            case RA_UPDATE:
                readAndUpdate(inStream);
                break;
            case RA_DESTROY:
                readAndDestroy(inStream);
                break;
            default:
                break;
        }
    }
}

void ReplicationManager::readAndCreate(InputStream& inStream){
    
    uint32_t nameCC;
    inStream.read(nameCC);

    switch (nameCC)
    {
    case 'FOOD':
        mFood = new Food(mGame);
        mFood->read(inStream);
        break;
    
    default:
        break;
    }
}

void ReplicationManager::readAndUpdate(InputStream& inStream){
    mFood->read(inStream);
}

void ReplicationManager::readAndDestroy(InputStream& inStream){
    mGame->removeActor(mFood);
}