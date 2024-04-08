#include<algorithm>
#include<iostream>
#include<vector>
#include<string>
#include<memory>

#include<raylib.h>

#include"screens.h"

#include"global_func.h"

#include"level.h"

#include"player.h"
#include"tile.h"
#include"chat.h"

#include"entity.h"
#include"npc.h"

#define INTERACT_KEY KEY_I

static bool is_debugging=false;
static bool isTyping=false;
static int finish_screen=0;

static Player player;
static std::vector<std::unique_ptr<Entity>> entities;

static Camera2D camera ;
static Sound pickupsound;

static std::vector<ChatText> texts;
static Level level;
static std::string user_input;

static std::vector<std::string> commands;

#define MAX_CHAT_TEXTS 100


void typeInChat(std::string text){
    if(texts.size()>MAX_CHAT_TEXTS)
        texts.erase(texts.begin());
    addChatText(texts, text);
}

void typeInChat(std::string text, Color color){
    if(texts.size()>MAX_CHAT_TEXTS)
        texts.erase(texts.begin());
    addChatText(texts, text, color);
}

void changeMainLevel(std::string levelName){
    level.changeLevel(levelName);
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
            }
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
    for(auto& tile : level.tiles){

        tile->setIsTouchingSelectAreaPlayer(false);
        tile->setIsTouchingPlayer(false);
        tile->setIsTouchingMouse(false);

        //Chechking for collision
        if(CheckCollisionRecs(tile->getBody(), player.getSelectArea())){
            tile->setIsTouchingSelectAreaPlayer(true);
        }

        if(CheckCollisionRecs(tile->getBody(), player.getBody())){
            tile->setIsTouchingPlayer(true);
        }

        if(CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tile->getBody())){
            tile->setIsTouchingMouse(true);
        }

        if(IsKeyPressed(KEY_I)){
            tile->Interact();
        }

        //Taking level.tiles
        if((tile->getType() == "Item" || tile->getType() == "BagOfSeed")&& tile->getIsTouchinSelectAreaPlayer() && tile->getIsTouchingMouse()){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                player.addItemInv(tile->asItem(1));
                std::erase(level.tiles, tile);
                break;
            }
        }

        //Crafting
        if(tile->getName() == "crafting_table" && tile->getIsTouchinSelectAreaPlayer()){
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && tile->getIsTouchingMouse()){
                player.toggleInvenCrafting();
            }else{
                player.setInvIsCrafting(false);
            }
        }

        //Placing level.tiles
        if(tile->getName() == "placearea" && tile->getIsTouchinSelectAreaPlayer()){
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                    tile->getIsTouchingMouse()){
                Vector2 belowPos = {tile->getBody().x, tile->getBody().y};
                int below_z = tile->getZ();
                auto it = std::find_if(level.tiles.begin(), level.tiles.end(),
                        [belowPos, below_z](const auto& item) {
                        return item->getPos().x == belowPos.x && item->getPos().y == belowPos.y && item->getZ() > below_z && item->getType() == "Item";
                        });
                if(it == level.tiles.end()){
                    level.tiles.push_back(std::make_unique<Tile>
                            (Tile(player.getCurrentInvIDSlot(), {tile->getX(), tile->getY()}, tile->getZ()+1)));
                    player.decreaseItemInv(player.getCurrentInvSlot());
                }
            }
        }
        //Planting
        if(tile->getName() == "farmland" && tile->getIsTouchinSelectAreaPlayer() && player.getInv().getItemFromCurrentSot().item_type == "BagOfSeed"){
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                    tile->getIsTouchingMouse()){
                Vector2 belowPos = {tile->getBody().x, tile->getBody().y};
                int below_z = tile->getZ();
                auto it = std::find_if(level.tiles.begin(), level.tiles.end(),
                        [belowPos, below_z](const auto& item) {
                        return item->getPos().x == belowPos.x && item->getPos().y == belowPos.y && item->getZ() > below_z && item->getType() == "Seeds";
                        });
                if(it == level.tiles.end()){
                    level.tiles.push_back(std::make_unique<Tile>
                            (Tile(Tile(player.getInv().getItemFromCurrentSot().tileID, {0,0}, 0).getSeed(),
                                  {tile->getX(), tile->getY()}, tile->getZ()+1)));
                    player.decreaseItemInv(player.getCurrentInvSlot());
                }
            }
        }
    }
}

static void drawInCamMode(){
    for(auto& tile:level.tiles){
        tile->Draw(is_debugging);
    }

    for(auto& entity:entities)
        if(entity->getLevelName() == level.level_name) entity->Draw();

    player.Draw(is_debugging);
}

void InitGameplayScreen(){
    player = Player(
            /*Body*/{50,200,18*9, 35*9},
            /*Speed*/500,
            /*texture_path=*/"res/img/player_atlas.png", /*selectArea*/{0,0,450, 450},
            /*collisionBody=*/{0,0,18*9,10*9},

            /*slots=*/10,
            /*inventory_pos=*/{10, 12},
            /*inventory_texture=*/"res/img/inventory_outline.png",
            /*inventory_selecting_texture=*/"res/img/Outline_selector.png",
            /*extra_inv_texture=*/"res/img/Extra_Inven.png",
            /*crafting_menu_texture=*/"res/img/Crafting_UI.png",

            /*display_name=*/"Daveeska"
            );
    level.changeLevel("res/maps/items.json");

    camera = { 0 };
    camera.target = { player.getBody().x + 18*7, player.getBody().y + 35*7 };
    camera.offset = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 0.65f;

    commands = {
        "/tell",
        "/reset",
        "/clear",
        "/debug"
    };

    entities.push_back(std::make_unique<NPC>(NPC("res/maps/items.json", {288, 572}, "Opening", "opening.json", 7)));

    pickupsound = LoadSound("res/sound/pickup.wav");
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
