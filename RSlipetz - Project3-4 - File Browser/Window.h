#if !defined(WINDOW_HPP)
#define WINDOW_HPP

#include <memory>
#include <functional>
#include "Console.h"
#include "Events.h"
#include "ObserverAndSubject.h"

class WindowComponent;

class Window {
public:
	Console::console_length_type _windowHeight;
	Console::console_length_type _windowLength;
	Console::console_location_type _startPoint;
	Console::Color _backgroundColor = Console::Color::Black;
	Console::Color _fontColor = Console::Color::White;
	std::map<std::string, std::shared_ptr<WindowComponent*>> _componentList;
public:
	Window() : _windowHeight(0), _windowLength(0), _startPoint({ 0, 0 }) {}
	Window(Console::console_length_type const& height, Console::console_length_type const& length, Console::console_location_type const& startPoint = { 0, 0 }) : _windowHeight(height), _windowLength(length), _startPoint(startPoint) {}
	virtual ~Window() {}
	void SetWindowHeight(Console::console_length_type const& height) { _windowHeight = height; }
	void SetWindowLength(Console::console_length_type const& length) { _windowLength = length; }
	void SetStartPoint(Console::console_location_type const& startPoint) { _startPoint = startPoint; }
	void SetBackgroundColor(Console::Color const& color) { _backgroundColor = color; }
	void SetFontColor(Console::Color const& color) { _fontColor = color; }

	Console::console_length_type GetWindowHeight() const { return _windowHeight; }
	Console::console_length_type GetWindowLength() const { return _windowLength; }
	Console::console_location_type GetStartPoint() const { return _startPoint; }
	Console::Color GetBackgroundColor() const { return _backgroundColor; }
	Console::Color GetFontColor() const { return _fontColor; }

	void AttachComponent(std::string const& key, std::shared_ptr<WindowComponent*> component) { _componentList[key] = component; }

	std::map<std::string, std::shared_ptr<WindowComponent*>> GetComponents() const { return _componentList; }

	virtual void RouteKeyEvent(KeyEvent const& ke) = 0;
	virtual void RouteMouseEvent(MouseEvent const& me) = 0;
};

class WindowComponent {
public:
	Console::console_length_type _componentHeight;
	Console::console_length_type _componentLength;
	Console::console_location_type _startPoint;
	Console::Color _backgroundColor;
	Console::Color _fontColor;

public:
	WindowComponent() {}
	virtual ~WindowComponent() {}

	Console::console_length_type GetComponentHeight() const { return _componentHeight; }
	Console::console_length_type GetComponentLength() const { return _componentLength; }
	Console::console_location_type GetStartPoint() const { return _startPoint; }
	Console::Color GetBackgroundColor() const { return _backgroundColor; }
	Console::Color GetFontColor() const { return _fontColor; }

	void SetBackroundColor(Console::Color const& color) { _backgroundColor = color; }
	void SetFontColor(Console::Color const& color) { _fontColor = color; }

	virtual std::vector<std::string> RenderOutputString() = 0;
};

class ToggleButton : public WindowComponent {
private:
	std::string _contentString;
	bool _toggled = false;
public:
	ToggleButton(Console::console_length_type const& height, Console::console_length_type const& length, Console::console_location_type const& startPoint) {
		_componentHeight = height;
		_componentLength = length;
		_startPoint = startPoint;
		_backgroundColor = Console::Color::Red;
		_fontColor = Console::Color::Black;
	}

	void Toggle() { _toggled = !_toggled; SetBackroundColor(_toggled ? Console::Color::Green : Console::Color::Red); }
	void SetContentString(std::string const& content) { _contentString = content; }
	std::string GetContentString() const { return _contentString; }

	bool IsToggled() const { return _toggled; }
	void SetToggled(bool const& isOn) { _toggled = isOn; }

	std::string GenerateStringRow(std::string const& content);
	std::vector<std::string> RenderOutputString() override;
};

class TextBox : public WindowComponent {
private:
	std::string _label;
	std::string _text;
	Console::Color _labelForeground;
	Console::Color _labelBackground;
	size_t _outputPos;
public:
	TextBox(Console::console_length_type const& length, Console::console_location_type const& startPoint, std::string const& label = "", std::string const& text = "") : _label(label), _text(text), _labelBackground(Console::Color::Grey), _labelForeground(Console::Color::Black), _outputPos(0) {
		_componentHeight = 1;
		_componentLength = length;
		_startPoint = startPoint;
		_backgroundColor = Console::Color::Black;
		_fontColor = Console::Color::White;
	}

	void SetLabel(std::string const& label) { _label = label; }
	std::string GetLabel() const { return _label; }

	void SetText(std::string const& text) { _text = text; }
	std::string GetText() const { return _text; }

	void SetLabelBackgroundColor(Console::Color const& color) { _labelBackground = color; }
	void SetLabelFontColor(Console::Color const& color) { _labelForeground = color; }

	Console::Color GetLabelBackgroundColor() const { return _labelBackground; }
	Console::Color GetLabelFontColor() const { return _labelForeground; }

	size_t GetStringPos() const { return _outputPos; }
	void SetStringPos(size_t const& pos) { _outputPos = pos; }

	std::vector<std::string> RenderOutputString() override;
};

class TextPad : public WindowComponent {
private:
	std::vector<std::string> _results;
	size_t _startNum;
public:
	TextPad(Console::console_length_type const& height, Console::console_length_type const& length, Console::console_location_type const& startPoint, std::vector<std::string> const& results) : _results(results), _startNum(0) {
		_componentHeight = height;
		_componentLength = length;
		_startPoint = startPoint;
		_fontColor = Console::Color::Black;
		_backgroundColor = Console::Color::White;
	}

	void SetResults(std::vector<std::string> const& results) { _results = results; }
	void SetStartNum(size_t const& start) { _startNum = start; }
	std::vector<std::string> RenderOutputString() override;

	size_t GetStartNum() const { return _startNum; }
	size_t GetResultsSize() const { return _results.size(); }
};


#endif