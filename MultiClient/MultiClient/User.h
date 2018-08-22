#pragma once
#include <iostream>
#include <string>
using namespace std;
class User
{
public:
	User();
	User(char username_[], string fullname_, char gender_[], char dateOfBirth_[]);
	~User();
	void create();
	char* getUsername();
	char* getFullname();
	char* getGender();
	char* getDateOfBirth();
	void writeUser();
private:
	char userName[31];
	string fullName;
	char gender[10];
	char dateOfBirth[12];
};

