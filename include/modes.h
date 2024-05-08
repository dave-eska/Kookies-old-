#pragma once

#include <map>
#include <string>

enum LevelEditorMode{
    Mode_One_Select,
    Mode_Pencil,
    Mode_Fill,
    Mode_Eraser,
    Mode_ColorPicker,
    Mode_SelectionTools,

    Mode_Max,
};

static std::map<LevelEditorMode, std::string> LevelEditorToStringMap = {
    { Mode_One_Select, "One Select" },
    { Mode_Pencil, "Pencil" },
    { Mode_Fill, "Fill" },
    { Mode_Eraser, "Eraser" },
    { Mode_ColorPicker, "Color Picker" },
    { Mode_SelectionTools, "Selection Tools" }
};

std::string LevelEditorModeToString(LevelEditorMode mode);
