#include "app/application.hpp"

int main (int argc, char *argv[]) {

	platform::windowOpts opts {
		500,
		500,
		"Test"
	};
	
	core::Application app(opts);
	app.Run();
  return 0;
}
