#pragma once
#include "Origin\Core\Application.h"

extern Origin::Application* Origin::CreateApplication();

int main() {
	Origin::Log::Init();
	Origin::Application* app = Origin::CreateApplication();
	app->Run();
	delete app;
}