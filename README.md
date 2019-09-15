# Cross++
Is a lightweight cross-platform game engine. Written mostly on C++. Main purpose of this engine to be easy to learn, read and understand.

### Build Status


| Windows            | iOS                  | Android            |
| ------------------ | -------------------- | ------------------ |
| [![Win][WB1]][WB2] | [![iOS][IOS1]][TRAV] |[![AND][AND1]][TRAV] |

[WB1]: https://ci.appveyor.com/api/projects/status/fmigb2qi7di0pqmq?svg=true
[WB2]: https://ci.appveyor.com/project/maxon887/cross
[IOS1]: https://travis-matrix-badges.herokuapp.com/repos/maxon887/Cross/branches/Main/1
[TRAV]: https://travis-ci.org/maxon887/Cross
[AND1]: https://travis-matrix-badges.herokuapp.com/repos/maxon887/Cross/branches/Main/2


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

### Screenshot Gallery
![Alt text](https://lh3.googleusercontent.com/BJFSvDKpm95iwEzEVnDBH3DsPA9TqqW0ogBlizwDwFtDTH3AATQy_7FC_esUYoqobu_yJl09GNjrsuS3DXR0GgEQr0oLUaEFF3M1bN_mcC_R0WQPpm6jlnjHVIhINZnxVxgZfQw-fG4UD4YutkxJrpc-KPYMcZJZHme2YaryJSrRFdPV1xwsbXiJ7m-dUvS-Hle7z6TWK_cMhb46zF8KjeGRgvJNFN2qPbMW5Fd5SmtDNNcP-src2qqPo9fK4ybVYsklr24ncflcagutl_XuCmBTzur0_85iYTqox_fxlzBBPV20uzkLEk95LLXOfIf2YlqRzR38VPIpBueuJqcdpFdchrzw4ry1mwgfa5UsAz7wZ1aprpRUSBgcpWeAsE4_kn-z3NoBpapzJ5jTT9zfMCPsounfzFV_fHULrzTzecDs8RX58E-7di4Qimv3peVVAyEKnxhvwCYrzLnrBxrKzawYsUvkZxPymonpen4B3BDBOwkr44Q2xz3yrGK3R6FuOVBR-jIymoTqUA8_HHFJ3a4_OGoc5JsBFI7uW6VWQeM_rpAt2PuxNzRZXdQCwk_56O6apB1a4Ij0tGiygl2ZCBDBLPwmovd3MMP89Nu8vDGuMtGXc0XJ4-uq8i6acn4uHHWrttVVqfFKl5GGsbDCIChkFK0rgJ9Bc6TSByB_JRfEhARfJb8HNCc=w1345-h785-no?raw=true "Optional Title")
![Alt text](https://github.com/maxon887/Cross/blob/0d9b437a067df05e3d42a8a4b4efef771fcac51c/Release/Screenshot06.png?raw=true "Optional Title")
![Alt text](https://github.com/maxon887/Cross/blob/0d9b437a067df05e3d42a8a4b4efef771fcac51c/Release/Screenshot08.png?raw=true "Optional Title")
![Alt text](https://github.com/maxon887/Cross/blob/0d9b437a067df05e3d42a8a4b4efef771fcac51c/Release/Screenshot01.png?raw=true "Optional Title")

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
