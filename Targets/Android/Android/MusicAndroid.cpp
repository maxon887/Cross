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
	
#include "MusicAndroid.h"
#include "Game.h"

static SLObjectItf engine;
//static SLObjectItf player;
const static SLInterfaceID pIDs[1] = { SL_IID_ENGINE };
const static SLboolean pIDsRequired[1] = { SL_BOOLEAN_TRUE };

MusicAndroid::MusicAndroid(int descriptor, off_t start, off_t length){

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

	SLDataLocator_AndroidFD locatorIn = {
	    SL_DATALOCATOR_ANDROIDFD,
		descriptor,
		start,
		length
	};

	SLDataFormat_MIME dataFormat = {
	    SL_DATAFORMAT_MIME,
	    NULL,
	    SL_CONTAINERTYPE_UNSPECIFIED
	};

	SLDataSource audioSrc = {&locatorIn, &dataFormat};

	SLDataLocator_OutputMix dataLocatorOut = {
	    SL_DATALOCATOR_OUTPUTMIX,
		outputMix
	};

	SLDataSink audioSnk = {&dataLocatorOut, NULL};
	const SLInterfaceID pIDs[2] = {SL_IID_PLAY, SL_IID_SEEK};
	const SLboolean pIDsRequired[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
	SLObjectItf playerObj;
	SLresult result = (*engineItf)->CreateAudioPlayer(engineItf, &playerObj, &audioSrc, &audioSnk, 2, pIDs, pIDsRequired);
	result = (*playerObj)->Realize(playerObj, SL_BOOLEAN_FALSE);

	SLPlayItf player;
	result = (*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &player);
	SLSeekItf seek;
	result = (*playerObj)->GetInterface(playerObj, SL_IID_SEEK, &seek);
	(*seek)->SetLoop(
	    seek,
	    SL_BOOLEAN_TRUE, /*Воспроизведение зациклено*/
	    0, /*Зациклено на начало файла(0 мс)*/
	    SL_TIME_UNKNOWN /*По достижению конца*/
	);
	(*player)->SetPlayState(player, SL_PLAYSTATE_PLAYING);
}

void MusicAndroid::Play(){

}

void MusicAndroid::Pause(){

}
void MusicAndroid::Stop(){

}
