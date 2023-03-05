#include "nucmd.hpp"

using namespace nucmd;

MenuItem::MenuItem(std::string name, std::string description, std::string command, int fkey)
    : name{name},
      description{description},
      command{command},
      fkey{fkey}
{
}
