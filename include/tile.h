#pragma once

#include<vector>
#include<string>
#include<string>

#include<raylib.h>

#include"item.h"
#include"animation.h"

#define TILE_SIZE 96
#define TOTAL_ANIM_FRAME (4)

class Tile{
    protected:
        Rectangle body;

        Texture2D texture;
        Texture2D debugbox;

        std::string name;
        std::string type;
        int id;

        int z_level;

        std::string fileName;

        //animations
        bool hasAnimation;
        bool isRunningAnimation;
        SpriteAnimation animation;

        //Debug var
        bool isTouchingMouse;
        bool hasCollision;
        bool isTouchingPlayer;
    public:

        //Getters
        std::string getName(){return name;}
        std::string getType(){return type;}

        float getX(){return body.x;}
        float getY(){return body.y;}

        int getZ(){return z_level;}
        int getID(){return id;}

        bool HasAnimFrame(){return hasAnimation;}
        bool HasCollision(){return hasCollision;}
        
        bool getIsRunningAnimation(){return isRunningAnimation;}

        std::string getTransitionLevel(){return fileName;}

        InventoryItem asItem(int total_count); //Will set idx/slot to 0
        
        Rectangle getBody(){return body;}
        Texture2D getTexture(){return texture;}
        
        //Seters
        void setBod(Rectangle bod){body=bod;}
        void setX(float x){body.x=x;}
        void setY(float y){body.y=y;}
        void runAnimation(){isRunningAnimation=true;}

        void Update();
        void Draw(bool is_debugging);

        void attachLevel(std::string levelName);

        //Constructors
        Tile();
        Tile(int id, Vector2 pos, int z_level);
};

std::vector<Tile> loadLevelFromFile(std::string file_path);

Tile newItem(int id);
Tile newItem(std::string name);

class TransitionTile: public Tile{
    public:
        TransitionTile(Vector2 pos, int z_level);
};
