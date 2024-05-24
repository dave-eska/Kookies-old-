#include "screens.h"

#include <fstream>

#include <iostream>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <json/json.h>

int finish_screen = 0;

float cameraZoom;

static void SaveZoomToJson()
{
    Json::Value root;
    root["camera_zoom"] = cameraZoom;
    std::cout<<root["camera_zoom"].asFloat()<<std::endl;

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
        SaveZoomToJson();
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
