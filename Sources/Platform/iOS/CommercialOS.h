//
//  CommercialOS.h
//  Jim the Snake
//
//  Created by mouse on 8/6/15.
//  Copyright (c) 2015 Lukyanau Maksim. All rights reserved.
//

#pragma once

#include "Commercial.h"

using namespace cross;

class CommercialOS : public Commercial{
public:
    CommercialOS();
    void DownloadAd();
    void ShowAd();
    void Purchase();
    void Restore();
};