#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include "SqlSystem.h"
#include "Doctor.h"
#include "Student.h"
#include "TeacherAssistant.h"
#include <sstream>

class LibrarySystem
{
    public:

        void DisplayDoctorMenu();

        void DisplayStudentMenu();

        void DisplayTeacherAssistantMenu();

        void RunDoctorOptions(Doctor Dr);

        void RunStudentOptions(Student Stu);

        void RunTeacherAssistantOptions(TeacherAssistant TA);

        void Login();

        void Signup();

        void SendInviteForTA(Doctor Dr);

        void QueryGetMessagesData_DB(vector<Message> &ListMyMessage,vector<Message> &ListReciveMessage , int UserID,string FullName);

        void QueryGetCourseData_DB(vector<Course> &MyListCourses,int UserID,int AccountType );

        void FillAllCourses();

        void SendMessage(Person person);
};

#endif // LIBRARYSYSTEM_H
