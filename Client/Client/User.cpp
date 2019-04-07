#include "User.h"

User::User(char username_[], string fullname_, char gender_[], char dateOfBirth_[]) {
	strcpy_s(userName, username_);
	fullName = fullname_;
	strcpy_s(gender, gender_);
	strcpy_s(dateOfBirth, dateOfBirth_);
}

void User::create() {
	cout << "Username: ";
	cin >> userName;
	cout << "Fullname: ";
	fflush(stdin);
	cin >> fullName;
	cout << "Gender: ";
	cin >> gender;
	cout << "Birthday: ";
	cin >> dateOfBirth;
}

char* User::getUsername() {
	char* result = userName;
	return result;
}

char* User::getFullname() {
	char* result;
	char res_temp[30];
	strcpy_s(res_temp, fullName.c_str());
	result = res_temp;
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

void User::writeUser() {
	fflush(stdin);
	cout << "Username: ";
	cin >> userName;
}

User::User()
{
}


User::~User()
{
}
