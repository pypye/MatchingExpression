#include "Audio.h"
#include "Menu.h"
#include <fstream>
#include <string>
Audio::Audio(){}
Audio::~Audio(){}
#define BASS 1
#define TREBLE 2
void Audio::Init()
{
    bassClef    = Mix_LoadWAV("MatchingExpression_data\\sounds\\MatchingExpression_bass_clef.wav");
    trebleClef  = Mix_LoadWAV("MatchingExpression_data\\sounds\\MatchingExpression_treble_clef.wav");

    Mix_PlayChannel(BASS, bassClef, -1);
    Mix_PlayChannel(TREBLE, trebleClef, -1);
    Mix_Volume(TREBLE, 0);
    Mix_Volume(BASS, 0);
}
int Audio::loadAudio(){
    std::ifstream fi;
    fi.open("MatchingExpression_data\\config.cfg");
    std::string s;
    fi >> s;
    fi.close();
    return s.back() - '0';
}
void Audio::EnableTreble()
{
    Mix_Volume(TREBLE, 128);
}
void Audio::DisableTreble(){
    Mix_Volume(TREBLE, 0);
}
void Audio::EnableBass()
{
    Mix_Volume(BASS, 128);
}
void Audio::DisableAll()
{
    Mix_Volume(TREBLE, 0);
    Mix_Volume(BASS, 0);
}
void Audio::saveAudioStatus(){
    std::ofstream fo;
    fo.open("MatchingExpression_data\\config.cfg");
    std::string s = "Audio=" + std::to_string(Menu::getInstance().musicEnabled);
    fo << s;
    fo.close();
}
void Audio::cleanUp(){
    Mix_CloseAudio();
}
