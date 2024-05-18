#include "global_func.h"
#include "raylib.h"
#include "screens.h"

#include<iostream>
#include<fstream>
#include<filesystem>
#include<thread>
#include<chrono>
#include<sstream>
#include<string>
#include<cctype>

#define MAX_CHAT_TEXTS 100

namespace fs=std::filesystem;

bool isEnglishAlphabet(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

std::string getSecondWord(const std::string& input) {
    std::istringstream iss(input);
    std::string firstWord, secondWord;

    // Read the first word
    iss >> firstWord;

    // Read the second word
    iss >> secondWord;

    return secondWord;
}

std::string getFirstWord(const std::string& input) {
    std::istringstream iss(input);
    std::string firstWord, secondWord;

    // Read the first word
    iss >> firstWord;

    // Read the second word
    iss >> secondWord;

    return firstWord;
}

std::string readFile(const std::string& filename){
    std::ifstream file(filename);
    std::string fileContents, line;

    if (file.is_open()) {
        while (getline(file, line)) {
            fileContents += line + "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file " << filename << std::endl;
    }

    return fileContents;
}

std::vector<std::string> getAllFileNamesInDirectory(const std::string& path){
    std::vector<std::string> fileNames;

    // Check if the given path exists and is a directory
    if (fs::exists(path) && fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_regular_file(entry.path())) {
                fileNames.push_back(entry.path().filename().string());
            }
        }
    } else {
        std::cerr << "Invalid directory path or directory doesn't exist." << std::endl;
    }

    return fileNames;
}

void clamp(int &val, int low, int high){
    if(val>high) val=high;
    if(val<low) val=low;
}

void runFunctionForSecond(int seconds, std::function<void()> func){
    // Start a new thread to run the specified function
    std::thread t(func);

    // Sleep for the specified duration
    std::this_thread::sleep_for(std::chrono::seconds(seconds));

    // Join the thread to wait for it to finish
    t.join();
}

void printText(std::string label, std::string information, Vector2 pos, int fontSize){
    DrawText(label.c_str(), pos.x, pos.y, fontSize, BLACK);
    DrawText(information.c_str(), pos.x+(label.size()*fontSize), pos.y, fontSize, BLACK);
};

bool IsAlphanumeric(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ' || c == ',' || c == '.' || c == '!';
}

bool isStringInVector(
        const std::vector<std::string>& vec,
        const std::string& str) {

    // Use std::find() algorithm to search
    // for the string in the vector
    auto it = std::find(
            vec.begin(),
            vec.end(),
            str);
    return it != vec.end();
}


int GetCharacterDistance(char* str, int char1Index, int char2Index) {
    // Get default font
    Font font = GetFontDefault();

    // Calculate the position of the characters
    int char1Width = MeasureText(TextSubtext(str, char1Index, 1), font.baseSize);
    int char2Width = MeasureText(TextSubtext(str, char2Index, 1), font.baseSize);

    int distance = std::abs(char2Width - char1Width);

    return distance;
}

void typeInChat(std::string text){
    if(texts.size()>MAX_CHAT_TEXTS)
        texts.erase(texts.begin());
    addChatText(texts, text);
}

void typeInChat(std::string text, Color color){
    if(texts.size()>MAX_CHAT_TEXTS)
        texts.erase(texts.begin());
    addChatText(texts, text, color);
}

bool AABBx(Rectangle rec1, Rectangle rec2){
    bool collision = false;

    if((rec1.x < (rec2.x + rec2.width) && (rec1.x + rec1.width) > rec2.x)) collision = true;

    return collision;
}

bool AABBy(Rectangle rec1, Rectangle rec2){
    bool collision = false;

    if((rec1.y < (rec2.y + rec2.height) && (rec1.y + rec1.height) > rec2.y)) collision = true;

    return collision;
}

bool isWalkableX(Rectangle newPos, Rectangle body) {
    // Store current level tiles to avoid multiple calls to getCurrentLevel()
    const auto& tiles = getCurrentLevel().tiles;

    // Array of rectangle positions to check
    std::array<std::pair<Rectangle, Color>, 4> checkRects = {{
        {{newPos.x, body.y, newPos.width / 2, newPos.height / 2}, RED},
            {{newPos.x, body.y + newPos.height / 2, newPos.width / 2, newPos.height / 2}, YELLOW},
            {{newPos.x + newPos.width, body.y, newPos.width / 2, newPos.height / 2}, BLUE},
            {{newPos.x + newPos.width, body.y + newPos.height / 2, newPos.width / 2, newPos.height / 2}, BROWN}
    }};

    // Lambda to check collision for a single rectangle
    auto hasCollision = [&tiles](const Rectangle& rect) {
        auto it = std::find_if(tiles.begin(), tiles.end(), [&rect](const auto& tile) {
                return CheckCollisionRecs(rect, tile->getBody()) && tile->getZ() == 0;
                });
        return it != tiles.end() && (*it)->HasCollision();
    };

    // Check all rectangles
    for (const auto& [rect, color] : checkRects) {
        //DrawRectangleRec(rect, color);
        if (hasCollision(rect)) {
            return false;
        }
    }

    return true;
}

bool isWalkableY(Rectangle newPos, Rectangle body){
    // Store current level tiles to avoid multiple calls to getCurrentLevel()
    const auto& tiles = getCurrentLevel().tiles;

    // Array of rectangle positions to check
    std::array<std::pair<Rectangle, Color>, 4> checkRects = {{
        {{body.x, newPos.y, newPos.width / 2, newPos.height / 2}, RED},
        {{body.x, newPos.y + newPos.height, newPos.width / 2, newPos.height / 2}, YELLOW},

        {{body.x + (newPos.width/2), newPos.y, newPos.width / 2, newPos.height / 2}, BLUE},
        {{body.x + (newPos.width/2), newPos.y + newPos.height, newPos.width / 2, newPos.height / 2}, BROWN}
    }};

    // Lambda to check collision for a single rectangle
    auto hasCollision = [&tiles](const Rectangle& rect) {
        auto it = std::find_if(tiles.begin(), tiles.end(), [&rect](const auto& tile) {
                return CheckCollisionRecs(rect, tile->getBody()) && tile->getZ() == 0;
                });
        return it != tiles.end() && (*it)->HasCollision();
    };

    // Check all rectangles
    for (const auto& [rect, color] : checkRects) {
        //DrawRectangleRec(rect, color);
        if (hasCollision(rect)) {
            return false;
        }
    }

    return true;
}
