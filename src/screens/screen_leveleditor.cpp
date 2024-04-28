#include"screens.h"
#include <iostream>

static int finish_screen = 0;

void InitLevelEditorScreen(){
}

void UpdateLevelEditorScreen(){
    std::cout<<1<<std::endl;
    if(IsKeyPressed(KEY_ESCAPE))
        finish_screen = 1;
}

void DrawLevelEditorScreen(){
}

void UnloadLevelEditorScreen(){
}

void ResetLevelEditorScreen(){
    finish_screen=0;
}

int FinishLevelEditorScreen(){
    return finish_screen;
}
