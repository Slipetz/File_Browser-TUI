#include "Events.h"
#include <sstream>

Event::Type Event::GenerateEventType() const {
	if(_inputRecord.EventType == MOUSE_EVENT)
		return Event::Type::MOUSE;

	return Event::Type::KEYBOARD;
}

bool MouseEvent::MouseLeftClick() const {
	return _mouseEvent.dwEventFlags == 0 && _mouseEvent.dwButtonState&FROM_LEFT_1ST_BUTTON_PRESSED;
}

bool MouseEvent::MouseRightClick() const {
	return _mouseEvent.dwEventFlags == 0 && _mouseEvent.dwButtonState&RIGHTMOST_BUTTON_PRESSED;
}

bool MouseEvent::MouseVerticalScroll() const {
	return _mouseEvent.dwEventFlags == MOUSE_WHEELED;
}