#ifndef ADMIN_H
#define ADMIN_H
#include <iostream>
#include "SqlSystem.h"
#include <vector>
#include "Doctor.h"
#include "TeacherAssistant.h"
#include "Student.h"
#include <map>
class Admin
{
    public:
        map<int,int> ViewSignupRequests();
        void AnswereOnRequests();
        void ViewAllUsers();
        void ViewAdminMenu();
        void RunAdimnOption();
};

#endif // ADMIN_H
