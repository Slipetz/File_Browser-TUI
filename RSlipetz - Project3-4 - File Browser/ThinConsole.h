#if !defined(THIN_CONSOLE)
#define THIN_CONSOLE

#include <iostream>
#include <vector>
#include <map>
#include <Windows.h>
#include <functional>

#include "ConsoleError.h"

class ThinConsole {
//Setting Data Types
public:
	using console_length_type = SHORT;
	using console_color_type = int;
	using console_location_type = COORD;
	using console_information_type = DWORD;
	using console_attribute_type = WORD;
	using console_buffer_type = CONSOLE_SCREEN_BUFFER_INFO;
	using console_cursor_type = CONSOLE_CURSOR_INFO;
	using console_window_type = SMALL_RECT;
	using buffer_data_type = CHAR_INFO;
	using string_type = std::wstring;
	using list_buffer_data = std::vector < buffer_data_type > ;

	using console_input_record = INPUT_RECORD;
	using console_key_event = KEY_EVENT_RECORD;
	using console_mouse_event = MOUSE_EVENT_RECORD;

	using input_events = std::vector < console_input_record > ;

	static const console_information_type EXIT_EVENT = CTRL_CLOSE_EVENT;
	static const console_attribute_type MOUSE_INPUT = ENABLE_MOUSE_INPUT;
	static const console_attribute_type WINDOW_INPUT = ENABLE_WINDOW_INPUT;
	static const console_attribute_type PROCESSED_INPUT = ENABLE_PROCESSED_INPUT;
	static const console_attribute_type INSERT_MODE = ENABLE_INSERT_MODE;
	static const console_attribute_type LINE_INPUT = ENABLE_LINE_INPUT;
	static const console_attribute_type EXTENDED_FLAGS = ENABLE_EXTENDED_FLAGS;
	static const console_attribute_type ECHO_INPUT = ENABLE_ECHO_INPUT;

	

private:
	HANDLE inputHandle, outputHandle;
	std::map<console_color_type, console_attribute_type> foregroundColorMap, backgroundColorMap;

public:
	ThinConsole(); 
	~ThinConsole();
	static bool _appRunning;
	ThinConsole& SetWindowSize(console_length_type const& columns, console_length_type const& rows);
	ThinConsole& SetBufferSize(console_length_type const& columns, console_length_type const& rows);
	ThinConsole& SetInputTypes(console_information_type const& consoleMode);
	ThinConsole& SetupCursorInfo(BOOL const& isVisible, console_information_type const& cursorSize);
	ThinConsole& WriteToConsole(string_type const& outputString, console_location_type const& location, console_color_type const& foreground, console_color_type const& background);

	//Console State Items
	console_buffer_type GetConsoleBuffer() const;
	console_cursor_type GetConsoleCursor() const;
	console_information_type GetConsoleInputType() const;
	string_type GetWindowTitle() const;
	list_buffer_data GetBufferData(console_buffer_type const& buffer, console_window_type& window, console_location_type const& coords) const;

	void SetConsoleCursor(console_cursor_type const& cursor);
	void SetCursorPosition(console_location_type const& buffer);
	void SetWindowInformation(console_window_type const& window);
	void SetWindowTitle(string_type const& title);
	void SetBufferData(list_buffer_data const& bufferData, console_location_type const& windowSize, console_window_type window, console_location_type const& coord);

	input_events GetInputEvents(size_t const& numToCapture);

	ThinConsole& SetCtrlHandler();

//Helper Functions
protected:
	void ExceptionCheck(BOOL const& booleanValue, std::string const& calledFunction, int const& lineNumber) const {
		if (!booleanValue)
			throw ConsoleError(std::to_string(GetLastError()), calledFunction, lineNumber);
	}

private:
	void InitializeForegroundColors();
	void InitializeBackgroundColors();
};

#endif