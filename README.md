# Cross++
Is a lightweight cross-platform game engine. Written mostly on C++. Biggest advantage of this framework that is not require high skill qualification to work with. You can write you scripts on C++ in favorite VisualStudio or Xcode. Only needed to transfer *.h files and binary libraries in to new project. Currently supported platforms windows, android, iOS.

### Source Example
Example which shows how to use fundamental engine features located in Games/Demo/ directory. VisualStudio project and other located in Games/Demo/Platform/Windows/ directories.

### Features
1. Text, Sprites drawing. Sprite animation. UI.
2. 
Point Lights !(/Release/Screenshot06.png)
Directional Lights !(/Release/Screenshot07.png)
Spot Lights !(/Release/Screenshot08.png)
3. Model loading. Materials systmem. Basic scene managment. 
!(/Release/Screenshot01.png)
Diffuse + Specular + Shininess maps support
!(/Release/Screenshot04.png)
Normal map support
!(/Release/Screenshot05.png)

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
