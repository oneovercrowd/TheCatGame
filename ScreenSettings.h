#pragma once
#include "Helpers.h"
#include <fstream>
using namespace std;

class ScreenSettings
{
public:
	Helpers::SCR_SZ size;
	int w, h;
	float aspect;
	bool full_screen;
	void SetWH() {
		switch (size)
		{
		case Helpers::N800X600:
			w = 800;
			h = 600;
			break;
		case Helpers::N1024X768:
			w = 1024;
			h = 768;
			break;
		case Helpers::N1280X1024:
			w = 1280;
			h = 1024;
			break;
		case Helpers::W1280X720:
			w = 1280;
			h = 720;
			break;
		case Helpers::W1366X768:
			w = 1366;
			h = 768;
			break;
		case Helpers::W1920X1080:
			w = 1920;
			h = 1080;
			break;
		default:
			break;
		}
		aspect = float(w) / float(h);
	}
	ScreenSettings(string filename) {
		ifstream fin(filename, ios::binary);
		if (fin) {
			fin.read((char*)&size, sizeof(size));
			fin.read((char*)&full_screen, sizeof(full_screen));
		}
		else {
			size = Helpers::N800X600;
			full_screen = false;
			printf("Video settings loading failed! Set to defaults!\n");
			SaveFile(filename);
		}
		SetWH();
	}
	void SaveFile(string filename) {
		ofstream fout(filename, ios::binary);
		if (fout) {
			fout.write((char*)&size, sizeof(size));
			fout.write((char*)&full_screen, sizeof(full_screen));
		}
		else {
			printf("Video settings writing failed!\n");
		}
	}
	~ScreenSettings() {

	}
};
