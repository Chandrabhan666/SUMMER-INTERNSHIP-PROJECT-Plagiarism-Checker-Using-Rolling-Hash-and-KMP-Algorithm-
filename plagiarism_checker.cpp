#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

// Rolling Hash function to find matching substrings of length >= minLen
void rollingHashMatch(const string& text1, const string& text2, int minLen) {
    const int base = 256;
    const long long mod = 1e9 + 7;
    unordered_set<long long> hashes;
    unordered_set<string> matches;

    if (text1.length() < minLen || text2.length() < minLen) {
        cout << "One of the texts is too short for the chosen minimum length." << endl;
        return;
    }

    // Precompute base^(minLen-1) % mod
    long long highestPow = 1;
    for (int i = 0; i < minLen - 1; ++i) highestPow = (highestPow * base) % mod;

    // Hash all substrings of length minLen in text1
    long long hash1 = 0;
    for (int i = 0; i < minLen; ++i) hash1 = (hash1 * base + text1[i]) % mod;
    hashes.insert(hash1);
    for (size_t i = minLen; i < text1.length(); ++i) {
        hash1 = (hash1 - text1[i - minLen] * highestPow % mod + mod) % mod;
        hash1 = (hash1 * base + text1[i]) % mod;
        hashes.insert(hash1);
    }

    // Check for matching substrings in text2
    long long hash2 = 0;
    for (int i = 0; i < minLen; ++i) hash2 = (hash2 * base + text2[i]) % mod;
    if (hashes.count(hash2)) matches.insert(text2.substr(0, minLen));
    for (size_t i = minLen; i < text2.length(); ++i) {
        hash2 = (hash2 - text2[i - minLen] * highestPow % mod + mod) % mod;
        hash2 = (hash2 * base + text2[i]) % mod;
        if (hashes.count(hash2)) matches.insert(text2.substr(i - minLen + 1, minLen));
    }

    cout << "\n[Rolling Hash] Matching substrings of length >= " << minLen << ":\n";
    if (matches.empty()) {
        cout << "No matches found." << endl;
    } else {
        int count = 1;
        for (const auto& s : matches) {
            cout << count++ << ". " << s << endl;
        }
        cout << "Total matches: " << matches.size() << endl;
    }
}

// KMP helper: build prefix table
vector<int> buildKMPTable(const string& pattern) {
    int n = pattern.size();
    vector<int> lps(n, 0);
    int len = 0, i = 1;
    while (i < n) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else if (len) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }
    return lps;
}

// KMP function to find all matching substrings of length >= minLen
void kmpMatch(const string& text1, const string& text2, int minLen) {
    unordered_set<string> matches;
    for (size_t i = 0; i + minLen <= text1.size(); ++i) {
        string pattern = text1.substr(i, minLen);
        vector<int> lps = buildKMPTable(pattern);
        int j = 0, k = 0;
        while (k < (int)text2.size()) {
            if (pattern[j] == text2[k]) {
                ++j; ++k;
            }
            if (j == minLen) {
                matches.insert(pattern);
                j = lps[j - 1];
            } else if (k < (int)text2.size() && pattern[j] != text2[k]) {
                if (j) j = lps[j - 1];
                else ++k;
            }
        }
    }
    cout << "\n[KMP] Matching substrings of length >= " << minLen << ":\n";
    if (matches.empty()) {
        cout << "No matches found." << endl;
    } else {
        int count = 1;
        for (const auto& s : matches) {
            cout << count++ << ". " << s << endl;
        }
        cout << "Total matches: " << matches.size() << endl;
    }
}

int main() {
    string file1, file2;
    cout << "Enter the path for the first file: ";
    cin >> file1;
    cout << "Enter the path for the second file: ";
    cin >> file2;

    ifstream fin1(file1), fin2(file2);
    if (!fin1.is_open() || !fin2.is_open()) {
        cerr << "Error opening one of the files." << endl;
        return 1;
    }

    string text1((istreambuf_iterator<char>(fin1)), istreambuf_iterator<char>());
    string text2((istreambuf_iterator<char>(fin2)), istreambuf_iterator<char>());

    cout << "File 1 length: " << text1.length() << " characters." << endl;
    cout << "File 2 length: " << text2.length() << " characters." << endl;

    int minLen;
    cout << "Enter minimum matching substring length: ";
    cin >> minLen;

    // Rolling Hash
    unordered_set<string> rh_matches;
    const int base = 256;
    const long long mod = 1e9 + 7;
    unordered_set<long long> hashes;
    if (text1.length() >= minLen && text2.length() >= minLen) {
        long long highestPow = 1;
        for (int i = 0; i < minLen - 1; ++i) highestPow = (highestPow * base) % mod;
        long long hash1 = 0;
        for (int i = 0; i < minLen; ++i) hash1 = (hash1 * base + text1[i]) % mod;
        hashes.insert(hash1);
        for (size_t i = minLen; i < text1.length(); ++i) {
            hash1 = (hash1 - text1[i - minLen] * highestPow % mod + mod) % mod;
            hash1 = (hash1 * base + text1[i]) % mod;
            hashes.insert(hash1);
        }
        long long hash2 = 0;
        for (int i = 0; i < minLen; ++i) hash2 = (hash2 * base + text2[i]) % mod;
        if (hashes.count(hash2)) rh_matches.insert(text2.substr(0, minLen));
        for (size_t i = minLen; i < text2.length(); ++i) {
            hash2 = (hash2 - text2[i - minLen] * highestPow % mod + mod) % mod;
            hash2 = (hash2 * base + text2[i]) % mod;
            if (hashes.count(hash2)) rh_matches.insert(text2.substr(i - minLen + 1, minLen));
        }
    }
    cout << "\n[Rolling Hash] Matching substrings of length >= " << minLen << ":\n";
    if (rh_matches.empty()) {
        cout << "No matches found." << endl;
    } else {
        int count = 1;
        for (const auto& s : rh_matches) {
            cout << count++ << ". " << s << endl;
        }
        cout << "Total matches: " << rh_matches.size() << endl;
    }
    double rh_similarity = (double)rh_matches.size() / (double)(text1.length() + text2.length() - 2 * minLen + 2) * 100.0;
    cout << "[Rolling Hash] Similarity: " << rh_similarity << "%\n";

    // KMP
    unordered_set<string> kmp_matches;
    for (size_t i = 0; i + minLen <= text1.size(); ++i) {
        string pattern = text1.substr(i, minLen);
        vector<int> lps = buildKMPTable(pattern);
        int j = 0, k = 0;
        while (k < (int)text2.size()) {
            if (pattern[j] == text2[k]) {
                ++j; ++k;
            }
            if (j == minLen) {
                kmp_matches.insert(pattern);
                j = lps[j - 1];
            } else if (k < (int)text2.size() && pattern[j] != text2[k]) {
                if (j) j = lps[j - 1];
                else ++k;
            }
        }
    }
    cout << "\n[KMP] Matching substrings of length >= " << minLen << ":\n";
    if (kmp_matches.empty()) {
        cout << "No matches found." << endl;
    } else {
        int count = 1;
        for (const auto& s : kmp_matches) {
            cout << count++ << ". " << s << endl;
        }
        cout << "Total matches: " << kmp_matches.size() << endl;
    }
    double kmp_similarity = (double)kmp_matches.size() / (double)(text1.length() + text2.length() - 2 * minLen + 2) * 100.0;
    cout << "[KMP] Similarity: " << kmp_similarity << "%\n";

    return 0;
} 