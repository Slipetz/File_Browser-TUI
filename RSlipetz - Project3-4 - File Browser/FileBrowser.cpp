#include "FileBrowserApp.h"
#include <filesystem>

FileBrowser::FileBrowser(bool const& isRecursive, std::string const& regex, std::string filepath) {
	SetCursorInfo(false);
	SetWindowTitle(L"FileBrowser - Ryan Slipetz");
	SetInputMode(Console::MOUSE_INPUT | Console::PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_EXTENDED_FLAGS);
	_model = std::make_shared<Subject*>(new FileBrowserModel(isRecursive, regex, filepath));
	ModelChangeWindow changeWindow(this);
	ModelStatsWindow statsWindow(this);
	ModelDisplayWindow displayWindow(this);
	(*_model)->Attach(std::make_shared<Observer*>(new ModelStatsWindow(statsWindow)));
	(*_model)->Attach(std::make_shared<Observer*>(new ModelDisplayWindow(displayWindow)));
	(*_model)->Attach(std::make_shared<Observer*>(new ModelChangeWindow(changeWindow)));
	_views.push_back(std::make_shared<Window*>(new ModelChangeWindow(changeWindow)));
	_views.push_back(std::make_shared<Window*>(new ModelDisplayWindow(displayWindow)));
	_views.push_back(std::make_shared<Window*>(new ModelStatsWindow(statsWindow)));
	_focusedWindow = std::make_shared<Window*>(new ModelDisplayWindow(displayWindow));
	(*_model)->UpdateObservers();
	RenderWindows();
}

int main(int argc, char* argv[]) try {
	std::string regex = R"rgx(.*)rgx";
	std::string filepath = "C:\\temp"; //Posting it to C:\\ and having recursive - probably going to have a bad time
	bool recursive = false;
	//Assuming that there is no switching between values on command line
	if (argc == 1) {
		//Means we have no arguments, we can continue ahead
	}
	else {
		for (int i = 1; i < argc; ++i) {
			//These are very rudimentary tests :(
			std::string argTest(argv[i]);
			if (argTest == "-r")
				recursive = true;
			else if (argTest.find(":\\") != std::string::npos) { //Simply searching for the colon and slash in a typical absolute file path
				filepath = argTest;
			}
			else {
				regex = argTest;
			}
		}
	}

	FileBrowser browser(recursive, regex, filepath);

	while (browser.InputInvoker()) {}
}
catch (ConsoleError ex) {
	MessageBoxA(NULL, ex.ToString().c_str(), "Error!", MB_OK);
	return EXIT_FAILURE;
}