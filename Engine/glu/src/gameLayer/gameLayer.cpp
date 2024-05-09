#include "Engine/glu/include/gameLayer/gameLayer.h"
#include "Engine/glu/gl2d.h"
#include "Engine/glu/include/platform/platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "Engine/glu/glui.h"

gl2d::Renderer2D renderer;

gl2d::Font font;
gl2d::Texture texture;
gl2d::Texture terrariaTexture;
gl2d::Texture logoTexture;
gl2d::Texture tick;
glui::RendererUi ui;

struct GameData
{

}gameData;


bool initGame()
{
	renderer.create();
	//font.createFromFile("Resources/resources/roboto_black.ttf");
	font.createFromFile("Resources/resources/font/ANDYB.TTF");
	texture.loadFromFile("Resources/resources/mc/beacon_button_default.png", true);
	terrariaTexture.loadFromFile("Resources/resources/terraria.png");
	//logoTexture.loadFromFile("Resources/resources/logo.png", true);
	tick.loadFromFile("Resources/resources/tick.png", true);

	if(!platform::readEntireFile("Resources/resources/gameData.data", &gameData, sizeof(GameData)))
	{
		gameData = GameData();
	}

	return true;
}

void render1()
{
	ui.Begin(6996);
		//ui.Text("Terraria", Colors_Gray);
		//ui.Texture(0, terrariaTexture);
		//ui.Texture(1, terrariaTexture);
		//ui.Texture(2, terrariaTexture);

		if (ui.ButtonWithTexture(1, texture))
		{
			//play
		}

		static float value = 0;
		static int valueInt = 0;
		ui.sliderFloat("Slider example", &value, -2, 5, Colors_White, texture, {1,1,1,1}, texture);
		ui.sliderInt("Slider example int", &valueInt, -2, 5, {1,1,1,1}, texture, {1,1,1,1}, texture);

		glm::vec4 customTransform = {};
		bool clicked = 0;
		bool hovered = 0;
		if (ui.CustomWidget(23, &customTransform, &hovered, &clicked))
		{
			if (clicked)
			{
				renderer.renderRectangle(customTransform, Colors_Blue);
			}
			else if (hovered)
			{
				renderer.renderRectangle(customTransform, Colors_Green);
			}
			else
			{
				renderer.renderRectangle(customTransform, Colors_Red);
			}
		}

		ui.BeginMenu("color test", Colors_Transparent, texture);
		{
			static glm::vec3 color = {};
			static glm::vec3 color2 = {};
			ui.colorPicker("color example", &color[0], texture, texture);
			ui.colorPicker("color example2", &color2[0], texture, texture);

			ui.newColum(0);

			ui.Texture(3, texture, {color, 1});
			ui.Texture(4, texture, {color2, 1});
		
		}
		ui.EndMenu();


		ui.BeginMenu("settings", Colors_Transparent, texture);
			ui.BeginMenu("volume settings", Colors_Transparent, texture);
				static bool sound = true;
				static bool music = true;
				ui.Toggle("sound", Colors_Gray, &sound, texture, tick);
				ui.Toggle("music", Colors_Gray, &music, texture, tick);
			ui.EndMenu();

			ui.BeginMenu("video settings", Colors_Transparent, texture);
				static bool vSync = true;
				static bool shadows = true;
				ui.Toggle("vSync", Colors_Gray, &vSync, texture, tick);
				ui.Toggle("shadows", Colors_Gray, &shadows, texture, tick);
			ui.EndMenu();
		ui.EndMenu();

		ui.BeginMenu("create new world", Colors_Green, texture);
			ui.Text("Enter world name", Colors_Gray);
			static char text[15];
			ui.InputText("input", text, sizeof(text));
			ui.Button("create", Colors_Transparent, texture);
		ui.EndMenu();
		ui.Button("Exit", Colors_Transparent, {});

	ui.End();
}

float masterVolume = 1;
float musicVolume = 1;
bool vSync = true;
bool shadows = true;

void render2()
{
	ui.Begin(100);

		ui.SetAlignModeFixedSizeWidgets({0, 100});
		ui.Button("Title", Colors_Black, texture);
		ui.newColum(0);
		if (ui.Button("Play", Colors_Green, texture)) {}

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
		ui.ToggleButton("Toggle test", Colors_White, &toggle, texture);

		static char text[20] = {};
		ui.InputText("Name: ", text, sizeof(text), Colors_White, texture);


		static size_t index = 0;
		static glm::vec4 colors[] = {
			Colors_Green,
			Colors_Yellow,
			Colors_Red,
		};

	static int val2 = 0;
	ui.sliderInt("test##2", &val2, 0, 100, Colors_White, texture, Colors_Gray, texture);
	if (ui.Button("Exit...", Colors_Gray, texture)) { }
	ui.newColum(1);
	if (ui.Button("Next", Colors_Black, texture)) { }
	ui.End();
}

bool change = 1;
bool gameLogic(float deltaTime) {

#pragma region init stuff

	int w = 0; int h = 0;
	w = platform::getWindowSizeX() * 2;
	h = platform::getWindowSizeY() * 2;
	renderer.updateWindowMetrics(w, h);
	renderer.clearScreen(gl2d::Color4f(0.2,0.2,0.3, 1));
#pragma endregion

	if (change) render2();
	else render1();
	if (platform::isKeyReleased(platform::Button::Q)) change = !change;

#pragma region set finishing stuff

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
