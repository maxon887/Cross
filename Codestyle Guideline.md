# Codestyle Guideline
### 1. include files
Header files can be include in other header files only if they extremely necessary there. In any other cases you must include files in source module and use [forward declaration](https://en.wikipedia.org/wiki/Forward_declaration). Good list of situations in which you can and can't do forward declaration can be found [there](http://stackoverflow.com/questions/553682/when-can-i-use-a-forward-declaration). Good practice is to keep include files in next order:

1. User files.
2. Linked libraries files.
3. Standard language and system files.

Example:
```
#include "Audio.h"
#include "Exception.h"
#include "Launcher.h"

#include "FMOD/fmod.hpp"
#include "FMOD/fmod_errors.h"

#include <string>
#include <vector>
```
Exception of this rule is file **Cross.h** you can add this file in your *.h files without any restrictions.

### 2. Braces and Names
We recommends use our name convention to keep project in one style. This provide any other programmers easy way to read and write code. 

We stick the idea that no need to waste vertical monitor space. Because of that we write braces in the same line of function declaration or if statement. 
```
bool Vector2D::operator==(const Vector2D &v2) const{
	if(this->x == v2.x && this->y && v2.y) {
		return true;
	} else {
		return false;
	}
}
```
And even if you don\'t plan to add another statement to your if case is better to places braces because why knows maybe somebody else will want to add some code in your :

Classes must be name with uppercase character in a word like "Game", "MonkeyBoss". Defines and enumeration entries fully uppercased separated by underscore "MAX_OBJECTS", "COLLISION_TRIGGER_LEVEL". Enum example:
```
enum Screen{
	ANIMATION,
	AUDIO,
	PRIMITIVES,
	MISC,
	NO_SCREEN
};
```
We use old C convention for naming class and global variables:
```
    extern mutex global_mutex;
class Sexy{
    Button* start_animation_btn;
};
```

Function named in the same way as classes no matter public, private, static, global this function is. We recommend to use Java "Get", "Set" conversion for class **properties** instead of public class field:
```
float GetWidth();
```

### 3. Class declarations
Best way to keep all classes easy readable is to place all class members in fixed order. By the field of view we declare first **public** then **protected** then **private** fields. By the logical meaning we declare fields in next order:

1. Static variables
2. Static functions
3. Pure virtual functions
4. Class members
5. Constructors
6. Destructor
7. Virtual functions
8. Functions

Example:
```
class Game{
public:
	virtual Screen* GetStartScreen() = 0;
	
	Game(Launcher* launcher)
	Game(Launcher* launcher, float width);
	Game(Launcher* launcher, float width, float height);
	~Game();
	
	virtual void Start();
	virtual void Suspend();
	virtual void Resume();
	
	float GetScaleFactor();
	float GetWidth();
	float GetHeight();
	void SetScreen(Screen* screen);
	Screen* GetCurrentScreen();
    void Exit();
protected:
	float width;
	float height;
	float scale_factor;
};
```

Function definition must be written in the same way.
