#if !defined(CONSOLE_APP_HPP)
#define CONSOLE_APP_HPP

#include "Console.h"
#include "ObserverAndSubject.h"
#include "Window.h"

class ConsoleApp {
private:
	Console::console_information_type _mode = 0;
protected:
	Console& _consoleHook;
	std::shared_ptr<Subject*> _model;
	std::vector<std::shared_ptr<Window*>> _views;
	std::shared_ptr<Window*> _focusedWindow;
public:
	ConsoleApp() : _consoleHook(Console::GetInstance()) { SetCtrlHandler(); }
	virtual ~ConsoleApp() { }
	void SetCursorInfo(bool const& cursorVisible, Console::console_information_type const& cursorSize = 1);
	void SetWindowTitle(Console::string_type const& title);
	void SetCursorPosition(Console::console_location_type const& location) { _consoleHook.SetCursorPosition(location); }
	Console::console_length_type GetConsoleLength() const { return _consoleHook.GetColumns(); }
	Console::console_length_type GetConsoleHeight() const { return _consoleHook.GetRows(); }
	Console& GetConsoleHook() { return _consoleHook; }

	void SetInputMode(Console::console_information_type const& mode) { _mode = _mode | mode; _consoleHook.SetConsoleInputType(_mode); }

	void RenderWindow(Window const& window);
	void RenderWindows();

	bool InputInvoker(size_t const& numToCapture = 128);

	void RouteMouseEvent(MouseEvent const& me);

	void RouteKeyEvent(KeyEvent const& ke);

	Console::console_location_type GetCursorPosition() const { return _consoleHook.GetCursorPosition(); }
	void WriteToScreen(Console::string_type const& string, Console::console_location_type const& location, Console::Color const& font, Console::Color const& background) { _consoleHook.WriteToConsole(string, location, font, background); }
	void SetCtrlHandler() { _consoleHook.SetCtrlHandler(); }
};



#endif