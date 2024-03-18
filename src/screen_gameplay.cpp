#include<vector>
#include<string>
#include<memory>

#include<raylib.h>

#include"screens.h"

#include"global_func.h"
#include"tiling_util.h"

#include"player.h"
#include"tile.h"
#include"chat.h"

#include"entity.h"
#include"cat.h"

#define INTERACT_KEY KEY_I

static bool is_debugging=false;
static bool isTyping=false;
static int finish_screen=0;

static Player player;
static std::vector<std::unique_ptr<Entity>> entities;

static Camera2D camera ;
static Sound pickupsound;

static std::vector<ChatText> texts;
static std::vector<std::unique_ptr<Tile>> tiles;
static std::string user_input;

#define MAX_CHAT_TEXTS 100

std::vector<std::string> commands;

void typeInChat(std::string text){
    if(texts.size()>MAX_CHAT_TEXTS)
        texts.erase(texts.begin());
    addChatText(texts, text);
}

void typeInChat(std::string text, Color color){
    typeInChat(std::to_string(texts.size()));
    if(texts.size()>MAX_CHAT_TEXTS)
        texts.erase(texts.begin());
    addChatText(texts, text, color);
}

static void drawDebugInfo(){
    printText("FPS: ", std::to_string(GetFPS()), {0,0}, 20);

    printText("Player X: ", std::to_string((int)player.getBody().x), {0,20}, 20);
    printText("Player Y: ", std::to_string((int)player.getBody().y), {0,40}, 20);

    printText("Player Is Toucing Item: ", std::to_string(player.getIsToucingItem()), {0,60}, 20);

    printText("Mouse X: ", std::to_string((int)GetScreenToWorld2D(GetMousePosition(), camera).x), {0,100}, 20);
    printText("Mouse Y: ", std::to_string((int)GetScreenToWorld2D(GetMousePosition(), camera).y), {0,120}, 20);

    printText("Total Tiles: ", std::to_string(tiles.size()), {0,160}, 20);

    printText("Total texts: ", std::to_string(texts.size()), {0,200}, 20);
}

static void switchLevel(std::string fileName){
    tiles = loadLevelFromFile(fileName);
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
    for(std::size_t i=0;i<tiles.size();i++){
        if(tiles[i]->getName() == "transitionarea"){
            if(CheckCollisionRecs(player.getBody(), tiles[i]->getBody()) && IsKeyPressed(INTERACT_KEY)){
                switchLevel(tiles[i]->getDestination());
                break;
            }
        }
        //Taking tiles
        if(tiles[i]->getType() == "Item"){
            if(CheckCollisionRecs(player.getSelectArea(), tiles[i]->getBody()) &&
                    CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tiles[i]->getBody())){
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    player.addItemInv(tiles[i]->asItem(1));
                    tiles.erase(tiles.begin() + i);

                    PlaySound(pickupsound);
                }
            }
        }
        //Placing tiles
        if(tiles[i]->getType() == "Special"){
            if(tiles[i]->getName() == "placearea" && CheckCollisionRecs(player.getSelectArea(), tiles[i]->getBody()) &&
                    CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tiles[i]->getBody())){
                if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                    tiles.push_back(std::make_unique<Tile>
                            (Tile(player.getCurrentInvIDSlot(), {tiles[i]->getX(), tiles[i]->getY()}, tiles[i]->getZ()+1)));
                    player.decreaseItemInv(player.getCurrentInvSlot());

                    PlaySound(pickupsound);
                }
            }
        }
    }
}

static void drawInCamMode(){
    for(auto& tile:tiles){
        tile->Draw(is_debugging);
    }

    for(auto& e:entities)
        e->Draw();

    player.Draw(is_debugging);
}

void InitGameplayScreen(){
    player = Player(
            /*Body*/{50,200,18*9, 35*9},
            /*Speed*/500,
            /*texture_path=*/"res/img/player_atlas.png",
            /*selectArea*/{0,0,450, 450},
            /*collisionBody=*/{0,0,18*9,10*9},

            /*slots=*/10,
            /*inventory_pos=*/{10, 12},
            /*inventory_texture=*/"res/img/inventory_outline.png",
            /*inventory_selecting_texture=*/"res/img/Outline_selector.png",
            /*extra_inv_texture=*/"res/img/Extra_Inven.png",

            /*display_name=*/"Daveeska"
            );

    tiles = loadLevelFromFile("res/maps/test.json");

    for(auto& e:tiles){
        if(e->getName()=="itemarea"){
            int probability=GetRandomValue(1,4);
            if(probability == 2 or probability == 3 or probability == 4){
                tiles.push_back(std::make_unique<Tile>(Tile(probability, {e->getX(), e->getY()}, e->getZ()+1)));
            }
        }
    }

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


    entities.push_back(std::make_unique<Cat>(Cat({40,50}, player)));

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

    for(auto& e:entities)
        e->Update(player);

    UpdateTiles();
}

void DrawGameplayScreen(){
    BeginMode2D(camera);

    drawInCamMode();

    EndMode2D();

    player.InventoryDraw(camera);

    for(auto e:texts) e.Draw();

    if(isTyping){
        DrawRectangleRec({30,(float)GetScreenHeight()-50,500,35}, {20,20,20,130});
        DrawText(user_input.c_str(), 30, (float)GetScreenHeight()-50, 35, BLACK);
    }

    DrawText("PRESS 'I' TO INTERACT", GetScreenWidth()-400, GetScreenHeight()-30, 30, BLACK);

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
