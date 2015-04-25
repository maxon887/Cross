/*	Copyright © 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/


#include "AudioAndroid.h"

static SLObjectItf engine;
static SLObjectItf player;
const SLInterfaceID pIDs[1] = { SL_IID_ENGINE };
const SLboolean pIDsRequired[1] = { SL_BOOLEAN_TRUE };

struct WAVHeader{
    char                RIFF[4];
    unsigned long       ChunkSize;
    char                WAVE[4];
    char                fmt[4];
    unsigned long       Subchunk1Size;
    unsigned short      AudioFormat;
    unsigned short      NumOfChan;
    unsigned long       SamplesPerSec;
    unsigned long       bytesPerSec;
    unsigned short      blockAlign;
    unsigned short      bitsPerSample;
    char                Subchunk2ID[4];
    unsigned long       Subchunk2Size;
};

struct SoundBuffer{
    WAVHeader* header;
    unsigned char* buffer;
    int length;
};

SoundBuffer* createSoundBuffer(unsigned char* data, int length){
	SoundBuffer* result = new SoundBuffer();
	WAVHeader* wavHeader = reinterpret_cast<WAVHeader*>(data);
	result->header = wavHeader;
	result->buffer = data + sizeof(WAVHeader);
	result->length = length;
    return result;
}

AudioAndroid::AudioAndroid(unsigned char* data, int length, bool loop){
	SLresult res = slCreateEngine(&engine, 0, NULL, 1, pIDs, pIDsRequired);
	if(res != SL_RESULT_SUCCESS)
		throw "Error at slCreateEngine";
	res = (*engine)->Realize(engine, SL_BOOLEAN_FALSE);
	if(res != SL_RESULT_SUCCESS)
		throw "Error at Realize engine";
	SLEngineItf engineItf;
	res = (*engine)->GetInterface(engine, SL_IID_ENGINE, &engineItf);
	if(res != SL_RESULT_SUCCESS)
		throw "Error at GetInterface engine";
	SLObjectItf outputMix;
	const SLInterfaceID pOutputMixIDs[] = {};
	const SLboolean pOutputMixRequired[] = {};
	res = (*engineItf)->CreateOutputMix(engineItf, &outputMix, 0, pOutputMixIDs, pOutputMixRequired);
	if(res != SL_RESULT_SUCCESS)
		throw "Error at CreateOutpuMix engineItf";
	res = (*outputMix)->Realize(outputMix, SL_BOOLEAN_FALSE);
	if(res != SL_RESULT_SUCCESS)
		throw "Error at Realize outputMix";

	SLDataLocator_AndroidSimpleBufferQueue locatorBufferQueue = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 1 };
	SLDataFormat_PCM formatPMC;
	formatPMC.formatType = SL_DATAFORMAT_PCM;
	formatPMC.numChannels = 1;
	formatPMC.samplesPerSec = SL_SAMPLINGRATE_44_1;
	formatPMC.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
	formatPMC.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
	formatPMC.channelMask = SL_SPEAKER_FRONT_CENTER;
	formatPMC.endianness = SL_BYTEORDER_LITTLEENDIAN;
	SLDataSource audioSrc = { &locatorBufferQueue, &formatPMC };
	SLDataLocator_OutputMix locatorOutputMix = { SL_DATALOCATOR_OUTPUTMIX, outputMix };
	SLDataSink audioSnk = { &locatorOutputMix, NULL };
	const SLInterfaceID pIDs[1] = {SL_IID_BUFFERQUEUE};
	const SLboolean pIDsRequired[1] = {SL_BOOLEAN_TRUE};

	res = (*engineItf)->CreateAudioPlayer(engineItf, &player, &audioSrc, &audioSnk, 1, pIDs, pIDsRequired);
	if(res != SL_RESULT_SUCCESS)
		throw "Error at CreateAudioPlayer";
	res = (*player)->Realize(player, SL_BOOLEAN_FALSE);
	if(res != SL_RESULT_SUCCESS)
		throw "Error at Realize player";

	SLPlayItf playItf;
	res = (*player)->GetInterface(player, SL_IID_PLAY, &playItf);
	if(res != SL_RESULT_SUCCESS)
		throw "Error at GetInterface player";
	SLBufferQueueItf bufferQueueItf;
	res = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &bufferQueueItf);
	if(res != SL_RESULT_SUCCESS)
		throw "Error at GetInterface player";
	res = (*playItf)->SetPlayState(playItf, SL_PLAYSTATE_PLAYING);

	SoundBuffer* sound = createSoundBuffer(data, length);
	(*bufferQueueItf)->Clear(bufferQueueItf);
	(*bufferQueueItf)->Enqueue(bufferQueueItf, sound->buffer, sound->length);
}

void AudioAndroid::Play(){

}

void AudioAndroid::Pause(){

}

void AudioAndroid::Stop(){

}
