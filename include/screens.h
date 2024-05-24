#pragma once

//--------------------------------------------------------------------------------//
// Global Include Files (used by multiple source files)
//--------------------------------------------------------------------------------//
#include <raylib.h>

#include <json/json.h>

#include "chat.h"

//--------------------------------------------------------------------------------//
// Types and Structures Definition
//--------------------------------------------------------------------------------//
typedef enum GameScreen { UNKNOWN, TITLE, OPTIONS, GAMEPLAY, LEVELEDITOR } GameScreen;

//--------------------------------------------------------------------------------//
// Global Variables Declaration (shared by several modules)
//--------------------------------------------------------------------------------//
extern GameScreen currentScreen;
extern Font font;

inline Json::Value config;

inline bool isDebugging;
inline bool isTyping;

inline std::vector<ChatText> texts;

//--------------------------------------------------------------------------------//
// Title Screen Functions Declaration
//--------------------------------------------------------------------------------//
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
void ResetTitleFinishScreen(void);
int FinishTitleScreen(void);

//--------------------------------------------------------------------------------//
// Options Screen Functions Declaration
//--------------------------------------------------------------------------------//
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
void ResetOptionsFinishScreen(void);
int FinishOptionsScreen(void);

//--------------------------------------------------------------------------------//
// Level Editor Screen Functions Declaration
//--------------------------------------------------------------------------------//
void InitLevelEditorScreen(void);
void UpdateLevelEditorScreen(void);
void DrawLevelEditorScreen(void);
void UnloadLevelEditorScreen(void);
void ResetLevelEditorScreen(void);
int FinishLevelEditorScreen(void);

//--------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------//
// Gameplay Screen Functions Declaration
//--------------------------------------------------------------------------------//
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
void ResetGameplayFinishScreen(void);
int FinishGameplayScreen(void);
//--------------------------------------------------------------------------------//
