#ifndef CORE_SETTINGS_HPP_
#define CORE_SETTINGS_HPP_

#include <mutex>

#include <rapidjson/document.h>

namespace rj = rapidjson;

namespace core {

class Settings {
private:
	mutable std::mutex _mutex;
	rj::Document _document;

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
	Settings(const Settings&) = delete;
	Settings& operator=(const Settings&) = delete;
	Settings(Settings&&) = delete;
	Settings& operator=(Settings&&) = delete;

	const char* getAuthToken();
	void setAuthToken(const char* newAuthToken);

	const char* getWsUrl();
	void setWsUrl(const char* newWsUrl);
};

};  // namespace core

#define SETTINGS (core::Settings::instance())

#endif  // CORE_SETTINGS_HPP_
