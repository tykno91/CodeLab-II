#include "ofApp.h"

void ofApp::setup() {
	ofSetWindowTitle("Weather Forecast Application");
	ofSetWindowShape(1024, 768);
	ofBackground(24, 28, 36);

	ofRegisterURLNotification(this);

	currentState = STATE_LOADING;
	fetchWeatherData(searchInput);
}

void ofApp::update() {
}

void ofApp::draw() {
	// 1. Header Section
	ofPushMatrix();
	ofTranslate(50, 45);
	ofScale(1.8, 1.8);
	ofSetColor(100, 180, 255);
	ofDrawBitmapString("WEATHER FORECAST SYSTEM", 0, 0);
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(50, 80);
	ofScale(1.2, 1.2);
	ofSetColor(180, 190, 205);
	ofDrawBitmapString("Type a city or country and press ENTER to search:", 0, 0);
	ofPopMatrix();

	// 2. Search Input Box
	int boxX = 50;
	int boxY = 100;
	int boxW = 550;
	int boxH = 50;

	ofSetColor(35, 42, 54);
	ofDrawRectRounded(boxX, boxY, boxW, boxH, 8);

	ofSetColor(80, 140, 230);
	ofSetLineWidth(2.0f);
	ofNoFill();
	ofDrawRectRounded(boxX, boxY, boxW, boxH, 8);
	ofFill();

	// Input text inside box (scaled up)
	ofPushMatrix();
	ofTranslate(boxX + 16, boxY + 31);
	ofScale(1.5, 1.5);
	ofSetColor(255);
	std::string cursor = ((ofGetFrameNum() / 25) % 2 == 0) ? "_" : " ";
	ofDrawBitmapString(searchInput + cursor, 0, 0);
	ofPopMatrix();

	// 3. Status or Main Weather Card
	int cardY = 180;

	if (currentState == STATE_LOADING) {
		ofPushMatrix();
		ofTranslate(50, cardY + 40);
		ofScale(1.4, 1.4);
		ofSetColor(255, 204, 0);
		ofDrawBitmapString("Fetching live data from WeatherAPI...", 0, 0);
		ofPopMatrix();
	} else if (currentState == STATE_ERROR) {
		ofSetColor(50, 30, 35);
		ofDrawRectRounded(50, cardY, 600, 120, 10);

		ofPushMatrix();
		ofTranslate(75, cardY + 45);
		ofScale(1.3, 1.3);
		ofSetColor(255, 90, 90);
		ofDrawBitmapString("Error: " + statusMessage, 0, 0);
		ofSetColor(200);
		ofDrawBitmapString("Check location spelling or connection.", 0, 25);
		ofPopMatrix();
	} else if (currentState == STATE_SUCCESS) {
		// Main Card
		ofSetColor(35, 42, 54);
		ofDrawRectRounded(50, cardY, 680, 390, 14);

		// Location Title
		ofPushMatrix();
		ofTranslate(80, cardY + 55);
		ofScale(2.0, 2.0);
		ofSetColor(120, 205, 255);
		ofDrawBitmapString(cityName + ", " + countryName, 0, 0);
		ofPopMatrix();

		// Condition
		ofPushMatrix();
		ofTranslate(80, cardY + 95);
		ofScale(1.3, 1.3);
		ofSetColor(220, 225, 235);
		ofDrawBitmapString("Condition: " + conditionText, 0, 0);
		ofPopMatrix();

		// Big Temperature Display
		ofPushMatrix();
		ofTranslate(80, cardY + 175);
		ofScale(3.2, 3.2);
		ofSetColor(255);
		ofDrawBitmapString(ofToString(temperatureC, 1) + " C", 0, 0);
		ofPopMatrix();

		// Additional Metrics
		ofPushMatrix();
		ofTranslate(80, cardY + 230);
		ofScale(1.35, 1.35);
		ofSetColor(200, 210, 220);
		ofDrawBitmapString("Feels Like:  " + ofToString(feelsLikeC, 1) + " C", 0, 0);
		ofDrawBitmapString("Humidity:    " + ofToString(humidity) + "%", 0, 28);
		ofDrawBitmapString("Wind Speed:  " + ofToString(windKph, 1) + " km/h", 0, 56);
		ofPopMatrix();

		// Timestamp
		ofPushMatrix();
		ofTranslate(80, cardY + 355);
		ofScale(1.1, 1.1);
		ofSetColor(130, 140, 155);
		ofDrawBitmapString("Last Updated: " + lastUpdated, 0, 0);
		ofPopMatrix();
	}

	// 4. Footer
	ofPushMatrix();
	ofTranslate(50, ofGetHeight() - 35);
	ofScale(1.1, 1.1);
	ofSetColor(110, 120, 135);
	ofDrawBitmapString("Powered by WeatherAPI | Backspace to edit | Enter to submit", 0, 0);
	ofPopMatrix();
}

void ofApp::keyPressed(int key) {
	if (key == OF_KEY_RETURN) {
		if (!searchInput.empty()) {
			currentState = STATE_LOADING;
			fetchWeatherData(searchInput);
		}
	} else if (key == OF_KEY_BACKSPACE) {
		if (!searchInput.empty()) {
			searchInput.pop_back();
		}
	} else if (key >= 32 && key <= 126) {
		searchInput += static_cast<char>(key);
	}
}

void ofApp::fetchWeatherData(const std::string & location) {
	std::string encodedLocation = location;
	ofStringReplace(encodedLocation, " ", "%20");

	std::string url = "http://api.weatherapi.com/v1/current.json?key=" + apiKey + "&q=" + encodedLocation;
	ofLoadURLAsync(url, "weatherQuery");
}

void ofApp::urlResponse(ofHttpResponse & response) {
	if (response.request.name != "weatherQuery") {
		return;
	}

	if (response.status != 200) {
		currentState = STATE_ERROR;
		try {
			auto jsonError = json::parse(response.data.getText());
			if (jsonError.contains("error") && jsonError["error"].contains("message")) {
				statusMessage = jsonError["error"]["message"].get<std::string>();
			} else {
				statusMessage = "HTTP " + ofToString(response.status) + " - Request failed.";
			}
		} catch (...) {
			statusMessage = "Unable to connect or location not found.";
		}
		return;
	}

	try {
		json data = json::parse(response.data.getText());

		cityName = data["location"]["name"].get<std::string>();
		countryName = data["location"]["country"].get<std::string>();
		temperatureC = data["current"]["temp_c"].get<float>();
		feelsLikeC = data["current"]["feelslike_c"].get<float>();
		conditionText = data["current"]["condition"]["text"].get<std::string>();
		humidity = data["current"]["humidity"].get<int>();
		windKph = data["current"]["wind_kph"].get<float>();
		lastUpdated = data["current"]["last_updated"].get<std::string>();

		currentState = STATE_SUCCESS;
	} catch (const std::exception & e) {
		currentState = STATE_ERROR;
		statusMessage = "Failed to parse API data.";
	}
}
