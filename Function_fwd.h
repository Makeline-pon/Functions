// Function_fwd.h - Forward declarations and Function class definition
// Defines the main Function class with public member variables and method declarations

#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

// Type alias for unsigned long long
using ull = unsigned long long;

// Forward declaration of BuchholzOrdinal class (defined in BuchOrdinal.h)
class BuchholzOrdinal;

// Main Function class: provides mathematical computation functionality
// Includes: Ackermann, Fibonacci, Euler's totient, Catalan numbers,
// and Buchholz Ordinal Collapsing Function (OCF)
class Function
{
public:
    // Member variables for function inputs and outputs
    std::string m;                        // Large number input m (Ackermann, etc.)
    std::string n;                        // Large number input n (Ackermann, etc.)
    ull id;                               // Index for Fibonacci and other indexed functions
    ull stp;                              // Step counter for tracking computation progress
    std::vector<std::string> f;           // Fibonacci sequence storage
    bool exit;                            // Program exit flag
    bool debug_mode;                      // Debug mode toggle
    std::map<std::string,std::string> wco; // Chinese number unit names (万字部首)
    std::map<std::string,std::string> eco; // English number unit names

    // Constructor and initialization
    Function();
    void init();                          // Initialize number unit mappings
    void setDebugMode(bool enable);       // Enable/disable debug logging
    void log(const std::string& msg) const; // Output debug message if enabled

    // Utility functions for string processing
    static std::string trim(const std::string& str); // Remove leading/trailing whitespace
    bool isZero(const std::string& s);              // Check if string represents zero

    // BigInteger arithmetic operations (delegates to BigNumber.h)
    std::string multiplyStrings(const std::string& num1, const std::string& num2);
    static bool isPureNumber(const std::string& s);  // Check if string contains only digits

    // Interactive menu: displays function list and processes user input
    void choose(Function& func);

    // ===== Mathematical Functions =====

    // Ackermann function: A(m, n) - a classic non-primitive recursive function
    // Returns result as string to support arbitrarily large outputs
    std::string Ackermann(std::string m, std::string n, int depth = 0);

    // Fibonacci sequence: computes f[1..id] with big integer arithmetic
    // Results stored in member variable f
    void Fibonacci(ull id);

    // Euler's totient function: counts integers ≤ n relatively prime to n
    // Uses prime factorization method: φ(n) = n * Π(1 - 1/p)
    std::string sieve_euler(std::string n);

    // Catalan number: C(n) = (2n)!/((n+1)!*n!)
    // Supports large n via big integer arithmetic
    std::string Catalan(std::string n_str);

private:
    // Internal utility functions (private)
    static std::string trimLeadingZeros(const std::string& num); // Remove leading zeros from number string
    static int compareStrings(Function& func, const std::string& a, const std::string& b); // Compare big integers
};
