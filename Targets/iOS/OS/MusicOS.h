//
//  MusicOS.h
//  iOS
//
//  Created by mouse on 4/25/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#pragma once

#include "Music.h"

class MusicOS : public Music{
public:
    MusicOS(const char* filename);
    void Play();
    void Stop();
    void Pause();
};