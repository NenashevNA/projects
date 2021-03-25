#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

void sig_handler(int sig) {
    signal(SIGINT, sig_handler);
};

struct Command {
    string name; // command name
    vector<string> argv; // arguments
    vector<string> samples; // template like (?*a*)
    string input; // if input (output) == file
    string output;
    int fd_in = -1; // used in pipe to connect
    int fd_out = -1;
    bool use_time = false;
};

class Timer {
public:
    Timer() {
        getrusage(RUSAGE_CHILDREN, &used);
        tv_st.tv_sec = 0;
        tv_st.tv_usec = 0;
        tv_st.tv_sec = 0;
        tv_fin.tv_usec = 0;
    }
    void Start() {
        gettimeofday(&tv_st, &tz);
    }
    void Stop() {
        gettimeofday(&tv_fin, &tz);
    }
    void Print() {
        Stop();
        double sys = (double)used.ru_stime.tv_sec +
                     (double)used.ru_stime.tv_usec / 1000000.0;
        double user = (double)used.ru_utime.tv_sec +
                      (double)used.ru_utime.tv_usec / 1000000.0;
        double real = (tv_fin.tv_sec - tv_st.tv_sec) + (tv_fin.tv_usec-tv_st.tv_usec) / 1000000.0;
        getrusage(RUSAGE_CHILDREN, &used);
        cout << "real: " << setprecision(3) << fixed << real
             << "s" << endl;
        cout << "sys: " << setprecision(3) << fixed << ((double)used.ru_stime.tv_sec +
                                                        (double)used.ru_stime.tv_usec / 1000000.0) - sys << "s" << endl;
        cout << "usr: " << setprecision(3) << fixed << ((double)used.ru_utime.tv_sec +
                                 (double)used.ru_utime.tv_usec / 1000000.0) - user
                                 << "s" << endl;
    }
private:
    struct rusage used;
    struct timeval tv_st, tv_fin;
    struct timezone tz;
};
namespace uty {
    Command ParseExpression(const string &line) {
        istringstream is(line);
        Command com;
        is >> com.name;
        if (com.name == "time") {
            com.use_time = true;
            is >> com.name;
        }
        string arg;
        while (is >> arg) {
            if (arg == "<") {
                is >> com.input;
                continue;
            }
            if (arg == ">") {
                is >> com.output;
                continue;
            }
            if (arg == "~") {
                com.argv.push_back(getenv("HOME"));
                continue;
            }
            if (arg.find('*') != arg.npos ||
            arg.find('?') != arg.npos) {
                com.samples.push_back(move(arg));
                continue;
            }
            com.argv.push_back(move(arg));
        }
        return com;
    }; // convert string into command

    vector<string> Split(const string &line, char separator) {
        vector<string> res;
        for (auto it = line.begin(); it != line.end(); ++it) {
            auto cur = it;
            it = find(cur, line.end(), separator);
            res.emplace_back(string(cur, it));
            if (it == line.end())
                break;
        }
        return res;
    } // split string by separator
}

class MicroShell {
public:
    MicroShell() {
        cur_dir = getenv("HOME");
        cur_user = getenv("USER");
        root = (strcmp("root", cur_user.c_str()) == 0);
        chdir(cur_dir.c_str());
    }

    void ExecPipe(vector<string> &pipe_) {
        if (pipe_.empty())
            return;
        if (pipe_.size() == 1) {
            Command com = uty::ParseExpression(pipe_[0]);
            if (com.use_time) {
                timer.Start();
                pid_t pid = Exec(com);
                if (pid != -1) {
                    int st;
                    waitpid(pid, &st, 0);
                }
                timer.Print();
            } else {
                pid_t pid = Exec(com);
                if (pid != -1) {
                    int st;
                    waitpid(pid, &st, 0);
                }
            }
        } else {
            bool used_timer = false;
            pid_t last_proc = 0;
            vector<int[2]> pipes(pipe_.size() - 1);
            for (size_t i = 0; i + 1 < pipe_.size(); i++) {
                if (pipe(pipes[i])) {
                    for (size_t j = 0; j < i; j++) {
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                    }
                    return;
                }
            }
            for (size_t i = 0; i < pipe_.size(); i++) {
                Command com = uty::ParseExpression(pipe_[i]);
                if (com.use_time && !used_timer) {
                    timer.Start();
                    used_timer = true;
                }
                if (i != 0)
                    com.fd_in = pipes[i - 1][0];
                if (i != pipe_.size() - 1)
                    com.fd_out = pipes[i][1];
                pid_t pid = Exec(com);
                if (pid != -1)
                    last_proc = pid;
                if (i != 0)
                    close(pipes[i - 1][0]);
                if (i != pipe_.size() - 1)
                    close(pipes[i][1]);
            }
            int st;
            waitpid(last_proc, &st, 0);
            if (used_timer)
                timer.Print();
        }
    } // pipe execution

    void GetCom() {
        if (root)
            cout << "\x1b[1;32m" << cur_user << ":\x1b[1;34m~" <<
                 cur_dir << "\x1b[0m" << "\x1b[1;31m!\x1b[0m";
        else
            cout << "\x1b[1;32m" << cur_user << ":\x1b[1;34m~" <<
                 cur_dir << "\x1b[0m" << "\x1b[1;31m>\x1b[0m";
    } // prints pretty message on the screen

private:
    void Pwd() {
        cout << cur_dir << endl;
    } // print current directory

    void Cd(const vector<string> &argv) {
        if (argv.empty()) {
            cur_dir = getenv("HOME");
            chdir(cur_dir.c_str());
            return;
        }
        if (argv.size() != 1) {
            cerr << "cd: Wrong number of arguments" << endl;
            return;
        }
        string dir_name = cur_dir + "/" + argv[0];
        DIR *dir = opendir(dir_name.c_str());
        if (dir != NULL) {
            cur_dir = dir_name;
            chdir(cur_dir.c_str());
            cur_dir = get_current_dir_name();
            return;
        } else {
            dir = opendir(argv[0].c_str());
            if (dir != NULL) {
                cur_dir = argv[0];
                chdir(cur_dir.c_str());
                cur_dir = get_current_dir_name();
            } else {
                cerr << "cd: Can't open" << endl;
            }
        }
    } // changes current directory
    pid_t Exec(Command& com) {
        pid_t pid = -1;
        if (com.name == "pwd") {
            Pwd();
        } else if (com.name == "cd") {
            Cd(com.argv);
        } else {
            pid = fork();
            if (pid == 0)
                Exec_extern(com);
        }
        return pid;
    }
    void Exec_extern(Command &com) {
        if (com.name.empty())
            return;
        if (!com.samples.empty()) {
            ReplaceSample(com);
        }
        char *name_ = (char *) (com.name.c_str());
        vector<char *> argv_;
        string path = com.name;
        argv_.push_back((char *) path.c_str());
        for (auto &a : com.argv)
            argv_.push_back((char *) a.c_str());
        argv_.push_back(nullptr);
        if (com.fd_in != -1) {
            dup2(com.fd_in, 0);
        } else if (!com.input.empty()) {
            close(0);
            open(com.input.c_str(), O_RDONLY | O_CREAT, 0777);
        }
        if (com.fd_out != -1) {
            dup2(com.fd_out, 1);
        } else if (!com.output.empty()) {
            close(1);
            open(com.output.c_str(), O_WRONLY | O_CREAT, 0777);
        }
        execvp(name_, argv_.data());
    } // extern command execution

    void ReplaceSample(Command &com) {
        for (auto &sample : com.samples) {
            if (sample.find('/') == sample.npos) {
                vector<string> files = ReplaceSampleDir(sample, cur_dir);
                if (files.empty()) {
                    com.argv.push_back(sample);
                } else {
                    for (const auto &file : files) {
                        com.argv.push_back(file);
                    }
                }
            } else {
                if (sample.empty() || sample[0] != '/') {
                    cerr << "wrong sample" << endl;
                    return;
                }
                vector<string> samples_arr = uty::Split(sample.substr(1), '/');
                vector<string> ans;
                ReplaceSampleDirRec(samples_arr, ans, "/", 0);
                if (ans.empty()) {
                    com.argv.push_back(sample);
                } else {
                    for (auto &arg : ans) {
                        com.argv.push_back(move(arg));
                    }
                }
            }
        }
    }// replace sample(template) as files in directories

    vector<string> ReplaceSampleDir(string &sample, const string &dir_name) {
        DIR *dir = opendir(dir_name.c_str());
        vector<string> ret;
        if (dir == nullptr) return ret;
        for (dirent *d = readdir(dir); d != nullptr; d = readdir(dir)) {
            if (Match(sample, d->d_name))
                ret.push_back(d->d_name);
        }
        closedir(dir);
        return ret;
    } // replace sample in particular directory
    void ReplaceSampleDirRec(const vector<string> &samples, vector<string> &ans,
                             string dir_name, size_t pos) {
        if (pos >= samples.size()) {
            ans.push_back(dir_name);
            return;
        }
        DIR *dir = opendir(dir_name.c_str());
        if (dir == nullptr) return;
        for (dirent *d = readdir(dir); d != nullptr; d = readdir(dir)) {
            if (Match(samples[pos], d->d_name))
                if (dir_name == "/")
                    ReplaceSampleDirRec(samples, ans,
                                        dir_name + d->d_name, pos + 1);
                else
                    ReplaceSampleDirRec(samples, ans,
                                        dir_name + "/" + d->d_name, pos + 1);
        }
        closedir(dir);
    }

    bool Match(const string &sample, const string &str) {
        if (sample.empty())
            return str.empty();
        if (sample[0] == '?') {
            if (str.empty())
                return false;
            else
                return Match(sample.substr(1, sample.size()), str.substr(1, str.size()));
        } else if (sample[0] == '*') {
            for (int i = 0; i <= str.size(); ++i) {
                if (Match(sample.substr(1, sample.size()), str.substr(i, str.size())))
                    return true;
            }
            return false;
        } else
            return (sample[0] == str[0] &&
                    Match(sample.substr(1, sample.size()), str.substr(1, str.size())));
    } // match string with sample
    Timer timer;
    string cur_dir;
    string cur_user;
    bool root;
};

int main() {
    signal(SIGINT, sig_handler);
    MicroShell msh;
    msh.GetCom();
    string s;
    while (getline(cin, s)) {
        vector<string> pipe_ = uty::Split(s, '|');
        msh.ExecPipe(pipe_);
        msh.GetCom();
    }
    cout << endl;
}
