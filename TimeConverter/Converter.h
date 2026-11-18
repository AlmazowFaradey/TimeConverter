#pragma once
#include <string>
#include <ctime>

class Converter
{
public:
	bool ifValidDateTime(int year, int month, int day, int hours, int minutes, int seconds) const;

	std::string currentDateTime(int year, int month, int day, int hours, int minutes, int seconds) const;

	std::string formatRelative(const std::tm& target) const;
private:
	static bool isLearpYear(int year);
	static int daysInMonth(int year, int month);
	static std::string plural(long long n,
		const std::string& one,
		const std::string& few,
		const std::string& many);
};