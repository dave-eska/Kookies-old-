#pragma once

#include<string>
#include<string>
#include<tuple>

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

        std::string filename;

        int seedID; /*Only being used by 'type=BagOfSeed' tile*/

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
        bool isTouchingSelectAreaPlayer;
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

        int getSeed(){return seedID;} /*Only being used by 'type=BagOfSeed' tile*/

        bool HasAnimFrame(){return hasAnimation;}
        bool HasCollision(){return hasCollision;}

        bool getIsRunningAnimation(){return isRunningAnimation;}

        bool getIsTouchingMouse(){return isTouchingMouse;}
        bool getIsTouchingPlayer(){return isTouchingPlayer;}
        bool getIsTouchinSelectAreaPlayer(){return isTouchingSelectAreaPlayer;}

        InventoryItem asItem(int total_count); //Will set idx/slot to 0

        Rectangle getBody(){return body;}
        Texture2D getTexture(){return texture;}
        Vector2 getPos(){return {body.x, body.y};}

        //Seters
        void setBod(Rectangle bod){body=bod;}
        void setX(float x){body.x=x;}
        void setY(float y){body.y=y;}
        void setSlot(int vec_slot){slot = vec_slot;}

        void setIsTouchingMouse(bool val){isTouchingMouse = val;}
        void setIsTouchingPlayer(bool val){isTouchingPlayer = val;}
        void setIsTouchingSelectAreaPlayer(bool val){isTouchingSelectAreaPlayer = val;}

        void runAnimation(){isRunningAnimation=true;}

        virtual void Update();
        virtual std::string Interact();
        virtual void Draw(bool is_debugging);

        //Constructors
        Tile();
        Tile(int id, Vector2 pos, int z_level);

        virtual ~Tile() = default;
};

enum TileID{
    AIR_TILE,
    GRASS_TILE,
    MILK_TILE,
    CHEST_TILE,
    COOKIE_TILE,
    BRICKWALL_TILE,
    TRANSITION_TILE,
    FLOOR_TILE,
    ITEMAREA_TILE,
    DOORUP_TILE,
    DOORDOWN_TILE,
    CRAFTINGTABLE_TILE,
    CHOCOLATE_TILE,
    CHERRY_TILE,
    CHOCOLATECAKE_TILE,
    BAGOFCHERRY_TILE,
    CHERRYSEEDS_TILE,
    FARMLAND_TILE,
    EMPTYBAG_TILE,
    COCOABEANS_TILE,
    BAGOFCOCOABEANS_TILE,
    COCOABEANSSEES_TILE,
};
