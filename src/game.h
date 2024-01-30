#ifndef GAME_H
#define GAME_H

/*
    TODO
    - global timer
    - timer objects
    - keybind cooldown_ms
*/

#include "types.h"

// assets

void InitAssets();
void UnloadAssets();

void CreateImage(const char* name, Image image);
void CreateImageFromFile(const char* name, const char* filepath);
Image GetImage(const char* name);
void DeleteImage(const char* name);

void CreateFont(const char* name, Font font);
void CreateFontFromFile(const char* name, const char* filepath);
Font GetFont(const char* name);
void DeleteFont(const char* name);

void CreateTexture(const char* name, Texture2D texture);
void CreateTextureFromFile(const char* name, const char* filepath);
Texture2D GetTexture(const char* name);
void DeleteTexture(const char* name);

// scene

void LoadScene(SceneType type);

// object

Object* CreateObject(ObjectType type);
Object* GetObject(ObjectType type, int id);
void DeleteObject(ObjectType type, int id);
void UpdateObject(ObjectType type, int id);
void DrawObject(ObjectType type, int id);

// specific create functions
Object* CreateBackgroundObject(Color background_color);
Object* CreateImageObject(const char* image_name, Vector2 pos);

// keybinds

void CreateKeybind(const char* name, int key, KeybindCondition condition,
    KeybindCallback action);
Keybind* GetKeybind(const char* name);
Keybind* DeleteKeybind(const char* name);
void EnableKeybind(const char* name);
void DisableKeybind(const char* name);

// presets

void LoadAssets();
void InitPresets();

// main

void Init();
void Quit();
void Start();
void Update();
void Draw();

// playing state

void Pause();
void Unpause();

// start screen

// end screen

// callback hell - update, draw, unload for each object type

void DrawBackgroundObjectCallback(Object* this);

void DrawImageObjectCallback(Object* this);
void UnloadImageObjectCallback(Object* this);

#endif // GAME_H