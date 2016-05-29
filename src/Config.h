#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdexcept>
#include "vendor/inih/cpp/INIReader.h"


class Config {
public:
	Config(std::string filename);
	
	void reload();

	int window_width() const { return window_width_; }
	int window_height() const { return window_height_; }

private:
	INIReader reader_;

	int window_width_ 	= 0;
	int window_height_ 	= 0;

};


Config::Config(std::string filename): reader_(filename) {
	if (reader_.ParseError() < 0) {
        throw std::runtime_error("Couldn't load " + filename);
    }
	reload();
}


void Config::reload() {

	window_width_	= (int) reader_.GetInteger("window", "width", window_width_);
	window_height_ = (int) reader_.GetInteger("window", "height", window_height_);

}


#endif // CONFIG_H_
