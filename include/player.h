#pragma once

#include<vector>
#include<string>

#include<raylib.h>

#include"animation.h"
#include"inventory.h"

#define DIRECTION_UP 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3
#define DIRECTION_RIGHT 4

class Player{
    private:
        Rectangle body;
        Rectangle selectArea;
        Rectangle collisionArea;
        Rectangle newPos;

        int speed;
        int default_speed;

        //Animation
        Texture2D texture;
        std::vector<SpriteAnimation> animations;

        int current_animation;
        int direction;

        Inventory inv;

        std::string display_name;

        //Debug variable
        bool isToucingItem;

        void takeItem();
        void placeItem();
        void interactWithItem();

    public:
        //Getter
        Rectangle getBody(){return body;}
        Rectangle getSelectArea(){return selectArea;}

        int getSpeed(){return speed;}
        bool getIsToucingItem(){return isToucingItem;}
        std::string getDisplayName(){return display_name;}
        Inventory getInv(){return inv;}

        //Setters
        void setTouchingBool(bool value){isToucingItem=value;}
        void setPos(Vector2 pos){body.x=pos.x; body.y=pos.y;}

        //Inventoy functions
        void addItemInv(InventoryItem item);
        void decreaseItemInv(int slot);
        void toggleInvenCrafting();
        void setInvIsCrafting(bool value);
        void updateCraftableItem();

        int getCurrentInvIDSlot();        
        int getCurrentInvSlot();
        int getCurrentInvCraftAbleID();

        bool invHas(int id);
        bool invHas(RecipeItem criteria);

        //Public Functions
        void move(float dt);

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
                std::string crafting_menu_texture,
                /*customization*/
                std::string display_name);
};
