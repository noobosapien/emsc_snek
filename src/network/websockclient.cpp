#include "websockclient.h"

WebsockClient::WebsockClient(){
    mState = ST_UNINITIALIZED;
    mPlayerID = 0;
}


WebsockClient::~WebsockClient(){

}

bool WebsockClient::staticInit(std::string name){
    sInstance = new WebsockClient();
    return sInstance->init("ws://localhost:3001", name);
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

    switch(packetType){
        case kWelcomeCC:
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
        mReplicationManager.read(inputStream);
    }
}

void WebsockClient::sendInputPacket(){
    //todo
}