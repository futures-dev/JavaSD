//
// Andrei Kolomiets 143-1
// CLion 2016.1.2 MinGW 3.21 GCC 4.9.1
// 22.05.2016
//

#include "SpellChecker.h"
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <regex>
#include <iostream>

bool SpellChecker::d_search(const string &word) const {
    unsigned char h = hash(word);
    vector<string> &it = hashset[h];
    auto it2 = find(it.begin(), it.end(), word);
    return it2 != it.end();
}

bool SpellChecker::dwd_insert(const string &word) {
    unsigned char h = hash(word);
    vector<string> &it = hashset[h];
    if (find(it.begin(), it.end(), word) == it.end()) {
        it.push_back(word);
        return true;
    }
    else {
        return false;
    }
}

bool SpellChecker::d_delete(const string &word) {
    unsigned char h = hash(word);
    vector<string> &it = hashset[h];
    auto it2 = find(it.begin(), it.end(), word);
    if (it2 != it.end()) {
        it.erase(it2);
        _size--;
        return true;
    }
}

size_t SpellChecker::d_count() const {
    /*
    int c = 0;
    for (auto it = hashset.begin();it!=hashset.end();it++){
        c += it->second.size();
    }
    return c;
     */
    return _size;
}

void SpellChecker::d_print(const string &output, bool sorted) const {
    if (sorted) {
        print_sorted(output);
    }
    else {
        ofstream fout(output);
        for (auto it = 0; it < UINT16_MAX; it++) {
            for (auto it2 = hashset[it].begin(); it2 != hashset[it].end(); it2++) {
                fout << *it2 << endl;
            }
        }
    }
}

void SpellChecker::load_words(const string &input, const string &output) {
    ifstream fin(input);
    if (!fin.is_open()) {
        throw invalid_argument("File" + input + " Not Found");
    }
    string buf;
    regex e(wordRegex, regex_constants::extended);
    ofstream fout(output, ios_base::app);
    while (getline(fin, buf)) {
        smatch match;
        regex_search(buf, match, e);
        for (auto word:match) {
            string tmp(word.str());
            transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
            if (!tmp.empty()) {
                dwd_insert(tmp);
                fout << tmp << endl;
            }
        }
    }
}

void SpellChecker::spell_check(const string &input, const string &output) const {
    ifstream fin(input);
    if (!fin.is_open()) {
        throw invalid_argument("File" + input + " Not Found");
    }
    string buf;
    regex e(wordRegex, regex_constants::basic);
    ofstream fout(output);
    set<string> ignored;
    set<pair<string, string>> replacements;
    bool skip = false;
    while (getline(fin, buf)) {
        smatch match;
        string prev_suffix;
        while (regex_search(buf, match, e)) {
            prev_suffix = match.suffix();
            fout << match.prefix();
            auto match_word = match.str();
            string word(match_word);
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            buf = match.suffix();

            // check replace all
            {
                auto rep = find_if(replacements.begin(), replacements.end(), [word](const pair<string, string> &q) {
                    return q.first == word;
                });
                if (rep != replacements.end()) {
                    fout << rep->second;
                    continue;
                }
            }

            // check ignore all and X exit
            if (ignored.find(word) != ignored.end() || skip) {
                fout << match_word;
                continue;
            }

            // check dictionary
            if (!d_search(word)) {
                cout << WORD_FOUND_STRING << word << WORD_MISSING_STRING << endl;
                bool bad_action1 = true;
                while (bad_action1) {
                    cout << POSSIBLE_ACTIONS_STRING << endl;
                    string action_buf;
                    getline(cin, action_buf);
                    switch (action_buf.back()) {
                        case 'R':
                        case 'r': {
                            cout << MAYBE_STRING << endl;
                            set<string> suggestions;
                            suggest_adjacent(word, suggestions);
                            suggest_insertion(word, suggestions);
                            suggest_odd(word, suggestions);
                            suggest_replacement(word, suggestions);
                            string suggested;
                            int i = 0;
                            for (auto it = suggestions.begin(); it != suggestions.end(); it++, i++) {
                                cout << i << ". " << *it << endl;
                            }
                            while (true) {
                                cout << POSSIBLE_REPLACEMENTS_STRING << endl;
                                string action_buf2;
                                getline(cin, action_buf2);
                                switch (action_buf2.back()) {
                                    case 'I':
                                    case 'i':
                                        goto case_ignore;
                                    default:
                                        break;
                                }
                                stringstream myStream(action_buf2);
                                int number;
                                if (myStream >> number) {
                                    if (number < i) {
                                        i = 0;
                                        auto it = suggestions.begin();
                                        while (i++ < number) {
                                            it++;
                                        }
                                        suggested = *it;
                                        if (match_word[0] < 'a') {
                                            suggested[0] = suggested[0] - 'a' + 'A';
                                        }
                                        fout << suggested;
                                        break;
                                    }
                                }
                            }

                            cout << REPLACE_ALL_STRING << endl;
                            bool bad_action4 = true;
                            string action_buf4;
                            while (bad_action4) {
                                getline(cin, action_buf4);
                                switch (action_buf4.back()) {
                                    case 'A':
                                    case 'a':
                                        replacements.emplace(word, suggested);
                                        bad_action4 = false;
                                        break;
                                    case 'S':
                                    case 's':
                                        bad_action4 = false;
                                        break;
                                    default:
                                        break;
                                }
                            }

                            bad_action1 = false;
                            break;
                        }
                        case_ignore:
                        case 'I':
                        case 'i': {
                            fout << match_word;
                            cout << IGNORE_ALL_STRING << endl;
                            bool bad_action3 = true;
                            string action_buf3;
                            while (bad_action3) {
                                getline(cin, action_buf3);
                                switch (action_buf3.back()) {
                                    case 'A':
                                    case 'a':
                                        ignored.insert(word);
                                        bad_action3 = false;
                                        break;
                                    case 'S':
                                    case 's':
                                        bad_action3 = false;
                                        break;
                                    default:
                                        break;
                                }
                            }
                            bad_action1 = false;
                            break;
                        }
                        case 'X':
                        case 'x':
                            fout << match_word;
                            skip = true;
                            bad_action1 = false;
                            break;
                        default:
                            break;
                    }
                }
            }
            else {
                fout << match_word;
            }
        }
        fout << prev_suffix << endl;
    }
}

void SpellChecker::print_sorted(const string &output) const {
    vector<string> sorted;
    for (auto it = 0; it != UINT16_MAX; it++) {
        for (auto it2 = hashset[it].begin(); it2 != hashset[it].end(); it2++) {
            sorted.emplace_back(*it2);
        }
    }
    sort(sorted.begin(), sorted.end());
    ofstream fout(output);
    for (auto it = sorted.begin(); it != sorted.end(); it++) {
        fout << *it << endl;
    }
}


void SpellChecker::suggest_insertion(const string &a_word, set<string> &suggestions) const {
    string word(a_word);
    int len = word.length();
    for (int i = 0; i <= len; i++) {
        // insert before i-th letter
        word.insert(i, 1, 'a');
        while (word[i] <= 'z') {
            if (d_search(word)) {
                suggestions.insert(word);
            }
            word[i]++;
        }
        word.erase(i, 1);
    }
}

void SpellChecker::suggest_replacement(const string &a_word, set<string> &suggestions) const {
    string word(a_word);
    int len = word.length();
    char temp;
    for (int i = 0; i < len; i++) {
        // replace i-th letter
        temp = word[i];
        for (char c = 'a'; c <= 'z'; c++) {
            // replace by c char
            if (c == temp) {
                // same word
                continue;
            }
            word[i] = c;
            if (d_search(word)) {
                suggestions.insert(word);
            }
        }
        word[i] = temp;
    }
}

void SpellChecker::suggest_odd(const string &a_word, set<string> &suggestions) const {
    string word(a_word);
    int len = word.length();
    char temp;
    for (int i = 0; i < len; i++) {
        // erase i-th letter
        temp = word[i];
        word.erase(i, 1);
        if (d_search(word)) {
            suggestions.insert(word);
        }
        // insert back
        word.insert(i, 1, temp);
    }
}

void SpellChecker::suggest_adjacent(const string &a_word, set<string> &suggestions) const {
    string word(a_word);
    int len = word.length();
    char temp;
    for (int i = 1; i < len; i++) {
        // swap i-1 -th and i-th letters
        temp = word[i - 1];
        word[i - 1] = word[i];
        word[i] = temp;
        if (d_search(word)) {
            suggestions.insert(word);
        }
        word[i] = word[i - 1];
        word[i - 1] = temp;
    }
}


unsigned char SpellChecker::hash(const std::string &str) const {
    // Rot13
    unsigned int hash = 0;
    int len = str.length();
    for (int i = 0; i < len; i++) {
        hash += (unsigned char) (str[i]);
        hash -= (hash << 13) | (hash >> 19);
    }
    return (unsigned char) hash;
}