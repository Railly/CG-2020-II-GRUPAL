#include <iostream>
#include "AppAR.hpp"

AppAR app;

int main() {

    app.init();
    app.setup();

    while (!app.shouldClose()) {
        app.display();
    }

    app.cleanAll();
}