#include "pad/manager.hpp"

#include <cstddef>
#include <memory>
#include <span>
#include <vector>

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_stdinc.h>

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

static constexpr SDL_JoystickID NONE_JOYSTICK_ID = 0;

[[nodiscard]]
static std::vector<SDL_JoystickID> getGamepads() {
	int             nPads = 0;
	SDL_JoystickID* raw   = SDL_GetGamepads(&nPads);
	const std::unique_ptr<SDL_JoystickID, decltype(&SDL_free)> guard(raw,
	                                                                 SDL_free);

	const std::span<SDL_JoystickID> spanPads{raw, static_cast<size_t>(nPads)};
	std::vector<SDL_JoystickID>     pads;
	pads.reserve(static_cast<size_t>(nPads) + 1);
	pads.emplace_back(NONE_JOYSTICK_ID);
	pads.insert(pads.end(), spanPads.begin(), spanPads.end());
	return pads;
}

void Manager::refreshDevices() {
	_ids   = getGamepads();
	_names = {"None"};
	for (size_t i = 1; i < _ids.size(); ++i) {
		_names.emplace_back(SDL_GetGamepadNameForID(_ids[i]));
	}
	if (_ids.size() > 1 && _activeGamepad == nullptr) {
		setActive(1);
	}
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
