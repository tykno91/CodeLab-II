#pragma once

#include "json.hpp"
#include "ofMain.h"

using json = nlohmann::json;

enum AppState {
	STATE_IDLE,
	STATE_LOADING,
	STATE_SUCCESS,
	STATE_ERROR
};

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;

	void urlResponse(ofHttpResponse & response);
	void fetchWeatherData(const std::string & location);

	AppState currentState;
	std::string apiKey = "2a5b924d48eb43a2a49115552262410";
	std::string searchInput = "London";
	std::string statusMessage = "";

	// Weather Data
	std::string cityName;
	std::string countryName;
	std::string conditionText;
	float temperatureC = 0.0f;
	float feelsLikeC = 0.0f;
	int humidity = 0;
	float windKph = 0.0f;
	std::string lastUpdated;
};
