#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdexcept>
#include "vendor/inih/cpp/INIReader.h"


class Config {
private:
	INIReader reader;

public:
	Config(std::string filename);
	~Config();

	void reload();

	int windowWidth 	= 0;
	int windowHeight 	= 0;
};


Config::Config(std::string filename): reader(filename) {
	if (reader.ParseError() < 0) {
        throw std::runtime_error("Couldn't load " + filename);
    }
	this->reload();
}


Config::~Config() {

}


void Config::reload() {

	this->windowWidth 	= (int) this->reader.GetInteger("window", "width", this->windowWidth);
	this->windowHeight 	= (int) this->reader.GetInteger("window", "height", this->windowHeight);

}


#endif // CONFIG_H_
