#pragma once

#include<vector>
#include<string>
#include<string>
#include<memory>

#include<raylib.h>

#include<json/value.h>

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

        std::string dest /*Only being used by `transation` tile*/;

        std::string filename /*Only being used by `transation` tile*/;

        //Collision
        bool collision;

        //animations
        bool hasAnimation;
        bool isRunningAnimation;
        SpriteAnimation animation;

        //Debug var
        bool isTouchingMouse;
        bool hasCollision;
        bool isTouchingPlayer;
        int slot;
    public:

        //Getters
        std::string getName(){return name;}
        std::string getType(){return type;}

        float getX(){return body.x;}
        float getY(){return body.y;}

        int getZ(){return z_level;}
        int getID(){return id;}

        int getSlot(){return slot;}

        bool HasAnimFrame(){return hasAnimation;}
        bool HasCollision(){return hasCollision;}

        bool getIsRunningAnimation(){return isRunningAnimation;}

        InventoryItem asItem(int total_count); //Will set idx/slot to 0

        std::string getDestination(){return dest;} /*Only being used by `transation` tile*/

        void attachLevel(std::string levelName); /*Only being used by `transation` tile*/
        void changeLevel(std::vector<std::unique_ptr<Tile>>& tiles); /*Only being used by `transation` tile*/

        Rectangle getBody(){return body;}
        Texture2D getTexture(){return texture;}
        Vector2 getPos(){return {body.x, body.y};}

        //Seters
        void setBod(Rectangle bod){body=bod;}
        void setX(float x){body.x=x;}
        void setY(float y){body.y=y;}
        void setSlot(int vec_slot){slot = vec_slot;}

        void runAnimation(){isRunningAnimation=true;}

        void Update();
        void Draw(bool is_debugging);


        //Constructors
        Tile();
        Tile(int id, Vector2 pos, int z_level);
};
