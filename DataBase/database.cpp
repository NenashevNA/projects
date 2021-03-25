#include "database.h"
using namespace std;

void Database::Add(const Date& date, const string& event) {
	if (dublicate_check[date].find(event) != dublicate_check[date].end())
		return;
	mp[date].push_back(event);
	dublicate_check[date].insert(event);
};
void Database::Find(const Date& date) const {
	for (auto it = mp.at(date).begin(); it != mp.at(date).end(); it++) {
		cout << *it << endl;
	}
};
void Database::Print(ostream& os) const {
	for (auto it = mp.begin(); it != mp.end(); it++) {
		for (auto vec_it = it->second.begin(); vec_it != it->second.end(); vec_it++) {
			os << it->first << " " << *vec_it << endl;
		}
	}
};
string Database::Last(const Date& date) const {
	auto it = mp.upper_bound(date);
	if (it == mp.begin()) {
		throw invalid_argument("No entries");
	}
	it--;
	string str;
	ostringstream os(str);
	os << it->first;
	return os.str() + " " + it->second.back();
}
int Database::RemoveIf(const function<bool(const Date& date, const string& event)>& f) {
	int deleted = 0;
	for (auto& pair : mp) {
		auto pr = [f, pair](const string& event) {
			return f(pair.first, event);
		};
		auto it = remove_if(pair.second.begin(), pair.second.end(), pr);
		deleted += pair.second.end() - it;
		pair.second.erase(it, pair.second.end());
	}
	for (auto it = mp.begin(); it != mp.end();) {
		if (it->second.empty())
			it = mp.erase(it);
		else
			it++;
	}
	for (auto& pair : dublicate_check) {
		for (auto set_it = pair.second.begin(); set_it != pair.second.end();) {
			if (f(pair.first, *set_it))
				set_it = pair.second.erase(set_it);
			else
				set_it++;
		}
	}
	for (auto it = dublicate_check.begin(); it != dublicate_check.end();) {
		if (it->second.empty())
			it = dublicate_check.erase(it);
		else
			it++;
	}
	return deleted;
}
vector<string> Database::FindIf(const function<bool(const Date& date, const string& event)>& f) const{
	vector<string> res;
	for (const auto& pair : mp) {
		string str;
		ostringstream os(str);
		os << pair.first;
		for (const auto& ev : pair.second) {
			if (f(pair.first, ev))
				res.push_back(os.str() + " " + ev);
		}
	}
	return res;
}