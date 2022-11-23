#ifndef DOCTOR_H
#define DOCTOR_H
#include "Teacher.h"
#include "SqlSystem.h"
#include <map>
#include <set>
class Doctor : public Teacher
{
    public:

        Doctor(int ID , string FullName , string UserName, string Email , string Password, int AccountType
                , vector<Message>ListMyMessages , vector<Message>ListRecivedMessages
                , vector<Course>MyListCourses , bool Confirmed);

        void CreateCourse();

        void QuarySendInviteForTA_DB(int CourseIndex);

        void DeleteCourse();

        void QueryDeleteCourse_DB(int CourseIndex);
};

#endif // DOCTOR_H
