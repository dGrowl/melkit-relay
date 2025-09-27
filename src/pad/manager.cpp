#include "pad/manager.hpp"

#include <SDL3/SDL_gamepad.h>

static constexpr auto NONE_INDEX = 0;

namespace pad {

Manager::Manager() :
    _activeGamepad(nullptr),
    _iActive(0),
    _names{"None"},
    _ids{0} {}

Manager::~Manager() {
	clearActive();
}

auto Manager::getNames() const -> const std::vector<const char*>& {
	return _names;
}

SDL_JoystickID Manager::getActiveId() const {
	return _ids[_iActive];
}

size_t Manager::getActiveIndex() const {
	return _iActive;
}

void Manager::clearActive() {
	if (_activeGamepad != nullptr) {
		SDL_CloseGamepad(_activeGamepad);
		_activeGamepad = nullptr;
		_iActive       = 0;
	}
}

void Manager::refreshDevices() {
	int  nPads    = 0;
	auto gamepads = SDL_GetGamepads(&nPads);
	_ids          = {0};
	_names        = {"None"};
	for (int i = 0; i < nPads; ++i) {
		_ids.emplace_back(gamepads[i]);
		_names.emplace_back(SDL_GetGamepadNameForID(gamepads[i]));
	}
	if (_ids.size() > 1 && _activeGamepad == nullptr) {
		setActive(1);
	}
	SDL_free(gamepads);
}

void Manager::setActive(const size_t i) {
	if (i == _iActive) {
		return;
	}
	clearActive();
	if (i > 0 && i < _ids.size()) {
		_activeGamepad = SDL_OpenGamepad(_ids[i]);
	}
	_iActive = i;
}

}  // namespace pad
