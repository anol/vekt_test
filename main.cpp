
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <iostream>

using namespace std;

#include "My_window.h"

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    My_window helloworld;
    helloworld.initialize();

    //Shows the window and returns when it is closed.
    return app->run(helloworld);
}
