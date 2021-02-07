#include "replicationmanager.h"

ReplicationManager::ReplicationManager(Game* game)
{
    mGame = game;
}

void ReplicationManager::read(InputStream& inStream){

    // while(inStream.getRemainingBitCount() > 8){

        uint32_t action;//doesn't have to be 32bits
        inStream.read(action);

        printf("Replication manager read Here action: %u\n", action);

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
    // }
}

void ReplicationManager::readAndCreate(InputStream& inStream){
    uint32_t nameCC;

    while(inStream.getRemainingBitCount() > 8){
        inStream.read(nameCC);

        if (nameCC == 'STOP')
            break;

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
    
}

void ReplicationManager::readAndUpdate(InputStream& inStream){
    mFood->read(inStream);
}

void ReplicationManager::readAndDestroy(InputStream& inStream){
    mGame->removeActor(mFood);
}