//
//  TouchCounter.cpp
//  Cross++
//
//  Created by User on 11/15/16.
//  Copyright © 2016 Vladas Boga4. All rights reserved.
//

#include "TouchCounter.hpp"

TouchTrack g_touchTracker[MAX_TOUCHES];

int GetFingerTrackIDByTouch(void* touch)
{
    for (int i=0; i < MAX_TOUCHES; i++)
    {
        if (g_touchTracker[i].m_touchPointer == touch)
        {
            return i;
        }
    }
    
    //LogMsg("Can't locate fingerID by touch %d", touch);
    return -1;
}

int AddNewTouch(void* touch)
{
    for (int i = 0; i < MAX_TOUCHES; i++)
    {
        if (!g_touchTracker[i].m_touchPointer)
        {
            //hey, an empty slot, yay
            g_touchTracker[i].m_touchPointer = touch;
            return i;
        }
    }
    
    return -1;
}

int GetTouchesActive()
{
    int count = 0;
    
    for (int i = 0; i < MAX_TOUCHES; i++)
    {
        if (g_touchTracker[i].m_touchPointer)
        {
            count++;
        }
    }
    return count;
}
