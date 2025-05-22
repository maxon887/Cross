#include "LinuxSystem.h"

#include <iostream>

using namespace std;
using namespace cross;

int main() {
    os = CREATE LinuxSystem();

    os->LogIt("Hello from Cross");

    delete os;
    return 0;
}