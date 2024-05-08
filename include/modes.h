#pragma once

#include <map>
#include <string>

enum LevelEditorMode{
    Mode_Pencil,
    Mode_Fill,
    Mode_Eraser,
    Mode_ColorPicker,
    Mode_SelectionTools,
};

static std::map<LevelEditorMode, std::string> LevelEditorToStringMap = {
    { Mode_Pencil, "Mode_Pencil" },
    { Mode_Fill, "Mode_Fill" },
    { Mode_Eraser, "Mode_Eraser" },
    { Mode_ColorPicker, "Mode_ColorPicker" },
    { Mode_SelectionTools, "Mode_SelectionTools" },
};

std::string LevelEditorModeToString(LevelEditorMode mode);
