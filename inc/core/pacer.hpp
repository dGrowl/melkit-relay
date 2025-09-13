#ifndef CORE_PACER_HPP_
#define CORE_PACER_HPP_

#include <SDL3/SDL_stdinc.h>

namespace core {

class Pacer {
private:
	const Uint64 _targetHertz;
	const Uint64 _targetFrameDurationNs;
	Uint64       _nextFrameTimeNs;

public:
	Pacer();

	void startFrame();
	void endFrame();
};

}  // namespace core

#endif  // CORE_PACER_HPP_
