#pragma once
#include <bits/stdc++.h>
#include "date.h"
using namespace std;

class Database {
public:
	void Add(const Date& date, const string& event);
	void Find(const Date& date) const;
	void Print(ostream& os) const;
	int RemoveIf(const function<bool(const Date& date, const string& event)>& f);
	vector<string> FindIf(const function<bool(const Date& date, const string& event)>& f) const;
	string Last(const Date& date) const;
private:
	map<Date, vector<string>> mp;
	map<Date, set<string>> dublicate_check;
};
