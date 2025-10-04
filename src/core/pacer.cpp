#include "core/pacer.hpp"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

constexpr Uint64 NS_PER_SECOND = 1'000'000'000;
constexpr Uint64 DEFAULT_HERTZ = 60;

namespace core {

Pacer::Pacer() :
    _targetHertz(DEFAULT_HERTZ),
    _targetFrameDurationNs(NS_PER_SECOND / _targetHertz),
    _nextFrameTimeNs(SDL_GetTicksNS()) {}

void Pacer::startFrame() {
	const Uint64 frameStartNs = SDL_GetTicksNS();
	if (frameStartNs > _nextFrameTimeNs + _targetFrameDurationNs) {
		_nextFrameTimeNs = frameStartNs;
	}
}

void Pacer::endFrame() {
	_nextFrameTimeNs += _targetFrameDurationNs;
	const Uint64 frameEndNs = SDL_GetTicksNS();
	if (frameEndNs < _nextFrameTimeNs) {
		SDL_DelayNS(_nextFrameTimeNs - frameEndNs);
	}
}

}  // namespace core
