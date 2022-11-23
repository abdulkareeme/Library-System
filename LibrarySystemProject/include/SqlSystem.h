#ifndef SQLSYSTEM_H
#define SQLSYSTEM_H

#include <vector>
#include <string>
#include "sqlite3.h"
#include <regex>
#define MakeDQLQuery sqlite3_prepare_v2
#define choose(DB,statment,Query) DB,Query,-1,statment,NULL
using namespace std;

void MakeDMLQuery(string Query);
void UpdateObjactOnDB(string TableName , string ColumnNmae , string Value,string Condition);
bool ValidateEmail(string Email);
bool ValidateText(string Text);

#endif // SQLSYSTEM_H
