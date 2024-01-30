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
    OBJ_PLAYER,
    // ...
    OBJ_TYPE_COUNT,
    OBJ_NONE
} ObjectType;

struct Object;
typedef void (*ObjectFunc)(struct Object* this);
typedef struct Object {
    int id;
    bool is_active;
    ObjectType type;
    ObjectFunc update;
    ObjectFunc draw;
} Object;

// game context

typedef struct GameContext {
    Object objects[OBJ_TYPE_COUNT][OBJ_COUNT];
    Scene scenes[SCENE_COUNT];

    AssetManager assets;

    SceneType current_scene_type;
    bool is_paused;

    Camera2D camera;
    Object* player;
} GameContext;
extern struct GameContext game;

#endif // TYPES_H