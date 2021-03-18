#ifndef PLAY_H
#define PLAY_H

class Play
{
public:
    Play();
    virtual ~Play();
    static Play& getInstance(){static Play instance;return instance;}
    bool pressedDown = false;

    void spawnTile();
    void relocateTile(int x, int y);
    void findLowestAndGo();
    bool checkPoint();
    void gravityFall();
    void addNumberToBlock();
};

#endif // PLAY_H
