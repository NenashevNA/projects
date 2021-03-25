#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Date {
	bool operator <(const Date& d) const {
		if (this->year == d.year) {
			if (this->month == d.month) {
				return this->day < d.day;
			}
			return this->month < d.month;
		}
		return this->year < d.year;
	}
	bool operator ==(const Date& d) const {
		return this->year == d.year && this->month == d.month && this->day == d.day;
	}
	bool operator >(const Date& d) const {
		if (this->year == d.year) {
			if (this->month == d.month) {
				return this->day > d.day;
			}
			return this->month > d.month;
		}
		return this->year > d.year;
	}
	bool operator <=(const Date& d) const {
		return (*this == d) || (*this < d);
	}
	bool operator >=(const Date& d) const {
		return (*this == d) || (*this > d);
	}
	bool operator !=(const Date& d) const {
		return !(*this == d);
	}
	int year = 0;
	int month = 0;
	int day = 0;
};
ostream& operator <<(ostream& stream, const Date& d);
Date ParseDate(istream& stream);
void WrongDateFormat(string str);
void ReadUntilMinus(istream& stream, string& s);
bool IsDigit(string s);