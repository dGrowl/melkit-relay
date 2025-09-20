#ifndef CORE_SETTINGS_HPP_
#define CORE_SETTINGS_HPP_

#include <mutex>
#include <string>
#include <vector>

#include <glaze/core/common.hpp>
#include <glaze/core/meta.hpp>

#include "vts/input.hpp"
#include "vts/parameter.hpp"

template <>
struct glz::meta<vts::BlendMode> {
	using enum vts::BlendMode;
	static constexpr auto value = glz::enumerate(MAX, BOUNDED_SUM);
};

namespace core {

struct SettingsInput {
	vts::InputId id;
	bool         isInverted;

	struct glaze {
		using T = SettingsInput;

		static constexpr auto value =
		    glz::object("id", &T::id, "is_inverted", &T::isInverted);
	};
};

struct SettingsParameter {
	std::string                name;
	vts::BlendMode             blendMode;
	std::vector<SettingsInput> inputs;

	struct glaze {
		using T = SettingsParameter;

		static constexpr auto value = glz::object("name",
		                                          &T::name,
		                                          "blend_mode",
		                                          &T::blendMode,
		                                          "inputs",
		                                          &T::inputs);
	};
};

struct Settings {
	std::string                    apiUrl           = "localhost:8001";
	std::string                    vtsToken         = "";
	int                            mouseSensitivity = 50;
	std::vector<SettingsParameter> parameters;

	struct glaze {
		using T = Settings;

		static constexpr auto value = glz::object("api_url",
		                                          &T::apiUrl,
		                                          "vts_token",
		                                          &T::vtsToken,
		                                          "mouse_sensitivity",
		                                          &T::mouseSensitivity,
		                                          "parameters",
		                                          &T::parameters);
	};
};

class SettingsManager {
private:
	mutable std::mutex _mutex;
	Settings           _data;

	SettingsManager();
	void backup(const std::string& contents);
	void load();
	void loadDefault();
	void save();
	void saveUnlocked();

public:
	static SettingsManager& instance();

	~SettingsManager();
	SettingsManager(const SettingsManager&)            = delete;
	SettingsManager& operator=(const SettingsManager&) = delete;
	SettingsManager(SettingsManager&&)                 = delete;
	SettingsManager& operator=(SettingsManager&&)      = delete;

	const std::string&                    getAuthToken() const;
	const std::string&                    getWsUrl() const;
	const std::vector<SettingsParameter>& getParameters() const;
	int                                   getMouseSensitivity() const;

	void setAuthToken(const char* newAuthToken);
	void setMouseSensitivity(const int newSensitivity);
	void setParameter(const vts::Parameter& parameter);
	void setWsUrl(const char* newWsUrl);

	void removeParameter(const std::string& name);
};

};  // namespace core

#define SETTINGS (core::SettingsManager::instance())

#endif  // CORE_SETTINGS_HPP_
