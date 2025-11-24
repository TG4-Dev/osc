#include "app/application.hpp"
#include "app/log.hpp"

int main(int argc, char *argv[]) {

  app::Log::Init();
  platform::windowOpts opts{500, 500, "osc"};

  core::Application app(opts);
  app.Run();
  return 0;
}
