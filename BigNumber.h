#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <memory>
#include <map>

using ull = unsigned long long;

// ===== BigNumber utility functions =====

inline std::string ull_to_string(ull x){
    if (x == 0) return "0";
    std::string res;
    res.reserve(20);
    while (x > 0) {
        res.push_back('0' + x % 10);
        x /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

inline std::string addStrings(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    if (b.empty()) return a;
    std::string res;
    res.reserve(std::max(a.size(), b.size()) + 1);
    int carry = 0;
    int i = (int)a.size() - 1;
    int j = (int)b.size() - 1;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        carry = sum / 10;
        res.push_back((sum % 10) + '0');
    }
    reverse(res.begin(), res.end());
    return res;
}

inline std::string subtractStrings(const std::string& a, const std::string& b) {
    if (a.empty()) return "0";
    if (b.empty()) return a;
    std::string res;
    res.reserve(a.size());
    int borrow = 0;
    int i = (int)a.size() - 1;
    int j = (int)b.size() - 1;
    while (i >= 0) {
        int digitA = a[i--] - '0' - borrow;
        int digitB = (j >= 0) ? (b[j--] - '0') : 0;
        borrow = 0;
        if (digitA < digitB) {
            digitA += 10;
            borrow = 1;
        }
        res.push_back((digitA - digitB) + '0');
    }
    while (res.size() > 1 && res.back() == '0') res.pop_back();
    reverse(res.begin(), res.end());
    return res;
}

inline std::string multiplyStrings(const std::string& num1, const std::string& num2){
    if (num1 == "0" || num2 == "0") return "0";
    const size_t n1 = num1.size();
    const size_t n2 = num2.size();
    std::vector<int> res(n1 + n2, 0);
    for (int i = (int)n1 - 1; i >= 0; --i) {
        int digit1 = num1[i] - '0';
        for (int j = (int)n2 - 1; j >= 0; --j) {
            int digit2 = num2[j] - '0';
            int mul = digit1 * digit2;
            int sum = mul + res[i + j + 1];
            res[i + j + 1] = sum % 10;
            res[i + j] += sum / 10;
        }
    }
    std::string finalRes;
    for (int num : res) {
        if (!finalRes.empty() || num != 0) {
            finalRes.push_back(num + '0');
        }
    }
    return finalRes.empty() ? "0" : finalRes;
}

inline std::string divideStringByInt(const std::string& num, int divisor){
    if (divisor == 0) return "";
    std::string result;
    result.reserve(num.size());
    int remainder = 0;
    for (char c : num) {
        int current = remainder * 10 + (c - '0');
        int quotientDigit = current / divisor;
        if (!result.empty() || quotientDigit != 0) {
            result.push_back(quotientDigit + '0');
        }
        remainder = current % divisor;
    }
    return result.empty() ? "0" : result;
}

inline std::string bigPowString(std::string baseStr, std::string expStr){
    if (expStr == "0") return "1";
    if (baseStr == "0") return "0";
    if (baseStr == "1") return "1";
    if (expStr[0] == '-') return "0";
    std::string result = "1";
    std::string base = baseStr;
    std::string exp = expStr;
    while (exp != "0") {
        char lastChar = exp.back();
        bool isOdd = (lastChar == '1' || lastChar == '3' || lastChar == '5' ||
                      lastChar == '7' || lastChar == '9');
        if (isOdd) {
            result = multiplyStrings(result, base);
        }
        base = multiplyStrings(base, base);
        exp = divideStringByInt(exp, 2);
    }
    return result;
}

inline std::string factorial(ull n){
    if (n == 0 || n == 1) return "1";
    std::string result = "1";
    for (ull i = 2; i <= n; ++i) {
        result = multiplyStrings(result, std::to_string(i));
    }
    return result;
}

inline std::string addInt(const std::string& a, int b){
    if (b == 0) return a;
    std::string res = a;
    int carry = b;
    for (int i = res.size() - 1; i >= 0 && carry > 0; --i) {
        int sum = (res[i] - '0') + carry;
        res[i] = '0' + sum % 10;
        carry = sum / 10;
    }
    while (carry) {
        res.insert(res.begin(), '0' + carry % 10);
        carry /= 10;
    }
    return res;
}

inline std::string subInt(const std::string& a, int b){
    std::string res = a;
    int borrow = b;
    for (int i = res.size() - 1; i >= 0 && borrow > 0; --i) {
        int diff = (res[i] - '0') - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res[i] = '0' + diff;
    }
    size_t start = res.find_first_not_of('0');
    if (start == std::string::npos) return "0";
    return res.substr(start);
}

inline std::string mulInt(const std::string& a, int b){
    if (b == 0) return "0";
    std::string res;
    int carry = 0;
    for (int i = (int)a.size() - 1; i >= 0; --i) {
        int product = (a[i] - '0') * b + carry;
        res.push_back('0' + product % 10);
        carry = product / 10;
    }
    while (carry) {
        res.push_back('0' + carry % 10);
        carry /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

inline std::pair<std::string, int> divInt(const std::string& a, int b){
    if (b == 0 || a.empty()) return {"", -1};
    size_t startPos = a.find_first_not_of('0');
    if (startPos == std::string::npos) return {"0", 0};
    std::string res;
    int remainder = 0;
    for (char c : a.substr(startPos)) {
        int current = remainder * 10 + (c - '0');
        res.push_back('0' + current / b);
        remainder = current % b;
    }
    size_t start = res.find_first_not_of('0');
    if (start == std::string::npos) return {"0", remainder};
    return {res.substr(start), remainder};
}

inline bool isGreater(const std::string& s1, const std::string& s2){
    if (s1.length() != s2.length())
        return s1.length() > s2.length();
    return s1 > s2;
}

inline bool isEqual(const std::string& s1, const std::string& s2) {
    auto strip = [](const std::string& s) -> std::string {
        size_t start = s.find_first_not_of('0');
        return (start == std::string::npos) ? "0" : s.substr(start);
    };
    return strip(s1) == strip(s2);
}

inline std::string inc(const std::string& s){
    return addStrings(s, "1");
}

inline std::string dec(const std::string& s){
    return subtractStrings(s, "1");
}

inline ull stringToUll(const std::string& s){
    if (s.length() > 20) {
        throw std::overflow_error("Input n is too large for iterative calculation.");
    }
    try {
        return stoull(s);
    } catch (...) {
        throw std::overflow_error("Invalid or too large input for n.");
    }
}

inline std::string ifss(ull input, int precision = 4){
    std::ostringstream oss;
    oss << std::scientific
        << std::setprecision(precision)
        << static_cast<double>(input);
    return oss.str();
}

inline std::string fss(const std::string& input, int precision = 4){
    if (input.length() <= 15) {
        try {
            ull val = stoull(input);
            std::ostringstream oss;
            oss << std::scientific << std::setprecision(precision) << static_cast<double>(val);
            return oss.str();
        } catch (...) {
            return input;
        }
    } else {
        std::string mantissa = input.substr(0, precision + 1);
        int exp = input.length() - 1;
        return mantissa + "e+" + std::to_string(exp);
    }
}

inline bool isPureNumber(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

inline std::string trim(const std::string& str) {
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}
