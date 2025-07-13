# Cross++
Is a lightweight cross-platform game engine. Written mostly on C++. Main purpose of this engine to be easy to learn, read and understand.

Questions can be asked via [facebook](https://www.facebook.com/profile.php?id=100001651879786)
[instagram](https://www.instagram.com/maxon8871/)

### Build Status

[![Windows](https://github.com/maxon887/Cross/actions/workflows/BuildWindows.yml/badge.svg)](https://github.com/maxon887/Cross/actions/workflows/BuildWindows.yml)
[![MacOS](https://github.com/maxon887/Cross/actions/workflows/BuildMacOS.yml/badge.svg)](https://github.com/maxon887/Cross/actions/workflows/BuildMacOS.yml)
[![Linux](https://github.com/maxon887/Cross/actions/workflows/BuildLinux.yml/badge.svg)](https://github.com/maxon887/Cross/actions/workflows/BuildLinux.yml)
[![Android](https://github.com/maxon887/Cross/actions/workflows/BuildAndroid.yml/badge.svg)](https://github.com/maxon887/Cross/actions/workflows/BuildAndroid.yml)
[![iOS](https://github.com/maxon887/Cross/actions/workflows/BuildIOS.yml/badge.svg)](https://github.com/maxon887/Cross/actions/workflows/BuildIOS.yml)


### Demo
Demo project can be found in Demo/Projects/{TargetPlatfor} directory.

### Features
1. Currently supported platforms Windows, iOS, Android.
2. Entity-Component system and scene management.
3. Renderer with materials, lighting and model loading.
4. Lightweight and fast. Distributable binaries and assets weight around 1.5mb.
5. Crossp-platform input system.
6. Audio system.
7. Documented most part of code.
8. Base scene editor
9. Many useful interesting features like: Event System, Assert System, Memory Manager.

### Tutorials:
1. Introduction and Windows Setup: https://www.youtube.com/watch?v=tnp09QB5sak
2. MacOS and iOS Setup: https://www.youtube.com/watch?v=NiSvKbGyHXU&t=2s

### Screenshot Gallery
![Alt text](https://sun9-26.userapi.com/c200820/v200820675/27ebb/RgV5CjNc_7s.jpg)
![Alt text](https://sun9-62.userapi.com/c854220/v854220397/16e46b/XNA1LrNMHXg.jpg)
![Alt text](https://sun1-15.userapi.com/c854220/v854220397/16e475/lSKKtcGWBFw.jpg)
![Alt text](https://sun9-48.userapi.com/c854220/v854220397/16e47f/auU-Vc_2Lt4.jpg)
![Alt text](https://sun9-3.userapi.com/c854220/v854220397/16e489/kmXUu0yMCv4.jpg)

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
