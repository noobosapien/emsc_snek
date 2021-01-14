
#ifndef WEBSOCKCLIENT_H
#define WEBSOCKCLIENT_H

#include <cstdint>
#include <string>
#include "emscripten/websocket.h"

class InputStream;
class OutputStream;

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

    static bool staticInit();
    void sendOutgoing();
    void processPacket(InputStream& inputStream);

    static WebsockClient* sInstance;

private:
    WebsockClient();
    ~WebsockClient();

    bool init(std::string address);

    void updateSayingHello();
    void sendHelloPacket();

    void handleWelcomePacket(InputStream& inputStream);

    void handleStatePacket(InputStream& inputStream);

    void updateSendingInputPacket();
    void sendInputPacket();

    static EM_BOOL onOpen(int eventType, const EmscriptenWebSocketOpenEvent* websockEvent, void* userData);
    static EM_BOOL onError(int eventType, const EmscriptenWebSocketErrorEvent* websockEvent, void* userData);
    static EM_BOOL onClose(int eventType, const EmscriptenWebSocketCloseEvent* websockEvent, void* userData);
    static EM_BOOL onMessage(int eventType, const EmscriptenWebSocketMessageEvent* websockEvent, void* userData);
    static EM_BOOL sendMessage();

    NetworkClientState mState;

    EMSCRIPTEN_WEBSOCKET_T mSocket;
    EMSCRIPTEN_RESULT mResult;
};

#endif