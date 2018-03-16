#include "MacSystme.h"

#include <sys/time.h>

using namespace cross;

void MacSystem::Log(const char *msg) {
    printf("%s", msg);
}

String MacSystem::AssetsPath() {
    return "../../../Assets/";
}

String MacSystem::DataPath() {
    return "Data/";
}

U64 MacSystem::GetTime() {
    struct timeval ptv;
    gettimeofday(&ptv, NULL);
    return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}
