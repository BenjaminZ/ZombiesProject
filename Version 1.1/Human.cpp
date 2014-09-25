/*
File: Human.cpp
*/

#include "Human.hpp"

Human::Human(int aux_gender, int aux_birth, int aux_age, int aux_status)
{
	gender = aux_gender;
	birthdate = aux_birth;
	age_group = aux_age;
	status = aux_status;

	exposed_date = 0;
	return;
}

int Human::getGender()
{
	return gender;
}
int Human::getBirthDate()
{
	return birthdate;
}
int Human::getStatus()
{
	return status;
}
int Human::getExposedDate()
{
	return exposed_date;
}
int Human::getAgeGroup()
{
	return age_group;
}
void Human::infect(int n)
{
	status = EXPOSED;
	exposed_date = n;
	return;
}