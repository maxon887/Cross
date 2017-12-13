# Cross++
Is a lightweight cross-platform game engine. Written mostly on C++.

### Demo
Demo project can be found in Demo/Projects/{TargetPlatfor} directroy.

### Features
1. Currently supported platfors Windows, iOS, Android.
2. Entity-Component system and scene managment.
3. Renderer with materials, lighting and model loading.
4. Crossplatform imput system.
5. Audio system.
6. Documented most part of code.
7. Base scene editor
8. Many usefull enterestiong features like: Event System, Assert System, Memory Manager.

### Screenshot Gallery
![Alt text](https://github.com/maxon887/Cross/blob/0d9b437a067df05e3d42a8a4b4efef771fcac51c/Release/Screenshot07.png?raw=true "Optional Title")
![Alt text](https://github.com/maxon887/Cross/blob/0d9b437a067df05e3d42a8a4b4efef771fcac51c/Release/Screenshot06.png?raw=true "Optional Title")
![Alt text](https://github.com/maxon887/Cross/blob/0d9b437a067df05e3d42a8a4b4efef771fcac51c/Release/Screenshot08.png?raw=true "Optional Title")
![Alt text](https://github.com/maxon887/Cross/blob/0d9b437a067df05e3d42a8a4b4efef771fcac51c/Release/Screenshot01.png?raw=true "Optional Title")
![Alt text](https://github.com/maxon887/Cross/blob/0d9b437a067df05e3d42a8a4b4efef771fcac51c/Release/Screenshot04.png?raw=true "Optional Title")
![Alt text](https://github.com/maxon887/Cross/blob/0d9b437a067df05e3d42a8a4b4efef771fcac51c/Release/Screenshot05.png?raw=true "Optional Title")

### 1.0 New Game Creation
In order to create new game you must do 3 things.

1. Create inherit class from Screen which will be reflection of you current game screen like "Menu", "Level 1", "Monkey Boss" etc. Every game needs at leas one screen to display. Most likely you will override virtual Screen function `void Update(float sec);` to provide game drawing or state updating mechanism or any your custom stuff.
2. Create inherit class from Game which will be reflection of you game. You must override one pure virtual function called `virtual Screen* GetStartScreen() = 0;` to lets Game know which Screen you want to play first. This class usually contains general game information. Like saves, configuration available screens etc.
3. Last thing to do is Create template function that returns you game to engine environment. This function declaration contains in Cross.h. Example of this function may looks like this: 
```
Game* CrossMain(Launcher* launcher){
	Game* superCoolGame = new YourSuperCoolGame(launcher);
	return superCoolGame;
}
```
