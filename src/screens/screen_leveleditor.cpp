#include "chat.h"
#include "global_func.h"
#include "item.h"
#include "screens.h"

#include <iostream>
#include <string>

#include <raylib.h>

#include "level.h"
#include "tile.h"

static int finish_screen = 0;

static Camera2D camera;
static int cam_speed;

static Level level;

static Rectangle selected_tile;
static int selected_tileID;
static int selected_tileSlot;
static bool has_selected_tile;

static std::vector<ChatText> texts;
static std::vector<std::string> commands;
static std::string user_input;

static int current_tileID;
static Texture2D currentTileTexture;

static bool is_debugging;
static bool is_typing;

static void typingCode(){
    for(int i = 0;i<texts.size();i++){
        texts[i].Update();
        if(texts[i].IsDone())
            texts.erase(texts.begin()+i);
    }

    char c = GetCharPressed();
    if (c){
        user_input.push_back(c);
    }

    if(IsKeyPressed(KEY_BACKSPACE) && !user_input.empty())
        user_input.pop_back();
    if(IsKeyPressed(KEY_ENTER) && !user_input.empty()){
        if(isStringInVector(commands, getFirstWord(user_input))){
            std::string command = getFirstWord(user_input);
            typeInChat(command);
            if(command == "/reset")
                InitLevelEditorScreen();
            else if(command ==  "/clear")
                texts.clear();
            else if(command == "/debug")
                is_debugging = !is_debugging;
            else if(command == "/tell"){
                if(getSecondWord(command) == "tiles"){
                    typeInChat("There are " + std::to_string(level.tiles.size()) + " tiles.");
                }else if(getSecondWord(command) == "ctiles"){
                    std::cout<<"Selected Tile: " + newItem(selected_tileID).item_name + "(" + std::to_string(selected_tileID) + ")"<<std::endl;
                    typeInChat("Selected Tile: " + newItem(selected_tileID).item_name + "(" + std::to_string(selected_tileID) + ")");
                }
                else{
                    typeInChat("Syntax Error: Expected Input Detail.", DARKPURPLE);
                }
            }
        }else{
            user_input.erase(user_input.begin());
            addChatText(texts, "Hot Chocolate Microwave: " + user_input);
        }
        user_input.clear();
        is_typing = false;
    }
    for(auto &text:texts) text.Update();
}

static void InteractWithTile(){
    if(IsKeyPressed(KEY_DELETE)){
        level.tiles.erase(std::remove_if(level.tiles.begin(), level.tiles.end(), [](const auto& tile){
                    return tile->getSlot() == selected_tileSlot;
                    }), level.tiles.end());
        has_selected_tile = false;
    }
}

void InitLevelEditorScreen(){
    is_debugging = true;
    is_typing = false;

    current_tileID = Brickwall_Tile;
    currentTileTexture = Tile(current_tileID, {0,0}, 0).getTexture();

    level.changeLevel("res/maps/test.json");
    std::cout<<level.highest_z<<std::endl;

    camera = { 0 };
    camera.target = { 0,0 };
    camera.offset = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 0.6;

    cam_speed = 500;

    selected_tile = {0,0,0,0};

    commands = {
        "/tell",
        "/reset",
        "/clear",
        "/debug"
    };
}

void UpdateLevelEditorScreen(){
    if(IsKeyPressed(KEY_ESCAPE))
        finish_screen = 1;

    if(IsKeyPressed(KEY_SLASH)){
        is_typing = true;
    }

    if(is_typing)
        typingCode();

    if(has_selected_tile)
        InteractWithTile();

    if(!is_typing){
        float inputX = IsKeyDown(KEY_D)-IsKeyDown(KEY_A);
        float inputY = IsKeyDown(KEY_S)-IsKeyDown(KEY_W);

        camera.target.x += (inputX * cam_speed) * GetFrameTime();
        camera.target.y += (inputY * cam_speed) * GetFrameTime();

        if(IsKeyPressed(KEY_UP)){
            current_tileID++;
            currentTileTexture = Tile(current_tileID, {0,0}, 0).getTexture();
        }

        if(IsKeyPressed(KEY_DOWN)){
            current_tileID--;
            currentTileTexture = Tile(current_tileID, {0,0}, 0).getTexture();
        }

        clamp(current_tileID, 0, 22);
    }


    for(auto& tile : level.tiles){
        tile->setIsTouchingMouse(false);

        if(CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tile->getBody())){
            tile->setIsTouchingMouse(true);
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
            if(((tile->getIsTouchingMouse() && level.highest_z > 0 && tile->getZ() == level.highest_z-1) or tile->getIsTouchingMouse()) && tile->getID() != Air_Tile){
                selected_tile = {
                    tile->getX(),
                    tile->getY(),
                    tile->getBody().width,
                    tile->getBody().height,
                };
                selected_tileID = tile->getID();
                selected_tileSlot = tile->getSlot();

                if(!has_selected_tile)
                    has_selected_tile = true;
            }
        }
    }
}

void DrawLevelEditorScreen(){
    BeginMode2D(camera);

    for(auto& tile : level.tiles){
        tile->Draw(is_debugging);
    }

    if(has_selected_tile)
        DrawRectangleRec(selected_tile, {200, 200, 200, 255/2});

    EndMode2D();

    DrawText("Current Tile: ", 20, 50, 25, BLACK);
    DrawText(std::to_string(current_tileID).c_str(), 280, 50, 25, BLACK);

        DrawTextureEx(currentTileTexture, {190, 30}, 0, 2, WHITE);

    for(auto e:texts) e.Draw();

    if(is_typing){
        DrawRectangleRec({30,(float)GetScreenHeight()-50,500,35}, {20,20,20,130});
        DrawText(user_input.c_str(), 30, (float)GetScreenHeight()-50, 35, BLACK);
    }
}

void UnloadLevelEditorScreen(){
}

void ResetLevelEditorScreen(){
    finish_screen=0;
}

int FinishLevelEditorScreen(){
    return finish_screen;
}
