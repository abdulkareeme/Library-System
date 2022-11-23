#include "Message.h"

Message::Message(int SenderID, int ReciverID,string SenderNmae, string ReciverName, string TextMessage, bool PublicMessage)
{
    this->SenderID=SenderID;
    this->ReciverID=ReciverID;
    this->TextMessage=TextMessage;
    this->PublicMessage=PublicMessage;
    this->SenderNmae=SenderNmae;
    this->ReciverName=ReciverName;

    string Query="INSERT INTO message (\"user id\" , \"reciver id\" , \"reciver name\" , \"message type\" , \"text message\" )\
    VALUES ( "+to_string(SenderID)+" , "+to_string(ReciverID)+" , '"+ReciverName+"' , "+to_string(PublicMessage)+" , '"+TextMessage+"' ) ; " ;
    MakeDMLQuery(Query.c_str());

    sqlite3* DB;
    sqlite3_stmt * statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    Query = "SELECT max(\"message id\") FROM message ; ";
    int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
    if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        this->MessageID=(int)sqlite3_column_int(statment,0);
    }
    sqlite3_close(DB);
}
Message::Message(int MessageID,int SenderID, int ReciverID,string SenderNmae, string ReciverName, string TextMessage, bool PublicMessage,vector<Replay>ListReplays)
{
    this->SenderID=SenderID;
    this->ReciverID=ReciverID;
    this->TextMessage=TextMessage;
    this->PublicMessage=PublicMessage;
    this->ListReplays=ListReplays;
    this->SenderNmae=SenderNmae;
    this->ReciverName=ReciverName;
    this->MessageID=MessageID;
}
void Message::AddReplay(int ReplayerID, string ReplayerName,string TextMessage)
{
    Replay replay = Replay(MessageID,ReplayerID,ReplayerName,TextMessage);
    ListReplays.push_back(replay);
}
void Message::DisplayMessageFrom()
{
    cout<<"------------------------------------------------------------------------------\n";
    cout<<" From "<<SenderNmae<<" : "<<TextMessage<<"\n";//remember to print message or post
    for(auto replay:ListReplays)
    {
        replay.DisplayReplay();
    }
    cout<<"------------------------------------------------------------------------------\n";
}
void Message::DisplayMessageTo()
{
    cout<<"------------------------------------------------------------------------------\n";
    cout<<"Message To "<<ReciverName<<" : "<<TextMessage<<"\n";
    for(auto replay:ListReplays)
    {
        replay.DisplayReplay();
    }
    cout<<"------------------------------------------------------------------------------\n";
}

int Message::GetID()
{
    return MessageID;
}
