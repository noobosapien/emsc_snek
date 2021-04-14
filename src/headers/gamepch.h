#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <math.h>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <sstream>
#include <fstream>
#include <functional>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <emscripten.h>
#include "emscripten/websocket.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_opengl.h>


#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include <GLES2/gl2.h>
#include <GLES3/gl3.h>
#include <GL/glfw.h>

class Actor;
class Component;
class UIScreen;

#include "actor.h"
#include "component.h"
#include "actorregistry.h"
#include "spritecomponent.h"
#include "animatedspritecomponent.h"
#include "body.h"
#include "bordercomponent.h"
#include "camera.h"
#include "circlecomponent.h"
#include "dialogbox.h"
#include "followcomponent.h"
#include "font.h"
#include "network/inoutstreams.h"
#include "food.h"
#include "game.h"
#include "movecomponent.h"
#include "inputcomponent.h"
#include "uiscreen.h"
#include "pausemenu.h"
#include "shader.h"
#include "snake.h"
#include "text.h"
#include "texture.h"
#include "network/replicationmanager.h"
#include "network/websockclient.h"
