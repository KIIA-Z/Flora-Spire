#pragma once
#include <SFML/Graphics.hpp>

class Save {
protected:

    std::string _folder;
    std::string _save;

public:

    std::string test;

    //Constructor
    Save();

    //Methods
    bool CheckFolder();
    bool CheckSave();
    void UpdateSave(int level);
    int LoadProgress();
};