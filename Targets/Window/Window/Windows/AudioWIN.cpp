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
	
#include "AudioWIN.h"

static int output_cb(	const void * input, void * output, 
						unsigned long frames_per_buffer,
						const PaStreamCallbackTimeInfo *time_info,
						PaStreamCallbackFlags flags, void * data){
	AudioWIN* sound = (AudioWIN*)data;
	//SNDFILE* file = (SNDFILE*)data;

    /* this should not actually be done inside of the stream callback
     * but in an own working thread 
     *
     * Note although I haven't tested it for stereo I think you have
     * to multiply frames_per_buffer with the channel count i.e. 2 for
     * stereo */
    //sf_read_short(file, (short*)output, frames_per_buffer);
	int count = sf_readf_float(sound->file, (float*)output, frames_per_buffer);
	if(count == 0){
		if(sound->loop){
			sf_seek(sound->file, 0, 0);
		}else{
			return paComplete;
		}
	}
    return paContinue;
}

static void end_cb(void * data){
	//SoundWIN* sound = (SoundWIN*)data;
	//PaError err = Pa_CloseStream(sound->stream);
	//sound->CheckPaError(err);
}

AudioWIN::AudioWIN(Launcher* launcher, string filename, bool loop){
	this->launcher = launcher;
	//strcpy(source, filename);
	source = filename;
	this->loop = loop;
	PaError err = Pa_Initialize();
	CheckPaError(err);
	PaStreamParameters outParams;
	SF_INFO sfinfo;
	file = sf_open(filename.c_str(), SFM_READ, &sfinfo);

	outParams.device = Pa_GetDefaultOutputDevice();
	outParams.channelCount = sfinfo.channels;
	outParams.sampleFormat = paFloat32;
	outParams.suggestedLatency = Pa_GetDeviceInfo(outParams.device)->defaultLowOutputLatency;
	outParams.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(&stream, NULL, &outParams, sfinfo.samplerate,
            paFramesPerBufferUnspecified, paClipOff,
            output_cb, this);
    CheckPaError(err);

	err = Pa_SetStreamFinishedCallback(stream, &end_cb);
    CheckPaError(err);
}

void AudioWIN::Play(){
	PaError err;
	err = Pa_StartStream(stream);
	CheckPaError(err);

}

void AudioWIN::Stop(){
	PaError err = Pa_StopStream(stream);
	CheckPaError(err);
	sf_seek(file, 0, 0);
}

void AudioWIN::Pause(){
	PaError err = Pa_StopStream(stream);
	CheckPaError(err);
}

void AudioWIN::CheckPaError(PaError err){
	if(err != paNoError){
		//sprintf(str_buf, "PortAudio error: %s", Pa_GetErrorText(err));
		string msg = "PortAudio error: " + string(Pa_GetErrorText(err));
		throw msg;
	}
}

AudioWIN::~AudioWIN(){
	PaError err = Pa_CloseStream(stream);
	CheckPaError(err);
	err = Pa_Terminate();
	CheckPaError(err);
}

