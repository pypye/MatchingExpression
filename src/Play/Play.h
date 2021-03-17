#ifndef PLAY_H
#define PLAY_H

class Play
{
    static Play *_instance;
public:
    Play();
    virtual ~Play();
    static Play *getInstance(){return _instance = (_instance != nullptr) ? _instance : new Play();}

    void spawnTile();
    void relocateTile(int x, int y);
    bool checkPoint();
    void gravityFall();
    void addNumberToBlock();
};

#endif // PLAY_H
