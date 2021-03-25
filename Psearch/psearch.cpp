#include <bits/stdc++.h>
#include <dirent.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <future>
#include <unistd.h>
using namespace std;

class Psearch {
public:
    void Find(const string &s, const string& dir, bool rec, size_t thread_count) {
        files.clear();
        ls(dir, rec, &files);
        size_t part_size = files.size() / thread_count;
        vector<future<void>> f(thread_count);
        if (part_size % thread_count != 0)
            part_size++;
        for (size_t i = 0; i < thread_count; i++) {
            f[i] = async([s, i, part_size, this]()
                    { return ThreadFind(ref(s), i * part_size, min(files.size(), (i + 1) * part_size)); });
        }
    }
private:
    void ThreadFind(const string &s, size_t st_pos, size_t fin_pos) {
        for (size_t i = st_pos; i < fin_pos; i++) {
            FindInFile(s, files[i]);
        }
    }
    void FindInFile(const string& s, const string& file_name) {
        ifstream is;
        is.open(file_name);
        string line;
        size_t num = 0;
        while (getline(is, line)) {
            if (prefix_find(s, line)) {
                cout << file_name + ": " + "line " +
                                to_string(num)/* + ": " + line */<< endl;
            }
            num++;
        }
    }
    vector<size_t> prefix_function(const string &s) {
        size_t n = s.length();
        vector<size_t> pi(n, 0);
        for (size_t i = 1; i < n; ++i) {
            size_t j = pi[i - 1];
            while ((j > 0) && (s[i] != s[j]))
                j = pi[j - 1];
            if (s[i] == s[j])
                ++j;
            pi[i] = j;
        }
        return pi;
    }

    bool prefix_find(const string &sample, const string& text) {
        vector<size_t> pi = prefix_function(sample);
        size_t j = 0;
        for (size_t i = 0; i < text.size(); ++i) {
            while ((j > 0) && (text[i] != sample[j]))
                j = pi[j - 1];
            if (text[i] == sample[j])
                ++j;
            if (j == sample.size())
                return true;
        }
        return false;
    }

    void ls(const string &dir_name, bool rec_flag, vector<string>* files) {
        DIR *dir = opendir(dir_name.c_str());
        if (dir == nullptr) return;
        for (dirent *d = readdir(dir); d != nullptr; d = readdir(dir)) {
            if (rec_flag && d->d_type == DT_DIR) {
                if (strcmp(d->d_name, ".") == 0) continue;
                if (strcmp(d->d_name, "..") == 0) continue;
                ls(dir_name + '/' + d->d_name, rec_flag, files);
            }
            if (d->d_type == DT_REG)
                files->push_back(dir_name + '/' + d->d_name);
        }
        closedir(dir);
    }
    vector<string> files;
};

int main(int argc, char* argv[]) {
    string dir_name = get_current_dir_name();
    string sample;
    size_t threads = 1;
    bool rec = true;
    if (argc > 1) {
        for (size_t i = 0; i < min(argc, 4); i++) {
            if (argv[i][0] == '-') {
                if (argv[i][1] == 'n')
                    rec = false;
                if (argv[i][1] == 't') {
                    string digit;
                    for (size_t j = 2; argv[i][j] != '\0'; j++) {
                        if (isdigit(argv[i][j]))
                            digit.push_back(argv[i][j]);
                    }
                    threads = atoi(digit.c_str());
                }
                continue;
            }
            if (argv[i][0] == '/') {
                dir_name = argv[i];
                continue;
            }
            sample = argv[i];
        }
    }
    Psearch searcher;
    searcher.Find(sample, dir_name, rec, threads);
}
