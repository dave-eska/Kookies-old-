#pragma once

#include<vector>
#include<string>

#include<raylib.h>

#include<box2d/box2d.h>

#include"animation.h"
#include"tile.h"
#include"inventory.h"

#define DIRECTION_UP 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3
#define DIRECTION_RIGHT 4

class Player{
    private:
        Rectangle body;
        Rectangle selectArea;

        int speed;
        int default_speed;

        //Animation
        Texture2D texture;
        std::vector<SpriteAnimation> animations;

        int current_animation;
        int direction;

        Inventory inv;

        std::string display_name;

        struct{
            b2BodyDef bodyDef;
            b2Body *body;
            b2PolygonShape shape;
            b2FixtureDef fixtureDef;
        }b2;

        //Debug variable
        bool isToucingItem;

        void takeItem();
        void placeItem();
        void interactWithItem();

    public:
        //Getter
        Rectangle getBody(){return body;}
        int getSpeed(){return speed;}
        bool getIsToucingItem(){return isToucingItem;}
        std::string getDisplayName(){return display_name;}

        //Setters
        void setTouchingBool(bool value){isToucingItem=value;}
        void setPos(Vector2 pos){body.x=pos.x; body.y=pos.y;}

        //Public Functions
        void move(float dt);

        void interactItem(std::vector<Tile>& tileVec, Camera2D& camera, Sound pickupsound);

        void UpdateVariables();
        void animate();

        void UpdateInventory();

        void Draw(bool isDebuggin);
        void InventoryDraw(Camera2D& camera);

        Player();

        //Constructor
        Player(Rectangle body, int speed, const char* texture_path, Rectangle selectArea, Rectangle collisionBody,
                /*inv*/
                int slots, Vector2 inventory_pos, 
                std::string inventory_texture, std::string inventory_selecting_texture, std::string extra_inv_texture,
                /*box2d stuff*/
                b2World &world,
                /*customization*/
                std::string display_name);
};

