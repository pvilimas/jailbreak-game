#ifndef GAME_H
#define GAME_H

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

// presets

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

// callback hell

void DrawBackgroundObjectCallback(Object* this);

#endif // GAME_H