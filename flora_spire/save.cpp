#include <filesystem>
#include <fstream>
#include <iostream>
#include "save.h"

using namespace std;
namespace fs = std::filesystem;

Save::Save() {

	_folder = "res/save";

	_save = _folder;
	_save.append("\\Save.txt");
}

bool Save::CheckFolder() {

	if (fs::exists(_folder) && fs::is_directory(_folder)) {
		return true;
	}
	else {
		if (fs::create_directory(_folder)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool Save::CheckSave() {

	if (fs::exists(_save)) {
		test = "file exists";
		return true;
	}
	else {
	
		ofstream MyFile(_save);
		MyFile << "0";
		MyFile.close();

		if (fs::exists(_save)) {
			return true;
		}
		else {
			return false;
		}
	}
}

void Save::UpdateSave(int level) {

	std::string saveData = std::to_string(level);
	ofstream MySave(_save, ofstream::trunc);
	MySave << saveData;
	MySave.close();
}

int Save::LoadProgress() {

	int level;

	ifstream MySave(_save);
	std::string String;

	getline(MySave, String);
	level = stoi(String);

	int data = level;

	return data;
}