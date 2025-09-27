#ifndef PAD_MANAGER_HPP_
#define PAD_MANAGER_HPP_

#include <SDL3/SDL_gamepad.h>
#include <vector>

namespace pad {

class Manager {
private:
	SDL_Gamepad*                _activeGamepad;
	size_t                      _iActive;
	std::vector<const char*>    _names;
	std::vector<SDL_JoystickID> _ids;

public:
	Manager();
	~Manager();

	auto           getNames() const -> const std::vector<const char*>&;
	SDL_JoystickID getActiveId() const;
	size_t         getActiveIndex() const;
	void           clearActive();
	void           refreshDevices();
	void           setActive(const size_t i);
};

}  // namespace pad

#endif  // PAD_MANAGER_HPP_
