#include "date.h"


void WrongDateFormat(string str) {
	throw logic_error("Wrong date format: " + str);
	return;
}

void ReadUntilMinus(istream& stream, string& s) {
	char c;
	if (stream.peek() == '-') {
		stream >> c;
		s.push_back(c);
	}
	while (stream.peek() != '-') {
		if (stream.peek() == '\0')
			break;
		stream >> c;
		s.push_back(c);
	}
	return;
}

bool IsDigit(string s) {
	if (s.empty())
		return false;
	unsigned int i = 0;
	if (s[i] == '+' || s[i] == '-') {
		i++;
	}
	while (isdigit(s[i]))
		i++;
	if (i < s.length())
		return false;
	return true;
}

Date ParseDate(istream& stream) {
	Date d;
	string str, year, month, day;
	stream >> str;
	str.push_back('\0');
	stringstream sin(str);
	ReadUntilMinus(sin, year);
	if (sin.peek() == '-')
		sin.ignore(1);
	else {
		WrongDateFormat(str);
	}
	ReadUntilMinus(sin, month);
	if (sin.peek() == '-')
		sin.ignore(1);
	else {
		WrongDateFormat(str);
	}
	ReadUntilMinus(sin, day);
	if (sin.peek() != '\0') {
		WrongDateFormat(str);
	}
	if (!IsDigit(year) || !IsDigit(month) || !IsDigit(day)) {
		WrongDateFormat(str);
	}
	stringstream cyear(year), cmonth(month), cday(day);
	cyear >> d.year;
	cmonth >> d.month;
	cday >> d.day;
	if (!(d.month >= 1 && d.month <= 12)) {
		string exc_what = "Month value is invalid: " + to_string(d.month);
		throw exc_what;
	}
	if (!(d.day >= 1 && d.day <= 31)) {
		string exc_what = "Day value is invalid: " + to_string(d.day);
		throw exc_what;
	}
	return d;
}

ostream& operator <<(ostream& stream, const Date& d) {
	stream << setw(4) << setfill('0') << d.year << '-' << setw(2) << setfill('0') << d.month << '-'
		<< setw(2) << setfill('0') << d.day;
	return stream;
}