#include "Replay.h"
using namespace std;
Replay::Replay(){}
Replay::Replay(int MessageID,int ReplayerID,string ReplayerName, string TextMessage)
{
    this->MessageID=MessageID;
    this->ReplayerID=ReplayerID;
    this->TextMessage=TextMessage;
    this->ReplayerName=ReplayerName;

    string Query="INSERT INTO repaly (\"message id\" , \"replayer id\" , \"replayer name\" , \"replayed message\" ) \
    VALUES ( "+to_string(MessageID)+" , "+to_string(ReplayerID)+" , '"+ReplayerName+"' , '"+TextMessage+"' ) ;";
    MakeDMLQuery(Query.c_str());

    sqlite3* DB;
    sqlite3_stmt * statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    Query = "SELECT max(\"replay id\") FROM replay ; ";
    int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
    if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        this->ReplayID=(int)sqlite3_column_int(statment,0);
    }
    sqlite3_close(DB);
}
Replay::Replay(int ReplayID, int MessageID,int ReplayerID,string ReplayerName, string TextMessage)
{
    this->MessageID=MessageID;
    this->ReplayerID=ReplayerID;
    this->TextMessage=TextMessage;
    this->ReplayerName=ReplayerName;
    this->ReplayID=ReplayID;
}

void Replay::DisplayReplay()
{
    cout<<"\t"<<"Repaly From "<<ReplayerName<<" : "<<TextMessage<<endl;
}

