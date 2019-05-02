#include <gtkmm/application.h>
#include <glibmm/fileutils.h>
#include <iostream>
#include "main-application.hpp"

int main(int argc, char *argv[])
{
    auto app = MainApplication::create();

    int ret = app->run(argc, argv);
    app->joinCommandThreads();

    return ret;
}
