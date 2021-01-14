#include "websockclient.h"

WebsockClient::WebsockClient(){
    mState = ST_UNINITIALIZED;
}


WebsockClient::~WebsockClient(){

}

bool WebsockClient::staticInit(){
    sInstance = new WebsockClient();
    return sInstance->init("ws://localhost:3001");
}

void WebsockClient::sendOutgoing(){
    switch (mState){
        case ST_SAYINGHELLO:
            updateSayingHello();
        case ST_WELCOMED:
            updateSendingInputPacket();
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

bool WebsockClient::init(std::string address){
    mState = ST_SAYINGHELLO;

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

void WebsockClient::updateSayingHello(){
    
}

void WebsockClient::sendHelloPacket(){
    
}

void WebsockClient::handleWelcomePacket(InputStream& inputStream){
    
}

void WebsockClient::handleStatePacket(InputStream& inputStream){
    
}

void WebsockClient::updateSendingInputPacket(){
    
}

void WebsockClient::sendInputPacket(){
    
}