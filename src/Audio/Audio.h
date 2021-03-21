#ifndef AUDIO_H
#define AUDIO_H
#include <SDL2/SDL_mixer.h>

class Audio
{
public:
    Audio();
    virtual ~Audio();
    static Audio& getInstance(){static Audio instance; return instance;}
    Mix_Chunk *bassClef = NULL;
    Mix_Chunk *trebleClef = NULL;
    void Init();
    void EnableTreble();
    void DisableTreble();
    void cleanUp();
};

#endif // AUDIO_H
