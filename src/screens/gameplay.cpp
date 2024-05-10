#include<algorithm>
#include <sys/types.h>
#include<vector>
#include<string>
#include<memory>

#include<raylib.h>

#include "cat.h"
#include"item.h"
#include"screens.h"

#include"global_func.h"

#include"level.h"

#include"player.h"
#include"tile.h"
#include"chat.h"

#include"entity.h"
#include"updateTile_func.h"

#define INTERACT_KEY KEY_I

static bool is_debugging=false;
static bool isTyping=false;
static int finish_screen=0;

static Player player;
static std::vector<std::unique_ptr<Entity>> entities;

static Camera2D camera ;
static Sound pickupsound;

static Level level;
static std::string user_input;

static std::vector<std::string> commands;

void changeMainLevel(std::string levelName){
    level.changeLevel(levelName);
}

Player &getPlayer(){
    return player;
}

Level &getCurrentLevel(){
    return level;
}

static void drawDebugInfo(){
    printText("FPS: ", std::to_string(GetFPS()), {0,0}, 20);

    printText("Player X: ", std::to_string((int)player.getBody().x), {0,20}, 20);
    printText("Player Y: ", std::to_string((int)player.getBody().y), {0,40}, 20);

    printText("Player Is Toucing Item: ", std::to_string(player.getIsToucingItem()), {0,60}, 20);

    printText("Mouse X: ", std::to_string((int)GetMousePosition().x), {0,100}, 20);
    printText("Mouse Y: ", std::to_string((int)GetMousePosition().y), {0,120}, 20);

    printText("Total Tiles: ", std::to_string(level.tiles.size()), {0,160}, 20);

    printText("Total texts: ", std::to_string(texts.size()), {0,200}, 20);

    printText("Inventory current craftable id: ", std::to_string(player.getCurrentInvCraftAbleID()), {0,240}, 20);
}

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
            if(command == "/reset")
                InitGameplayScreen();
            else if(command ==  "/clear")
                texts.clear();
            else if(command == "/debug")
                is_debugging = !is_debugging;
            else if(command == "/tell"){
                if(getSecondWord(user_input) == "player.pos"){
                    typeInChat("X: " + std::to_string(player.getBody().x));
                    typeInChat("Y: " + std::to_string(player.getBody().y));
                }else{
                    typeInChat("Syntax Error: Expected Input Detail.", DARKPURPLE);
                }
            }else if(command == "/give"){
                if(!getSecondWord(user_input).empty()){
                    player.addItemInv(newItem(stoi(getSecondWord(user_input)), 1));
                }else{
                    typeInChat("Syntax Error: Expected Input Detail.", DARKPURPLE);
                }
            }else if(command == "/load" && !getSecondWord(user_input).empty())  level.changeLevel(getSecondWord(user_input));
        }else{
            user_input.erase(user_input.begin());
            addChatText(texts, player.getDisplayName() + ": " + user_input);
        }
        user_input.clear();
        isTyping=false;
    }
    for(auto &text:texts) text.Update();
}

static void UpdateTiles(){
    std::string tile_interect_return_code;
    for(auto& tile : level.tiles){
        tile->setIsTouchingSelectAreaPlayer(false);
        tile->setIsTouchingPlayer(false);
        tile->setIsTouchingMouse(false);

        if(CheckCollisionRecs(tile->getBody(), player.getSelectArea())){
            tile->setIsTouchingSelectAreaPlayer(true);
        }

        if(CheckCollisionRecs(tile->getBody(), player.getBody())){
            tile->setIsTouchingPlayer(true);
        }

        if(CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tile->getBody())){
            tile->setIsTouchingMouse(true);
        }

        tile->Update();

        if((tile->getType() == "Item" || tile->getType() == "BagOfSeed")&& tile->getIsTouchinSelectAreaPlayer() && tile->getIsTouchingMouse()){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                player.addItemInv(tile->asItem(1));
                std::erase(level.tiles, tile);
                break;
            }
        }

        TileUpdateFunction::Interact(tile, tile_interect_return_code);

        TileUpdateFunction::UseCraftingTable(tile);

        TileUpdateFunction::PlaceItem(tile, level);

        TileUpdateFunction::PlantSeed(tile, level);
    }

    if(!tile_interect_return_code.empty()){
        //x001 is the code to remove that tile. For now being use only to remove SeedTIle when harved.
        if(getFirstWord(tile_interect_return_code) == "x001" && !getSecondWord(tile_interect_return_code).empty()){
            level.tiles.erase(std::remove_if(level.tiles.begin(), level.tiles.end(), [tile_interect_return_code](const auto& tile){
                        return tile->getSlot() == std::stoi(getSecondWord(tile_interect_return_code));
                        }), level.tiles.end());
        }
        else{
            level.changeLevel(tile_interect_return_code);
        }
    }
}

static void drawInCamMode(){
    for(auto& tile : level.tiles)
        tile->Draw(is_debugging);


    for(auto& entity:entities)
        if(entity->getLevelName() == level.level_name) entity->Draw();

    player.Draw(is_debugging);
}

void InitGameplayScreen(){
    player = Player(
            /*Body*/{50,200,18*8, 35*8},
            /*Speed*/500,
            /*texture_path=*/"res/img/player_atlas.png",
            /*selectArea*/{0,0,350, 350},
            /*collisionBody=*/{0,0,18*9,10*9},

            /*slots=*/10,
            /*inventory_pos=*/{13, 13},
            /*inventory_texture=*/"res/img/inventory_outline.png",
            /*inventory_selecting_texture=*/"res/img/Outline_selector.png",
            /*extra_inv_texture=*/"res/img/Extra_Inven.png",
            /*crafting_menu_texture=*/"res/img/Crafting_UI.png",

            /*display_name=*/"Daveeska"
            );

    level.changeLevel("res/maps/test.json");

    camera = { 0 };
    camera.target = { player.getBody().x + 18*7, player.getBody().y + 35*7 };
    camera.offset = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 0.6;

    commands = {
        "/tell",
        "/reset",
        "/clear",
        "/debug",
        "/give",
        "/load"
    };

    player.addItemInv(newItem(Bagofcherry_Tile, 10));
    entities.push_back(std::make_unique<Cat>(Cat({100,70}, player)));
}

void UpdateGameplayScreen(){
    if(!isTyping){
        player.UpdateInventory();
        player.UpdateVariables();
        player.move(GetFrameTime());

        camera.target = { player.getBody().x + 18*7, player.getBody().y + 35*7 };
    }

    if(IsKeyPressed(KEY_SLASH)){
        isTyping=true;
    }

    if(isTyping)
        typingCode();

    for(auto& entity:entities)
        if(entity->getLevelName() == level.level_name) entity->Update(player);

    UpdateTiles();

    if(IsKeyPressed(KEY_ESCAPE))
        finish_screen = 1;
}

void DrawGameplayScreen(){
    BeginMode2D(camera);

    drawInCamMode();

    EndMode2D();

    for(auto& entity : entities){
        if(entity->getLevelName() == level.level_name) entity->Draw_UI();
    }

    player.InventoryDraw(camera);

    for(auto e:texts) e.Draw();

    if(isTyping){
        DrawRectangleRec({30,(float)GetScreenHeight()-50,500,35}, {20,20,20,130});
        DrawText(user_input.c_str(), 30, (float)GetScreenHeight()-50, 35, BLACK);
    }

    if(is_debugging)
        drawDebugInfo();
}

void UnloadGameplayScreen(){
    //TODO
}

void ResetGameplayFinishScreen(){
    finish_screen=0;
}

int FinishGameplayScreen(){
    return finish_screen;
}
