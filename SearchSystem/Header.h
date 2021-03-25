#pragma once
void TestLoad();
//void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
//    //mutex lock_output;
//    string query;
//    vector<future<string>> threads(THREADS_COUNT);
//    vector<string> queries;
//    size_t q_count = 0;
//    if (THREADS_COUNT == 1) {
//        while (getline(query_input, query)) {
//            string out = Search(query);
//            //cout << out;
//            search_results_output << out;
//        }
//        return;
//    }
//    string out;
//    while (getline(query_input, query)) {
//        queries.push_back(move(query));
//        if (queries.size() == THREADS_COUNT) {
//            for (size_t j = 0; j < threads.size(); j++) {
//                threads[j] = async(launch::async, &SearchServer::Search, this, ref(queries[j]));
//            }
//            for (size_t j = 0; j < threads.size(); j++) {
//                string out_ = threads[j].get();
//                search_results_output << out_;
//                //cout << out_;
//            }
//            queries.clear();
//        }
//    }
//    for (size_t j = 0; j < queries.size(); j++) {
//        threads[j] = async(launch::async, &SearchServer::Search, this, ref(queries[j]));
//    }
//    for (size_t j = 0; j < queries.size(); j++) {
//        string out_ = threads[j].get();
//        search_results_output << out_;
//        //cout << out_;
//    }
//    queries.clear();
//}
