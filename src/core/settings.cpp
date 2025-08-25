#include <fstream>
#include <sstream>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_time.h>
#include <SDL3/SDL_timer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>

#include "core/settings.hpp"

namespace core {

static const char* DEFAULT_WS_URL = "localhost:8001";

static const char* DEFAULT_AUTH_TOKEN = "";

static const char* SCHEMA_STRING = R"({
	"type": "object",
	"properties": {
		"api_url": {
			"type": "string"
		},
		"vts_token": {
			"type": "string"
		},
		"parameters": {
			"type": "array",
			"items": {
				"type": "object",
				"properties": {
					"name": {
						"type": "string"
					},
					"inputs": {
						"type": "array",
						"items": {
							"type": "integer",
							"minimum": 0
						}
					}
				},
				"required": [
					"name",
					"inputs"
				]
			}
		}
	},
	"required": [
		"api_url",
		"vts_token",
		"parameters"
	]
})";

static const char* FILE_PATH = "settings.json";

Settings::Settings() :
    _document() {
	load();
}

Settings& Settings::instance() {
	static Settings instance;
	return instance;
}

Settings::~Settings() {
	save();
}

bool Settings::validate() {
	rj::Document baseDoc;
	baseDoc.Parse(SCHEMA_STRING);
	rj::SchemaDocument schemaDoc(baseDoc);
	rj::SchemaValidator validator(schemaDoc);
	return _document.Accept(validator);
}

void Settings::backup(const std::string& contents) {
	SDL_Time timestamp;
	SDL_GetCurrentTime(&timestamp);

	std::ostringstream path;
	path << "settings_" << SDL_NS_TO_SECONDS(timestamp) << ".backup.json";

	std::ofstream file(path.str());
	if (!file.is_open()) {
		return;
	}
	file << contents;
	file.close();
}

void Settings::load() {
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

	_document.Parse(contents.c_str());
	if (_document.HasParseError() || !validate()) {
		backup(contents);
		loadDefault();
		saveUnlocked();
	}
}

void Settings::loadDefault() {
	_document.SetObject();
	auto& allocator = _document.GetAllocator();

	rj::Value apiUrl;
	apiUrl = rj::StringRef(DEFAULT_WS_URL, SDL_strlen(DEFAULT_WS_URL));

	_document.AddMember("api_url", apiUrl, allocator);

	rj::Value authToken;
	authToken = rj::StringRef(DEFAULT_AUTH_TOKEN, SDL_strlen(DEFAULT_AUTH_TOKEN));

	_document.AddMember("vts_token", authToken, allocator);

	_document.AddMember("parameters", rj::Value(rj::kArrayType), allocator);
}

void Settings::save() {
	std::lock_guard<std::mutex> lock(_mutex);
	saveUnlocked();
}

void Settings::saveUnlocked() {
	rj::StringBuffer buffer;
	rj::PrettyWriter<rj::StringBuffer> writer(buffer);
	_document.Accept(writer);

	std::ofstream file(FILE_PATH);
	if (!file.is_open()) {
		return;
	}

	file << buffer.GetString();
	file.close();
}

const char* Settings::getAuthToken() {
	std::lock_guard<std::mutex> lock(_mutex);

	auto& value = _document["vts_token"];
	return value.GetString();
}

void Settings::setAuthToken(const char* newAuthToken) {
	std::lock_guard<std::mutex> lock(_mutex);

	rj::Value value(newAuthToken, _document.GetAllocator());
	_document["vts_token"] = value;

	saveUnlocked();
}

const char* Settings::getWsUrl() {
	std::lock_guard<std::mutex> lock(_mutex);

	auto& value = _document["api_url"];
	return value.GetString();
}

void Settings::setWsUrl(const char* newWsUrl) {
	std::lock_guard<std::mutex> lock(_mutex);

	rj::Value value(newWsUrl, _document.GetAllocator());
	_document["api_url"] = value;

	saveUnlocked();
}

}  // namespace core
