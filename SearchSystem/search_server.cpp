#include "search_server.h"
#include <chrono>
bool comp_for_dochit(const Doc_hit& lhs, const Doc_hit& rhs) {
    if (lhs.hit == rhs.hit)
        return lhs.id < rhs.id;
    return lhs.hit > rhs.hit;
}
void InvertedIndex::Add(const string& document) {
    istringstream is(document);
    string word;
    while (is >> word) {
        if (index[word].empty() || index[word].back().id != docs.size())
            index[word].push_back({ docs.size(), 1 });
        else
            index[word].back().hit++;
    }
    docs.push_back(document);
}
const vector<Doc_hit>& InvertedIndex::Lookup(const string& word) const {
    if (index.find(word) != index.end()) {
        return index.at(word);
    }
    return empty;
}
size_t InvertedIndex::size() {
    return docs.size();
}
const string& InvertedIndex::GetDocument(size_t id) const {
    return docs[id];
}
SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}


///
void SearchServer::UpdateDocumentBase(istream& document_input) {
    threads.push_back(future<void>
        (async([&document_input, this] {this->Update(document_input);})));
}

void SearchServer::Update(istream& document_input) {
    string doc;
    InvertedIndex index_;
    while (getline(document_input, doc)) {
        index_.Add(doc);
    }
    index_mutex.lock();
    index = move(index_);
    index_mutex.unlock();
}
///
InvertedIndex::InvertedIndex() {};




/////////////
void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
    //Search(&query_input, &search_results_output);
    this_thread::sleep_for(0.01s);
    threads.push_back(future<void>(async([&query_input, &search_results_output, this]
    {this->Search(query_input, search_results_output);})));
}
////////////





void SearchServer::Search(istream& query_input, ostream& search_results_output) {
    string query;
    while (getline(query_input, query)) {
        vector<Doc_hit> ans(index.size() + 5);
        for (size_t i = 0; i < index.size() + 5; i++)
            ans[i] = { i, 0 };
        istringstream is(query);
        string word;
        while (is >> word) {
            index_mutex.lock();
            for (const auto& docid : index.Lookup(word))
                ans[docid.id].hit += docid.hit;
            index_mutex.unlock();
        }
        partial_sort(ans.begin(), ans.begin() + 5, ans.end(), comp_for_dochit);
        search_results_output << query << ":";
        for (size_t i = 0; i < 5 && i < ans.size(); i++) {
            if (ans[i].hit) {
                search_results_output <<
                    " {docid: " << ans[i].id << ", " <<
                    "hitcount: " << ans[i].hit << "}";
            }
        }
        search_results_output << endl;
    }
}