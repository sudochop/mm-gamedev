#include "Config.h"
#include "Engine.h"

int main(int argc, char* argv[]) {

	Engine engine(Config("config.ini"));

	engine.run();

	return 0;

}
