#include <cassert>
#include <sstream>
#include "Window.h"


std::vector<std::string> ToggleButton::RenderOutputString() {
	//Need to break down the parts into a string
	//Few asserts, just for generalized help
	assert(static_cast<size_t>(_componentLength) > _contentString.size() && "ToggleButton is too small to fit content. Increase it's size");
	assert(_componentHeight >= 2 && "Button needs to be at least two rows high. Increase it's size");

	//First Row: Content String + space (if applicable)
	//Second Row Row: On/Off + Space
	//Any additional Row - Spaces

	std::vector<std::string> button;
	button.push_back(GenerateStringRow(_contentString));
	button.push_back(GenerateStringRow(_toggled ? std::string("On") : std::string("Off")));
	for (Console::console_length_type i = 2; i < GetComponentHeight(); ++i) {
		button.push_back(GenerateStringRow(""));
	}
	return button;
}

std::string ToggleButton::GenerateStringRow(std::string const& content) {
	std::ostringstream wss;
	//Need to parse where the content string should be centered
	int contentStringStart = (GetComponentLength() - content.size()) / 2;
	wss << std::string(contentStringStart, ' ') << content.c_str() << std::string(GetComponentLength() - (contentStringStart + content.size()), ' ');
	return wss.str();
}

std::vector<std::string> TextBox::RenderOutputString() {
	// +1 -> One for the colon
	assert(static_cast<size_t>(_componentLength) > _label.size() + 1 && "Textbox is too small for content. Increase it's size");
	//Only one row
	std::vector<std::string> textBox;
	textBox.push_back(_label + ":");
	if (_text.size() > (_componentLength - _label.size() - 1)) {
		//Means our string is too big for output, need to render it
		textBox.push_back(_text.substr(_outputPos, _componentLength - _label.size() - 1));
	}
	else
		textBox.push_back(_text + std::string(_componentLength - (_label.size() + _text.size() + 1), ' '));
	return textBox;
}

std::vector<std::string> TextPad::RenderOutputString() {
	std::vector<std::string> outputDisplay;
	for (size_t i = 0; i < _results.size(); ++i) {
		std::string parseString = _results[i];
		//1 for the scroll bar on the right side
		if (parseString.size() < static_cast<size_t>(_componentLength - 1)) {
			outputDisplay.push_back(_results[i]);
		}
		else {
			while (parseString.size() > static_cast<size_t>((_componentLength - 1))) {
				outputDisplay.push_back(parseString.substr(0, _componentLength - 1));
				parseString = parseString.erase(0, _componentLength - 1);
			}
			if (parseString.size() > 0) {
				outputDisplay.push_back(parseString);
			}
		}
	}
	return outputDisplay;
}