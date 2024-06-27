#include "level.h"

#include <iostream>

#include "raylib.h"
#include "screens.h"
#include "global_func.h"
#include "tiling_util.h"

#include "seed.h"

void Level::changeLevel(std::string levelName){
    level_name = levelName;
    tiles = loadLevelFromFile(levelName, total_layers, canvas_size, starting_pos);
    std::cout<<"Level changed to "<<level_name<<std::endl;
}

void Level::Interact(std::unique_ptr<Tile>& tile, std::string& tile_interect_return_code){
    if(tile->getIsTouchinSelectAreaPlayer() && tile->getIsTouchingMouse() && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        if(auto ret = tile->Interact(); !ret.empty())
        {
            tile_interect_return_code = ret;
        }
    }
}

void Level::UseCraftingTable(std::unique_ptr<Tile>& tile){
    if(tile->getID() == Craftingtable_Tile){
        if(tile->getIsTouchingPlayer()){
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && tile->getIsTouchingMouse()){
                getPlayer().toggleInvenCrafting();
            }
        }else{
            getPlayer().setInvIsCrafting(false);
        }
    }
}

void Level::PlaceItem(std::unique_ptr<Tile>& tile){
    if(tile->getID() == PlaceArea_Tile && tile->getIsTouchinSelectAreaPlayer()){
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                tile->getIsTouchingMouse()){
            Vector2 belowPos = {tile->getBody().x, tile->getBody().y};
            int below_z = tile->getZ();
            std::vector<std::string> temp_string = {"Item", "BagOfSeed"};
            auto it = std::find_if(tiles.begin(), tiles.end(),
                    [belowPos, below_z, temp_string](const auto& item) {
                    return item->getPos().x == belowPos.x && item->getPos().y == belowPos.y && item->getZ() > below_z && std::count(temp_string.begin(), temp_string.end(), item->getType());
                    });
            if(it == tiles.end()){
                tiles.push_back(std::make_unique<Tile>
                        (Tile(getPlayer().getCurrentInvIDSlot(), {tile->getX(), tile->getY()}, tile->getZ()+1)));
                getPlayer().decreaseItemInv(getPlayer().getCurrentInvSlot());
            }
        }
    }
}

void Level::PlantSeed(std::unique_ptr<Tile>& tile){
    if(tile->getID() == Farmland_Tile && tile->getIsTouchinSelectAreaPlayer() && getPlayer().getInv().getItemFromCurrentSlot().item_type == "BagOfSeed"){
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && tile->getIsTouchingMouse()){
            Vector2 belowPos = {tile->getBody().x, tile->getBody().y};
            int below_z = tile->getZ();
            auto it = std::find_if(tiles.begin(), tiles.end(),
                    [belowPos, below_z](const auto& item) {
                    return item->getPos().x == belowPos.x && item->getPos().y == belowPos.y && item->getZ() != below_z && item->getType() == "Seeds";
                    });
            if(it == tiles.end()){
                int fruitID = Tile(getPlayer().getCurrentInvIDSlot(), {0,0}, 0).getFruitID();
                SeedTile temp_tile = SeedTile(fruitID, tile->getPos(), tile->getZ()+1);
                temp_tile.setSlot(tiles.size());
                tiles.push_back(std::make_unique<SeedTile>(temp_tile));
                getPlayer().decreaseItemInv(getPlayer().getCurrentInvSlot());
            }
        }
    }
}

void Level::AnimateTile(std::unique_ptr<Tile>& tile){
    if(tile->HasAnimFrame() && tile->getIsTouchinSelectAreaPlayer() && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        tile->runAnimation();
}

void Level::removeEntity(int slot){
    entities.erase(entities.begin() + slot);
}

void Level::Update(){
    std::string tile_interect_return_code;
    for(auto& tile : tiles){
        tile->setIsTouchingSelectAreaPlayer(false);
        tile->setIsTouchingPlayer(false);
        tile->setIsTouchingMouse(false);

        if(CheckCollisionRecs(tile->getBody(), getPlayer().getSelectArea())){
            tile->setIsTouchingSelectAreaPlayer(true);
        }

        if(CheckCollisionRecs(tile->getBody(), getPlayer().getBody())){
            tile->setIsTouchingPlayer(true);
        }

        if(CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tile->getBody())){
            tile->setIsTouchingMouse(true);
        }

        tile->Update();

        if((tile->getType() == "Item" || tile->getType() == "BagOfSeed")&& tile->getIsTouchinSelectAreaPlayer() && tile->getIsTouchingMouse()){
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                getPlayer().addItemInv(tile->asItem(1));
                std::erase(tiles, tile);
                break;
            }
        }

        Interact(tile, tile_interect_return_code);
        UseCraftingTable(tile);
        PlaceItem(tile);
        PlantSeed(tile);
        AnimateTile(tile);
    }

    if(!tile_interect_return_code.empty()){
        //X001 is the code to remove that tile. For now being use only to remove SeedTIle when harved.
        if(getFirstWord(tile_interect_return_code) == "x001" && !getSecondWord(tile_interect_return_code).empty()){
            tiles.erase(std::remove_if(tiles.begin(), tiles.end(), [tile_interect_return_code](const auto& tile){
                        return tile->getSlot() == std::stoi(getSecondWord(tile_interect_return_code));
                        }), tiles.end());
        }
        else{
            changeLevel(tile_interect_return_code);
        }
    }

    for(auto& entity:entities){
        if(entity->getLevelName() == level_name){
            entity->Update(getPlayer(), camera);
            if(entity->isDead()){
                std::erase(entities, entity);
                break;
            }
        }
    }

    igTime += GetFrameTime() * 6;

}

void Level::DrawUI(){
    for(auto& entity : entities){
        if(entity->getLevelName() == level_name) entity->Draw_UI();
    }

    DrawTextureEx(timeSquare, {13, (float)GetScreenHeight()/2}, 0, 3, WHITE);

    DrawTextEx(font, "Time: ", {30, (float)GetScreenHeight()/2+12}, 25, 0, WHITE);

    DrawTextEx(font, floatToTime(igTime).c_str(), {114, (float)GetScreenHeight()/2+12}, 25, 0, GREEN);
}

void Level::DrawSKY(){
    Color skyColor = BLACK;
    skyColor.a = igTime;
    DrawRectangleRec({0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, skyColor);
}

void Level::Draw(){
    for(auto& tile : tiles){
        tile->Draw(isDebugging);
    }
    for(auto& tile : tiles)
        if(tile->HasCollision() && isDebugging) DrawRectangleRec(tile->getBody(), {255,255,255,50});

    for(auto& entity : entities)
        if(entity->getLevelName() == level_name) entity->Draw();
}

Level::Level(std::string levelName){
    level_name = levelName;
    tiles = loadLevelFromFile(levelName, total_layers, canvas_size, starting_pos);

    timeSquare = LoadTexture("res/img/MoneySquare.png");
    igTime = 0.0f;

    std::cout<<"Created Level: "<<level_name<<std::endl;
}

Level::~Level(){
    tiles.clear();
    level_name.clear();

    entities.clear();
}
