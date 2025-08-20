#include "core/app.hpp"

int main(int, char*[]) {
	core::App app;
	if (app.init() != 0) {
		return 1;
	}
	app.run();
	return 0;
}
