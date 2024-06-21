#pragma once

#include <vector>
#include <memory>
#include <string>

#include "raylib.h"
#include "tile.h"
#include "entity.h"

class Level{
    private:
        std::vector<std::unique_ptr<Entity>> entities;
        std::string level_name;

        Vector2 starting_pos;
        Vector2 canvas_size;
        int total_layers;

        Texture2D timeSquare;

        float igTime;

        //! Update Tile Functions
        void Interact(std::unique_ptr<Tile>& tile, std::string& tile_interect_return_code);
        void UseCraftingTable(std::unique_ptr<Tile>& tile);
        void PlaceItem(std::unique_ptr<Tile>& tile);
        void PlantSeed(std::unique_ptr<Tile>& tile);
        void AnimateTile(std::unique_ptr<Tile>& tile);

    public:
        Vector2 getStartingPos(){return starting_pos;}
        Vector2 getCanvasSize(){return canvas_size;}
        int getTotalLayers(){return total_layers;}
        std::string getLevelName(){return level_name;}

        int totalEntity(){return entities.size();}

        std::vector<std::unique_ptr<Tile>> tiles;
        int getTotalTiles(){return tiles.size();}

        void removeTile(int slot){tiles.erase(tiles.begin() + slot);}

        void setTotalLayers(int val){total_layers = val;}

        template<typename T>
            void AddEntity(T entity, int& slot){
                entities.push_back(std::make_unique<T>(entity));
                slot = entities.size()-1;
            }
        template<typename T>
            void AddEntity(T entity){
                entities.push_back(std::make_unique<T>(entity));
            }

        void removeEntity(int slot);

        void changeLevel(std::string levelName);

        void Update();

        void DrawUI();
        void DrawSKY();
        void Draw();

        Level();
        Level(std::string levelName);

        ~Level();
};
