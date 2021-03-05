
#ifndef WEBSOCKCLIENT_H
#define WEBSOCKCLIENT_H

#include <cstdint>
#include <stdio.h>
#include <queue>
#include <list>
#include <string>
#include "emscripten/websocket.h"
#include "inoutstreams.h"
#include "replicationmanager.h"
#include "../game.h"

class WebsockClient{
    
public:
    enum NetworkClientState{
        ST_UNINITIALIZED,
        ST_SAYINGHELLO,
        ST_WELCOMED
    };

    static const uint32_t kHelloCC  = 'HELO';
    static const uint32_t kWelcomeCC  = 'WLCM';
    static const uint32_t kStateCC  = 'STAT';
    static const uint32_t kInputCC  = 'INPT';

    static bool staticInit(Game* game, std::string name);
    void sendOutgoing();
    void processPacket(InputStream& inputStream);
    void processAllPackets(); //call this from the game object.
    static EM_BOOL sendMessage(OutputStream& out);

    static WebsockClient* sInstance;

private:
    WebsockClient(Game* game);
    ~WebsockClient();

    Game* mGame;

    class mReceivedPacket{
        public:
            mReceivedPacket(InputStream& inStream): mPacketBuffer(inStream){

            }
            InputStream& getInStream(){return mPacketBuffer;}
        private:
            InputStream mPacketBuffer;
    };

    std::queue<mReceivedPacket, std::list<mReceivedPacket>> mPacketQueue;

    bool init(std::string address, std::string name);

    void sendHelloPacket();

    void handleWelcomePacket(InputStream& inputStream);

    void handleStatePacket(InputStream& inputStream);

    void sendInputPacket();

    static EM_BOOL onOpen(int eventType, const EmscriptenWebSocketOpenEvent* websockEvent, void* userData);
    static EM_BOOL onError(int eventType, const EmscriptenWebSocketErrorEvent* websockEvent, void* userData);
    static EM_BOOL onClose(int eventType, const EmscriptenWebSocketCloseEvent* websockEvent, void* userData);
    static EM_BOOL onMessage(int eventType, const EmscriptenWebSocketMessageEvent* websockEvent, void* userData);

    NetworkClientState mState;
    bool helloPacketSent;

    class ReplicationManager* mReplicationManager;

    EMSCRIPTEN_WEBSOCKET_T mSocket;
    EMSCRIPTEN_RESULT mResult;
    bool connected;

    uint32_t mPlayerID;
    std::string mName;
};

#endif