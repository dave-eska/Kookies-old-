#include <fstream>

#include <iostream>
#include <raylib.h>
#include <raygui.h>

#include <json/json.h>
#include <json/reader.h>

#include"screens.h"    // NOTE: Declares global (extern) variables and screens functions

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = TITLE;
Font font = { 0 };
//Music music = { 0 };
//Sound fxCoin = { 0 };

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 1600;
static const int screenHeight = 900;

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static GameScreen transToScreen = UNKNOWN;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Change to screen, no transition effect
static void ChangeToFullScreen(void);          // Change window into fullscreen mode

static void TransitionToScreen(int screen); // Request transition to next screen
static void UpdateTransition(void);         // Update transition effect
static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void){
    // Initialization

    Json::Reader jsonreader;

    std::ifstream file("config.json");
    jsonreader.parse(file, config);

    //---------------------------------------------------------
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "Kookies");

    InitAudioDevice();      // Initialize audio device
                            //
    // Load the icon image
    Image icon = LoadImage("res/icon.png");

    // Set the window icon
    SetWindowIcon(icon);

    // Load global data (assets that must be available in all screens, i.e. font)
    std::cout<<config["FontPath"].asString().c_str()<<std::endl;
    font = LoadFont(config["FontPath"].asString().c_str());
    GuiSetFont(font);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    isDebugging = false;
    isTyping = false;

    // Setup and init first screen
    currentScreen = TITLE;
    InitTitleScreen();

    SetExitKey(KEY_NULL);
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    if(config["fullscreen"].asBool())
        ToggleFullscreen();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload current screen data before closing
    switch (currentScreen)
    {
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        default: break;
    }

    // Unload global data loaded
    UnloadFont(font);

    CloseAudioDevice();     // Close audio context

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
// Change to next screen, no transition
static void ChangeToScreen(GameScreen screen)
{
    // Unload current screen
    switch (currentScreen)
    {
        case TITLE: UnloadTitleScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case LEVELEDITOR: UnloadLevelEditorScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        default: break;
    }

    // Init next screen
    switch (screen)
    {
        case TITLE: InitTitleScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case LEVELEDITOR: InitLevelEditorScreen(); break;
        case OPTIONS: InitOptionsScreen(); break;
        default: break;
    }

    currentScreen = screen;
}

static void ChangeToFullScreen(void){
    ToggleFullscreen();
}

// Request transition to next screen
static void TransitionToScreen(GameScreen screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
        // For that reason we compare against 1.01f, to avoid last frame loading stop
        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Unload current screen
            switch (transFromScreen)
            {
                case TITLE: UnloadTitleScreen(); break;
                case OPTIONS: UnloadOptionsScreen(); break;
                case GAMEPLAY: UnloadGameplayScreen(); break;
                case LEVELEDITOR: UnloadLevelEditorScreen(); break;
                default: break;
            }

            // Load next screen
            switch (transToScreen)
            {
                case TITLE: InitTitleScreen(); break;
                case GAMEPLAY: InitGameplayScreen(); break;
                case LEVELEDITOR: InitLevelEditorScreen(); break;
                case OPTIONS: InitOptionsScreen(); break;
                default: break;
            }

            currentScreen = transToScreen;

            // Activate fade out effect to next loaded screen
            transFadeOut = true;
        }
    }
    else  // Transition fade out logic
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = UNKNOWN;
        }
    }
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------

    if (!onTransition)
    {
        switch(currentScreen)
        {
            case TITLE:
            {
                UpdateTitleScreen();

                if (FinishTitleScreen() == 1) {ResetTitleFinishScreen(); TransitionToScreen(LEVELEDITOR);}
                else if (FinishTitleScreen() == 2) {ResetTitleFinishScreen(); TransitionToScreen(GAMEPLAY);}
                else if(FinishTitleScreen() == 3) {ResetTitleFinishScreen(); TransitionToScreen(OPTIONS);}

            } break;
            case OPTIONS:
            {
                UpdateOptionsScreen();

                if (FinishOptionsScreen()) {ResetOptionsFinishScreen(); TransitionToScreen(TITLE);}

            } break;
            case GAMEPLAY:
            {
                UpdateGameplayScreen();

                if (FinishGameplayScreen() == 1) {ResetGameplayFinishScreen(); TransitionToScreen(TITLE);}
                else if (FinishGameplayScreen() == 2) TransitionToScreen(OPTIONS);

            } break;
            case LEVELEDITOR:
            {
                UpdateLevelEditorScreen();

                if (FinishLevelEditorScreen() == 1) {ResetLevelEditorScreen(); TransitionToScreen(TITLE);}
                else if (FinishLevelEditorScreen() == 2) TransitionToScreen(OPTIONS);
            }
            default: break;
        }
    }
    else UpdateTransition();    // Update transition (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(DARKGRAY);

        switch(currentScreen)
        {
            case TITLE: DrawTitleScreen(); break;
            case OPTIONS: DrawOptionsScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(); break;
            case LEVELEDITOR: DrawLevelEditorScreen(); break;
            default: break;
        }

        // Draw full screen rectangle in front of everything
        if (onTransition) DrawTransition();

        DrawFPS(10, GetScreenHeight()-30);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
