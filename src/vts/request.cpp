#include <fstream>
#include <string>

#include <mongoose.h>

#include "core/settings.hpp"
#include "vts/parameter.hpp"
#include "vts/request.hpp"
#include "ws/controller.hpp"

namespace vts {

static char buffer[1024];

static constexpr const char* AUTHENTICATION_REQUEST = R"({
	"apiName": "VTubeStudioPublicAPI",
	"apiVersion": "1.0",
	"requestID": "SomeID",
	"messageType": "AuthenticationRequest",
	"data": {
		"pluginName": "MelKIT::Relay",
		"pluginDeveloper": "github->dGrowl",
		"authenticationToken": "%s"
	}
})";

void authenticate(ws::IController& wsController) {
	std::string token = SETTINGS.getAuthToken();
	if (token.size() > 0) {
		size_t nChars = mg_snprintf(buffer,
		                            sizeof(buffer),
		                            AUTHENTICATION_REQUEST,
		                            token.c_str());
		wsController.sendMessage(std::string(buffer, nChars));
	}
	else {
		requestToken(wsController);
	}
}

static std::string AUTHENTICATION_TOKEN_REQUEST = R"({
	"apiName": "VTubeStudioPublicAPI",
	"apiVersion": "1.0",
	"requestID": "SomeID",
	"messageType": "AuthenticationTokenRequest",
	"data": {
		"pluginName": "MelKIT::Relay",
		"pluginDeveloper": "github->dGrowl",
		"pluginIcon": "iVBORw0KGgoAAAANSUhEUgAAAIAAAACACAMAAAD04JH5AAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAh9QTFRFBwcH/yBByxo0gRIjoBYqWQ8afhIi+h9ADwcIBwYHzxs1sRguJAkNYA8bdhEgEwcJxhoztxgwHQgMZhAdbREf/SBAGAgKBQcHvhgxvRgxbBEethgwwxky+iBAchEfXQ8brhguyBo0DAcH9x8/eBEgVA4YpRYr0Bs2BwcGCQYH8x4+TA0W7R48Sg0WxBkzqBcsvBcwwBkx1hw34xw6rBctjBQm3hw5KgkOnRUpIAkM7B48gBIjFwgKNwsSwhkyuBgvSw0WQgwU0Rs2BwYG7h49kBQnCwYHRw0V3xw5Rg0VBwUHzRs1hRMkRA0V8R49UQ0XjhQmJgkOFggKKAoOMwsRlRUoCgYHBgcH3Rw45R06lRQnVQ4YxRkzBwcFMAoQVw4ZbxEfdA8gOgsSFQcJtxcwGggLTg4Y6B07OAsS4xw7zhs1pxYsphcskRQndREg+x9A0xs2exIhfxIijxQmQAwUuxgw2xw4IgkNHgkM1Rs3GwgLwRkyPQwT5x47HwkMFAcJ3Bw6vxkxEAcIlxQo/CBAzBo1cxEfYhAcRQ0V6R07NAsRHAgLBQcG2Bw4EQcJ9R8+nhUqjRQm5B06iBQllBUoTg0YfxIgGQgKohYrPAwTKwkOaxAeBgYGeREhBQUHxxoz3Bw5Ww8acBEfhxMkUA0XQwwVmhUoCAYG1Bs36h086x087x49KgkNnBMpDQcIsBguqhctIQkM5h07hhMkrRcuooikPAAABGNJREFUeJztmH1M1VUYx5/HF+ZFkjcTnXO6VMS1cL7MdL4ko2XhcL7BjREEERTOFKWIwJcyCyGmoqUYpBFKkLccaWRuzLeSsbZcy6lNyuXUiiXm22xi4jnPOfdyu/z4w1+/y88/nrPd8z3Pc15+n995ec69F8HmhAzAAAzAAAzAAAzwwAH4m6iDARiAARiAARiAARiAARiAARiAAXoAANUY+K9dAH3aSQLabQIIwH/AcQsg8JZNAIE3SYJuGtb2AEDQdZIB120CCOh3lTTkqk0AIVdIwq4YV/sfIOwyycDLNgEMxFYpEfinTQCDfycZ1PGXTQBDLpGEt3VT73eAh2kFYOglmwCGXiAJHnDBJoBh50mGn++ugb8BQtXaj/jNJoAR50gewV9tAhjZQjK6pdsWfgbof4Mk8qxNAJE/R50RMhZP2wTQ9zbJo6coR/zJOoDokySP/QiRgXjC7Z2A2NHr+85Wk5shQtwBU5rlQI83hUU1WQagA9y01ohvhczAYyKfiYelLwYP6UbjfyCJxUaAJ/EgwOyDVgHoK6YzzTw2G7+mUtSZOGxQzjn7SeL3QzzWU3FevUUA4b532/y28O88F+7CL7S6SBJckRfVaYBEl+8DTQKEtjkMvuPGiTd31onCkD+k6axV7iSs8TRJrvHpYw4geZe3lVLtztxp1C8yn6S2YypWgWNuHZXTqnyHMgeQthPkYhtVKUB6z/QdZETQ0sgZy8CPujQ1B6ADHKVBrV4VmRU0E1kVMtvu0+tlLO86lCmA7K2e4vRxx2UUkO8X15DhOPcVeen7z5It/+21FDcbjGUKQAU4ud0y+5flbPTARG9bvoFKuVJyS7Wf1mD61FLfYcwDTG7WhaDF70FesTbycT288S7I6SgUktXS6HVSVuI6w+ebAsgv0oXVZ2thxlFtrMG34M01dBbXCoG1qzp7xEaHrwLjZAZABziAd1aKT4E2njjquf6LpK8o390+pTpxYj50k8wAFOepM0h3TMlryph2XBjz9lKD0L/FS1/0uoJLCwz/GzAJ4A5ws46IbF2hMkpel8/JpfIGXAGwMce7T2alhQCbloEjpgGC314mjM2vKGdOmci2LKFy3zsie//V4TJQbcVs8pVnWwdQ/hIF/RV0/LZnka+CpE+7OJsq8FUWqqspb0yGlB0vkJXgsgCgbKnSp7+RA6ergPxxOuXPe1pVvkiSWj0hpNHjjH2q6168f4BFe0ieOSDzqlQ6d7QdAD5J8QXofReq99W5nbuf6/J8EwC7kklqSBZ8TsYUFZnc21OkT5NIapNg/Kh9Oh45ClZbAFDrVKH1M6fIdqap+39Poqp86JrzS3qangBXAniFDSg2iAb3DeBaSKImXc95zGFdObZd/wDQAOqvwd39FpClVu3/AiyfSBJIY1b1JqPpA0/13sUlskeQuq9Priepxw+fFRIcbwWA1YkBGIABGIABGIABGIABGIABGODBB+jpxAAMwAAMwAAMwAC2A9wDoeBukM0YGKoAAAAASUVORK5CYII="
	}
})";

void requestToken(ws::IController& wsController) {
	wsController.sendMessage(AUTHENTICATION_TOKEN_REQUEST);
}

static constexpr const char* PARAMETER_CREATION_REQUEST = R"({
	"apiName": "VTubeStudioPublicAPI",
	"apiVersion": "1.0",
	"requestID": "SomeID",
	"messageType": "ParameterCreationRequest",
	"data": {
		"parameterName": "%s",
		"explanation": "This is my new parameter.",
		"min": %f,
		"max": %f,
		"defaultValue": 0.0
	}
})";

void createParameter(ws::IController& wsController,
                     const ParameterData& parameter) {
	size_t nChars = mg_snprintf(buffer,
	                            sizeof(buffer),
	                            PARAMETER_CREATION_REQUEST,
	                            parameter.name.c_str(),
	                            parameter.min,
	                            parameter.max);
	wsController.sendMessage(std::string(buffer, nChars));
}

void createParameter(ws::IController& wsController,
                     const Parameter& parameter) {
	size_t nChars = mg_snprintf(buffer,
	                            sizeof(buffer),
	                            PARAMETER_CREATION_REQUEST,
	                            parameter.getName().c_str(),
	                            parameter.min,
	                            parameter.max);
	wsController.sendMessage(std::string(buffer, nChars));
}

static constexpr const char* PARAMETER_DELETION_REQUEST = R"({
	"apiName": "VTubeStudioPublicAPI",
	"apiVersion": "1.0",
	"requestID": "SomeID",
	"messageType": "ParameterDeletionRequest",
	"data": {
		"parameterName": "%s"
	}
})";

void deleteParameter(ws::IController& wsController,
                     const Parameter& parameter) {
	size_t nChars = mg_snprintf(buffer,
	                            sizeof(buffer),
	                            PARAMETER_DELETION_REQUEST,
	                            parameter.getName().c_str());
	wsController.sendMessage(std::string(buffer, nChars));
}

static constexpr const char* INPUT_PARAMETER_LIST_REQUEST = R"({
	"apiName": "VTubeStudioPublicAPI",
	"apiVersion": "1.0",
	"requestID": "SomeID",
	"messageType": "InputParameterListRequest"
})";

void getParameters(ws::IController& wsController) {
	wsController.sendMessage(std::string(INPUT_PARAMETER_LIST_REQUEST));
}

static constexpr const char* INJECT_PARAMETER_DATA_REQUEST = R"({
	"apiName": "VTubeStudioPublicAPI",
	"apiVersion": "1.0",
	"requestID": "SomeID",
	"messageType": "InjectParameterDataRequest",
	"data": {
		"faceFound": false,
		"mode": "set",
		"parameterValues": [
			{
				"id": "%s",
				"value": %f
			}
		]
	}
})";

void setParameter(ws::IController& wsController, const Parameter& parameter) {
	size_t nChars = mg_snprintf(buffer,
	                            sizeof(buffer),
	                            INJECT_PARAMETER_DATA_REQUEST,
	                            parameter.getName().c_str(),
	                            parameter.getOutput());
	wsController.sendMessage(std::string(buffer, nChars));
}

};  // namespace vts
