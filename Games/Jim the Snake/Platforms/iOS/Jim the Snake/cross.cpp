//
//  cross.cpp
//  Jim the Snake
//
//  Created by mouse on 7/9/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#include "cross.h"
#include "JimTheSnake.h"

using namespace cross;

Game* CrossMain(Launcher* launcher){
    return new JimTheSnake(launcher);
}