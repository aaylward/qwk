#include "project_creator.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

std::string qwk::ProjectCreator::write_contents() {
  std::string contents = "cmake_minimum_required(VERSION 3.20)\n\n";
  contents += "project(" + o->project + " VERSION 0.1 LANGUAGES CXX)\n\n";

  contents += "set(CMAKE_CXX_STANDARD " + std::to_string(o->standard) + ")\n";
  contents += "set(CMAKE_CXX_STANDARD_REQUIRED True)\n\n";

  contents += "file(GLOB SOURCES \"src/*.cpp\")\n\n";
  contents += "add_compile_options(-Wall -Wextra -Werror)\n";
  contents += "add_executable(" + o->project + " ${SOURCES})\n";

  if (o->parser) {
    contents += "find_package(Boost " + o->boost_version +
                " COMPONENTS program_options REQUIRED)\n\n";
    contents +=
        "target_link_libraries(" + o->project + " Boost::program_options)\n\n";
  }

  contents += "\ninstall(TARGETS " + o->project + " DESTINATION bin)\n";

  return contents;
}

bool qwk::ProjectCreator::write_project_to_disc() {
  namespace fs = std::filesystem;

  std::string contents = this->write_contents();

  if (o->debug) {
    std::cout << contents << std::endl;
    return 0;
  }

  if (!fs::create_directories(o->project + "/src")) {
    std::cout << "failed to create project directories!" << std::endl;
    return 1;
  }
  std::ofstream c(o->project + "/CMakeLists.txt");
  c << contents;

  std::ofstream b(o->project + "/build.sh");
  b << "#/usr/bin/env bash\n\ncmake -Bbuild\ncmake --build build\n";
  b.close();

  fs::permissions(o->project + "/build.sh", fs::perms::owner_exec,
                  fs::perm_options::add);

  std::ofstream i(o->project + "/install.sh");
  i << "#/usr/bin/env bash\n\ncmake -Bbuild\ncmake --build build\ncmake "
       "--install build\n";
  i.close();

  fs::permissions(o->project + "/install.sh", fs::perms::owner_exec,
                  fs::perm_options::add);

  std::ofstream f(o->project + "/src/" + o->project + ".cpp");
  f << "#include <iostream>\n\nint main() {\n  std::cout << \"sup.\" << "
       "std::endl;\n}"
    << std::endl;

  return 0;
}
