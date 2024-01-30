#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "array.h"
#include "map.h"
#include "vector.h"

// defines

#define OBJ_COUNT   1000
#define SCENE_COUNT 10

#define BREAKPOINT()                \
    printf("breakpoint @ %s:%d\n",  \
    (strrchr((__FILE__), '/')       \
    ? strrchr((__FILE__), '/') + 1  \
    : (__FILE__)), __LINE__)

// assets

typedef struct {
    map(Image) images;
    map(Font) fonts;
    map(Texture) textures;
} AssetManager;

// scene

typedef enum {
    SCENE_START_SCREEN,
    SCENE_PLAYING,
    SCENE_END_SCREEN,
    // ...
    SCENE_TYPE_COUNT
} SceneType;

struct Scene;
typedef void (*SceneFunc)(struct Scene* this);
typedef struct Scene {
    SceneType type;
    SceneFunc load;
    SceneFunc unload;
} Scene;

// object

typedef enum {
    OBJ_BACKGROUND,     // Color
    OBJ_IMAGE,          // ImageData
    OBJ_PLAYER,         // PlayerObjData
    // ...
    OBJ_TYPE_COUNT,
    OBJ_NONE
} ObjectType;

typedef struct {
    Image image;
    Texture texture;
    Vector2 pos;
} ImageData;
typedef struct { int _; } PlayerObjectData;
// ...

struct Object;
typedef void (*ObjectFunc)(struct Object* this);
typedef struct Object {
    int id;
    bool is_active;
    bool is_auto; // (default=true) if false, draw/update called manually
    ObjectType type;
    ObjectFunc update;
    ObjectFunc draw;
    ObjectFunc unload;
    union {
        Color background_color;
        PlayerObjectData player_data;
        ImageData image_data;
        // ...
    };
} Object;

// input

typedef bool (*KeybindCondition)();
typedef void (*KeybindCallback)();

typedef struct {
    bool is_enabled;
    int key;
    // if this is not null, will execute keybind if this returns true
    // can be used to limit key to certain scenes for example
    KeybindCondition condition;
    KeybindCallback action;
} Keybind;

// game context

typedef struct GameContext {
    Object presets[OBJ_TYPE_COUNT];

    Object objects[OBJ_TYPE_COUNT][OBJ_COUNT];
    Scene scenes[SCENE_COUNT];

    map(Keybind) keybinds;

    AssetManager assets;

    SceneType current_scene_type;
    bool is_paused;

    Camera2D camera;
    Object* player;
} GameContext;
extern struct GameContext game;

#endif // TYPES_H