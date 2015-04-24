#include "FileBrowserApp.h"

FileBrowser::ModelChangeWindow::ModelChangeWindow(FileBrowser* const& hook) : _controllerHook(std::make_shared<FileBrowser*>(hook)) {
	//Here, we need to create everything that we need and set it
	//Top item, startPoint default is set to {0,0}
	_windowHeight = 2;
	_windowLength = (*_controllerHook)->GetConsoleLength();
	_backgroundColor = Console::Color::DarkBlue;

	//Add each item we are going to use
	TextBox regexTextBox(40, { 0, 0 }, "Regex");
	_componentList["regex"] = std::make_shared<WindowComponent*>(new TextBox(regexTextBox));
	TextBox filePathTextBox(40, { 0, 1 }, "File Path");
	_componentList["filepath"] = std::make_shared<WindowComponent*>(new TextBox(filePathTextBox));
	ToggleButton recursiveButton(2, 20, { _windowLength - 20, 0 });
	recursiveButton.SetContentString("Recursion");
	_componentList["recursion"] = std::make_shared<WindowComponent*>(new ToggleButton(recursiveButton));
}

FileBrowser::ModelStatsWindow::ModelStatsWindow(FileBrowser* const& hook) : _controllerHook(std::make_shared<FileBrowser*>(hook)) {
	//Bottom Item
	_windowHeight = 1;
	_windowLength = (*_controllerHook)->GetConsoleLength();
	_backgroundColor = Console::Color::DarkGreen;
	_startPoint = { 0, (*_controllerHook)->GetConsoleHeight() -1 };

	TextBox searchedTextBox(25, { 0, (*_controllerHook)->GetConsoleHeight() - 1}, "Searched");
	_componentList["searched"] = std::make_shared<WindowComponent*>(new TextBox(searchedTextBox));
	TextBox matchedTextBox(25, { searchedTextBox.GetComponentLength(), (*_controllerHook)->GetConsoleHeight() - 1}, "Matched");
	_componentList["matched"] = std::make_shared<WindowComponent*>(new TextBox(matchedTextBox));
	TextBox sizesTextBox(20, { searchedTextBox.GetComponentLength() + matchedTextBox.GetComponentLength(), (*_controllerHook)->GetConsoleHeight() - 1}, "Sizes");
	_componentList["sizes"] = std::make_shared<WindowComponent*>(new TextBox(sizesTextBox));
}

FileBrowser::ModelDisplayWindow::ModelDisplayWindow(FileBrowser* const& hook) : _controllerHook(std::make_shared<FileBrowser*>(hook)) {
	_windowHeight = (*_controllerHook)->GetConsoleHeight() - 3;
	_windowLength = (*_controllerHook)->GetConsoleLength();
	_backgroundColor = Console::Color::White;
	_fontColor = Console::Color::Black;
	_startPoint = { 0, 2 };

	TextPad resultsTextPad(_windowHeight, _windowLength, _startPoint, std::vector<std::string>());
	_componentList["display"] = std::make_shared<WindowComponent*>(new TextPad(resultsTextPad));
}

void FileBrowser::ModelChangeWindow::RouteMouseEvent(MouseEvent const& me) {
	for (std::pair < std::string, std::shared_ptr<WindowComponent*>> item : _componentList) {
		if ((*item.second)->_startPoint.Y <= me._mouseEvent.dwMousePosition.Y && me._mouseEvent.dwMousePosition.Y < (*item.second)->_startPoint.Y + (*item.second)->_componentHeight) {
			if ((*item.second)->_startPoint.X <= me._mouseEvent.dwMousePosition.X && me._mouseEvent.dwMousePosition.X < (*item.second)->_startPoint.X + (*item.second)->_componentLength) {
				//We now have the right component we are dealing with - Need to figure out what to do with it
				TextBox* tb = dynamic_cast<TextBox*>(*item.second);
				if (tb != nullptr) {
					(*_controllerHook)->SetCursorPosition({ tb->_startPoint.X + tb->GetLabel().size() + 1, tb->_startPoint.Y });
					(*_controllerHook)->SetCursorInfo(true);
				} else {
					//Dealing with the toggle.. just need to toggle it!
					(dynamic_cast<ToggleButton*>(*item.second))->Toggle();
					FileBrowserModel* model = dynamic_cast<FileBrowserModel*>(*(*_controllerHook)->_model);
					TextBox* regex = (dynamic_cast<TextBox*>(*_componentList["regex"]));
					TextBox* filepath = (dynamic_cast<TextBox*>(*_componentList["filepath"]));
					model->SetRecursive((dynamic_cast<ToggleButton*>(*_componentList["recursion"]))->IsToggled());
					model->SetRegex(regex->GetText());
					model->SetFilePath(filepath->GetText());
					model->RefreshModel();
				}
			}
		}
	}
	(*_controllerHook)->RenderWindows();
}

void FileBrowser::ModelChangeWindow::RouteKeyEvent(KeyEvent const& ke) {
	for (std::pair < std::string, std::shared_ptr<WindowComponent*>> item : _componentList) {
		TextBox* tbPtr = dynamic_cast<TextBox*>(*item.second);
		if (tbPtr == nullptr)
			continue;
		Console::console_location_type cursorPosition = (*_controllerHook)->GetCursorPosition();
		if (tbPtr->_startPoint.Y != cursorPosition.Y)
			continue;
		if (ke.KeyDown())
		{
			switch (ke.VirtualKeyCode()) {
			case VK_BACK:
				//Means the string is too big to fit, we need to figure out where we are in the string
				if (tbPtr->GetText().size() > 0) {
					if (cursorPosition.X != _startPoint.X + tbPtr->GetLabel().size() + 1) {
						cursorPosition.X--;
						tbPtr->SetText(tbPtr->GetText().erase((cursorPosition.X - tbPtr->GetLabel().size()) + tbPtr->GetStringPos() - 1, 1));
						if (tbPtr->GetStringPos() > 0) {
							tbPtr->SetStringPos(tbPtr->GetStringPos() - 1);
						}
					}
				}
				break;
				break;
			case VK_DELETE:
				if (tbPtr->GetText().size() > 0) {
					tbPtr->SetText(tbPtr->GetText().erase((cursorPosition.X - tbPtr->GetLabel().size()) + tbPtr->GetStringPos() - 1, 1));
				}
				if (tbPtr->GetStringPos() > 0) {
					tbPtr->SetStringPos(tbPtr->GetStringPos() - 1);
				}
				break;
			case VK_LEFT:
				if (cursorPosition.X != _startPoint.X + tbPtr->GetLabel().size() + 1)
					cursorPosition.X--;
				else if (tbPtr->GetStringPos() > 0) {
					tbPtr->SetStringPos(tbPtr->GetStringPos() - 1);
				}
				break;
			case VK_RIGHT:
				if (cursorPosition.X + 1 != _startPoint.X + tbPtr->_componentLength) {
					if ((cursorPosition.X - tbPtr->GetLabel().size() - 1) < tbPtr->GetText().size()) {
						cursorPosition.X++;
					}
				}
				else {
					if (tbPtr->GetStringPos() + (cursorPosition.X + 1 - tbPtr->GetLabel().size() - 1) < tbPtr->GetText().size()) {
						tbPtr->SetStringPos(tbPtr->GetStringPos() + 1);
					}
				}
				break;
			case VK_END:
				if (tbPtr->GetText().size() >= (tbPtr->_componentLength - tbPtr->GetLabel().size() - 1)) {
					tbPtr->SetStringPos(tbPtr->GetText().size() - (tbPtr->_componentLength - tbPtr->GetLabel().size() - 1));
					cursorPosition.X = tbPtr->_componentLength + _startPoint.X;
				}
				else {
					cursorPosition.X = tbPtr->GetLabel().size() + 1 + _startPoint.X + tbPtr->GetText().size();
				}
				break;
			case VK_HOME:
				tbPtr->SetStringPos(0);
				cursorPosition.X = tbPtr->GetLabel().size() + 1 + _startPoint.X;
				break;
			case VK_RETURN: {
				//Need to get the model and update it
				FileBrowserModel* model = dynamic_cast<FileBrowserModel*>(*(*_controllerHook)->_model);
				TextBox* regex = (dynamic_cast<TextBox*>(*_componentList["regex"]));
				TextBox* filepath = (dynamic_cast<TextBox*>(*_componentList["filepath"]));
				model->SetRecursive((dynamic_cast<ToggleButton*>(*_componentList["recursion"]))->IsToggled());
				model->SetRegex(regex->GetText());
				model->SetFilePath(filepath->GetText());
				model->RefreshModel();
				(*_controllerHook)->RenderWindows();
			}
				break;
			default:
				//Write the character to the screen
				if (isprint(ke.AsciiChar())) {
					tbPtr->SetText(tbPtr->GetText() + ke.AsciiChar());
					if (tbPtr->GetText().size() > (tbPtr->_componentLength - tbPtr->GetLabel().size() - 1)) {
						tbPtr->SetStringPos(tbPtr->GetStringPos() + 1);
					}
					if (cursorPosition.X + 1 != _startPoint.X + tbPtr->_componentLength)
						cursorPosition.X++;
				}
				break;
			}
		}
		(*_controllerHook)->SetCursorPosition(cursorPosition);
	}
}

void FileBrowser::ModelStatsWindow::Update() {
	FileBrowserModel* modelRepresentation = dynamic_cast<FileBrowserModel*>(*(*_controllerHook)->_model);
	(dynamic_cast<TextBox*>(*_componentList["searched"]))->SetText(std::to_string(modelRepresentation->GetNumSearched()));
	(dynamic_cast<TextBox*>(*_componentList["matched"]))->SetText(std::to_string(modelRepresentation->GetNumMatched()));
	(dynamic_cast<TextBox*>(*_componentList["sizes"]))->SetText(std::to_string(modelRepresentation->GetFileSizes()));
}

void FileBrowser::ModelDisplayWindow::Update() {
	FileBrowserModel* modelRepresentation = dynamic_cast<FileBrowserModel*>(*(*_controllerHook)->_model);
	(dynamic_cast<TextPad*>(*_componentList["display"]))->SetResults(modelRepresentation->GetModelResults());
	(dynamic_cast<TextPad*>(*_componentList["display"]))->SetStartNum(0);
}

void FileBrowser::ModelChangeWindow::Update() {
	FileBrowserModel* modelRepresentation = dynamic_cast<FileBrowserModel*>(*(*_controllerHook)->_model);
	std::string regex = modelRepresentation->GetRegex();
	std::string filepath = modelRepresentation->GetFilepath();
	(dynamic_cast<TextBox*>(*_componentList["regex"]))->SetText(regex);
	(dynamic_cast<TextBox*>(*_componentList["filepath"]))->SetText(filepath);
	(dynamic_cast<ToggleButton*>(*_componentList["recursion"]))->SetToggled(modelRepresentation->GetRecursive());
}

void FileBrowser::ModelDisplayWindow::RouteKeyEvent(KeyEvent const& ke) {
	TextPad* tpPtr = dynamic_cast<TextPad*>(*_componentList["display"]);
	if (ke.KeyDown()) {
		switch (ke.VirtualKeyCode()) {
		case VK_UP:
			tpPtr->SetStartNum(static_cast<int>((tpPtr->GetStartNum() - 1)) < 0 ? 0 : tpPtr->GetStartNum() - 1);
			break;
		case VK_DOWN:
			tpPtr->SetStartNum((tpPtr->GetResultsSize() - tpPtr->GetStartNum() + 1) == tpPtr->_componentHeight ? tpPtr->GetStartNum() : tpPtr->GetStartNum() + 1);
			break;
		}
	}
}

void FileBrowser::ModelDisplayWindow::RouteMouseEvent(MouseEvent const& me) {
	TextPad* tpPtr = dynamic_cast<TextPad*>(*_componentList["display"]);
	if (!me.MouseVerticalScroll())
		return;
	short state = HIWORD(me._mouseEvent.dwButtonState);
	if (state > 0) {
		//Means we are scrolling away from the user - need to move up
		tpPtr->SetStartNum(static_cast<int>((tpPtr->GetStartNum() - 1)) < 0 ? 0 : tpPtr->GetStartNum() - 1);
	}
	else {
		tpPtr->SetStartNum((tpPtr->GetResultsSize() - tpPtr->GetStartNum() + 1) == tpPtr->_componentHeight ? tpPtr->GetStartNum() : tpPtr->GetStartNum() + 1);
	}
}