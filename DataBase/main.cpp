
//int main() {
//	Database db;
//	string str;
//	while (getline(cin, str)) {
//		if (str.empty())
//			continue;
//		stringstream sin(str);
//		string command;
//		sin >> command;
//		if (command == "Add") {
//			Date date;
//			string event;
//			try {
//				date = ParseDate(sin);
//				sin >> event;
//				if (event.empty())
//					continue;
//				db.AddEvent(date, event);
//				continue;
//			}
//			catch (string exc) {
//				cout << exc << endl;
//				continue;
//			}
//		}
//		if (command == "Del") {
//			Date date;
//			string event;
//			try {
//				date = ParseDate(sin);
//				sin >> event;
//			}
//			catch (string exc) {
//				cout << exc << endl;
//				continue;
//			}
//			if (event.empty()) {
//				try {
//					cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
//				}
//				catch (out_of_range) {
//					cout << "Deleted 0 events" << endl;
//				}
//				continue;
//			}
//			try {
//				if (db.DeleteEvent(date, event)) {
//					cout << "Deleted successfully" << endl;
//					continue;
//				}
//				cout << "Event not found" << endl;
//				continue;
//			}
//			catch (out_of_range) {
//				cout << "Event not found" << endl;
//				continue;
//			}
//		}
//		if (command == "Find") {
//			Date date;
//			try {
//				date = ParseDate(sin);
//			}
//			catch (string exc) {
//				cout << exc << endl;
//				continue;
//			}
//			try {
//				db.Find(date);
//			}
//			catch (out_of_range) {
//				continue;
//			}
//			continue;
//		}
//		if (command == "Print") {
//			db.Print(cout);
//			continue;
//		}
//		if (command == "Last") {
//			Date date;
//			try {
//				date = ParseDate(sin);
//				db.Last(date, cout);
//				continue;
//			}
//			catch (string exc) {
//				cout << exc << endl;
//				continue;
//			}
//		}
//		cout << "Unknown command: " << command << endl;
//	}
//	return 0;
//}
#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
//#include "test_runner.h"
#include <bits/stdc++.h>

using namespace std;
template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
	os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
	os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
	os << "{";
	bool first = true;
	for (const auto& kv : m) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << kv.first << ": " << kv.second;
	}
	return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
	if (t != u) {
		ostringstream os;
		os << "Assertion failed: " << t << " != " << u;
		if (!hint.empty()) {
			os << " hint: " << hint;
		}
		throw runtime_error(os.str());
	}
}

void Assert(bool b, const string& hint) {
	AssertEqual(b, true, hint);
}

class TestRunner {
public:
	template <class TestFunc>
	void RunTest(TestFunc func, const string& test_name) {
		try {
			func();
			cerr << test_name << " OK" << endl;
		}
		catch (exception& e) {
			++fail_count;
			cerr << test_name << " fail: " << e.what() << endl;
		}
		catch (...) {
			++fail_count;
			cerr << "Unknown exception caught" << endl;
		}
	}

	~TestRunner() {
		if (fail_count > 0) {
			cerr << fail_count << " unit tests failed. Terminate" << endl;
			exit(1);
		}
	}

private:
	int fail_count = 0;
};
string ParseEvent(istream& is) {
	while (is.peek() == ' ')
		is.get();
	string res;
	getline(is, res, '\n');
	return res;
}

void TestAll();

int main() {
	//TestAll();
	Database db;
	for (string line; getline(cin, line); ) {
		istringstream is(line);
		string command;
		is >> command;
		if (command == "Add") {
			const auto date = ParseDate(is);
			const auto event = ParseEvent(is);
			db.Add(date, event);
		}
		else if (command == "Print") {
			db.Print(cout);
		}
		else if (command == "Del") {
			auto condition = ParseCondition(is);
			auto predicate = [condition](const Date& date, const string& event) {
				return condition->Evaluate(date, event);
			};
			int count = db.RemoveIf(predicate);
			cout << "Removed " << count << " entries" << endl;
		}
		else if (command == "Find") {
			auto condition = ParseCondition(is);
			auto predicate = [condition](const Date& date, const string& event) {
				return condition->Evaluate(date, event);
			};

			const auto entries = db.FindIf(predicate);
			for (const auto& entry : entries) {
				cout << entry << endl;
			}
			cout << "Found " << entries.size() << " entries" << endl;
		}
		else if (command == "Last") {
			try {
				cout << db.Last(ParseDate(is)) << endl;
			}
			catch (invalid_argument&) {
				cout << "No entries" << endl;
			}
		}
		else if (command.empty()) {
			continue;
		}
		else {
			throw logic_error("Unknown command: " + command);
		}
	}
	return 0;
}

void TestParseEvent() {
	{
		istringstream is("event");
		AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
	}
	{
		istringstream is("   sport event ");
		AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
	}
	{
		istringstream is("  first event  \n  second event");
		vector<string> events;
		events.push_back(ParseEvent(is));
		events.push_back(ParseEvent(is));
		AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
	}
}

void TestAll() {
	TestRunner tr;
	tr.RunTest(TestParseEvent, "TestParseEvent");
	//tr.RunTest(TestParseCondition, "TestParseCondition");
}
