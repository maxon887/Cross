#include "common.h"
#include <jni.h>
#include "AudioAndroid.h"

std::vector<char *> gPathList;

void Common_Close()
{
    for (std::vector<char *>::iterator item = gPathList.begin(); item != gPathList.end(); ++item)
    {
    	free((void*)*item);
    }

    gPathList.clear();
}

const char *Common_MediaPath(const char *fileName)
{
    char *filePath = (char *)calloc(256, sizeof(char));

    strcat(filePath, "file:///android_asset/");
    strcat(filePath, fileName);
    gPathList.push_back(filePath);

    return filePath;
}

extern "C"{
	void Java_com_cross_CrossActivity_InitAudio(JNIEnv *env, jobject thiz)
	{
		AudioAndroid::Init();
	}
	void Java_com_cross_CrossActivity_ReleaseAudio(JNIEnv* env, jobject thiz){
		AudioAndroid::Release();
		Common_Close();
	}
}
 /* extern "C" */
