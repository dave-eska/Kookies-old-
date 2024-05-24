#include "screens.h"

#include <fstream>

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <json/json.h>

int finish_screen = 0;

float cameraZoom;

char plr_display_name[10] = "Daveeska";
bool isEditingPDN;

static void SaveConfigToJson(){
    Json::Value root;
    root["camera_zoom"] = cameraZoom;
    root["DisplayName"] = plr_display_name;

    std::ofstream file_id("config.json");
    file_id << root;
    file_id.close();

    //Reloading the config
    Json::Reader jsonreader;
    std::ifstream file("config.json");
    jsonreader.parse(file, config);
}

void InitOptionsScreen(){
    cameraZoom = 0.6f;
}

void UpdateOptionsScreen(){
    if(IsKeyPressed(KEY_ESCAPE)) finish_screen = 1;
}

void DrawOptionsScreen(){
    //! Camera Zoom
    // Draw grey box
    Rectangle box = { 10, 10, 285, 30 };
    DrawRectangleRec(box, GRAY);

    // Draw "Camera zoom" text
    DrawText("Camera zoom = ", box.x + 10, box.y + 5, 20, BLACK);
    DrawText(std::to_string(cameraZoom).c_str(), box.x + 170, box.y + 5, 20, BLACK);

    // Draw slider
    GuiSlider({ box.x + 20, box.y + 40, 260, 20 }, "0.1f", "3.0f", &cameraZoom, 0.1f, 3.0f);

    // Draw Save button
    if (GuiButton((Rectangle){ (float)GetScreenWidth() - 100, (float)GetScreenHeight() - 50, 80, 30 }, "Save"))
    {
        SaveConfigToJson();
    }

    //! Player Display Name
    box = { 10, 100, 320, 30 };
    DrawRectangleRec(box, GRAY);

    // Draw "Camera zoom" text
    DrawText("Player Name = ", box.x + 10, box.y + 5, 20, BLACK);

    // Draw slider
    if(GuiTextBox({ box.x + 170, box.y, 150, 30 }, plr_display_name, 10, isEditingPDN)){
        isEditingPDN = !isEditingPDN;
    }
}

void UnloadOptionsScreen(){
}

void ResetOptionsFinishScreen(){
    finish_screen = 0;
}

int FinishOptionsScreen(){
    return finish_screen;
}
