//
//  TouchCounter.hpp
//  Cross++
//
//  Created by User on 11/15/16.
//  Copyright © 2016 Vladas Boga4. All rights reserved.
//

#ifndef TouchCounter_hpp
#define TouchCounter_hpp


class TouchTrack
{
public:
    
    TouchTrack()
    {
        m_touchPointer = nullptr;
    }
    
    void *m_touchPointer;
};

extern const int MAX_TOUCHES = 11;
extern TouchTrack g_touchTracker[MAX_TOUCHES];

int GetFingerTrackIDByTouch(void* touch);

int AddNewTouch(void* touch);

int GetTouchesActive();

#endif /* TouchCounter_hpp */
