#include "Screen.h"

using namespace cross;

class MainScreen : public Screen{
public:
	MainScreen(Game* game){}
	void Start();
	void Update(float sec);
};