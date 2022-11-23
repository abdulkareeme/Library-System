#ifndef PERSON_H
#define PERSON_H
#include "Message.h"

class Person
{
    private:


        int ID;

        string Email , FullName , UserName , Password;

        int AccountType;

        vector<Message>ListMyMessages;

        vector<Message>ListRecivedMessages;

    public:

        Person(int ID , string FullName , string UserName, string Email , string Password, int AccountType , vector<Message>ListMyMessages , vector<Message>ListRecivedMessages);

        string GetFullName();

        void GetEmail();

        void GetUserName();

        int GetAccountType();

        void ViewMyMessages();

        void ViewRecivedMessages();

        void SendMessage(int SenderID, int ReciverID,string SenderNmae, string TextMessage);

        int GetUserID();

        static string QueryGetName_DB(int ID);

        //Check if there is something else
};

#endif // PERSON_H
