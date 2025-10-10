#include "gui/image.hpp"

#include <algorithm>
#include <cmath>
#include <span>

#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include "imgui/imgui.h"

#include "gui/utility.hpp"

namespace gui {

Surface::Surface(const char* path) {
	_surface = IMG_Load(path);
}

Surface::Surface(SDL_Surface* surface) {
	_surface = SDL_DuplicateSurface(surface);
}

Surface::~Surface() {
	SDL_DestroySurface(_surface);
}

constexpr auto ICON_PATH = "./img/icon.png";

Surface Surface::APP_ICON() {
	static Surface icon(ICON_PATH);
	return Surface(icon._surface);
}

Surface::operator SDL_Surface*() const {
	return _surface;
}

void Surface::shiftHue(float amount) {
	if (_surface == nullptr) {
		return;
	}
	if (SDL_LockSurface(_surface) == false) {
		SDL_Log("SDL_LockSurface failed: %s", SDL_GetError());
		return;
	}

	SDL_PixelFormat format        = _surface->format;
	auto*           formatDetails = SDL_GetPixelFormatDetails(format);
	Uint8           bpp           = formatDetails->bytes_per_pixel;
	auto            pixels =
	    std::span<Uint8>(static_cast<Uint8*>(_surface->pixels),
	                     static_cast<size_t>(_surface->pitch * _surface->h));

	for (int y = 0; y < _surface->h; ++y) {
		size_t rowOffset =
		    static_cast<size_t>(y) * static_cast<size_t>(_surface->pitch);

		for (int x = 0; x < _surface->w; ++x) {
			size_t pixelOffset = rowOffset + static_cast<size_t>(x * bpp);

			Uint32 pixelValue = 0;
			if (bpp == 1) {
				pixelValue = pixels[pixelOffset];
			}
			else if (bpp == 2) {
				pixelValue = static_cast<Uint32>(pixels[pixelOffset])
				             | (static_cast<Uint32>(pixels[pixelOffset + 1]) << 8);
			}
			else if (bpp == 3) {
				pixelValue = static_cast<Uint32>(pixels[pixelOffset])
				             | (static_cast<Uint32>(pixels[pixelOffset + 1]) << 8)
				             | (static_cast<Uint32>(pixels[pixelOffset + 2]) << 16);
			}
			else if (bpp == 4) {
				pixelValue = static_cast<Uint32>(pixels[pixelOffset])
				             | (static_cast<Uint32>(pixels[pixelOffset + 1]) << 8)
				             | (static_cast<Uint32>(pixels[pixelOffset + 2]) << 16)
				             | (static_cast<Uint32>(pixels[pixelOffset + 3]) << 24);
			}
			else {
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
				            "Unsupported BPP: %d for pixel format %s. Skipping pixel.",
				            bpp,
				            SDL_GetPixelFormatName(format));
				break;
			}

			Uint8 redU8, greenU8, blueU8, alphaU8;
			SDL_GetRGBA(pixelValue,
			            formatDetails,
			            nullptr,
			            &redU8,
			            &greenU8,
			            &blueU8,
			            &alphaU8);

			ImVec4 rgba{redU8 / 255.0f,
			            greenU8 / 255.0f,
			            blueU8 / 255.0f,
			            alphaU8 / 255.0f};
			ImVec4 rgbaShifted = hueShiftRgba(rgba, amount);

			auto redU8Shifted = static_cast<Uint8>(
			    std::round(std::clamp(rgbaShifted.x, 0.0f, 1.0f) * 255.0f));
			auto greenU8Shifted = static_cast<Uint8>(
			    std::round(std::clamp(rgbaShifted.y, 0.0f, 1.0f) * 255.0f));
			auto blueU8Shifted = static_cast<Uint8>(
			    std::round(std::clamp(rgbaShifted.z, 0.0f, 1.0f) * 255.0f));
			auto alphaU8Shifted = static_cast<Uint8>(
			    std::round(std::clamp(rgbaShifted.w, 0.0f, 1.0f) * 255.0f));

			Uint32 newPixelValue = SDL_MapRGBA(formatDetails,
			                                   nullptr,
			                                   redU8Shifted,
			                                   greenU8Shifted,
			                                   blueU8Shifted,
			                                   alphaU8Shifted);

			if (bpp == 1) {
				pixels[pixelOffset] = static_cast<Uint8>(newPixelValue);
			}
			else if (bpp == 2) {
				pixels[pixelOffset]     = static_cast<Uint8>(newPixelValue & 0xFF);
				pixels[pixelOffset + 1] = static_cast<Uint8>((newPixelValue >> 8) & 0xFF);
			}
			else if (bpp == 3) {
				pixels[pixelOffset]     = static_cast<Uint8>(newPixelValue & 0xFF);
				pixels[pixelOffset + 1] = static_cast<Uint8>((newPixelValue >> 8) & 0xFF);
				pixels[pixelOffset + 2] = static_cast<Uint8>((newPixelValue >> 16) & 0xFF);
			}
			else if (bpp == 4) {
				pixels[pixelOffset]     = static_cast<Uint8>(newPixelValue & 0xFF);
				pixels[pixelOffset + 1] = static_cast<Uint8>((newPixelValue >> 8) & 0xFF);
				pixels[pixelOffset + 2] = static_cast<Uint8>((newPixelValue >> 16) & 0xFF);
				pixels[pixelOffset + 3] = static_cast<Uint8>((newPixelValue >> 24) & 0xFF);
			}
		}
	}

	SDL_UnlockSurface(_surface);
}

}  // namespace gui
