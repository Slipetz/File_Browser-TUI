#include <cassert>
#include <memory>
#include "Console.h"

Console& Console::SetWindowSize(console_length_type const& columns, console_length_type const& rows) {
	_columns = columns;
	_rows = rows;
	SetConsoleWindow();
	return *this;
}

Console& Console::GetInstance() {
	static std::unique_ptr<Console> _instance(new Console);
	return *_instance;
}

void Console::SetConsoleWindow() {
	assert((_columns > 0 && _rows > 0) && "Window Length/Height Must Be > 0");
	//Set the windows size -> ensure we don't crash
	_thinConsole.SetWindowSize(0, 0);

	//Create the buffer to the correct size
	_thinConsole.SetBufferSize(_columns, _rows);

	//Re-create the window to the correct size
	_thinConsole.SetWindowSize(_columns - 1, _rows - 1);
}

Console& Console::SetConsoleInputType(console_information_type const& inputType) {
	this->inputType = inputType;
	_thinConsole.SetInputTypes(this->inputType);
	return *this;
}

Console& Console::WriteToConsole(string_type const& outputString, console_location_type const& startLocation, Color const& foreground, Color const& background) {
	if (foreground != Color::Default)
		SetForegroundColor(foreground);
	if (background != Color::Default)
		SetBackgroundColor(background);

	_thinConsole.WriteToConsole(outputString, startLocation, static_cast<int>(_foreground), static_cast<int>(_background));

	return *this;
}

void Console::SetConsoleState(Console::ConsoleState const& stateObject) {

	_thinConsole.SetConsoleCursor(stateObject._cursorType);
	_thinConsole.SetCursorPosition(stateObject._consoleBuffer.dwCursorPosition);
	_thinConsole.SetInputTypes(stateObject._consoleMode);
	_thinConsole.SetBufferSize(stateObject._consoleBuffer.dwSize.X, stateObject._consoleBuffer.dwSize.Y);
	_thinConsole.SetWindowInformation(stateObject._consoleBuffer.srWindow);
	_thinConsole.SetWindowTitle(stateObject._title);
	_thinConsole.SetBufferData(stateObject._bufferData, stateObject._consoleBuffer.dwSize, stateObject._savedWindow, stateObject._bufferCoords);
}

Console::ConsoleState Console::GetConsoleState() {
	ConsoleState savedState;
	savedState._consoleBuffer = _thinConsole.GetConsoleBuffer();
	savedState._savedWindow = savedState._consoleBuffer.srWindow;
	savedState._bufferCoords = { 0 };

	savedState._cursorType = _thinConsole.GetConsoleCursor();
	savedState._consoleMode = _thinConsole.GetConsoleInputType();
	savedState._title = _thinConsole.GetWindowTitle();
	savedState._bufferData = _thinConsole.GetBufferData(savedState._consoleBuffer, savedState._savedWindow, savedState._bufferCoords);
	return savedState;
}

Console::input_events Console::GetInputEvents(size_t const& numToCapture) {
	ThinConsole::input_events lowLevelEvents = _thinConsole.GetInputEvents(numToCapture);
	input_events objectEvents;
	for (auto it = lowLevelEvents.begin(); it != lowLevelEvents.end(); ++it) {
		objectEvents.push_back(Event(*it));
	}
	return objectEvents;
}

Console::console_location_type Console::GetCursorPosition() const {
	console_buffer_type buffer = _thinConsole.GetConsoleBuffer();
	return buffer.dwCursorPosition;
}