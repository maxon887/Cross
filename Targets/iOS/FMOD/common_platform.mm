#import "common.h"
#import <Foundation/NSString.h>
#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioSession.h>

const Common_Button BTN_IDS[] = {BTN_ACTION1, BTN_UP, BTN_ACTION2, BTN_LEFT, BTN_MORE, BTN_RIGHT, BTN_ACTION3, BTN_DOWN, BTN_ACTION4};
const unsigned int BTN_COUNT = sizeof(BTN_IDS) / sizeof(BTN_IDS[0]);
const unsigned int BTN_COUNT_PER_ROW = 3;

NSMutableString *gOutputBuffer;
uint32_t gButtons;
uint32_t gButtonsDown;
uint32_t gButtonsPress;
bool gSuspendState;

void interruptionListenerCallback(void *inUserData, UInt32 interruptionState)
{
    if (interruptionState == kAudioSessionBeginInterruption)
    {
        gSuspendState = true;
    }
    else if (interruptionState == kAudioSessionEndInterruption)
    {
        UInt32 sessionCategory = kAudioSessionCategory_PlayAndRecord;
        AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
        AudioSessionSetActive(true);

        gSuspendState = false;
    }
}

void Common_Init(void **extraDriverData)
{
    gSuspendState = false;
    gOutputBuffer = [NSMutableString stringWithCapacity:(NUM_COLUMNS * NUM_ROWS)];

    AudioSessionInitialize(NULL, NULL, interruptionListenerCallback, NULL);

    // Default to 'play and record' so we have recording available for examples that use it
    UInt32 sessionCategory = kAudioSessionCategory_PlayAndRecord;
    AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
    AudioSessionSetActive(true);
}

void Common_Close()
{

}


void Common_Sleep(unsigned int ms)
{
    [NSThread sleepForTimeInterval:(ms / 1000.0f)];
}

void Common_Exit(int returnCode)
{
    exit(-1);
}

void Common_DrawText(const char *text)
{   
    [gOutputBuffer appendFormat:@"%s\n", text];
}

bool Common_BtnPress(Common_Button btn)
{
    return ((gButtonsPress & (1 << btn)) != 0);
}

bool Common_BtnDown(Common_Button btn)
{
    return ((gButtonsDown & (1 << btn)) != 0);
}

const char *Common_BtnStr(Common_Button btn)
{
    switch (btn)
    {
        case BTN_ACTION1: return "A";
        case BTN_ACTION2: return "B";
        case BTN_ACTION3: return "C";
        case BTN_ACTION4: return "D";
        case BTN_UP:      return "Up";
        case BTN_DOWN:    return "Down";
        case BTN_LEFT:    return "Left";
        case BTN_RIGHT:   return "Right";
        case BTN_MORE:    return "E";
        case BTN_QUIT:    return "Home";
    }
}

const char *Common_MediaPath(const char *fileName)
{
    return [[NSString stringWithFormat:@"%@/%s", [[NSBundle mainBundle] resourcePath], fileName] UTF8String];
}

void Common_LoadFileMemory(const char *name, void **buff, int *length)
{
    FILE *file = fopen(name, "rb");

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);

    void *mem = malloc(len);
    fread(mem, 1, len, file);

    fclose(file);

    *buff = mem;
    *length = len;
}

void Common_UnloadFileMemory(void *buff)
{
    free(buff);
}

bool Common_SuspendState()
{
    return gSuspendState;
}


