#pragma once
#include <bits/stdc++.h>
#include <mutex>
#include <future>
#include <thread>
#include <functional>
using namespace std;

#define DOCS_COUNT 50'000
#define THREADS_COUNT 4u

struct Doc_hit {
    size_t id;
    size_t hit;
};

class InvertedIndex {
public:
    InvertedIndex();
    void Add(const string& document);
    const vector<Doc_hit>& Lookup(const string& word) const;
    const string& GetDocument(size_t id) const;
    size_t size();
private:
    map<string, vector<Doc_hit>> index;
    vector<string> docs;
    vector<Doc_hit> empty;
};

bool comp_for_dochit(const Doc_hit& lhs, const Doc_hit& rhs);

class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void Update(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);
    void Search(istream& query_input, ostream& search_results_output);
private:
    InvertedIndex index;
    vector<future<void>> threads;
    mutex index_mutex;
};
