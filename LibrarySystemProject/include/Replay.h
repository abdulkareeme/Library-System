#ifndef REPLAY_H
#define REPLAY_H
#include <iostream>
#include "SqlSystem.h"
using namespace std;
class Replay
{
    private:

        int ReplayID;

        int MessageID;

        int ReplayerID;

        string ReplayerName;

        string TextMessage;

    public:

        Replay ();

        Replay(int MessageID,int ReplayerID,string ReplayerName, string TextMessage);

        Replay(int ReplayID, int MessageID,int ReplayerID,string ReplayerName, string TextMessage);

        void DisplayReplay();
};

#endif // REPLAY_H
