#if !defined(CONSOLE__HPP)
#define CONSOLE__HPP

#include <string>
#include <functional>
#include <vector>

#include "ThinConsole.h"
#include "Events.h"

class Console
{
public:
	//Setting the types we need
	enum class Color { Default = 0, Black, DarkBlue, DarkGreen, DarkRed, DarkYellow, DarkCyan, DarkMagenta, Grey, Blue, Red, Green, Magenta, Yellow, Cyan, White };

	using string_type = ThinConsole::string_type;
	using color_list_type = std::vector < Color >;
	using ctrl_handler_type = std::function < bool(void) >;
	using console_length_type = ThinConsole::console_length_type;
	using console_location_type = ThinConsole::console_location_type;
	using console_information_type = ThinConsole::console_information_type;
	using console_attribute_type = ThinConsole::console_attribute_type;
	using console_buffer_type = ThinConsole::console_buffer_type;
	using console_cursor_type = ThinConsole::console_cursor_type;
	using console_window_type = ThinConsole::console_window_type;
	using buffer_data_type = ThinConsole::buffer_data_type;
	using list_buffer_data = ThinConsole::list_buffer_data;
	using console_input_record = ThinConsole::console_input_record;

	static const console_attribute_type MOUSE_INPUT = ThinConsole::MOUSE_INPUT;
	static const console_attribute_type WINDOW_INPUT = ThinConsole::WINDOW_INPUT;
	static const console_attribute_type PROCESSED_INPUT = ThinConsole::PROCESSED_INPUT;
	static const console_information_type EXIT_EVENT = ThinConsole::EXIT_EVENT;

	using console_key_event = ThinConsole::console_key_event;
	using console_mouse_event = ThinConsole::console_mouse_event;

	using input_events = std::vector < Event > ;

private:
	class ConsoleState {
		friend Console;
		console_buffer_type _consoleBuffer;
		console_cursor_type _cursorType;
		console_location_type _bufferCoords;
		console_information_type _consoleMode;
		console_window_type _savedWindow;
		list_buffer_data _bufferData;
		string_type _title;
	};

	//ConsoleState Functions
	void SetConsoleState(ConsoleState const& stateObject);
	ConsoleState GetConsoleState();

private:
	ThinConsole _thinConsole;
	Color _foreground;
	Color _background;
	console_length_type _columns, _rows;
	console_information_type inputType;
	ConsoleState _originalState;

private:
	//Constructor
	Console(console_length_type const& columns = 70, console_length_type const& rows = 50) : _columns(columns), _rows(rows), _foreground(Color::White), _background(Color::Black) { _originalState = GetConsoleState(); SetConsoleWindow(); }
public:
	~Console() { SetConsoleState(_originalState); }

	//Singleton Pattern - C++11 Variety
	static Console& GetInstance();

	console_length_type GetRows() { return _rows; }
	console_length_type GetColumns() { return _columns; }

	//Console Helper Methods
	Console& SetWindowSize(console_length_type const& columns, console_length_type const& rows);

	console_information_type GetConsoleInputType() const { return inputType; }
	Console& SetConsoleInputType(console_information_type const& inputType);

	Console& SetCursorInfo(bool const& isVisible, console_information_type const& cursorSize) { _thinConsole.SetupCursorInfo(isVisible, cursorSize); return *this; }

	Console& SetForegroundColor(Color const& foreground) { _foreground = foreground; return *this; }
	Console& SetBackgroundColor(Color const& background) { _background = background; return *this; }

	Console& SetWindowTitle(Console::string_type const& title) { _thinConsole.SetWindowTitle(title); return *this; }

	ThinConsole& GetLowLevelConsole() { return _thinConsole; }

	//Output types
	Console& WriteToConsole(string_type const& outputString, console_location_type const& startLocation = { 0 }, Color const& foreground = Color::Default, Color const& background = Color::Default);

	input_events GetInputEvents(size_t const& numToCapture);

	void SetCursorPosition(Console::console_location_type const& location) { _thinConsole.SetCursorPosition(location); }

	console_location_type GetCursorPosition() const;

	Console& SetCtrlHandler() { _thinConsole.SetCtrlHandler(); return *this; }

	void RestoreOriginalState() { SetConsoleState(_originalState); }

	bool AppRunning() { return ThinConsole::_appRunning; }
private:
	void SetConsoleWindow();

};

#endif