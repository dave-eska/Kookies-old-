#include "LevelEditorButton.h"
#include"screens.h"

#include<StartGameButton.h>

//Variables
static bool menu_should_load=true;
static int finish_screen=0;

static StartGameButton *Start_Game_Button;
static LevelEditorButton *Level_Editor_Button;

//Textures
static Texture2D main_menu_texture;

void InitTitleScreen(){
    Start_Game_Button = new StartGameButton({110, 370});
    Level_Editor_Button = new LevelEditorButton({110, 590});

    main_menu_texture=LoadTexture("res/img/main_menu.png");

    menu_should_load=false;
}

void UpdateTitleScreen(){
    Start_Game_Button->Update(finish_screen);
    Level_Editor_Button->Update(finish_screen);
}

void DrawTitleScreen(){
    DrawTexture(main_menu_texture, 0, 0, WHITE);

    Start_Game_Button->Draw();
    Level_Editor_Button->Draw();
}

void UnloadTitleScreen(){
}

void ResetTitleFinishScreen(){
    finish_screen=0;
}

int FinishTitleScreen(){
    return finish_screen;
}
