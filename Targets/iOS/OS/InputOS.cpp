//
//  InputOS.cpp
//  iOS
//
//  Created by mouse on 4/3/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#include "InputOS.h"

InputOS::InputOS(){
    input_state = false;
    key_state = false;
    key_key = KEY_UNDEFINED;
}

bool InputOS::HaveInput(){
    return input_state;
}

PointX InputOS::GetInput(){
    return input_point;
}
bool InputOS::HaveKey(){
    return key_state;
}
Key InputOS::GetKey(){
    return key_key;
}