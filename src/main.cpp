#include <SDL3/SDL.h>

#include "../lib/imgui/backends/imgui_impl_sdl3.h"
#include "../lib/imgui/backends/imgui_impl_sdlgpu3.h"
#include "../lib/imgui/imgui.h"

#include "config_window.hpp"

const SDL_InitFlags SDL_FLAGS = (SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);

int main(int, char*[]) {
	if (!SDL_Init(SDL_FLAGS)) {
		return 1;
	}

	SDL_GPUDevice* gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV |
	                                             SDL_GPU_SHADERFORMAT_DXIL |
	                                             SDL_GPU_SHADERFORMAT_METALLIB,
	                                         true,
	                                         nullptr);
	if (gpu == nullptr) {
		return 2;
	}

	IMGUI_CHECKVERSION();
	ConfigWindow config;
	if (config.create(gpu) != 0) {
		return 3;
	}

	SDL_DestroyGPUDevice(gpu);
	SDL_Quit();

	return 0;
}
