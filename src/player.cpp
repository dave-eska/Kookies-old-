#include"player.h"

#include<raylib.h>

#include<string>

#include"global_func.h"
#include"inventory.h"

//Private functions
void Player::move(float dt){
    float inputX=IsKeyDown(KEY_D)-IsKeyDown(KEY_A);
    float inputY=IsKeyDown(KEY_S)-IsKeyDown(KEY_W);

    body.x+=(inputX*speed)*dt;
    body.y+=(inputY*speed)*dt;

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

void Player::interactItem(std::vector<Tile>& tileVec, Camera2D& camera, Sound pickupsound){
    for(int i=0;i<tileVec.size();i++){
        auto tile=tileVec[i];
        //Taking item
        if(CheckCollisionRecs(selectArea, tile.getBody()) && tile.getType()=="Item"){
            isToucingItem=true;
            if(CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tile.getBody()) && 
                    IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                inv.addItem(tile.asItem(1));
                
                tileVec.erase(tileVec.begin()+i);
                PlaySound(pickupsound);
            }
        }
        //Placing Item
        if(CheckCollisionRecs(selectArea, tile.getBody()) && tile.getName()=="placearea" 
                && inv.getItemFromCurrentSot().item_count > 0){

            if(CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tile.getBody()) && 
                    IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){

                tileVec.push_back(Tile(inv.getItemFromCurrentSot().tileID, {tile.getX(), tile.getY()}, tile.getZ()+1));
                inv.decreaseItemCount(inv.getCurrentSlot());

                PlaySound(pickupsound);
            }
        }
        //Animating Item
        if(CheckCollisionRecs(selectArea, tile.getBody()) && tile.HasAnimFrame()){
            if(CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), tile.getBody()) &&
                    IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
                tile.runAnimation();
                typeInChat(tile.getName() + " Tile Is_Running_Animation: " + std::to_string(tile.getIsRunningAnimation()));
            }
        }
    }
    if(inv.getItemFromCurrentSot().item_count==0) inv.deleteItem(inv.getCurrentSlot());
}

//Public Functions
void Player::UpdateVariables(){
    isToucingItem=false;
}

void Player::UpdateInventory(){
    inv.changeCurrentSlot();
    inv.toggleDrawUI();
}

void Player::Draw(bool isDebuggin){
    animate();
    DrawRectangleV({(body.x-(float)display_name.size()+20)-60, body.y-40}, {(float)display_name.size()*30+40,45}, {50,50,50,100});
    DrawText(display_name.c_str(), (body.x+35)-60, body.y-40, 50, BLACK);
    if(isDebuggin) {
        DrawRectangleRec(body, {0,200,0,150});
        //DrawRectangleRec(selectArea, {0,0,0,100});
    }
}

void Player::InventoryDraw(Camera2D& camera){
    inv.Draw(camera);;
}

Player::Player(Rectangle body, int speed, const char* texture_path, Rectangle selectArea, Rectangle collisionBody,
                /*inv*/
                int slots, Vector2 inventory_pos, 
                std::string inventory_texture, std::string inventory_selecting_texture, std::string extra_inv_texture,
                /*box2d stuff*/
                b2World &world,
                /*customization*/
                std::string display_name)
    :body{body},selectArea{selectArea},default_speed{speed},display_name{display_name},speed{speed}{
        texture=LoadTexture(texture_path);

        inv = Inventory(inventory_pos, slots, 
                LoadTexture(inventory_texture.c_str()), LoadTexture(inventory_selecting_texture.c_str()), 
                LoadTexture(extra_inv_texture.c_str()));

        b2.bodyDef.type = b2_dynamicBody;
        b2.bodyDef.position.Set(body.x, body.y);

        b2.body = world.CreateBody(&b2.bodyDef);

        b2.shape.SetAsBox(collisionBody.width/2, collisionBody.height/2);

        b2.fixtureDef.shape = &b2.shape;
        b2.fixtureDef.density = 1.0f;
        b2.fixtureDef.friction = 0.3;

        b2.body->CreateFixture(&b2.fixtureDef);

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

}

Player::Player(){
}
