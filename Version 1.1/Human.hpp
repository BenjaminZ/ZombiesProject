/*
File: Human.hpp
*/
#ifndef __HUMAN_HPP_INCLUDED__
#define __HUMAN_HPP_INCLUDED__

#include "zombiesim_parameters.hpp"

class Human
{
	private:
		int gender;
		int birthdate;
		int age_group;
		int status;
		int exposed_date;

	public:
		Human(int, int, int, int);
		int getGender();
		int getBirthDate();
		int getStatus();
		int getExposedDate();
		int getAgeGroup();
		void infect(int);
};

#endif // __HUMAN_HPP_INCLUDED__