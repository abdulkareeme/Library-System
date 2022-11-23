#include "TeacherAssistant.h"

TeacherAssistant::TeacherAssistant(int ID, string FullName, string UserName, string Email, string Password, int AccountType
                                   , vector<Message>ListMyMessages, vector<Message>ListRecivedMessages
                                   , vector<Course>MyListCourses, bool Confirmed, vector<Course>Invitations )
    : Teacher (ID, FullName, UserName, Email, Password, AccountType, ListMyMessages
               , ListRecivedMessages, MyListCourses, Confirmed)
{
    this->Invitations=Invitations;
}

void TeacherAssistant::ViewInvitations()
{
    cout<<"------------------------------------------------------------------------------\n";
    cout<<"Total Invitations : "<<TeacherAssistant::Invitations.size()<<"\n";
    int Index=1;
    for(auto inv : TeacherAssistant::Invitations)
    {
        cout<<Index<<" - Invotation from Dr."<<inv.GetCourseOwner()<<"\n";
        inv.DisplayCourseInfo();
        cout<<"\n";
        Index++;
    }
    cout<<"------------------------------------------------------------------------------\n";
}

void TeacherAssistant::AnswereInvitation()
{

    while(true)
    {
        TeacherAssistant::ViewInvitations();
        cout<<"\n1 - Answere on Invitation \n2 - Back \nInput Number : ";
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois>1)
        {
            if(!cin.fail() && Chois==2)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"Enter Invitation Number : ";
        int Index;
        cin>>Index;
        if(cin.fail() || Index<1 || Index >(int) TeacherAssistant::Invitations.size())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }

        cout<<"\n1 - Accept \n2 - Reject\n3 - Back\nInput Numper : ";
        int Acception;
        cin>>Acception;
        if(cin.fail() || Acception<1 || Acception>3)
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Acception==3)continue;

        TeacherAssistant::QuaryAcceptionOnInvitstion_DB( Acception , Index );

        auto it=TeacherAssistant::Invitations.begin();
        TeacherAssistant::Invitations.erase(it+(Index-1));
        cout<<"Process Complete :)\n";
    }
}

void TeacherAssistant::QuaryAcceptionOnInvitstion_DB(int Acception, int Index)
{
    if(Acception==1)
    {
        string Query="UPDATE \"requests and enrolled\" SET \"account type\" = 3 WHERE \"user id\" = "+to_string(GetUserID())
                     +" AND \"course id\" = "+to_string(TeacherAssistant::Invitations[Index-1].GetCourseID())
                     +" AND \"account type\" = 5 ;";

        MakeDMLQuery(Query.c_str());
        TeacherAssistant::Invitations[Index-1].AddTA(GetUserID());
        Teacher::AddToMyListCources(TeacherAssistant::Invitations[Index-1]);

    }
    else
    {
        string Query="DELETE FROM \"requests and enrolled\" WHERE \"user id\" = "+to_string(GetUserID())
                     +" AND \"course id\" = "+to_string(TeacherAssistant::Invitations[Index-1].GetCourseID())
                     +" AND \"account type\" = 5 ;";

        MakeDMLQuery(Query.c_str());
    }
}
