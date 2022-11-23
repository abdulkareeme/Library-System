#ifndef MESSAGE_H
#define MESSAGE_H

#include<Replay.h>
#include <iostream>
#include <vector>
using namespace std;
class Message
{
    private:

        int MessageID;

        int SenderID,ReciverID;

        string SenderNmae,ReciverName;

        string TextMessage;

        bool PublicMessage; // for posts in courses PublicMessage = 1

        vector<Replay>ListReplays;

    public:

        Message(int SenderID, int ReciverID,string SenderNmae, string ReciverName, string TextMessage, bool PublicMessage);

        Message(int MessageID, int SenderID, int ReciverID,string SenderNmae, string ReciverName, string TextMessage, bool PublicMessage,vector<Replay>ListReplays);

        void AddReplay(int ReplayerID, string ReplayerName,string TextMessage);

        void DisplayMessageFrom();

        void DisplayMessageTo();

        int GetID();
};

#endif // MESSAGE_H
