#pragma once
#include <iostream>
class User
{
public:
	User();
	~User();

	void setUsername(char userName_[30]);
	void setFullName(char fullName_[30]);
	void setGender(char gender_[10]);
	void setBirthday(char dateOfBirth_[12]);

	char* getUsername();
	char* getFullname();
	char* getGender();
	char* getDateOfBirth();

private:
	char userName[30];
	char fullName[30];
	char gender[10];
	char dateOfBirth[12];

};

