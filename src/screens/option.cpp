#include"screens.h"

int finish_screen = 0;

void InitOptionsScreen(){
}

void UpdateOptionsScreen(){
    if(IsKeyPressed(KEY_ESCAPE)) finish_screen = 1;
}

void DrawOptionsScreen(){
}

void UnloadOptionsScreen(){
}

void ResetOptionsFinishScreen(){
    finish_screen = 0;
}

int FinishOptionsScreen(){
    return finish_screen;
}
