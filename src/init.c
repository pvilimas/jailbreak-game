#include "game.h"

// this file contains all the functions that initialize stuff when game starts
// basically all actual game data is inside this file

// load all assets needed at the start of the game 
void LoadAssets() {
    CreateImageFromFile("test", "assets/images/test.gif");
}

// init all the game presets
void InitPresets() {
    game.presets[OBJ_BACKGROUND] = (Object) {
        .draw = DrawBackgroundObjectCallback,
        .update = NULL,
        .unload = NULL,
        .background_color = RAYWHITE
    };
    game.presets[OBJ_IMAGE] = (Object) {
        .draw = DrawImageObjectCallback,
        .update = NULL,
        .unload = UnloadImageObjectCallback,
    };
}