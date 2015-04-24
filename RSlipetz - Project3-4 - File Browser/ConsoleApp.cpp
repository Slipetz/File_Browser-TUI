#include "ConsoleApp.h"

void ConsoleApp::RenderWindow(Window const& window) {
	//We need to figure out where in the window it's going. 
	//We use the list top-down. First item attached gets rendered first
	//Since we are painting backgrounds only, we just use a "blank space" to render
	Console::string_type outputString(window.GetWindowHeight() * window.GetWindowLength(), ' ');
	_consoleHook.WriteToConsole(outputString, window.GetStartPoint(), window.GetFontColor(), window.GetBackgroundColor());

	//Now we need to render each of the components that make up the window
	for (std::pair<std::string, std::shared_ptr<WindowComponent*>> component : window.GetComponents()) {
		std::vector<std::string> outputVector = (*component.second)->RenderOutputString();
		Console::console_location_type printPoint = (*component.second)->_startPoint;
		if (dynamic_cast<ToggleButton*>(*component.second) != nullptr) {
			for (size_t i = 0; i < outputVector.size(); ++i) {
				Console::string_type outputString(outputVector[i].begin(), outputVector[i].end());
				_consoleHook.WriteToConsole(outputString, printPoint, (*component.second)->_fontColor, (*component.second)->_backgroundColor);
				printPoint.Y += 1;
			}
		} else if(dynamic_cast<TextBox*>(*component.second) != nullptr) {
			//Vector[0] == Label Text + Attributes
			//Vector[1] == Field Text + Attributes
			std::shared_ptr<TextBox*> tbComp = std::make_shared<TextBox*>(dynamic_cast<TextBox*>(*component.second));
			_consoleHook.WriteToConsole(Console::string_type(outputVector[0].begin(), outputVector[0].end()), printPoint, (*tbComp)->GetLabelFontColor(), (*tbComp)->GetLabelBackgroundColor());

			//Now we need to set the text length
			printPoint.X += outputVector[0].size();
			_consoleHook.WriteToConsole(Console::string_type(outputVector[1].begin(), outputVector[1].end()), printPoint, (*tbComp)->_fontColor, (*tbComp)->_backgroundColor);
		}
		else if (dynamic_cast<TextPad*>(*component.second) != nullptr) {
			TextPad* padResults = dynamic_cast<TextPad*>(*component.second);
			for (size_t i = padResults->GetStartNum(); i < outputVector.size() && i != static_cast<size_t>(padResults->_componentHeight + padResults->GetStartNum()); ++i) {
				Console::string_type outputString(outputVector[i].begin(), outputVector[i].end());
				_consoleHook.WriteToConsole(outputString, printPoint, (*component.second)->_fontColor, (*component.second)->_backgroundColor);
				printPoint.Y += 1;
			}
		}
	}
}

void ConsoleApp::SetCursorInfo(bool const& cursorVisible, Console::console_information_type const& cursorSize) {
	_consoleHook.SetCursorInfo(cursorVisible, cursorSize);
}

void ConsoleApp::SetWindowTitle(Console::string_type const& title) {
	_consoleHook.SetWindowTitle(title);
}

bool ConsoleApp::InputInvoker(size_t const& numToCapture) {
	Console::input_events inputEvents = _consoleHook.GetInputEvents(numToCapture);
	for (Console::input_events::iterator it = inputEvents.begin(); it != inputEvents.end(); ++it) {
		switch (it->GetType())
		{
		case Event::Type::KEYBOARD:
			RouteKeyEvent(it->GetKeyEvent());
			break;
		case Event::Type::MOUSE:
			RouteMouseEvent(it->GetMouseEvent());
			break;
		default:
			break;
		}
	}

	return _consoleHook.AppRunning();
}

void ConsoleApp::RouteMouseEvent(MouseEvent const& me) {
	if (!me.MouseLeftClick() && !me.MouseVerticalScroll()) {
		return;
	}

	if (me.MouseVerticalScroll()) {
		(*_focusedWindow)->RouteMouseEvent(me);
		RenderWindow(*(*_focusedWindow));
		return;
	}

	Console::console_location_type mouseClicked = me.GetMouseCoord();
	//Now we know we are dealing with a left click. Need to find out what Window we are dealing with and send it there
	for (std::shared_ptr<Window*> window : _views) {
		if ((*window)->_startPoint.Y <= mouseClicked.Y && mouseClicked.Y <= (*window)->_startPoint.Y + (*window)->_windowHeight) {
			_focusedWindow = window;
			break;
		}
	}
	(*_focusedWindow)->RouteMouseEvent(me);
	RenderWindow(*(*_focusedWindow));
}

void ConsoleApp::RenderWindows() {
	for (std::shared_ptr<Window*> view : _views) {
		RenderWindow(*(*view));
	}
}

void ConsoleApp::RouteKeyEvent(KeyEvent const& ke) {
	if (_focusedWindow) {
		(*_focusedWindow)->RouteKeyEvent(ke);
		RenderWindow(*(*_focusedWindow));
	}
}