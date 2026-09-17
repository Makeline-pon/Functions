#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

using ull = unsigned long long;

class BuchholzOrdinal;

class Function
{
public:
    std::string m;
    std::string n;
    ull id;
    ull stp;
    std::vector<std::string> f;
    bool exit;
    bool debug_mode;
    std::map<std::string,std::string> wco;
    std::map<std::string,std::string> eco;

    Function();
    void init();
    void setDebugMode(bool enable);
    void log(const std::string& msg) const;
    static std::string trim(const std::string& str);
    bool isZero(const std::string& s);
    std::string multiplyStrings(const std::string& num1, const std::string& num2);
    static bool isPureNumber(const std::string& s);
    void choose(Function& func);
    std::string Ackermann(std::string m, std::string n, int depth = 0);
    void Fibonacci(ull id);
    std::string sieve_euler(std::string n);
    std::string Catalan(std::string n_str);

private:
    static std::string trimLeadingZeros(const std::string& num);
    static int compareStrings(Function& func, const std::string& a, const std::string& b);
};