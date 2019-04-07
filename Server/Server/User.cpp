#include "User.h"



User::User()
{
}


User::~User()
{
}

void User::setUsername(char userName_[30]) {
	strcpy_s(userName, userName_);
}

void User::setFullName(char fullName_[30]) {
	strcpy_s(fullName, fullName_);
}

void User::setBirthday(char dateOfBirth_[12]) {
	strcpy_s(dateOfBirth, dateOfBirth_);
}

void User::setGender(char gender_[10]) {
	strcpy_s(gender, gender_);
}


char* User::getUsername() {
	char* result = userName;
	return result;
}

char* User::getFullname() {
	char* result = fullName;
	return result;
}

char* User::getGender() {
	char* result = gender;
	return result;
}

char* User::getDateOfBirth() {
	char* result = dateOfBirth;
	return result;
}

