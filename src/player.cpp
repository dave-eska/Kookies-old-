#include"player.h"

#include <cmath>
#include<raylib.h>

#include<string>

#include"global_func.h"
#include"inventory.h"

//Private functions
void Player::move(float dt){
    float inputX = IsKeyDown(KEY_D)-IsKeyDown(KEY_A);
    float inputY = IsKeyDown(KEY_S)-IsKeyDown(KEY_W);

    Vector2 npOffSet = {55, 180};

    if(inputX != 0){
        newPos.x = (body.x + npOffSet.x) + (inputX*speed)*dt;
        if(isWalkableX(newPos, {body.x+npOffSet.x, body.y+npOffSet.y, body.width, body.height})){
            body.x = newPos.x - npOffSet.x;
        }
    }

    if(inputY != 0){
        newPos.y = (body.y + npOffSet.y) + (inputY*speed)*dt;

        if(isWalkableY(newPos, {body.x+npOffSet.x, body.y+npOffSet.y, body.width, body.height})){
            body.y = newPos.y - npOffSet.y;;
        }
    }

    if(inputX==1) direction=DIRECTION_RIGHT;
    else if(inputX==-1) direction=DIRECTION_LEFT;

    if(inputY==1) direction=DIRECTION_DOWN;
    else if(inputY==-1) direction=DIRECTION_UP;

    //Diogonal Animations;
    if(inputX==1&&(inputY==1||inputY==-1)) direction=DIRECTION_RIGHT;
    if(inputX==-1&&(inputY==1||inputY==-1)) direction=DIRECTION_LEFT;

    //Making the selectArea follows the player;
    selectArea.x = body.x-selectArea.width/2/2;
    selectArea.y = body.y-selectArea.height/2/2 + 40;
}

void Player::animate(){
    if(IsKeyDown(KEY_W) || IsKeyDown(KEY_S) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D)){
        if(direction==DIRECTION_UP) current_animation=5;
        else if(direction==DIRECTION_DOWN) current_animation=1;
        else if(direction==DIRECTION_LEFT) current_animation=3;
        else if(direction==DIRECTION_RIGHT) current_animation=7;
    }else{
        if(direction==DIRECTION_UP) current_animation=4;
        else if(direction==DIRECTION_DOWN) current_animation=0;
        else if(direction==DIRECTION_LEFT) current_animation=2;
        else if(direction==DIRECTION_RIGHT) current_animation=6;
    }

    Rectangle dest={body.x,body.y,body.width, body.height};
    Vector2 origin = { 0 };
    DrawSpriteAnimationPro(animations[current_animation], dest, origin, 0, WHITE);
}

//Public Functions
void Player::UpdateVariables(){
    isToucingItem=false;
}

void Player::UpdateInventory(){
    inv.changeCurrentSlot();

    inv.toggleDrawUI();
    if(inv.getIsCrafting())
        inv.UpdateCraftableTileID();

    if(IsKeyDown(KEY_LEFT_SHIFT)){
        for (int i = KEY_ONE; i <= KEY_NINE; ++i) {
            if (IsKeyPressed(i))
                inv.moveItemToSlot(i - KEY_ONE, inv.getCurrentSlot());
        }
        if(IsKeyPressed(KEY_ZERO)){
            inv.moveItemToSlot(9, inv.getCurrentSlot());
        }
    }
}

void Player::updateCraftableItem(){
    inv.UpdateCraftableTileID();
}

void Player::setInvIsCrafting(bool value){
    inv.setIsCrafting(value);
}

void Player::toggleInvenCrafting(){
    inv.toggleCrafting();
}

void Player::Draw(bool isDebuggin){
    animate();
    DrawRectangleV({(body.x-(float)display_name.size()+20)-60, body.y-40}, {(float)display_name.size()*30+40,45}, {50,50,50,100});
    DrawText(display_name.c_str(), (body.x+35)-60, body.y-40, 50, BLACK);


    if(isDebuggin) {
        DrawRectangleRec(selectArea, {150,150,150,100});
        DrawRectangleRec(newPos, {255, 255, 255, 255/2});
    }
}

bool Player::invHas(int id){
    return inv.has(id);
}

bool Player::invHas(RecipeItem criteria){
    return inv.has(criteria);
}

void Player::addItemInv(InventoryItem item){
    inv.addItem(item);
}

int Player::getCurrentInvSlot(){
    return inv.getCurrentSlot();
}
int Player::getCurrentInvCraftAbleID(){
    return inv.getCurrent_craftableTileId();
}

int Player::getCurrentInvIDSlot(){
    return inv.getItemFromCurrentSot().tileID;
}

void Player::decreaseItemInv(int slot){
    inv.decreaseItemCount(slot);
}

void Player::InventoryDraw(Camera2D& camera){
    inv.Draw(camera);
}

Player::Player(Rectangle body, int speed, const char* texture_path, Rectangle selectArea, Rectangle collisionBody,
        /*inv*/
        int slots, Vector2 inventory_pos, 
        std::string inventory_texture, std::string inventory_selecting_texture, std::string extra_inv_texture,
        std::string crafting_menu_texture,
        /*customization*/
        std::string display_name)
:body{body}, selectArea{selectArea}, collisionArea{collisionBody}, default_speed{speed}, display_name{display_name}, speed{speed}{
    texture=LoadTexture(texture_path);

    inv = Inventory(inventory_pos, slots, 
            LoadTexture(inventory_texture.c_str()), LoadTexture(inventory_selecting_texture.c_str()), 
            LoadTexture(extra_inv_texture.c_str()), LoadTexture(crafting_menu_texture.c_str()));

    current_animation=1;
    //0=up || 1=down || 2=left || 3=right//
    direction=DIRECTION_DOWN;
    // Define separate arrays for rectangles
    Rectangle idleDownRect[] = {{0, 0, 18, 35},
        {18, 0, 18, 35},
        {36, 0, 18, 35},
        {54, 0, 18, 35},
        {72, 0, 18, 35}};

    Rectangle walkDownRect[] = {{0, 35, 18, 35},
        {18, 35, 18, 35},
        {36, 35, 18, 35},
        {54, 35, 18, 35},
        {72, 35, 18, 35},
        {90, 35, 18, 35}};

    Rectangle idleLeftRect[] = {{0, 70, 18, 35},
        {18, 70, 18, 35},
        {36, 70, 18, 35},
        {54, 70, 18, 35},
        {72, 70, 18, 35}};

    Rectangle walkLeftRect[] = {{0, 105, 18, 35},
        {18, 105, 18, 35},
        {36, 105, 18, 35},
        {54, 105, 18, 35},
        {72, 105, 18, 35},
        {90, 105, 18, 35}};

    Rectangle idleUpRect[] = {{0, 140, 18, 35},
        {18, 140, 18, 35},
        {36, 140, 18, 35},
        {54, 140, 18, 35},
        {72, 140, 18, 35}};

    Rectangle walkUpRect[] = {{0, 175, 18, 35},
        {18, 175, 18, 35},
        {36, 175, 18, 35},
        {54, 175, 18, 35},
        {72, 175, 18, 35},
        {90, 175, 18, 35}};

    Rectangle idleRightRect[] = {{0, 210, 18, 35},
        {18, 210, 18, 35},
        {36, 210, 18, 35},
        {54, 210, 18, 35},
        {72, 210, 18, 35}};

    Rectangle walkRightRect[] = {{0, 245, 18, 35},
        {18, 245, 18, 35},
        {36, 245, 18, 35},
        {54, 245, 18, 35},
        {72, 245, 18, 35},
        {90, 245, 18, 35}};

    // Use the arrays as arguments in CreateSpriteAnimation
    animations = {
        CreateSpriteAnimation(texture, 7, idleDownRect, 5), //0
        CreateSpriteAnimation(texture, 11, walkDownRect, 6), //1
        CreateSpriteAnimation(texture, 7, idleLeftRect, 5), //2
        CreateSpriteAnimation(texture, 11, walkLeftRect, 6), //3
        CreateSpriteAnimation(texture, 7, idleUpRect, 5), //4
        CreateSpriteAnimation(texture, 11, walkUpRect, 6), //5
        CreateSpriteAnimation(texture, 7, idleRightRect, 5), //6
        CreateSpriteAnimation(texture, 11, walkRightRect, 6), //7
    };

    newPos = body;
    newPos.width /= 2;
    newPos.height /= 2;
    newPos.height -= 40;
}

Player::Player(){
}
