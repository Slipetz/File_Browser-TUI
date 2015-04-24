#include <cassert>
#include "ThinConsole.h"

BOOL CtrlHandler(DWORD ctrlType) {
	if (ctrlType <= CTRL_CLOSE_EVENT) {
		ThinConsole::_appRunning = false;
		return TRUE;
	}
	return FALSE;
}

bool ThinConsole::_appRunning = true;

ThinConsole::ThinConsole() : inputHandle(GetStdHandle(STD_INPUT_HANDLE)), outputHandle(GetStdHandle(STD_OUTPUT_HANDLE)) {
	InitializeForegroundColors();
	InitializeBackgroundColors();
}


ThinConsole::~ThinConsole() {
	ExceptionCheck(CloseHandle(inputHandle), __FUNCTION__ ,__LINE__);
	ExceptionCheck(CloseHandle(outputHandle), __FUNCTION__ ,__LINE__);
}

ThinConsole& ThinConsole::SetWindowSize(console_length_type const& columns, console_length_type const& rows) {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	console_window_type window{ 0 };
	window.Right = columns;
	window.Bottom = rows;
	ExceptionCheck(SetConsoleWindowInfo(outputHandle, true, &window), __FUNCTION__, __LINE__);
	return *this;
}

ThinConsole& ThinConsole::SetBufferSize(console_length_type const& columns, console_length_type const& rows) {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	console_location_type bufferSize;
	bufferSize.X = columns;
	bufferSize.Y = rows;
	ExceptionCheck(SetConsoleScreenBufferSize(outputHandle, bufferSize), __FUNCTION__, __LINE__);
	return *this;
}

ThinConsole& ThinConsole::SetInputTypes(console_information_type const& consoleMode) {
	assert(inputHandle != NULL && "Input Handle Not Initialized");
	ExceptionCheck(SetConsoleMode(inputHandle, consoleMode), __FUNCTION__, __LINE__);
	return *this;
}

ThinConsole& ThinConsole::SetupCursorInfo(BOOL const& isVisible, console_information_type const& cursorSize) {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	console_cursor_type cursor;
	cursor.bVisible = isVisible;
	cursor.dwSize = cursorSize;
	ExceptionCheck(SetConsoleCursorInfo(outputHandle, &cursor), __FUNCTION__, __LINE__);
	return *this;
}

ThinConsole& ThinConsole::WriteToConsole(string_type const& outputString, console_location_type const& location, console_color_type const& foreground, console_color_type const& background) {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	std::vector<console_attribute_type> outputAttributes(outputString.size(), foregroundColorMap[foreground] | backgroundColorMap[background]);
	console_information_type charsWritten;
	ExceptionCheck(WriteConsoleOutputCharacter(outputHandle, std::wstring(outputString.begin(), outputString.end()).c_str(), outputString.size(), location, &charsWritten), __FUNCTION__, __LINE__);
	ExceptionCheck(WriteConsoleOutputAttribute(outputHandle, outputAttributes.data(), outputAttributes.size(), location, &charsWritten), __FUNCTION__, __LINE__);
	return *this;
}

ThinConsole::console_buffer_type ThinConsole::GetConsoleBuffer() const {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	console_buffer_type oldBuffer;
	ExceptionCheck(GetConsoleScreenBufferInfo(outputHandle, &oldBuffer), __FUNCTION__, __LINE__);
	return oldBuffer;
}

ThinConsole::console_cursor_type ThinConsole::GetConsoleCursor() const {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	console_cursor_type oldCursor;
	ExceptionCheck(GetConsoleCursorInfo(outputHandle, &oldCursor), __FUNCTION__, __LINE__);
	return oldCursor;
}

ThinConsole::console_information_type ThinConsole::GetConsoleInputType() const {
	assert(inputHandle != NULL && "Input Handle Not Initialized");
	console_information_type consoleMode;
	ExceptionCheck(GetConsoleMode(inputHandle, &consoleMode), __FUNCTION__, __LINE__);
	return consoleMode;
}

ThinConsole::string_type ThinConsole::GetWindowTitle() const {
	wchar_t charArray[25];
	ExceptionCheck(GetConsoleTitle(charArray, 25), __FUNCTION__, __LINE__);
	return string_type(charArray);
}

ThinConsole::list_buffer_data ThinConsole::GetBufferData(console_buffer_type const& buffer, console_window_type& window, console_location_type const& coords) const {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	list_buffer_data bufferData(buffer.dwSize.X * buffer.dwSize.Y);
	ExceptionCheck(ReadConsoleOutput(outputHandle, bufferData.data(), buffer.dwSize, { 0 }, &window), __FUNCTION__, __LINE__);
	return std::move(bufferData);
}

void ThinConsole::SetConsoleCursor(ThinConsole::console_cursor_type const& cursor) {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	ExceptionCheck(SetConsoleCursorInfo(outputHandle, &cursor), __FUNCTION__, __LINE__);
}

void ThinConsole::SetCursorPosition(console_location_type const& cursorLocation) {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	ExceptionCheck(SetConsoleCursorPosition(outputHandle, cursorLocation), __FUNCTION__, __LINE__);
}

void ThinConsole::SetWindowInformation(console_window_type const& window) {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	ExceptionCheck(SetConsoleWindowInfo(outputHandle, TRUE, &window), __FUNCTION__, __LINE__);
}

void ThinConsole::SetWindowTitle(string_type const& title) {
	ExceptionCheck(SetConsoleTitle(title.c_str()), __FUNCTION__, __LINE__);
}

void ThinConsole::SetBufferData(list_buffer_data const& bufferData, console_location_type const& windowSize, console_window_type window, console_location_type const& coords) {
	assert(outputHandle != NULL && "Output Handle Not Initialized");
	ExceptionCheck(WriteConsoleOutput(outputHandle, bufferData.data(), windowSize, coords, &window), __FUNCTION__, __LINE__);
}

void ThinConsole::InitializeBackgroundColors() {
	//DARK COLORS
	backgroundColorMap[1] = 0;
	backgroundColorMap[2] = BACKGROUND_BLUE;
	backgroundColorMap[3] = BACKGROUND_GREEN;
	backgroundColorMap[4] = BACKGROUND_RED;
	backgroundColorMap[5] = BACKGROUND_GREEN | BACKGROUND_RED;
	backgroundColorMap[6] = BACKGROUND_GREEN | BACKGROUND_BLUE;
	backgroundColorMap[7] = BACKGROUND_RED | BACKGROUND_BLUE;
	backgroundColorMap[8] = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;

	//LIGHTER COLORS
	backgroundColorMap[9] = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	backgroundColorMap[10] = BACKGROUND_RED | BACKGROUND_INTENSITY;
	backgroundColorMap[11] = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	backgroundColorMap[12] = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
	backgroundColorMap[13] = BACKGROUND_GREEN | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	backgroundColorMap[14] = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	backgroundColorMap[15] = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
}

void ThinConsole::InitializeForegroundColors() {

	//DARK COLORS
	foregroundColorMap[1] = 0;
	foregroundColorMap[2] = FOREGROUND_BLUE;
	foregroundColorMap[3] = FOREGROUND_GREEN;
	foregroundColorMap[4] = FOREGROUND_RED;
	foregroundColorMap[5] = FOREGROUND_GREEN | FOREGROUND_RED;
	foregroundColorMap[6] = FOREGROUND_GREEN | FOREGROUND_BLUE;
	foregroundColorMap[7] = FOREGROUND_RED | FOREGROUND_BLUE;
	foregroundColorMap[8] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;

	//LIGHTER COLORS
	foregroundColorMap[9] = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	foregroundColorMap[10] = FOREGROUND_RED | FOREGROUND_INTENSITY;
	foregroundColorMap[11] = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	foregroundColorMap[12] = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
	foregroundColorMap[13] = FOREGROUND_GREEN | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	foregroundColorMap[14] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	foregroundColorMap[15] = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
}

ThinConsole::input_events ThinConsole::GetInputEvents(size_t const& numToCapture) {
	assert(inputHandle != NULL && "Input Handle Not Initialized");
	input_events capturedEvents(numToCapture);
	console_information_type eventsCaptured;
	ExceptionCheck(ReadConsoleInput(inputHandle, capturedEvents.data(), static_cast<console_information_type>(capturedEvents.size()), &eventsCaptured), __FUNCTION__, __LINE__);
	capturedEvents.resize(eventsCaptured);
	return capturedEvents;
}

ThinConsole& ThinConsole::SetCtrlHandler() {
	ExceptionCheck(SetConsoleCtrlHandler((PHANDLER_ROUTINE)(CtrlHandler), TRUE), __FUNCTION__, __LINE__);
	return *this;
}

