#ifndef CORE_SETTINGS_HPP_
#define CORE_SETTINGS_HPP_

#include <glaze/core/common.hpp>
#include <glaze/core/meta.hpp>
#include <mutex>
#include <string>
#include <vector>

#include "impulse/code.hpp"
#include "math/geometry.hpp"
#include "vts/parameter.hpp"

template <>
struct glz::meta<vts::BlendMode> {
	using enum vts::BlendMode;
	static constexpr auto value = glz::enumerate(MAX, BOUNDED_SUM);
};

template <typename T>
struct glz::meta<math::Rectangle<T>> {
	using Type                  = math::Rectangle<T>;
	static constexpr auto value = object("top",
	                                     &Type::top,
	                                     "left",
	                                     &Type::left,
	                                     "bottom",
	                                     &Type::bottom,
	                                     "right",
	                                     &Type::right);
};

namespace core {

struct SettingsReceiver {
	imp::Code code;
	bool      isInverted;

	struct glaze {
		using T = SettingsReceiver;

		static constexpr auto value =
		    glz::object("code", &T::code, "is_inverted", &T::isInverted);
	};
};

struct SettingsParameter {
	std::string                   name;
	vts::BlendMode                blendMode;
	std::vector<SettingsReceiver> receivers;

	struct glaze {
		using T = SettingsParameter;

		static constexpr auto value = glz::object("name",
		                                          &T::name,
		                                          "blend_mode",
		                                          &T::blendMode,
		                                          "inputs",
		                                          &T::receivers);
	};
};

struct Settings {
	std::string          apiUrl           = "localhost:8001";
	std::string          vtsToken         = "";
	int                  mouseSensitivity = 20;
	math::Rectangle<int> mouseBounds{
	    .top    = 0,
	    .left   = 0,
	    .bottom = 127,
	    .right  = 127,
	};
	std::vector<SettingsParameter> parameters;

	struct glaze {
		using T = Settings;

		static constexpr auto value = glz::object("api_url",
		                                          &T::apiUrl,
		                                          "vts_token",
		                                          &T::vtsToken,
		                                          "mouse_sensitivity",
		                                          &T::mouseSensitivity,
		                                          "mouse_bounds",
		                                          &T::mouseBounds,
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

	const math::Rectangle<int>           getMouseBounds() const;
	const std::string                    getAuthToken() const;
	const std::string                    getWsUrl() const;
	const std::vector<SettingsParameter> getParameters() const;
	int                                  getMouseSensitivity() const;

	void setAuthToken(const char* newAuthToken);
	void setMouseBounds(const math::Rectangle<int>& bounds);
	void setMouseSensitivity(const int newSensitivity);
	void setParameter(const vts::Parameter& parameter);
	void setWsUrl(const char* newWsUrl);

	void removeParameter(const std::string& name);
};

};  // namespace core

#define SETTINGS (core::SettingsManager::instance())

#endif  // CORE_SETTINGS_HPP_
