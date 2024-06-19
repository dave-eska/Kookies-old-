#include <vector>
#include <string>

#include<raylib.h>

#include "cat.h"
#include "enchant.h"
#include "item.h"
#include "npc.h"
#include "screens.h"

#include "global_func.h"

#include "level.h"

#include "player.h"
#include "tile.h"
#include "chat.h"

#include "tool.h"

#define INTERACT_KEY KEY_I

static int finish_screen=0;

static Player player;

static Sound pickupsound;

static Level *level;
static std::string user_input;

static std::vector<std::string> commands;

static bool isOnEscMenu;
static Texture2D escButtonTexture;

void changeMainLevel(std::string levelName){
    level->changeLevel(levelName);
}

Player &getPlayer(){
    return player;
}

Level &getCurrentLevel(){
    return *level;
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
                isDebugging = !isDebugging;
            else if(command == "/tell"){
                if(getSecondWord(user_input) == "player.pos"){
                    typeInChat("X: " + std::to_string(player.getBody().x));
                    typeInChat("Y: " + std::to_string(player.getBody().y));
                }else{
                    typeInChat("Syntax Error: Expected Input Detail.", DARKPURPLE);
                }
            }else if(command == "/give"){
                if(!getSecondWord(user_input).empty()){
                    player.addItemInv(newItem<Tile>(stoi(getSecondWord(user_input)), 1));
                }else{
                    typeInChat("Syntax Error: Expected Input Detail.", DARKPURPLE);
                }
            }else if(command == "/givet"){
                if(!getSecondWord(user_input).empty()){
                    player.addItemInv(newItem<Tool>(stoi(getSecondWord(user_input)), 1));
                }else{
                    typeInChat("Syntax Error: Expected Input Detail.", DARKPURPLE);
                }
            }else if(command == "/load" && !getSecondWord(user_input).empty()){
                level->changeLevel(getSecondWord(user_input));
            }else if(command == "/enchant" && !getSecondWord(user_input).empty()){
                player.enchantInvItem(player.getCurrentInvSlot(), Enchant::Sharpness);
            }
        }else{
            user_input.erase(user_input.begin());
            addChatText(texts, player.getDisplayName() + ": " + user_input);
        }
        user_input.clear();
        isTyping=false;
    }
    for(auto &text:texts) text.Update();

    isOnEscMenu = false;
}

static void drawInCamMode(){
    level->Draw();
    player.Draw(isDebugging, camera);
}

void InitGameplayScreen(){
    isDebugging = false;
    isTyping = false;

    player = Player(
        /*Body*/{TILE_SIZE*14, -TILE_SIZE, 18*9, 35*9},
        /*Speed*/500,
        /*texture_path=*/"res/img/player_atlas.png",
        /*selectArea*/{0,0,430, 430},
        /*collisionBody=*/{0,0,18*9,10*9},

        /*slots=*/10,
        /*inventory_pos=*/{13, 13},
        /*inventory_texture=*/"res/img/inventory_outline.png",
        /*inventory_selecting_texture=*/"res/img/Outline_selector.png",
        /*extra_inv_texture=*/"res/img/Extra_Inven.png",
        /*crafting_menu_texture=*/"res/img/Crafting_UI.png",

        /*display_name=*/config["DisplayName"].asString()
    );

    escButtonTexture = LoadTexture("res/img/EscMenu.png");

    level = new Level(config["defaultMap"].asString());

    camera = { 0 };
    camera.target = {player.getBody().x + (player.getBody().width/2), player.getBody().y + (player.getBody().height/2)};
    camera.offset = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = config["camera_zoom"].asFloat();

    commands = {
        "/tell",
        "/reset",
        "/clear",
        "/debug",
        "/give",
        "/givet",
        "/load",
        "/enchant",
    };

    player.addItemInv(newItem<Tool>(Sword_Tool));
    player.addItemInv(newItem<Tool>(WoodenFishingRod_Tool));
    level->AddEntity<Cat>(Cat({TILE_SIZE*2, TILE_SIZE*3}, player, "save.json"));
    level->AddEntity<NPC>(NPC("save.json", {TILE_SIZE*10, -TILE_SIZE}, "Seller", "opening.json", 3));
    level->AddEntity<NPC>(NPC("save.json", {TILE_SIZE*12, -TILE_SIZE}, "Buyer", "opening.json", 2));
}

void UpdateGameplayScreen(){
    if(!isTyping){
        camera.target = {player.getBody().x + (player.getBody().width/2), player.getBody().y + (player.getBody().height/2)};
        player.move(GetFrameTime());
    }

    player.UpdateTools(camera);
    player.UpdateInventory();
    player.UpdateVariables();

    level->Update();

    if(IsKeyPressed(KEY_SLASH)){
        isTyping=true;
    }

    if(isTyping)
        typingCode();

    if(IsKeyPressed(KEY_ESCAPE))
        isOnEscMenu = true;
}

void DrawGameplayScreen(){
    BeginMode2D(camera);

    drawInCamMode();

    EndMode2D();

    level->DrawUI();

    player.Draw_UI(camera);

    for(auto e:texts) e.Draw();

    if(isTyping){
        DrawRectangleRec({30,(float)GetScreenHeight()-50, (float)(GetScreenWidth()-60), 35}, {20,20,20,130});
        DrawText(user_input.c_str(), 30, (float)GetScreenHeight()-50, 35, BLACK);
    }
}

void UnloadGameplayScreen(){
    delete level;
}

void ResetGameplayFinishScreen(){
    finish_screen=0;
}

int FinishGameplayScreen(){
    return finish_screen;
}
