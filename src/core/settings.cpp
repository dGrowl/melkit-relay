#include "core/settings.hpp"

#include <iostream>

#include <format>
#include <fstream>
#include <iterator>
#include <mutex>
#include <string>
#include <vector>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_time.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <glaze/core/opts.hpp>
#include <glaze/core/reflect.hpp>
#include <glaze/core/write.hpp>
#include <glaze/json/read.hpp>
#include <glaze/json/write.hpp>  // NOLINT(misc-include-cleaner)

#include "math/geometry.hpp"
#include "vts/parameter.hpp"

static constexpr auto FILE_PATH = "settings.json";

namespace core {

SettingsManager::SettingsManager() :
    _mutex(),
    _data() {
	load();
}

SettingsManager& SettingsManager::instance() {
	static SettingsManager instance;
	return instance;
}

SettingsManager::~SettingsManager() {
	save();
}

void SettingsManager::backup(const std::string& contents) {
	SDL_Time timestamp;
	SDL_GetCurrentTime(&timestamp);

	const std::string path =
	    std::format("settings_{}.backup.json", SDL_NS_TO_SECONDS(timestamp));

	std::ofstream file(path);
	if (!file.is_open()) {
		return;
	}
	file << contents;
	file.close();
}

void SettingsManager::load() {
	const std::lock_guard<std::mutex> lock(_mutex);

	std::ifstream file(FILE_PATH);
	if (!file.is_open()) {
		loadDefault();
		saveUnlocked();
		return;
	}

	std::string contents((std::istreambuf_iterator<char>(file)),
	                     std::istreambuf_iterator<char>());
	file.close();

	auto error = glz::read_json(_data, contents);

	if (error) {
		backup(contents);
		loadDefault();
		saveUnlocked();
	}
}

void SettingsManager::loadDefault() {
	_data = Settings();

	const SDL_DisplayID    primaryDisplay = SDL_GetPrimaryDisplay();
	const SDL_DisplayMode* desktopMode = SDL_GetDesktopDisplayMode(primaryDisplay);
	if (desktopMode != nullptr) {
		_data.mouseBounds.bottom = desktopMode->h - 1;
		_data.mouseBounds.right  = desktopMode->w - 1;
	}
}

void SettingsManager::save() {
	const std::lock_guard<std::mutex> lock(_mutex);
	saveUnlocked();
}

void SettingsManager::saveUnlocked() {
	std::ofstream file(FILE_PATH);
	if (!file.is_open()) {
		return;
	}

	std::string jsonString;
	auto        error = glz::write<glz::opts{.prettify = true}>(_data, jsonString);

	if (error) {
		std::cerr
		    << "Error serializing settings: "
		    << glz::format_error(error, jsonString)
		    << std::endl;
		return;
	}

	file << jsonString;
	file.close();
}

const math::Rectangle<int> SettingsManager::getMouseBounds() const {
	const std::lock_guard<std::mutex> lock(_mutex);

	return _data.mouseBounds;
}

const std::string SettingsManager::getAuthToken() const {
	const std::lock_guard<std::mutex> lock(_mutex);

	return _data.vtsToken;
}

const std::string SettingsManager::getWsUrl() const {
	const std::lock_guard<std::mutex> lock(_mutex);

	return _data.apiUrl;
}

const std::vector<SettingsParameter> SettingsManager::getParameters() const {
	const std::lock_guard<std::mutex> lock(_mutex);

	return _data.parameters;
}

float SettingsManager::getThemeHueShift() const {
	const std::lock_guard<std::mutex> lock(_mutex);

	return _data.themeHueShift;
}

int SettingsManager::getMouseSensitivity() const {
	const std::lock_guard<std::mutex> lock(_mutex);

	return _data.mouseSensitivity;
}

void SettingsManager::setAuthToken(const char* newAuthToken) {
	const std::lock_guard<std::mutex> lock(_mutex);

	_data.vtsToken = newAuthToken;

	saveUnlocked();
}

void SettingsManager::setMouseBounds(const math::Rectangle<int>& bounds) {
	const std::lock_guard<std::mutex> lock(_mutex);

	_data.mouseBounds = bounds;

	saveUnlocked();
}

void SettingsManager::setMouseSensitivity(const int newSensitivity) {
	const std::lock_guard<std::mutex> lock(_mutex);

	_data.mouseSensitivity = newSensitivity;

	saveUnlocked();
}

void SettingsManager::setParameter(const vts::Parameter& parameter) {
	const std::lock_guard<std::mutex> lock(_mutex);

	for (auto it = _data.parameters.begin(); it != _data.parameters.end(); ++it) {
		if (it->name == parameter.getName()) {
			_data.parameters.erase(it);
			break;
		}
	}

	auto& newParameter = _data.parameters.emplace_back(parameter.getName(),
	                                                   parameter.getBlendMode());

	for (const auto& [code, receiver] : parameter.getReceivers()) {
		newParameter.receivers.emplace_back(code, receiver.getIsInverted());
	}

	saveUnlocked();
}

void SettingsManager::setThemeHueShift(float shift) {
	const std::lock_guard<std::mutex> lock(_mutex);

	_data.themeHueShift = shift;

	saveUnlocked();
}

void SettingsManager::setWsUrl(const char* newWsUrl) {
	const std::lock_guard<std::mutex> lock(_mutex);

	_data.apiUrl = newWsUrl;

	saveUnlocked();
}

void SettingsManager::removeParameter(const std::string& name) {
	const std::lock_guard<std::mutex> lock(_mutex);

	for (auto it = _data.parameters.begin(); it != _data.parameters.end(); ++it) {
		if (it->name == name) {
			_data.parameters.erase(it);
			saveUnlocked();
			return;
		}
	}
}

}  // namespace core
