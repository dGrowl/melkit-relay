#ifndef SYS_SETTINGS_HPP_
#define SYS_SETTINGS_HPP_

#include <mutex>

#include <rapidjson/document.h>

namespace rj = rapidjson;

namespace sys {

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

	const char* getWsUrl();
	void setWsUrl(const char* newWsUrl);
};

};  // namespace sys

#define SETTINGS (sys::Settings::instance())

#endif  // SYS_SETINGS_HPP_
