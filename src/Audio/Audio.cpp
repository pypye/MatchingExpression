#include "Audio.h"

Audio::Audio(){}
Audio::~Audio(){}

void Audio::Init()
{
    bassClef    = Mix_LoadWAV("MatchingExpression_data\\sounds\\MatchingExpression_bass_clef.wav");
    trebleClef  = Mix_LoadWAV("MatchingExpression_data\\sounds\\MatchingExpression_treble_clef.wav");
    #define BASS 1
    #define TREBLE 2
    Mix_PlayChannel(BASS, bassClef, -1);
    Mix_PlayChannel(TREBLE, trebleClef, -1);
    Mix_Volume(TREBLE, 0);
}
void Audio::EnableTreble()
{
    Mix_Volume(TREBLE, 128);
}
void Audio::DisableTreble(){
    Mix_Volume(TREBLE, 0);
}
void Audio::cleanUp(){
    Mix_CloseAudio();
}
