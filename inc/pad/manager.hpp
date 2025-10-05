#ifndef PAD_MANAGER_HPP_
#define PAD_MANAGER_HPP_

#include <cstddef>
#include <vector>

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_joystick.h>

namespace pad {

using NameList = std::vector<const char*>;

class Manager {
private:
	SDL_Gamepad*                _activeGamepad;
	size_t                      _iActive;
	NameList                    _names;
	std::vector<SDL_JoystickID> _ids;

public:
	Manager();
	~Manager();

	[[nodiscard]] auto           getNames() const -> const NameList&;
	[[nodiscard]] SDL_JoystickID getActiveId() const;
	[[nodiscard]] size_t         getActiveIndex() const;

	void clearActive();
	void refreshDevices();
	void setActive(const size_t i);
};

}  // namespace pad

#endif  // PAD_MANAGER_HPP_
