em++ src/main.cpp src/game.cpp src/actor.cpp src/component.cpp src/shader.cpp src/spritecomponent.cpp src/texture.cpp src/movecomponent.cpp src/inputcomponent.cpp src/snake.cpp src/circlecomponent.cpp src/body.cpp src/followcomponent.cpp src/camera.cpp src/bordercomponent.cpp src/network/inoutstreams.cpp src/network/websockclient.cpp src/network/replicationmanager.cpp src/food.cpp src/animatedspritecomponent.cpp src/actorregistry.cpp src/uiscreen.cpp src/font.cpp -o game.html --preload-file src/shaders --preload-file src/textures -std=c++17 -DDEBUG -lwebsocket.js -s --shell-file shell.html -s EXPORTED_FUNCTIONS=['_main'] -s NO_EXIT_RUNTIME=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS=["png"] -s EXTRA_EXPORTED_RUNTIME_METHODS="['cwrap', 'ccall']" -s LLD_REPORT_UNDEFINED -fdebug-compilation-dir='./src' -g -s ASSERTIONS=2 -s SAFE_HEAP=1 --source-map-base http://localhost:3000