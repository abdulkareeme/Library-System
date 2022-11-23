#ifndef TEACHERASSISTANT_H
#define TEACHERASSISTANT_H
#include "Teacher.h"

class TeacherAssistant : public Teacher
{
    private:

        vector<Course> Invitations;

    public:

        TeacherAssistant(int ID , string FullName , string UserName, string Email , string Password, int AccountType
                , vector<Message>ListMyMessages , vector<Message>ListRecivedMessages
                , vector<Course>MyListCourses , bool Confirmed , vector<Course>Invitations );

        void ViewInvitations();

        void AnswereInvitation();

        void QuaryAcceptionOnInvitstion_DB(int Acception, int Index);
};

#endif // TEACHERASSISTANT_H
