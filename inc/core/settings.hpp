#ifndef CORE_SETTINGS_HPP_
#define CORE_SETTINGS_HPP_

#include <mutex>
#include <vector>

#include <rapidjson/document.h>

#include "vts/input.hpp"
#include "vts/parameter.hpp"

namespace rj = rapidjson;

namespace core {

struct SettingsParameter {
	std::string                 name;
	vts::BlendMode              blendMode;
	std::vector<vts::InputData> inputs;
};

class Settings {
private:
	mutable std::mutex _mutex;
	rj::Document       _document;

	Settings();
	bool validate();
	void backup(const std::string& contents);
	void load();
	void loadDefault();
	void save();
	void saveUnlocked();

public:
	static Settings& instance();

	~Settings();
	Settings(const Settings&)            = delete;
	Settings& operator=(const Settings&) = delete;
	Settings(Settings&&)                 = delete;
	Settings& operator=(Settings&&)      = delete;

	const char* getAuthToken();
	void        setAuthToken(const char* newAuthToken);

	float getMouseSensitivity();
	void  setMouseSensitivity(const int newSensitivity);

	const char* getWsUrl();
	void        setWsUrl(const char* newWsUrl);

	std::vector<SettingsParameter> getParameters();
	void                           removeParameter(const std::string& name);
	void                           setParameter(const vts::Parameter& parameter);
};

};  // namespace core

#define SETTINGS (core::Settings::instance())

#endif  // CORE_SETTINGS_HPP_
