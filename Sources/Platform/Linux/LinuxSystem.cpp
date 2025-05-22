#include "LinuxSystem.h"

#include <sys/time.h>

using namespace cross;

String LinuxSystem::AssetsPath() {
	return "Assets/";
}

String LinuxSystem::DataPath() {
	return "Data/";
}

U64 LinuxSystem::GetTime() {
	struct timeval ptv{};
	gettimeofday(&ptv, NULL);
	return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}
