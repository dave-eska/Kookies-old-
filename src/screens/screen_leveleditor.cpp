#include "chat.h"
#include "global_func.h"
#include "item.h"
#include "raymath.h"
#include "screens.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include <raylib.h>

#include "level.h"
#include "tile.h"
#include "tiling_util.h"

static int finish_screen = 0;

static Camera2D camera;
static int cam_speed;

static Level level;
static std::string canvas_sizeStr;

/*
static Rectangle selected_tile;
static Texture2D selected_tileTexture;
static int selected_tileID;
static int selected_tileSlot;
*/
static Tile selectedTile;
static bool has_selected_tile;
static int selectedTileZ;

static std::vector<std::string> commands;

static std::string user_input;
static std::string last_input;
static std::string prev_user_input;

static int currentTileID;
static Texture2D currentTileTexture;

static bool is_debugging;
static bool is_typing;

static void savingCode(){
    writeTileJson(level.tiles, 0, 0, "GUGUGAGA.json");
}

static void typingCode(){
    for(int i = 0;i<texts.size();i++){
        texts[i].Update();
        if(texts[i].IsDone())
            texts.erase(texts.begin()+i);
    }

    char c = GetCharPressed();
    if(c){
        user_input.push_back(c);
    }

    if(IsKeyPressed(KEY_UP) && !user_input.empty()){
        prev_user_input = user_input;
        user_input = "/" + last_input;
    }
    if(IsKeyPressed(KEY_DOWN) && !user_input.empty()){
        user_input = prev_user_input;
    }

    if(IsKeyPressed(KEY_BACKSPACE) && !user_input.empty()) user_input.pop_back();

    if(IsKeyPressed(KEY_ENTER) && !user_input.empty()){
        if(isStringInVector(commands, getFirstWord(user_input))){
            std::string command = getFirstWord(user_input);
            std::string argument = getSecondWord(user_input);

            if(command == "/reset")
                InitLevelEditorScreen();
            else if(command ==  "/clear")
                texts.clear();
            else if(command == "/debug")
                is_debugging = !is_debugging;
            else if(command == "/tell"){
                if(argument == "tiles"){
                    typeInChat("There are " + std::to_string(level.tiles.size()) + " tiles.");
                }else if(argument == "ctiles"){
                    typeInChat("Selected Tile: " + selectedTile.getName() + "(" + std::to_string(selectedTile.getID()) + ")");
                    typeInChat("Z layer: " + std::to_string(selectedTile.getZ()));
                }
                else{
                    typeInChat("Syntax Error: Expected Input Detail.", DARKPURPLE);
                }
            }
            else if(command == "/change"){
                if(has_selected_tile && !argument.empty()){
                    auto it = std::find_if(level.tiles.begin(), level.tiles.end(),
                            [](const auto& item) {
                            return item->getSlot() == selectedTile.getSlot();
                            });
                    if(it != level.tiles.end()){
                        int prevSlot = (*it)->getSlot();
                        *it = std::make_unique<Tile>(Tile(currentTileID, {selectedTile.getX(), selectedTile.getY()}, selectedTile.getZ()));
                        (*it)->setSlot(prevSlot);
                    }
                }
            }else if(command == "/save"){
                savingCode();
            }
            user_input.erase(user_input.begin());
        }else{
            user_input.erase(user_input.begin());
            addChatText(texts, "Hot Chocolate Microwave: " + user_input);
        }
        last_input = user_input;
        prev_user_input.clear();
        user_input.clear();
        is_typing = false;
    }
    for(auto &text:texts) text.Update();
}

static void InteractWithTile(){
    if(IsKeyPressed(KEY_DELETE)){
        level.tiles.erase(std::remove_if(level.tiles.begin(), level.tiles.end(), [](const auto& tile){
                    return tile->getSlot() == selectedTile.getSlot();
                    }), level.tiles.end());
        selectedTile = Tile(Air_Tile, {}, 0);
        has_selected_tile = false;
    }
    if(IsKeyPressed(KEY_C)){
        auto it = std::find_if(level.tiles.begin(), level.tiles.end(),
                [](const auto& item) {
                return item->getSlot() == selectedTile.getSlot();
                });
        if(it != level.tiles.end()){
            int prevSlot = (*it)->getSlot();
            *it = std::make_unique<Tile>(Tile(currentTileID, {selectedTile.getX(), selectedTile.getY()}, selectedTile.getZ()));
            (*it)->setSlot(prevSlot);
        }
    }
}

void InitLevelEditorScreen(){
    is_debugging = true;
    is_typing = false;

    currentTileID = Brickwall_Tile;
    currentTileTexture = Tile(currentTileID, {0,0}, 0).getTexture();

    level.changeLevel("res/maps/test.json");
    canvas_sizeStr = std::to_string((int)level.canvas_size.x) + ", " + std::to_string((int)level.canvas_size.y);

    camera = { 0 };
    camera.target = { 0,0 };
    camera.offset = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 0.6;

    cam_speed = 500;

    selectedTile = Tile(Air_Tile, {}, 0);
    has_selected_tile = false;
    selectedTileZ = level.highest_z;

    commands = {
        "/tell",
        "/reset",
        "/clear",
        "/debug",
        "/change",
        "/save"
    };
}

void UpdateLevelEditorScreen(){
    if(IsKeyPressed(KEY_ESCAPE))
        finish_screen = 1;

    // Translate based on mouse right click
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) or IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera.zoom);

        camera.target = Vector2Add(camera.target, delta);
    }

    // Zoom based on mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        // Set the offset to where the mouse is
        camera.offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space point 
        // under the cursor to the screen space point under the cursor at any zoom
        camera.target = mouseWorldPos;

        // Zoom increment
        const float zoomIncrement = 0.125f;

        camera.zoom += (wheel*zoomIncrement);
        if (camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
    }


    if(!is_typing){
        float inputX = IsKeyDown(KEY_D)-IsKeyDown(KEY_A);
        float inputY = IsKeyDown(KEY_S)-IsKeyDown(KEY_W);

        camera.target.x += (inputX * cam_speed) * GetFrameTime();
        camera.target.y += (inputY * cam_speed) * GetFrameTime();

        if(IsKeyPressed(KEY_UP) && !IsKeyDown(KEY_LEFT_CONTROL)) { currentTileID++; currentTileTexture = newItem(currentTileID).iconTexture; }
        if(IsKeyPressed(KEY_DOWN) && !IsKeyDown(KEY_LEFT_CONTROL)) { currentTileID--; currentTileTexture = newItem(currentTileID).iconTexture; }
        clamp(currentTileID, 0, 22);

        if(IsKeyDown(KEY_LEFT_CONTROL)){
            if(IsKeyPressed(KEY_S)){
                savingCode();
            }
        }

        if(IsKeyPressed(KEY_SLASH)){
            is_typing = true;
        }

        if(has_selected_tile)
            InteractWithTile();
    }

    if(is_typing)
        typingCode();

    for(auto& tile : level.tiles){
        tile->setIsTouchingMouse(false);

        if(CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tile->getBody())){
            tile->setIsTouchingMouse(true);
        }

        // TODO: Add multi selecting
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) or IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            if(((tile->getIsTouchingMouse() && level.highest_z > 0 && tile->getZ() == level.highest_z-1) or tile->getIsTouchingMouse()) && tile->getID() != Air_Tile){
                selectedTile = Tile(tile->getID(), {tile->getX(), tile->getY()}, tile->getZ());
                selectedTile.setSlot(tile->getSlot());
                selectedTile.setTexture(tile->getTexture());

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
        DrawRectangleRec(selectedTile.getBody(), {200, 200, 200, 255/2});

    EndMode2D();

    //! Top Part
    DrawTextEx(font, "Current Tile: ", {20, 50}, 25, 0, WHITE);
    DrawTextEx(font, std::to_string(currentTileID).c_str(), {270, 50}, 25, 0, BLACK);
    DrawRectangleRec({187, 27, 32*2+6, 32*2+6}, BLACK);
    DrawTextureEx(currentTileTexture, {190, 30}, 0, 2, WHITE);

    DrawTextEx(font, "Selected Tile: ", {320, 50}, 25, 0, WHITE);
    DrawTextEx(font, std::to_string(selectedTile.getID()).c_str(), {570, 50}, 25, 0, BLACK);
    DrawRectangleRec({487, 27, 32*2+6, 32*2+6}, BLACK);
    DrawTextureEx(selectedTile.getTexture(), {490, 30}, 0, 2, WHITE);

    //!Bottom Part
    DrawTextEx(font, "Canvas Size: ", {20, 140}, 25, 0, WHITE);
    DrawTextEx(font, canvas_sizeStr.c_str(), {185, 140}, 25, 0, BLACK);

    DrawTextEx(font, "Layers: ", {20, 170}, 25, 0, WHITE);
    DrawTextEx(font, std::to_string(level.highest_z).c_str(), {120, 170}, 25, 0, BLACK);

    for(auto e:texts) e.Draw();

    if(is_typing){
        DrawRectangleRec({30,(float)GetScreenHeight()-50,500,35}, {20,20,20,130});
        DrawText(user_input.c_str(), 30, (float)GetScreenHeight()-50, 35, WHITE);
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
