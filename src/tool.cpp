#include "tool.h"

#include <fstream>

#include <json/json.h>
#include <json/reader.h>

Tool::Tool(){
}

Tool::Tool(int id):id{id}{
    Json::Reader jsonreader;

    std::ifstream file("res/tools.json");
    Json::Value jsonvalue;
    jsonreader.parse(file, jsonvalue);

    if(!jsonvalue.isArray()) return;

    if(id <= jsonvalue.size()){
        name = jsonvalue[id]["name"].asString();
        texture = LoadTexture(jsonvalue[id]["texture"].asString().c_str());
    }
}

InventoryItem Tool::asItem(int count){
    std::vector<RecipeItem> resep;

    Json::Reader jsonreader;

    std::ifstream file("res/tools.json");
    Json::Value jsonvalue;
    jsonreader.parse(file, jsonvalue);

    if(jsonvalue[id].isMember("recipe")){
        const Json::Value recipeArray = jsonvalue[id]["recipe"];

        // Iterate through the JSON array and populate the vector
        for(const auto& elem : recipeArray) {
            RecipeItem arr;
            arr.id = elem[0].asInt();
            arr.count = elem[1].asInt();

            resep.push_back(arr);
        }
    }

    return{
        .tileID=id,
            .item_type="Tool",
            .item_name=name,
            .item_invslot=0,
            .item_count=count,
            .recipe = resep,
            .iconTexture=texture
    };
}
