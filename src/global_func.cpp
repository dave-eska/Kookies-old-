#include"global_func.h"

#include<iostream>
#include<fstream>
#include<filesystem>
#include<thread>
#include<chrono>
#include<sstream>
#include<string>
#include<cctype>

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
