#include "websockclient.h"

WebsockClient::WebsockClient(Game* game): mGame(game){
    mReplicationManager = new ReplicationManager(game);
    mState = ST_UNINITIALIZED;
    mPlayerID = 0;
}


WebsockClient::~WebsockClient(){

}

WebsockClient* WebsockClient::sInstance = nullptr;

bool WebsockClient::staticInit(Game* game, std::string name){
    WebsockClient::sInstance = new WebsockClient(game);
    return WebsockClient::sInstance->init("ws://localhost:3001", name);
}

void WebsockClient::sendOutgoing(){
    switch (mState){
        case ST_UNINITIALIZED:
            break;
        case ST_SAYINGHELLO:
            sendHelloPacket();
        case ST_WELCOMED:
            sendInputPacket();
        default:
            break;
    }
}

void WebsockClient::processPacket(InputStream& inputStream){
    uint32_t packetType;

    inputStream.read(packetType);
    // printf("%u\n", packetType);

    switch(packetType){
        case kWelcomeCC:
            printf("Here\n");
            handleWelcomePacket(inputStream);
        case kStateCC:
            handleStatePacket(inputStream);
    }
}

bool WebsockClient::init(std::string address, std::string name){
    mState = ST_SAYINGHELLO;
    mName = name;

    if(!emscripten_websocket_is_supported())
        return false;
    
    EmscriptenWebSocketCreateAttributes ws_attr{
        address.c_str(), NULL, EM_TRUE
    };

    mSocket = emscripten_websocket_new(&ws_attr);

    emscripten_websocket_set_onopen_callback(mSocket, NULL, WebsockClient::onOpen);
    emscripten_websocket_set_onerror_callback(mSocket, NULL, WebsockClient::onError);
    emscripten_websocket_set_onclose_callback(mSocket, NULL, WebsockClient::onClose);
    emscripten_websocket_set_onmessage_callback(mSocket, NULL, WebsockClient::onMessage);

    return true;

}

void WebsockClient::sendHelloPacket(){
    OutputStream helloPacket;

    helloPacket.write(kHelloCC);
    helloPacket.write(mName);

    sendMessage(helloPacket);
}

void WebsockClient::handleWelcomePacket(InputStream& inputStream){
    if(mState == ST_SAYINGHELLO){
        int playerId;
        inputStream.read(playerId);

        mPlayerID = playerId;
        mState = ST_WELCOMED;
    }
}

void WebsockClient::handleStatePacket(InputStream& inputStream){
    if(mState == ST_WELCOMED){
        mReplicationManager->read(inputStream);
    }
}

void WebsockClient::processAllPackets(){
    while(!mPacketQueue.empty()){
        mReceivedPacket& packet = mPacketQueue.front();
        processPacket(packet.getInStream());
        mPacketQueue.pop();
    }
}

void WebsockClient::sendInputPacket(){
    //todo
}


EM_BOOL WebsockClient::onOpen(int eventType, const EmscriptenWebSocketOpenEvent* websockEvent, void* userData){
    puts("Connected to server\n");
    return EM_TRUE;
}


EM_BOOL WebsockClient::onError(int eventType, const EmscriptenWebSocketErrorEvent* websockEvent, void* userData){
    puts("Error\n");
    return EM_TRUE;
}


EM_BOOL WebsockClient::onClose(int eventType, const EmscriptenWebSocketCloseEvent* websockEvent, void* userData){
    puts("Connection closed\n");
    return EM_TRUE;
}


EM_BOOL WebsockClient::onMessage(int eventType, const EmscriptenWebSocketMessageEvent* websockEvent, void* userData){

    unsigned char* packet = static_cast<unsigned char*>(websockEvent->data);
    InputStream inStream(packet, sizeof(packet) * 8);
    mReceivedPacket recvPacket(inStream);
    printf("%s\n", inStream.getBufferPtr());
    WebsockClient::sInstance->mPacketQueue.emplace(recvPacket);

    return EM_TRUE;
}


EM_BOOL WebsockClient::sendMessage(OutputStream& out){

    return EM_TRUE;
}