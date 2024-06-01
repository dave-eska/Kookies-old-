#include "screens.h"

#include <algorithm>
#include <memory>
#include <string>
#include <unistd.h>
#include <unordered_set>

#include <raylib.h>
#include <raygui.h>
#include <raymath.h>

#include "global_func.h"
#include "tiling_util.h"

#include "level.h"
#include "tile.h"
#include "chat.h"
#include "item.h"
#include "modes.h"
#include "transition.h"

static int finish_screen = 0;

static Camera2D camera;
static int cam_speed;

static Level level;
static std::string canvas_sizeStr;

static Texture2D BlockOutlineTexture;

static Tile selectedTile;
static bool has_selected_tile;
static int selectedTileZ;

static int current_mode;

static std::vector<std::string> commands;

static std::string user_input;
static std::string last_input;
static std::string prev_user_input;

static int currentTileID;
static Texture2D currentTileTexture;

static int font_size;

static char tr_dest[128];
static bool isEditingTD;

// Function to perform fill operation
static void fill(std::vector<std::unique_ptr<Tile>>& canvas, int x, int y, int z, int targetID, int fillID, std::unordered_set<int>& visited) {
    // Check if current position is within canvas bounds
    if(x < 0 || y < 0 || x >= (level.canvas_size.x*TILE_SIZE) || y >= (level.canvas_size.y*TILE_SIZE)) return;

    // Calculate index of the tile in the vector
    auto it = std::find_if(canvas.begin(), canvas.end(), [x, y, z](const auto& tile){
            return tile->getX() == x && tile->getY() == y && tile->getZ() == z;
            });


    if(it == canvas.end()) return;
    int index = (*it)->getSlot();

    // Check if current position has already been visited or has different ID or z value
    if(visited.find(index) != visited.end() || canvas[index]->getID() != targetID || canvas[index]->getZ() != z)
        return;

    // Fill current tile with fill ID
    int prevSlot = canvas[index]->getSlot();
    canvas[index] = std::make_unique<Tile>(Tile(fillID, canvas[index]->getPos(), canvas[index]->getZ()));
    canvas[index]->setSlot(prevSlot);

    // Add current index to visited set
    visited.insert(index);

    // Perform fill operation recursively in all four directions
    fill(canvas, x + TILE_SIZE, y, z, targetID, fillID, visited); // Right
    fill(canvas, x - TILE_SIZE, y, z, targetID, fillID, visited); // Left
    fill(canvas, x, y + TILE_SIZE, z, targetID, fillID, visited); // Down
    fill(canvas, x, y - TILE_SIZE, z, targetID, fillID, visited); // Up
}

static void savingCode(){
    writeTileJson(level, {0,0}, "save.json");
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

            if(command == "/reset") InitLevelEditorScreen();
            else if(command == "/clear") texts.clear();
            else if(command == "/debug") isDebugging = !isDebugging;
            else if(command == "/save") savingCode();

            else if(command == "/load"){
                if(!argument.empty()) level.changeLevel(argument);
                else level.changeLevel("save.json");
            }
            else if(command == "/tell"){
                if(argument == "tiles"){
                    typeInChat("There are " + std::to_string(level.getTiles().size()) + " tiles.");
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
                    level.getTiles()[selectedTile.getSlot()] = std::make_unique<Tile>(Tile(currentTileID, {selectedTile.getX(), selectedTile.getY()}, selectedTile.getZ()));
                }
            }else if(command == "/del" && level.getTotalLayes() > 2){
                std::erase_if(level.getTiles(),
                        [](const auto& tile) {
                        return tile->getZ() == level.getTotalLayers()-1;
                        });
                level.getTotalLayers() = level.getTiles()[level.getTiles().size()-1]->getZ()+1;
            }

            user_input.erase(user_input.begin());
        }else{
            user_input.erase(user_input.begin());
            addChatText(texts, "Hot Chocolate Microwave: " + user_input);
        }
        last_input = user_input;
        prev_user_input.clear();
        user_input.clear();
        isTyping = false;
    }
    for(auto &text:texts) text.Update();
}

static void InteractWithTile(){
    if(IsKeyPressed(KEY_DELETE)){
        level.getTiles()[selectedTile.getSlot()] = std::make_unique<Tile>(Tile(Air_Tile, {selectedTile.getX(), selectedTile.getY()}, selectedTile.getZ()));
        has_selected_tile = false;
    }
    if(IsKeyPressed(KEY_C)){
        level.getTiles()[selectedTile.getSlot()] = std::make_unique<Tile>(Tile(currentTileID, {selectedTile.getX(), selectedTile.getY()}, selectedTile.getZ()));
    }
}

void InitLevelEditorScreen(){
    isDebugging = true;

    currentTileID = Brickwall_Tile;
    currentTileTexture = Tile(currentTileID, {0,0}, 0).getTexture();

    level.changeLevel("res/maps/test.json");
    canvas_sizeStr = std::to_string((int)level.canvas_size.x) + ", " + std::to_string((int)level.canvas_size.y);
    current_mode = Mode_One_Select;

    font_size = 20;
    BlockOutlineTexture = LoadTexture("res/img/inventory_outline.png");

    camera = { 0 };
    camera.target = { 1000, 500 };
    camera.offset = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 0.45;

    cam_speed = 500;

    selectedTile = Tile(Air_Tile, {}, 0);
    has_selected_tile = false;
    selectedTileZ = 0;

    commands = {
        "/tell",
        "/reset",
        "/clear",
        "/debug",
        "/change",
        "/save",
        "/load",
        "/del"
    };
}

void UpdateLevelEditorScreen(){
    if(IsKeyPressed(KEY_ESCAPE))
        finish_screen = 1;

    // Camera Movement
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) or IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)){
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera.zoom);

        camera.target = Vector2Add(camera.target, delta);
    }

    // Zoom with mouse wheel
    if(!IsKeyDown(KEY_LEFT_CONTROL)){
        float wheel = GetMouseWheelMove();
        if(wheel != 0){
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.target = mouseWorldPos;
            const float zoomIncrement = 0.125f;
            camera.zoom += (wheel*zoomIncrement);

            if (camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
        }
    }

    // Selecting Z
    if(IsKeyPressed(KEY_UP) or IsKeyPressed(KEY_DOWN)){
        if(IsKeyPressed(KEY_UP)) selectedTileZ++;
        if(IsKeyPressed(KEY_DOWN)) selectedTileZ--;

        auto it = std::find_if(level.getTiles().begin(), level.getTiles().end(), [](const auto& tile){
                return (tile->getX() == selectedTile.getX() && tile->getY() == selectedTile.getY()) && tile->getZ() == selectedTileZ;
                });
        if(it != level.getTiles().end()){
            selectedTile = Tile((*it)->getID(), {(*it)->getX(), (*it)->getY()}, selectedTileZ);
            selectedTile.setSlot((*it)->getSlot());
            selectedTile.setTexture((*it)->getTexture());
        }
    }

    if(isTyping) typingCode();

    if(!isTyping && !isEditingTD){
        float inputX = IsKeyDown(KEY_D)-IsKeyDown(KEY_A);
        float inputY = IsKeyDown(KEY_S)-IsKeyDown(KEY_W);

        camera.target.x += (inputX * cam_speed) * GetFrameTime();
        camera.target.y += (inputY * cam_speed) * GetFrameTime();

        if(IsKeyDown(KEY_LEFT_CONTROL)){
            currentTileID += (GetMouseWheelMove() * 1);
            clamp(currentTileID, 0, MaxTileID-1);
            if(GetMouseWheelMove() != 0) currentTileTexture = newItem<Tile>(currentTileID).iconTexture;

            if(IsKeyPressed(KEY_S)){
                savingCode();
            }
        }

        // Make new layer
        if(IsKeyPressed(KEY_N)){
            for(int i=0;i<level.canvas_size.x*level.canvas_size.x;i++){
                auto& tile = level.getTiles()[i];
                level.getTiles().push_back(std::make_unique<Tile>(Tile(Air_Tile, {tile->getX(), tile->getY()}, level.getTotalLayers())));
            }
            level.getTotalLayers()++;
        }

        if(IsKeyPressed(KEY_I)) current_mode = Mode_ColorPicker;
        if(IsKeyPressed(KEY_B)) current_mode = Mode_Pencil;
        if(IsKeyPressed(KEY_G)) current_mode = Mode_Fill;
        if(IsKeyPressed(KEY_E)) current_mode = Mode_Eraser;

        // Switch Betweeen Modes
        if(IsKeyPressed(KEY_LEFT)) current_mode--;
        if(IsKeyPressed(KEY_RIGHT)) current_mode++;

        if(IsKeyPressed(KEY_SLASH)){
            isTyping = true;
        }

        if(has_selected_tile && current_mode == Mode_One_Select) InteractWithTile();

    }

    for(auto& tile : level.getTiles()){
        tile->setIsTouchingMouse(false);

        if(CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tile->getBody())){
            tile->setIsTouchingMouse(true);
        }

        switch(current_mode){
            case Mode_One_Select:
                {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) or IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                        if(tile->getIsTouchingMouse()){
                            selectedTile = Tile(tile->getID(), {tile->getX(), tile->getY()}, tile->getZ());
                            selectedTile.setSlot(tile->getSlot());
                            selectedTile.setTexture(tile->getTexture());

                            selectedTileZ = selectedTile.getZ();

                            if(!has_selected_tile)
                                has_selected_tile = true;
                        }
                    }
                }
                break;
            case Mode_Pencil:
                {
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tile->getIsTouchingMouse() && tile->getZ() == selectedTileZ){
                        int prev_slot = tile->getSlot();
                        tile = std::make_unique<Tile>(Tile(currentTileID, {tile->getX(), tile->getY()}, tile->getZ()));
                        tile->setSlot(prev_slot);
                    }
                }
                break;
            case Mode_Eraser:
                {
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tile->getIsTouchingMouse() && tile->getZ() == selectedTileZ){
                        int prev_slot = tile->getSlot();
                        tile = std::make_unique<Tile>(Tile(Air_Tile, {tile->getX(), tile->getY()}, tile->getZ()));
                        tile->setSlot(prev_slot);
                    }
                }
                break;
            case Mode_ColorPicker:
                {
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tile->getIsTouchingMouse() && tile->getZ() == selectedTileZ){
                        currentTileID = tile->getID();
                        currentTileTexture = tile->getTexture();
                    }
                }
                break;
            case Mode_Fill:
                {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && tile->getIsTouchingMouse() && tile->getZ() == selectedTileZ){
                        std::unordered_set<int> visited;
                        fill(level.getTiles(), tile->getX(), tile->getY(), tile->getZ(), tile->getID(), currentTileID, visited);
                    }
                }
                break;
            default:
                break;
        }
    }

    // Clamps
    clamp(selectedTileZ, 0, level.getTotalLayers()-1);
    clamp(currentTileID, 0, MaxTileID-1);
    clamp(current_mode, 0, Mode_Max-1);
}

void DrawLevelEditorScreen(){
    BeginMode2D(camera);

    for(auto& tile : level.getTiles()){
        tile->Draw(isDebugging);
    }

    if(has_selected_tile && current_mode == Mode_One_Select) 
        DrawRectangleRec(selectedTile.getBody(), {200, 200, 200, 255/2});

    EndMode2D();

    // Drawing UI
    DrawTextEx(font, "Current Tile", {10, 40}, font_size, 0, WHITE);
    DrawTextureEx(BlockOutlineTexture, {29, 80}, 0, 2.2, WHITE);
    DrawTexturePro(currentTileTexture, {0,0,32,32}, {32, 83, 32*2, 32*2}, {0, 0}, 0, WHITE);
    DrawTextEx(font, std::to_string(currentTileID).c_str(), {57.5, 103}, font_size+3, 0, WHITE);

    DrawTextEx(font, "Selected Tile", {160, 40}, font_size, 0, WHITE);
    DrawTextureEx(BlockOutlineTexture, {179, 80}, 0, 2.2, WHITE);
    DrawTexturePro(selectedTile.getTexture(), {0,0,32,32}, {182, 83, 32*2, 32*2}, {0, 0}, 0, WHITE);
    DrawTextEx(font, std::to_string(selectedTile.getID()).c_str(), {57.5+150, 103}, font_size+3, 0, WHITE);

    // Modes
    DrawTextEx(font, "     Modes   ", {(float)GetScreenWidth()-200, 40}, font_size+5, 0, WHITE);
    DrawTextEx(font, "<=                    =>", {(float)GetScreenWidth()-230, 70}, font_size+5, 0, WHITE);
    DrawTextEx(font, LevelEditorModeToString((LevelEditorMode)current_mode).c_str(), {(float)GetScreenWidth()-(float)193, 70}, font_size+1, 0, WHITE);

    // Texts
    std::string canvsaSizeStr = "Canvas Size: " + canvas_sizeStr;
    DrawTextEx(font, canvsaSizeStr.c_str(), {20, 190}, font_size, 0, WHITE);

    std::string layerStr = "Layers: " + std::to_string(level.getTotalLayers());
    DrawTextEx(font, layerStr.c_str(), {20, 215}, font_size, 0, WHITE);

    std::string zStr = "Current Layer: " + std::to_string(selectedTileZ+1);
    DrawTextEx(font, zStr.c_str(), {20, 240}, font_size, 0, WHITE);

    Rectangle box = { 100, 85, 285, 30 };
    if(level.getTiles()[selectedTile.getSlot()]->getID() == Transition_Tile){
        if(GuiTextBox({ box.x + 170, box.y, 200, 30 }, tr_dest, 128, isEditingTD)){
            if(isEditingTD){
                auto transitionTile = dynamic_cast<TransitionTile*>(level.getTiles()[selectedTile.getSlot()].get());
                transitionTile->attachLevel(tr_dest);
            }
            isEditingTD = !isEditingTD;
        }
    }

    // Draw Save button
    if(GuiButton((Rectangle){ (float)GetScreenWidth() - 100, (float)GetScreenHeight() - 50, 80, 30 }, "Save"))
        savingCode();

    if(GuiButton((Rectangle){ (float)GetScreenWidth() - 230, (float)GetScreenHeight() - 50, 110, 30 }, "Re-Load"))
        level.changeLevel("save.json");

    for(auto e:texts) e.Draw();

    if(isTyping){
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
