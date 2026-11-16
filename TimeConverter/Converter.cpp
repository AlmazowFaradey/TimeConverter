#include "Converter.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#define localtime_safe(tmPtr, timePtr) localtime_s((tmPtr), (timePtr))
#else
#define localtime_safe(tmPtr, timePtr) localtime_r((timePtr), (tmPtr))
#endif

bool Converter::isLearpYear(int y)
{
	return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Converter::daysInMonth(int y, int m)
{
	static const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (m == 2 && isLearpYear(y))
		return 29;
	return d[m - 1];
}

bool Converter::ifValidDateTime(int year, int month, int day, int hours, int minutes, int seconds) const
{
	if (year < 1900 || year > 9999) return false;
	if (month < 1 || month > 12) return false;
	if (day < 1 || day > daysInMonth(year, month)) return false;
	if (hours < 0 || hours > 23) return false;
	if (minutes < 0 || minutes > 59) return false;
	if (seconds < 0 || seconds > 59) return false;
	return true;
}

std::string Converter::plural(long long n,
	const std::string& one,
	const std::string& few,
	const std::string& many)
{
	long long m100 = n % 100;
	if (m100 >= 11 && m100 <= 14) return many;
	long long m10 = n % 10;
	if (m10 == 1) return one;
	if (m10 >= 2 && m10 <= 4) return few;
	return many;
}

std::string Converter::formatRelative(const std::tm& target) const {
	std::tm targetCopy = target;
	targetCopy.tm_isdst = -1;
	std::time_t targetT = std::mktime(&targetCopy);

	std::time_t nowT = std::time(nullptr);
	std::tm nowTm = {};
	localtime_safe(&nowTm, &nowT);

	long long totalDiff = static_cast<long long>(std::difftime(targetT, nowT));
	if (totalDiff == 0) return "сейчас";

	bool future = totalDiff > 0;
	std::tm earlier = future ? nowTm : targetCopy;
	std::time_t laterT = future ? targetT : nowT;
	earlier.tm_isdst = -1;

	int years = 0;
	while (true) {
		std::tm test = earlier;
		test.tm_year += 1;
		int maxDay = daysInMonth(test.tm_year + 1900, test.tm_mon + 1);
		if (test.tm_mday > maxDay) test.tm_mday = maxDay;
		test.tm_isdst = -1;
		if (std::mktime(&test) <= laterT) { earlier = test; ++years; }
		else break;
	}

	int months = 0;
	while (true) {
		std::tm test = earlier;
		test.tm_mon += 1;
		if (test.tm_mon > 11) { test.tm_mon = 1; test.tm_year += 1; }
		int maxDay = daysInMonth(test.tm_year + 1900, test.tm_mon + 1);
		if (test.tm_mday > maxDay) test.tm_mday = maxDay;
		test.tm_isdst = -1;
		if (std::mktime(&test) <= laterT) { earlier = test; ++months; }
		else break;
	}

	long long rem = static_cast<long long>(std::difftime(laterT, std::mktime(&earlier)));
	int days = static_cast<int>(rem / 86400); rem %= 86400;
	int hours = static_cast<int>(rem / 3600); rem %= 3600;
	int minutes = static_cast<int>(rem / 60);
	int seconds = static_cast<int>(rem % 60);

	std::vector<std::string> parts;
	if (years)
		parts.push_back(std::to_string(years) + " " + plural(years, "год", "года", "лет"));
	if (months)
		parts.push_back(std::to_string(months) + " " + plural(months, "месяц", "месяца", "месяцев"));
	if (days)
		parts.push_back(std::to_string(days) + " " + plural(days, "день", "дня", "дней"));
	if (hours)
		parts.push_back(std::to_string(hours) + " " + plural(hours, "час", "часа", "часов"));
	if (minutes)
		parts.push_back(std::to_string(minutes) + " " + plural(minutes, "минута", "минуты", "минут"));
	if (seconds)
		parts.push_back(std::to_string(seconds) + " " + plural(seconds, "секунда", "секунды", "секунд"));

	if (parts.empty()) parts.push_back("0 секунд");

	std::string result;
	for (size_t i = 0; i < parts.size(); ++i) {
		if (i) result += " ";
		result += parts[i];
	}

	return future ? ("будет через " + result) : ("было " + result + " назад");
}

std::string Converter::currentDateTime(int year, int month, int day, int hours, int minutes, int seconds) const
{
	std::ostringstream oss;
	oss << std::setw(2) << std::setfill('0') << day << "."
		<< std::setw(2) << std::setfill('0') << month << "."
		<< year << " "
		<< std::setw(2) << std::setfill('0') << hours << ":"
		<< std::setw(2) << std::setfill('0') << minutes << ":"
		<< std::setw(2) << std::setfill('0') << seconds;
	return oss.str();
}