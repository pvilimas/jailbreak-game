#include "game.h"

// ASSETS

void InitAssets() {
    game.assets.images = map_new(Image);
    game.assets.fonts = map_new(Font);
    game.assets.textures = map_new(Texture2D);
}

void UnloadAssets() {
    map_free(game.assets.images);
    map_free(game.assets.fonts);
    map_free(game.assets.textures);
}

void CreateImage(const char* name, Image image) {
    map_insert(game.assets.images, name, image);
}

void CreateImageFromFile(const char* name, const char* filepath) {
    CreateImage(name, LoadImage(filepath));
}

Image GetImage(const char* name) {
    return map_get(game.assets.images, name);
}

void DeleteImage(const char* name) {
    map_remove(game.assets.images, name);
}

void CreateFont(const char* name, Font font) {
    map_insert(game.assets.fonts, name, font);
}

void CreateFontFromFile(const char* name, const char* filepath) {
    CreateFont(name, LoadFont(filepath));
}

Font GetFont(const char* name) {
    return map_get(game.assets.fonts, name);
}

void DeleteFont(const char* name) {
    map_remove(game.assets.fonts, name);
}

void CreateTexture(const char* name, Texture2D texture) {
    map_insert(game.assets.textures, name, texture);
}

void CreateTextureFromFile(const char* name, const char* filepath) {
    CreateTexture(name, LoadTexture(filepath));
}

Texture2D GetTexture(const char* name) {
    return map_get(game.assets.textures, name);
}

void DeleteTexture(const char* name) {
    map_remove(game.assets.textures, name);
}

// SCENE

void LoadScene(SceneType type) {
    if (type == game.current_scene_type) {
        TraceLog(LOG_ERROR, "Cannot load already loaded scene\n");
        return;
    }

    Scene* s = &game.scenes[type];
    Scene* cs = &game.scenes[game.current_scene_type];
    cs->unload(cs);
    s->load(s);
    game.current_scene_type = type;
}

// OBJECT

int CreateObject(ObjectType type, ObjectFunc update, ObjectFunc draw) {
    for (int i = 0; i < OBJ_COUNT; i++) {
        Object* o = &game.objects[type][i];

        if (o->is_active) {
            continue;
        }

        o->id = i;
        o->is_active = true;
        o->type = type;
        o->update = update;
        o->draw = draw;

        return o->id;
    }

    TraceLog(LOG_ERROR, "Failed to create object\n");
    return -1;
}

inline Object* GetObject(ObjectType type, int id) {
    return &game.objects[type][id];
}

void DeleteObject(ObjectType type, int id) {
    Object* o = GetObject(type, id);
    o->id = -1;
    o->is_active = true;
}

void UpdateObject(ObjectType type, int id) {
    Object* o = GetObject(type, id);
    o->update(o);
}
void DrawObject(ObjectType type, int id) {
    Object* o = GetObject(type, id);
    o->draw(o);
}

// MAIN

// init raylib and alloc all memory when starting up
void Init() {
    game = (GameContext) {
        .is_paused = false
    };

    for (int type = 0; type < OBJ_TYPE_COUNT; type++) {
        for (int id = 0; id < OBJ_COUNT; id++) {
            Object* o = GetObject(type, id);
            *o = (Object) {
                .id = -1,
                .is_active = false,
                .type = OBJ_NONE,
                .update = NULL,
                .draw = NULL
            };
        }
    }

    // raylib init
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(800, 600, "Jailbreak");
    SetWindowState(FLAG_VSYNC_HINT || FLAG_WINDOW_RESIZABLE);
}

// game's main loop
void Start() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        Update();
        Draw();
        EndDrawing();
    }
}

// update everything
void Update() {
    for (int type = 0; type < OBJ_TYPE_COUNT; type++) {
        for (int id = 0; id < OBJ_COUNT; id++) {
            Object* o = GetObject(type, id);
            if (o->is_active) {
                o->update(o);
            }
        }
    }
}

// draw everything
void Draw() {
    // remove later
    ClearBackground((Color){255, 0, 0, 0});

    for (int type = 0; type < OBJ_TYPE_COUNT; type++) {
        for (int id = 0; id < OBJ_COUNT; id++) {
            Object* o = GetObject(type, id);
            if (o->is_active) {
                o->draw(o);
            }
        }
    }
}

// quit raylib and free all memory
void Quit() {
    // raylib quit + free
    CloseWindow();

    // free all memory
}

// PLAYING STATE

void Pause() {
    game.is_paused = true;
}

void Unpause() {
    game.is_paused = false;
}
