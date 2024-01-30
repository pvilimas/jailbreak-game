#include "game.h"

// only have 1 function here which will init all the game presets

void InitPresets() {
    game.presets[OBJ_BACKGROUND] = (Object){
        .draw = DrawBackgroundObjectCallback,
        .update = NULL,
        .background_color = RAYWHITE
    };
}