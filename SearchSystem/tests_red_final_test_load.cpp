#include "search_server.h"
#include "Header.h"
#include <profile.h>
#include <bits/stdc++.h>
#include <random>
void TestLoad() {

	std::default_random_engine rd(34);
	
	 std::uniform_int_distribution<char> char_gen('a', 'z');
	 auto random_word = [&](size_t len) {
	 string result(len, ' ');
	 std::generate(begin(result), end(result), [&] {return char_gen(rd);});
	 return result;
	 };

	 std::uniform_int_distribution<size_t> len_gen(5, 21);

	 vector<string> key_words(15000); //15000 max words
	 for (auto &w : key_words) {
	 w = random_word(len_gen(rd));
	 }

	 ofstream out("words.txt");
	 for (const auto &w : key_words) {
	 out << w << ' ';
	 }
	 

	vector<string> key_words;
	ifstream word_input("words.txt"); // corpus from Moby Dick
	for (string word; getline(word_input, word);) {
		key_words.push_back(word);
	}

	std::uniform_int_distribution<size_t> line_len_gen(1000, 1000); //max 1000 words in 1 doc

	ofstream text_out("docs_input.txt");
	for (int line = 0; line < 80; ++line) { //50000 docs max
		ostringstream line_out;
		auto line_len = line_len_gen(rd);
		for (size_t i = 0; i < line_len; ++i) {
			std::uniform_int_distribution<size_t> word_index(0, key_words.size() - 1);
			line_out << key_words[word_index(rd)];
			line_out << ' ';
		}
		text_out << line_out.str() << '\n';
	}

	std::uniform_int_distribution<size_t> q_line_len_gen(10, 10); // [1; 10] words in query
	ofstream query_out("queries_input.txt");
	for (int line = 0; line < 3000; ++line) { //500000 queries max
		ostringstream line_out;
		auto line_len = q_line_len_gen(rd);
		for (size_t i = 0; i < line_len; ++i) {
			std::uniform_int_distribution<size_t> word_index(0, key_words.size() - 1);
			line_out << key_words[word_index(rd)];
			line_out << ' ';
		}
		query_out << line_out.str() << '\n';
	}

	ifstream docs_input("docs_input.txt");
	ifstream queries_input("queries_input.txt");
	{SearchServer srv;
		{LOG_DURATION("BIG TEST UPDATE")
			{
				srv.UpdateDocumentBase(docs_input);
			}
		}
		{LOG_DURATION("BIG TEST UPDATE")
			ofstream queries_output("queries_output.txt");
			{
				srv.AddQueriesStream(queries_input, queries_output);
			}
		}
	}
}