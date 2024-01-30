#include "game.h"

// ASSETS

// just init asset manager, doesn't load anything
void InitAssets() {
    game.assets.images = map_new(Image);
    game.assets.fonts = map_new(Font);
    game.assets.textures = map_new(Texture2D);
}

// unload asset manager stuff
void UnloadAssets() {

    map_iter_start(game.assets.images);
    while (map_iter_has_next(game.assets.images)) {
        const char* key = map_iter_next_key(game.assets.images);
        UnloadImage(map_get(game.assets.images, key));
    }

    map_iter_start(game.assets.fonts);
    while (map_iter_has_next(game.assets.fonts)) {
        const char* key = map_iter_next_key(game.assets.fonts);
        UnloadFont(map_get(game.assets.fonts, key));
    }

    map_iter_start(game.assets.textures);
    while (map_iter_has_next(game.assets.textures)) {
        const char* key = map_iter_next_key(game.assets.textures);
        UnloadTexture(map_get(game.assets.textures, key));
    }

    map_free(game.assets.images);
    map_free(game.assets.fonts);
    map_free(game.assets.textures);
}

// TODO add error prints in here:

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
        TraceLog(LOG_ERROR, "Cannot load already loaded scene");
        return;
    }

    Scene* s = &game.scenes[type];
    Scene* cs = &game.scenes[game.current_scene_type];
    cs->unload(cs);
    s->load(s);
    game.current_scene_type = type;
}

// OBJECT

Object* CreateObject(ObjectType type) {
    Object preset = game.presets[type];
    for (int i = 0; i < OBJ_COUNT; i++) {
        Object* o = &game.objects[type][i];

        if (o->is_active) {
            continue;
        }

        *o = preset;
        o->id = i;
        o->is_active = true;
        o->type = type;

        return o;
    }

    TraceLog(LOG_ERROR, "Failed to create object");
    return NULL;
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
    if (o->is_active && o->update) {
        o->update(o);
    }
}

void DrawObject(ObjectType type, int id) {
    Object* o = GetObject(type, id);
    if (o->is_active && o->draw) {
        o->draw(o);
    }
}

// SPECIFIC

Object* CreateBackgroundObject(Color background_color) {
    Object* o = CreateObject(OBJ_BACKGROUND);
    o->background_color = background_color;
    return o;
}

Object* CreateImageObject(const char* image_name, Vector2 pos) {
    Object* o = CreateObject(OBJ_IMAGE);
    o->image_data.image = GetImage(image_name);
    o->image_data.texture = LoadTextureFromImage(o->image_data.image);
    o->image_data.pos = pos;
    return o;
}

// MAIN

// init raylib and alloc all memory when starting up
void Init() {
    game = (GameContext) {
        .is_paused = false
    };

    for (int type = 0; type < OBJ_TYPE_COUNT; type++) {
        game.presets[type] = (Object) {
            .id = -1,
            .is_active = false,
            .is_auto = true,
            .type = OBJ_NONE,
            .update = NULL,
            .draw = NULL
        };
        for (int id = 0; id < OBJ_COUNT; id++) {
            Object* o = GetObject(type, id);
            *o = (Object) {
                .id = -1,
                .is_active = false,
                .is_auto = true,
                .type = OBJ_NONE,
                .update = NULL,
                .draw = NULL
            };
        }
    }

    // load assets first then presets
    InitAssets();
    LoadAssets();
    InitPresets();

    // raylib init
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(800, 600, "Jailbreak");
    SetWindowState(FLAG_VSYNC_HINT || FLAG_WINDOW_RESIZABLE);

    CreateBackgroundObject(RED);
    CreateImageObject("test", (Vector2){50, 50});
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
            UpdateObject(type, id);
        }
    }
}

// draw everything
void Draw() {
    for (int type = 0; type < OBJ_TYPE_COUNT; type++) {
        for (int id = 0; id < OBJ_COUNT; id++) {
            DrawObject(type, id);
        }
    }
}

// quit raylib and free all memory
void Quit() {
    // raylib quit + free
    CloseWindow();

    // free all memory
    // ...
}

// PLAYING STATE

void Pause() {
    game.is_paused = true;
}

void Unpause() {
    game.is_paused = false;
}

// START SCREEN

// END SCREEN

// CALLBACK HELL

void DrawBackgroundObjectCallback(Object* this) {
    ClearBackground(this->background_color);
}

void DrawImageObjectCallback(Object* this) {
    DrawTextureV(this->image_data.texture, this->image_data.pos, WHITE);
}

void UnloadImageObjectCallback(Object* this) {
    UnloadTexture(this->image_data.texture);
}