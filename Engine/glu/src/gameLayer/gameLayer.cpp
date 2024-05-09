#include "Engine/glu/include/gameLayer/gameLayer.h"
#include "Engine/glu/gl2d.h"
#include "Engine/glu/include/platform/platformInput.h"
#include "imgui.h"
#include "raudio.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "Engine/glu/glui.h"

gl2d::Renderer2D renderer;

gl2d::Font font;
gl2d::Texture texture;
gl2d::Texture terrariaTexture;
gl2d::Texture logoTexture;
gl2d::Texture tick;
glui::RendererUi ui;

struct GameData{}gameData;

Music music;
float vol = 100.0f;
int* screenPnt;
bool initGame(int& chng) {

	InitAudioDevice();
	renderer.create();
	//font.createFromFile("Resources/resources/roboto_black.ttf");
	font.createFromFile("Resources/resources/font/ANDYB.TTF");
	texture.loadFromFile("Resources/resources/mc/beacon_button_default.png", true);
	terrariaTexture.loadFromFile("Resources/resources/terraria.png");
	//logoTexture.loadFromFile("Resources/resources/logo.png", true);
	tick.loadFromFile("Resources/resources/tick.png", true);
	music = LoadMusicStream("Resources/resources/tf.ogg");
	SetMusicVolume(music, vol);
	PlayMusicStream(music);
	screenPnt = &chng;
	if(!platform::readEntireFile("Resources/resources/gameData.data", &gameData, sizeof(GameData))) {
		gameData = GameData();
	}
	return true;
}

float masterVolume = 1;
float musicVolume = 1;
bool vSync = true;
bool shadows = true;
int prevVolume = 0;
void render() {

	ui.Begin(100);

		ui.SetAlignModeFixedSizeWidgets({300, 200});
		ui.Button("Title", Colors_Black, texture);
		ui.newColum(0);
		if (ui.Button("Play", Colors_Green, texture)) {
			UnloadMusicStream(music);
			*screenPnt = 1;
		}

		ui.BeginMenu("settings", Colors_Transparent, texture);
			ui.BeginMenu("Sound settings", Colors_Transparent, texture);
				ui.sliderFloat("Master volume", &masterVolume, 0, 1, Colors_White, texture, Colors_White, texture);
				ui.sliderFloat("Music volume", &musicVolume, 0, 1, Colors_White, texture, Colors_White, texture);
			ui.EndMenu();

			ui.BeginMenu("video settings", Colors_Transparent, texture);
				ui.Toggle("vSync", Colors_Gray, &vSync, texture, tick);
				ui.Toggle("shadows", Colors_Gray, &shadows, texture, tick);
			ui.EndMenu();
		ui.EndMenu();

		static glm::vec3 color = {1,1,1};
		ui.colorPicker("color", &color[0], texture, texture, Colors_White, Colors_Gray);

		static bool toggle = 0;
		ui.ToggleButton("Music:", Colors_White, &toggle, texture);
		if(toggle) PauseMusicStream(music);
		else ResumeMusicStream(music);


		static size_t index = 0;
		static glm::vec4 colors[] = {
			Colors_Green,
			Colors_Yellow,
			Colors_Red,
		};

	static float val2 = vol;
	ui.sliderFloat("Volume: ", &val2, 0.0f, 100.0f, Colors_White, texture, Colors_Gray, texture);
	if((float)val2 != prevVolume) {
		SetMusicVolume(music, val2 / 100.0f);
		prevVolume = val2;
	}
	if (ui.Button("Exit", Colors_Gray, texture)) { exit(EXIT_SUCCESS); }
	ui.newColum(1);
	if (ui.Button("Next", Colors_Black, texture)) { }
	ui.End();
}


bool gameLogic(float deltaTime) {

#pragma region init stuff

	int w = 0; int h = 0;
	w = platform::getWindowSizeX() * 2;
	h = platform::getWindowSizeY() * 2;
	renderer.updateWindowMetrics(w, h);
	renderer.clearScreen(gl2d::Color4f(0.2,0.2,0.3, 1));
#pragma endregion
	render();

#pragma region set finishing stuff
	UpdateMusicStream(music);
	
	ui.renderFrame(renderer, font, platform::getRelMousePosition() * 2,
		platform::isLMousePressed(), platform::isLMouseHeld(), platform::isLMouseReleased(),
		platform::isKeyReleased(platform::Button::Escape), platform::getTypedInput(), deltaTime);
	renderer.flush();

	return true;
#pragma endregion

}

void closeGame() {
	platform::writeEntireFile("Resources/resources/gameData.data", &gameData, sizeof(GameData));
}
