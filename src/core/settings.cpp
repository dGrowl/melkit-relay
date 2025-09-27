#include "core/settings.hpp"

#include <iostream>
#include <SDL3/SDL_time.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <format>
#include <fstream>
#include <glaze/json/read.hpp>
#include <glaze/json/write.hpp>

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

	std::string path =
	    std::format("settings_{}.backup.json", SDL_NS_TO_SECONDS(timestamp));

	std::ofstream file(path);
	if (!file.is_open()) {
		return;
	}
	file << contents;
	file.close();
}

void SettingsManager::load() {
	std::lock_guard<std::mutex> lock(_mutex);

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
	std::lock_guard<std::mutex> lock(_mutex);
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
	std::lock_guard<std::mutex> lock(_mutex);

	return _data.mouseBounds;
}

const std::string SettingsManager::getAuthToken() const {
	std::lock_guard<std::mutex> lock(_mutex);

	return _data.vtsToken;
}

const std::string SettingsManager::getWsUrl() const {
	std::lock_guard<std::mutex> lock(_mutex);

	return _data.apiUrl;
}

const std::vector<SettingsParameter> SettingsManager::getParameters() const {
	std::lock_guard<std::mutex> lock(_mutex);

	return _data.parameters;
}

int SettingsManager::getMouseSensitivity() const {
	std::lock_guard<std::mutex> lock(_mutex);

	return _data.mouseSensitivity;
}

void SettingsManager::setAuthToken(const char* newAuthToken) {
	std::lock_guard<std::mutex> lock(_mutex);

	_data.vtsToken = newAuthToken;

	saveUnlocked();
}

void SettingsManager::setMouseBounds(const math::Rectangle<int>& bounds) {
	std::lock_guard<std::mutex> lock(_mutex);

	_data.mouseBounds = bounds;

	saveUnlocked();
}

void SettingsManager::setMouseSensitivity(const int newSensitivity) {
	std::lock_guard<std::mutex> lock(_mutex);

	_data.mouseSensitivity = newSensitivity;

	saveUnlocked();
}

void SettingsManager::setParameter(const vts::Parameter& parameter) {
	std::lock_guard<std::mutex> lock(_mutex);

	for (auto it = _data.parameters.begin(); it != _data.parameters.end(); ++it) {
		if (it->name == parameter.getName()) {
			_data.parameters.erase(it);
			break;
		}
	}

	auto& newParameter = _data.parameters.emplace_back(parameter.getName(),
	                                                   parameter.getBlendMode());

	for (const auto& [inputId, input] : parameter.getInputs()) {
		newParameter.inputs.emplace_back(inputId, input.getIsInverted());
	}

	saveUnlocked();
}

void SettingsManager::setWsUrl(const char* newWsUrl) {
	std::lock_guard<std::mutex> lock(_mutex);

	_data.apiUrl = newWsUrl;

	saveUnlocked();
}

void SettingsManager::removeParameter(const std::string& name) {
	std::lock_guard<std::mutex> lock(_mutex);

	for (auto it = _data.parameters.begin(); it != _data.parameters.end(); ++it) {
		if (it->name == name) {
			_data.parameters.erase(it);
			saveUnlocked();
			return;
		}
	}
}

}  // namespace core
