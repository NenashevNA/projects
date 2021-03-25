#include <vector>
#include <iostream>
using namespace std;

vector<size_t> prefix_function(const string& s) {
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

bool prefix_find(const string& sample, const char* text) {
    vector<size_t> pi = prefix_function(sample);
    size_t j = 0;
    for (size_t i = 0; text[i]; ++i) {
            while ((j > 0) && (text[i] != sample[j]))
                j = pi[j - 1];
        if (text[i] == sample[j])
            ++j;
        if (j == sample.size())
            return true;
    }
    return false;
}

int main() {
    while (true) {
        string text;
        string sample;
        cin >> text >> sample;
        cout << prefix_find(sample, text.c_str()) << endl;
    }
}
