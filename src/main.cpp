#include "app.hpp"

int main(int, char*[]) {
	App app;
	if (app.init() != 0) {
		return 1;
	}
	app.run();
	return 0;
}
