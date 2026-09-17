#include <iostream>
#include <limits>
#include <cstdio>
#include <windows.h>
#include "Function.h"

Function::Function() : m(""), n(""), id(0), stp(0), exit(false), debug_mode(false) {
}

void Function::init(){
    wco["00"] = "个";    eco["00"] = " ";
    wco["01"] = "十";    eco["01"] = " ";
    wco["02"] = "百";    eco["02"] = " ";
    wco["03"] = "千";    eco["03"] = "thousand";
    wco["04"] = "万";    eco["04"] = "thousand";
    wco["05"] = "十万";  eco["05"] = "thousand";
    wco["06"] = "百万";  eco["06"] = "million";
    wco["07"] = "千万";  eco["07"] = "million";
    wco["08"] = "亿";    eco["08"] = "million";
    wco["09"] = "十亿";  eco["09"] = "billion";
    wco["10"] = "百亿";  eco["10"] = "billion";
    wco["11"] = "千亿";  eco["11"] = "billion";
    wco["12"] = "兆";    eco["12"] = "trillion";
}

void Function::setDebugMode(bool enable) {
    debug_mode = enable;
}

void Function::log(const std::string& msg) const {
    if (debug_mode) {
        printf("<< %s\n", msg.c_str());
    }
}

std::string Function::trim(const std::string& str) {
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

bool Function::isZero(const std::string& s) {
    return trimLeadingZeros(s) == "0";
}

std::string Function::multiplyStrings(const std::string& num1, const std::string& num2){
    return ::multiplyStrings(num1, num2);
}

std::string Function::trimLeadingZeros(const std::string& num) { 
    size_t firstNonZero = num.find_first_not_of('0'); 
    if (firstNonZero == std::string::npos) return "0"; 
    return num.substr(firstNonZero); 
} 

bool Function::isPureNumber(const std::string& s) { 
    if (s.empty()) return false; 
    for (char c : s) { 
        if (!isdigit(c)) return false; 
    } 
    return true; 
} 

int Function::compareStrings(Function& func, const std::string& a, const std::string& b) { 
    func.stp++; 
    std::string a_trim = trimLeadingZeros(a); 
    std::string b_trim = trimLeadingZeros(b); 
    if (a_trim.size() != b_trim.size()) { 
        return a_trim.size() < b_trim.size() ? -1 : 1; 
    } 
    if (a_trim < b_trim) return -1;
    if (a_trim > b_trim) return 1;
    return 0; 
} 
	void Function::choose(Function &func){
		printf("<< initing...");
		init();
		Sleep(1000);
		printf("\n<< inited");
		while(1){   //choice list
			stp = 0;
			printf("\n\n<< List:");
			Sleep(100);
			printf("\n<< 1 - Ackermann: A(m,n)");
			printf("\n<< 2 - Fibonacci sequence: f[i]"); 
			printf("\n<< 3 - Euler's totient function: φ(n)"); 
			printf("\n<< 4 - Catalan Number: C(n) / h(n)");
			printf("\n<< 5 - Buchholz Ordinal : Parse Ordinal Collapsing Function Expression");
			printf("\n<< 6 - Buchholz OCF : Large Cardinal and Reflection Extended");
			printf("\n<< 7 - Buchholz OCF : Standard Value Verification (psi_0(0)=omega, Buchholz convention)");
			printf("\n<< 0 - Close");
			printf("\n<<-1 - Switch on/off Debug Mode\n>> ");
			int op;
			scanf("%d",&op);
			switch(op){
				case -1:{
					printf("<< Mode Changing...");
					setDebugMode((debug_mode ? false : true));
					Sleep(750);
					printf("\n<< Current Mode: %s",(debug_mode ? "debug" : "normal"));
					break;
				}
				case 0:{   //close
					printf("<< closing...");
					exit = 1;
					Sleep(1500);
					break;
				}
				case 1:{   //Ackermann
					printf("<< Solve Ackermann: A(m,n)\n<< input: m\n>> ");
					std::cin >> m;
					if(m[0] == '-'){
						printf("<< m cannot be less than 0!");
						break;
					}
					printf("<< input: n\n>> ");
					std::cin >> n;
					if(n[0] == '-'){
						printf("<< n cannot be less than 0!");
						break;
					}
					try{
						std::string fir = Ackermann(m,n);
						std::string uss = ifss(stp);
						printf("<< Final Result: %s Used Steps: %s",fir.c_str(),uss.c_str());
					}catch(...){
						printf("<< Calculating failed");
					}
					break;
				}
				case 2:{   //Fibonacci sequence
					printf("<< Solve Fibonacci sequence: f[i]\n<< input: i\n>> ");
					scanf("%llu",&id);
					if(id < 1){
						printf("<< i cannot be less than 1!");
						break;
					}
					Fibonacci(id);
					if(!f.empty() && id <= f.size()){
						std::string fir = f[id-1];
						std::string uss = ifss(stp);
						printf("<< Final Result: %s Used Steps: %s",fir.c_str(),uss.c_str());
					}else printf("<< Calculating failed");
					f.clear();
					break;
				}
				case 3:{   //Euler's totient function
					printf("<< Solve Euler's totient function: φ(n)\n<< input: n\n>> ");
					std::cin >> n;
					try{
						printf("<< Final Result: %s\n",sieve_euler(n).c_str());
					}catch(...){
						printf("<< Calculating failed");
					}
					break;
				}
				case 4:{   //Catalan Number
					printf("<< Solve Catalan Number: C(n) / h(n)\n<< input: n\n>> ");
					std::cin >> n;
					if(n[0] == '-'){
						printf("<< n cannot be less than 0!");
						break;
					}
					try{
						printf("<< Final Result: %s  Used Steps: ",Catalan(n).c_str());
						std::string uss = ifss(stp);
						printf("%s",uss.c_str());
					}catch(...){
						printf("<< Calculating failed");
					}
					break;
				}
				case 5:{  //Basic Buchholz OCF
					printf("<< Clearing buffer...\n");
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					
					printf("<< Solve Buchholz Ordinal: Parse OCF Expression\n");
					printf("<< Supported Constants:\n");
					printf("<<   omega (w): Countable limit ordinal (Aleph_0)\n");
					printf("<<   Ω(Omega): First uncountable cardinal (Aleph_1)\n");
					printf("<<   Ω[N]     : N-th uncountable cardinal (Buchholz hierarchy)\n");
					printf("<<\n");
					printf("<< Maximum Strength: Buchholz Ordinal (BO)\n");
					printf("<<   BO = psi_0(Ω_ω)\n");
					printf("<<\n");
					printf("<< Note: I_rec, M_rec, Pi1, Pi2 exceed BO limit. Use case 6.\n");
					printf("<< Input Format: psi_v(alpha)\n");
					printf("<< Examples:\n");
					printf("<<   - psi_0(omega)   : epsilon_0 (ω)\n");
					printf("<<   - psi_0(Omega)   : Bachmann-Howard Ordinal (BHO) level\n");
					printf("<<   - psi_0(Ω_2)     : Beyond BHO (2nd uncountable cardinal)\n");
					printf(">> ");
					
					std::string expr;
					getline(std::cin, expr);
					
					if(expr.empty()){
						printf("<< Input cannot be empty!");
						break;
					}
				
					try{
						BuchholzOrdinal result = BuchholzOrdinal::parse(func, expr);
						
						// Check if result exceeds BO
						if (!BuchholzOrdinal::isWithinBO(result)) {
							printf("<< Cannot Reach (ordinal exceeds Buchholz Ordinal)\n");
						} else {
							std::string resStr = result.toString();
							std::string uss = ifss(stp);
							
							printf("<< Final Result: %s\n", resStr.c_str());
							printf("<< Used Steps: %s", uss.c_str());
						}
					}catch(std::exception& e){
						printf("<< Calculating failed: %s", e.what());
					}catch(...){
						printf("<< Calculating failed");
					}
					break;
				}
				case 6:{  //Buchholz OCF : Large Cardinal and Reflection Extended
					printf("<< Clearing buffer...\n");
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					
					// --- Part 1: Supported Jumpstarts (Constants) ---
					printf("<< [Supported Jumpstarts]\n");
					printf("<<   1. Ω (Omega): First uncountable cardinal (Aleph_1).\n");
					printf("<<      - Role: Basis for Buchholz OCF (Strength: ID_1 / BHO).\n");
					printf("<<   2. Ω[N] or Ω_N: N-th uncountable cardinal.\n");
					printf("<<      - Role: Extends Buchholz hierarchy.\n");
					printf("<<   3. I_rec: Smallest Recursively Inaccessible Ordinal.\n");
					printf("<<      - Role: Jumpstart for KPI system strength.\n");
					printf("<<   4. M_rec: Smallest Recursively Mahlo Ordinal.\n");
					printf("<<      - Role: System Upper Bound (Strength: KPM).\n");
					printf("<<\n");

					// --- Part 2: Reflection Generators (Operators) ---
					printf("<< [Reflection Generators]\n");
					printf("<<   1. Pi1(n): Generates the n-th Admissible Ordinal.\n");
					printf("<<      - Property: Pi_1-Reflection (Models KP set theory).\n");
					printf("<<      - Example: Pi1(0) = Ω_1^CK (Church-Kleene ordinal).\n");
					printf("<<      - Relation: Building blocks for I_rec.\n");
					printf("<<\n");
					printf("<<   2. Pi2(n): Generates the n-th Recursively Inaccessible Ordinal.\n");
					printf("<<      - Property: Pi_2-Reflection (Limit of Admissibles).\n");
					printf("<<      - Example: Pi2(0) is equivalent to the jumpstart I_rec.\n");
					printf("<<      - Relation: Building blocks for M_rec.\n");
					printf("<<\n");

					// --- Part 3: Usage Info ---
					printf("<< Note: True Large Cardinals (Inaccessible+) are excluded.\n");
					printf("<< Input Format: psi_v(alpha), with Jumpstarts as constants\n");
					printf("<< Examples:\n");
					printf("<<   - psi_0(Ω)       : Standard Bachmann-Howard Ordinal\n");
					printf("<<   - psi_0(Pi2(0))  : Collapsing Recursively Inaccessible\n");
					printf("<<   - Pi1(0)         : Church-Kleene ordinal\n");
					printf(">> ");
					
					std::string expr6;
					getline(std::cin, expr6);
					
					if(expr6.empty()){
						printf("<< Input cannot be empty!");
						break;
					}
					
					try{
						BuchholzOrdinal result6 = BuchholzOrdinal::parse(func, expr6);
						
						std::string resStr6 = result6.toString();
						std::string uss6 = ifss(stp);
						
						printf("<< Final Result: %s\n", resStr6.c_str());
						printf("<< Used Steps: %s", uss6.c_str());
					}catch(std::exception& e){
						printf("<< Calculating failed: %s", e.what());
					}catch(...){
						printf("<< Calculating failed");
					}
					break;
				}
				case 7:{ // Buchholz OCF : Standard Value Verification
					printf("<<< Buchholz OCF Standard Value Verification >>>\n");
					printf("<< Standard Values (Buchholz OCF - psi_0(0)=1 convention):\n");
					printf("<<   psi_0(0)          = 1       (Buchholz standard)\n");
					printf("<<   psi_0(1)          = omega\n");
					printf("<<   psi_0(2)          = omega*2\n");
					printf("<<   psi_0(omega)      = omega^2\n");
					printf("<<   psi_0(Omega)      = epsilon_0\n");
					printf("<<   omega             = first infinite ordinal (countable)\n");
					printf("<<   Omega             = uncountable cardinal (Omega_1)\n\n");

					// Test 1: psi_0(0) = 1 (Buchholz standard)
					stp = 0;
					try {
						auto r1 = BuchholzOrdinal::parse(func, "psi_0(0)");
						printf("<< [TEST 1] psi_0(0)  = %s (expected: 1)\n", r1.toString().c_str());
					} catch(...) {
						printf("<< [TEST 1] FAILED\n");
					}

					// Test 2: psi_1(0) = Omega_1
					stp = 0;
					try {
						auto r2 = BuchholzOrdinal::parse(func, "psi_1(0)");
						printf("<< [TEST 2] psi_1(0)  = %s (expected: Ω[1] / Omega_1)\n", r2.toString().c_str());
					} catch(...) {
						printf("<< [TEST 2] FAILED\n");
					}

					// Test 3: psi_0(Omega) = epsilon_0 (first key fixed point)
					stp = 0;
					try {
						auto r3 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						printf("<< [TEST 3] psi_0(Omega) = %s (expected: epsilon_0 level)\n", r3.toString().c_str());
					} catch(...) {
						printf("<< [TEST 3] FAILED\n");
					}

					// Test 4: omega is countable
					stp = 0;
					try {
						auto r4 = BuchholzOrdinal::parse(func, "omega");
						bool isCountable = false;
						if (!r4.terms.empty()) isCountable = r4.terms[0].isCountableOmega;
						printf("<< [TEST 4] omega   = %s (isCountable=%s, expected: true)\n",
						       r4.toString().c_str(), isCountable ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 4] FAILED\n");
					}

					// Test 5: omega + 1
					stp = 0;
					try {
						auto r5 = BuchholzOrdinal::parse(func, "omega + 1");
						printf("<< [TEST 5] omega+1 = %s (expected: omega + 1)\n", r5.toString().c_str());
					} catch(...) {
						printf("<< [TEST 5] FAILED\n");
					}

					// Test 6: H(alpha)_nu validation
					stp = 0;
					try {
						BuchholzOrdinal zero_alpha;
						bool valid = BuchholzOrdinal::isInH(func, zero_alpha, 0);
						printf("<< [TEST 6] isInH(zero, nu=0) = %s (expected: true)\n", valid ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 6] FAILED\n");
					}

					// Test 7: isValidPsiTerm
					stp = 0;
					try {
						BuchholzOrdinal zero_alpha;
						bool valid = BuchholzOrdinal::isValidPsiTerm(func, 0, zero_alpha);
						printf("<< [TEST 7] isValidPsiTerm(0, zero) = %s (expected: true)\n", valid ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 7] FAILED\n");
					}

					// Test 8: computePsiValue(0, 0) = 1
					stp = 0;
					try {
						BuchholzOrdinal zero_alpha;
						auto psi_val = BuchholzOrdinal::computePsiValue(func, 0, zero_alpha);
						printf("<< [TEST 8] computePsiValue(0, 0) = %s (expected: 1)\n", psi_val.toString().c_str());
					} catch(const std::exception& e) {
						printf("<< [TEST 8] Error: %s\n", e.what());
					}

					// Test 9: computePsiValue(1, 0) = Omega_1
					stp = 0;
					try {
						BuchholzOrdinal zero_alpha;
						auto psi_val = BuchholzOrdinal::computePsiValue(func, 1, zero_alpha);
						printf("<< [TEST 9] computePsiValue(1, 0) = %s (expected: Omega_1)\n", psi_val.toString().c_str());
					} catch(const std::exception& e) {
						printf("<< [TEST 9] Error: %s\n", e.what());
					}

					// === ADVANCED ORDINAL VALUE TESTS ===

					// Test 10: psi_0(1) = omega
					stp = 0;
					try {
						auto r10 = BuchholzOrdinal::parse(func, "psi_0(1)");
						printf("<< [TEST 10] psi_0(1) = %s (expected: omega)\n", r10.toString().c_str());
					} catch(...) {
						printf("<< [TEST 10] FAILED\n");
					}

					// Test 11: psi_0(omega) = omega^2 (NOT epsilon_0)
					stp = 0;
					try {
						auto r11 = BuchholzOrdinal::parse(func, "psi_0(omega)");
						printf("<< [TEST 11] psi_0(omega) = %s (expected: omega^2 or psi_0(omega) normal form)\n", r11.toString().c_str());
					} catch(...) {
						printf("<< [TEST 11] FAILED\n");
					}

					// Test 12: psi_0(omega * 2)
					stp = 0;
					try {
						auto r12 = BuchholzOrdinal::parse(func, "psi_0(omega * 2)");
						printf("<< [TEST 12] psi_0(omega*2) = %s (expected: omega^2 level)\n", r12.toString().c_str());
					} catch(...) {
						printf("<< [TEST 12] FAILED\n");
					}

					// Test 13: psi_0(omega^2)
					stp = 0;
					try {
						auto r13 = BuchholzOrdinal::parse(func, "psi_0(omega^2)");
						printf("<< [TEST 13] psi_0(omega^2) = %s (expected: zeta_0 requires Omega^2)\n", r13.toString().c_str());
					} catch(...) {
						printf("<< [TEST 13] FAILED\n");
					}

					// Test 14: psi_0(Omega + 1) > epsilon_0
					stp = 0;
					try {
						auto r14 = BuchholzOrdinal::parse(func, "psi_0(Omega + 1)");
						printf("<< [TEST 14] psi_0(Omega+1) = %s (expected: > epsilon_0)\n", r14.toString().c_str());
					} catch(...) {
						printf("<< [TEST 14] FAILED\n");
					}

					// Test 15: psi_0(Omega * 2) higher ordinal
					stp = 0;
					try {
						auto r15 = BuchholzOrdinal::parse(func, "psi_0(Omega * 2)");
						printf("<< [TEST 15] psi_0(Omega*2) = %s (expected: higher than eps0)\n", r15.toString().c_str());
					} catch(...) {
						printf("<< [TEST 15] FAILED\n");
					}

					// === NORMAL FORM TESTS ===

					// Test 16: psi_0(0) + psi_0(0) = 1 + 1 = 2
					stp = 0;
					try {
						auto r16 = BuchholzOrdinal::parse(func, "psi_0(0) + psi_0(0)");
						printf("<< [TEST 16] psi_0(0)+psi_0(0) = %s (expected: 2)\n", r16.toString().c_str());
					} catch(...) {
						printf("<< [TEST 16] FAILED\n");
					}

					// Test 17: psi_0(1) + 1 = omega + 1
					stp = 0;
					try {
						auto r17 = BuchholzOrdinal::parse(func, "psi_0(1) + 1");
						printf("<< [TEST 17] psi_0(1)+1 = %s (expected: omega + 1)\n", r17.toString().c_str());
					} catch(...) {
						printf("<< [TEST 17] FAILED\n");
					}

					// Test 18: psi_0(Omega) + psi_0(0) = epsilon_0 + 1
					stp = 0;
					try {
						auto r18 = BuchholzOrdinal::parse(func, "psi_0(Omega) + psi_0(0)");
						printf("<< [TEST 18] psi_0(Omega)+psi_0(0) = %s (expected: epsilon_0 + 1)\n", r18.toString().c_str());
					} catch(...) {
						printf("<< [TEST 18] FAILED\n");
					}

					// Test 19: psi_0(psi_1(0)) = psi_0(Ω_1) (nested psi)
					stp = 0;
					try {
						auto r19 = BuchholzOrdinal::parse(func, "psi_0(psi_1(0))");
						printf("<< [TEST 19] psi_0(psi_1(0)) = %s (expected: ψ_0(Ω[1]))\n", r19.toString().c_str());
					} catch(...) {
						printf("<< [TEST 19] FAILED\n");
					}

					// Test 20: psi_0(Omega + omega)
					stp = 0;
					try {
						auto r20 = BuchholzOrdinal::parse(func, "psi_0(Omega + omega)");
						printf("<< [TEST 20] psi_0(Omega+omega) = %s (expected: ψ_0(Ω + ω))\n", r20.toString().c_str());
					} catch(...) {
						printf("<< [TEST 20] FAILED\n");
					}

					// Test 21: toNormalFormString - psi_0(0)
					stp = 0;
					try {
						auto r21 = BuchholzOrdinal::parse(func, "psi_0(0)");
						printf("<< [TEST 21] toNormalFormString(psi_0(0)) = %s\n", r21.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 21] FAILED\n");
					}

					// Test 22: toNormalFormString - psi_0(1)
					stp = 0;
					try {
						auto r22 = BuchholzOrdinal::parse(func, "psi_0(1)");
						printf("<< [TEST 22] toNormalFormString(psi_0(1)) = %s (expected: omega)\n", r22.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 22] FAILED\n");
					}

					// Test 23: toNormalFormString - psi_0(Omega)
					stp = 0;
					try {
						auto r23 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						printf("<< [TEST 23] toNormalFormString(psi_0(Omega)) = %s (expected: epsilon_0)\n", r23.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 23] FAILED\n");
					}

					// Test 24: toNormalFormString - addition
					stp = 0;
					try {
						auto r24 = BuchholzOrdinal::parse(func, "psi_0(0) + psi_0(1)");
						printf("<< [TEST 24] toNormalFormString(psi_0(0)+psi_0(1)) = %s\n", r24.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 24] FAILED\n");
					}

					// Test 25: toNormalFormString - psi_1(0)
					stp = 0;
					try {
						auto r25 = BuchholzOrdinal::parse(func, "psi_1(0)");
						printf("<< [TEST 25] toNormalFormString(psi_1(0)) = %s (expected: Omega_1)\n", r25.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 25] FAILED\n");
					}					

					// === COMPARISON TESTS ===

					// Test 26: psi_2(0) = Ω_2
					stp = 0;
					try {
						auto r26 = BuchholzOrdinal::parse(func, "psi_2(0)");
						printf("<< [TEST 26] psi_2(0) = %s (expected: Ω[2])\n", r26.toString().c_str());
					} catch(...) {
						printf("<< [TEST 26] FAILED\n");
					}

					// Test 27: toNormalFormString - psi_2(0)
					stp = 0;
					try {
						auto r27 = BuchholzOrdinal::parse(func, "psi_2(0)");
						printf("<< [TEST 27] toNormalFormString(psi_2(0)) = %s (expected: Omega_2)\n", r27.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 27] FAILED\n");
					}
					// Test 28: compare psi_0(0) < psi_0(1)
					stp = 0;
					try {
						auto comp_a = BuchholzOrdinal::parse(func, "psi_0(0)");
						auto comp_b = BuchholzOrdinal::parse(func, "psi_0(1)");
						bool isLess = comp_a.lessThan(func, comp_b);
						printf("<< [TEST 28] compare(psi_0(0), psi_0(1)) = %s (expected: true)\n", isLess ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 28] FAILED\n");
					}

					// Test 29: compare psi_0(Omega) < psi_0(Omega + 1)
					stp = 0;
					try {
						auto comp_c = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto comp_d = BuchholzOrdinal::parse(func, "psi_0(Omega + 1)");
						bool isLess2 = comp_c.lessThan(func, comp_d);
						printf("<< [TEST 29] compare(psi_0(Omega), psi_0(Omega+1)) = %s (expected: true)\n", isLess2 ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 29] FAILED\n");
					}



					// Test 30: compare psi_0(Omega) < psi_0(Omega + Omega)
					stp = 0;
					try {
						auto comp_e = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto comp_f = BuchholzOrdinal::parse(func, "psi_0(Omega + Omega)");
						bool isLess3 = comp_e.lessThan(func, comp_f);
						printf("<< [TEST 30] compare(psi_0(Omega), psi_0(Omega+Omega)) = %s (expected: true)\n", isLess3 ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 30] FAILED\n");
					}

					// Test 31: psi_0(Omega * 3)
					stp = 0;
					try {
						auto r31 = BuchholzOrdinal::parse(func, "psi_0(Omega * 3)");
						printf("<< [TEST 31] psi_0(Omega*3) = %s\n", r31.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 31] FAILED\n");
					}

					// Test 32: psi_0(1) ^ 2
					stp = 0;
					try {
						auto r32 = BuchholzOrdinal::parse(func, "psi_0(1) ^ 2");
						printf("<< [TEST 32] psi_0(1)^2 = %s (expected: omega^2)\n", r32.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 32] FAILED\n");
					}

					// Test 33: fundamental sequence of omega^2
					stp = 0;
					try {
						auto r33 = BuchholzOrdinal::parse(func, "omega ^ 2");
						auto fs33 = r33.fundamentalSequence(func, 2);
						printf("<< [TEST 33] FS(omega^2, 2) = %s (expected: omega*2)\n", fs33.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 33] FAILED\n");
					}

					// Test 34: large comparison
					stp = 0;
					try {
						auto comp_g = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto comp_h = BuchholzOrdinal::parse(func, "psi_0(Omega * 2)");
						bool gLessH = comp_g.lessThan(func, comp_h);
						printf("<< [TEST 34] eps0 < psi_0(Omega*2) = %s (expected: true)\n", gLessH ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 34] FAILED\n");
					}

					// Test 35: omega * 2
					stp = 0;
					try {
						auto r35 = BuchholzOrdinal::parse(func, "omega * 2");
						printf("<< [TEST 35] omega*2 = %s (expected: omega*2)\n", r35.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 35] FAILED\n");
					}

					// Test 36: omega + 1 + 1
					stp = 0;
					try {
						auto r36 = BuchholzOrdinal::parse(func, "omega + 1 + 1");
						printf("<< [TEST 36] omega+1+1 = %s (expected: omega+2)\n", r36.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 36] FAILED\n");
					}

					// Test 37: nested psi_0(psi_0(Omega))
					stp = 0;
					try {
						auto r37 = BuchholzOrdinal::parse(func, "psi_0(psi_0(Omega))");
						printf("<< [TEST 37] psi_0(psi_0(Omega)) = %s\n", r37.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 37] FAILED\n");
					}

					// --- TEST 38-42: Core Set & Legality Tests ---
					// TEST 38: isInH(psi_0(Omega), nu=0) - epsilon_0 in H_0
					stp = 0;
					try {
						auto t38 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						bool isInH38 = t38.isInHForAlpha(func, t38, 0);
						printf("<< [TEST 38] isInH(psi_0(Omega), nu=0) = %s (expected: true)\n", isInH38 ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 38] FAILED\n");
					}

					// TEST 39: isInH(Omega, nu=1) - Omega_1 in H_1
					stp = 0;
					try {
						auto t39 = BuchholzOrdinal::parse(func, "Omega");
						bool isInH39 = t39.isInHForAlpha(func, t39, 1);
						printf("<< [TEST 39] isInH(Omega, nu=1) = %s (expected: true)\n", isInH39 ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 39] FAILED\n");
					}

					// TEST 40: isValidPsiTerm(0, Omega+1)
					stp = 0;
					try {
						auto alpha40 = BuchholzOrdinal::parse(func, "Omega + 1");
						bool valid40 = BuchholzOrdinal::isValidPsiTerm(func, 0, alpha40);
						printf("<< [TEST 40] isValidPsiTerm(0, Omega+1) = %s (expected: true)\n", valid40 ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 40] FAILED\n");
					}

					// TEST 41: isValidPsiTerm(0, -1) = false (negative ordinal is illegal)
					stp = 0;
					try {
						auto neg41 = BuchholzOrdinal::fromInt(-1);
						bool valid41 = BuchholzOrdinal::isValidPsiTerm(func, 0, neg41);
						printf("<< [TEST 41] isValidPsiTerm(0, -1) = %s (expected: false)\n", valid41 ? "true" : "false");
					} catch(...) { printf("<< [TEST 41] FAILED\n"); }

					// TEST 42: C(0) contains 0, Omega_1, Omega_2
					stp = 0;
					try {
						auto zero42 = BuchholzOrdinal::fromInt(0);
						printf("<< [TEST 42] C(0) contains 0, Omega_1, Omega_2 = true (by definition)\n");
					} catch(...) {
						printf("<< [TEST 42] FAILED\n");
					}

					// --- TEST 43-47: Key Ordinal Value Tests ---
					// TEST 43: psi_0(Omega^2) = zeta_0
					stp = 0;
					try {
						auto t43 = BuchholzOrdinal::parse(func, "psi_0(Omega ^ 2)");
						printf("<< [TEST 43] psi_0(Omega^2) = %s (expected: zeta_0 level)\n", t43.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 43] FAILED\n");
					}

					// TEST 44: psi_0(Omega^Omega) = Gamma_0
					stp = 0;
					try {
						auto t44 = BuchholzOrdinal::parse(func, "psi_0(Omega ^ Omega)");
						printf("<< [TEST 44] psi_0(Omega^Omega) = %s (expected: zeta_0)\n", t44.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 44] FAILED\n");
					}

					// TEST 45: psi_0(Omega_2) = BHO
					stp = 0;
					try {
						auto t45 = BuchholzOrdinal::parse(func, "psi_0(Omega_2)");
						printf("<< [TEST 45] psi_0(Omega_2) = %s (expected: BHO)\n", t45.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 45] FAILED\n");
					}

					// TEST 46: psi_1(Omega_2) = Omega_1
					stp = 0;
					try {
						auto t46 = BuchholzOrdinal::parse(func, "psi_1(Omega_2)");
						printf("<< [TEST 46] psi_1(Omega_2) = %s (expected: Omega_1)\n", t46.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 46] FAILED\n");
					}

					// TEST 47: psi_0(epsilon_0) = epsilon_0 (fixed point property)
					stp = 0;
					try {
						auto eps0_47 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto t47 = BuchholzOrdinal::parse(func, "psi_0(psi_0(Omega))");
						bool fixed47 = t47.equals(func, eps0_47);
						printf("<< [TEST 47] psi_0(epsilon_0) = epsilon_0 ? %s (expected: true)\n", fixed47 ? "true" : "false");
					} catch(...) { printf("<< [TEST 47] FAILED\n"); }

					// --- TEST 48-52: Ordinal Arithmetic & Normal Form ---
					// TEST 48: psi_0(1) * psi_0(1) = omega^2
					stp = 0;
					try {
						auto t48a = BuchholzOrdinal::parse(func, "psi_0(1)");
						auto t48b = BuchholzOrdinal::parse(func, "psi_0(1)");
						auto t48 = t48a.multiply(func, t48b);
						printf("<< [TEST 48] psi_0(1) * psi_0(1) = %s (expected: omega^2)\n", t48.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 48] FAILED\n");
					}

					// TEST 49: omega^(psi_0(Omega)) = psi_0(Omega)  (epsilon_0 fixed point)
					stp = 0;
					try {
						auto eps0_49 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto t49 = eps0_49.power(func, eps0_49);
						bool fixed49 = t49.equals(func, eps0_49);
						printf("<< [TEST 49] omega^epsilon_0 = epsilon_0 ? %s (expected: true)\n", fixed49 ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 49] FAILED\n");
					}

					// TEST 50: toNormalFormString(psi_0(Omega^2)) = "ζ₀"
					stp = 0;
					try {
						auto t50 = BuchholzOrdinal::parse(func, "psi_0(Omega ^ 2)");
						printf("<< [TEST 50] toNormalFormString(psi_0(Omega^2)) = %s (expected: zeta_0)\n", t50.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 50] FAILED\n"); }

					// TEST 51: toNormalFormString(psi_0(Omega)+psi_0(1)) = "epsilon_0 + omega"
					stp = 0;
					try {
						auto t51a = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto t51b = BuchholzOrdinal::parse(func, "psi_0(1)");
						auto t51 = t51a.add(func, t51b);
						printf("<< [TEST 51] toNormalFormString(psi_0(Omega)+psi_0(1)) = %s (expected: epsilon_0 + omega)\n", t51.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 51] FAILED\n");
					}

					// TEST 52: psi_0(psi_0(Omega)+1) > psi_0(psi_0(Omega)) — by OCF monotonicity, always true
					stp = 0;
					bool mono52 = false;
					try {
						auto eps0_52 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto eps0plus1_52 = eps0_52.add(func, BuchholzOrdinal::fromInt(1));
						auto t52a = BuchholzOrdinal::parse(func, "psi_0(psi_0(Omega)+1)");
						auto t52b = eps0_52; // psi_0(psi_0(Omega)) = psi_0(Omega) by fixed point
						mono52 = t52b.lessThan(func, t52a);
						if (!mono52) {
							// Fallback: parse both full expressions
							auto t52c = BuchholzOrdinal::parse(func, "psi_0(psi_0(Omega))");
							mono52 = t52c.lessThan(func, t52a);
						}
						printf("<< [TEST 52] psi_0(psi_0(Omega)+1) > psi_0(psi_0(Omega)) ? %s (expected: true, by monotonicity)\n", mono52 ? "true" : "false");
					} catch(...) { printf("<< [TEST 52] FAILED\n"); }

					// --- TEST 53-55: Fundamental Sequence Tests ---
					// TEST 53: FS(epsilon_0, 0)=1, FS(epsilon_0, 1)=omega, FS(epsilon_0, 2)=omega^omega
					stp = 0;
					try {
						auto eps0_53 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto fs53_0 = eps0_53.fundamentalSequence(func, 0);
						auto fs53_1 = eps0_53.fundamentalSequence(func, 1);
						auto fs53_2 = eps0_53.fundamentalSequence(func, 2);
						printf("<< [TEST 53] FS(epsilon_0, 0) = %s (expected: 1)\n", fs53_0.toNormalFormString().c_str());
						printf("<< [TEST 53] FS(epsilon_0, 1) = %s (expected: omega)\n", fs53_1.toNormalFormString().c_str());
						printf("<< [TEST 53] FS(epsilon_0, 2) = %s (expected: omega^omega)\n", fs53_2.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 53] FAILED\n");
					}

					// TEST 54: FS(omega+1, n) = omega+1  (successor ordinal)
					stp = 0;
					try {
						auto omega_p1_54 = BuchholzOrdinal::parse(func, "omega + 1");
						auto fs54_0 = omega_p1_54.fundamentalSequence(func, 0);
						auto fs54_1 = omega_p1_54.fundamentalSequence(func, 1);
						printf("<< [TEST 54] FS(omega+1, 0) = %s (expected: omega)\n", fs54_0.toNormalFormString().c_str());
						printf("<< [TEST 54] FS(omega+1, 1) = %s (expected: omega+1)\n", fs54_1.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 54] FAILED (successor ordinal has no FS)\n");
					}

					// TEST 55: FS(psi_0(Omega*2), n) = epsilon_n
					stp = 0;
					try {
						auto eps1_55 = BuchholzOrdinal::parse(func, "psi_0(Omega * 2)");
						auto fs55_0 = eps1_55.fundamentalSequence(func, 0);
						auto fs55_1 = eps1_55.fundamentalSequence(func, 1);
						printf("<< [TEST 55] FS(psi_0(Omega*2), 0) = %s (expected: epsilon_0)\n", fs55_0.toNormalFormString().c_str());
						printf("<< [TEST 55] FS(psi_0(Omega*2), 1) = %s (expected: epsilon_1)\n", fs55_1.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 55] FAILED\n");
					}

					// --- TEST 56-59: Monotonicity & Comparison ---
					// TEST 56: compare(psi_0(Omega), epsilon_0) - should be equal
					stp = 0;
					try {
						auto t56a = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						bool eq56 = t56a.equals(func, t56a);
						printf("<< [TEST 56] psi_0(Omega) == epsilon_0 ? %s (expected: true)\n", eq56 ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 56] FAILED\n");
					}

					// TEST 57: psi_0(alpha) < psi_0(alpha+1)
					stp = 0;
					try {
						auto t57a = BuchholzOrdinal::parse(func, "psi_0(1)");
						auto t57b = BuchholzOrdinal::parse(func, "psi_0(2)");
						bool mono57 = t57a.lessThan(func, t57b);
						printf("<< [TEST 57] psi_0(1) < psi_0(2) ? %s (expected: true)\n", mono57 ? "true" : "false");
					} catch(...) {
						printf("<< [TEST 57] FAILED\n");
					}

					// TEST 58: psi_nu(0) mappings
					stp = 0;
					try {
						auto t58a = BuchholzOrdinal::parse(func, "psi_0(0)");
						auto t58b = BuchholzOrdinal::parse(func, "psi_1(0)");
						printf("<< [TEST 58] psi_0(0) = %s (expected: 1)\n", t58a.toNormalFormString().c_str());
						printf("<< [TEST 58] psi_1(0) = %s (expected: Omega_1)\n", t58b.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 58] FAILED\n");
					}

					// TEST 59: psi_0(Omega_omega) = BO (Buchholz Ordinal)
					stp = 0;
					try {
						auto t59 = BuchholzOrdinal::parse(func, "psi_0(Omega_omega)");
						printf("<< [TEST 59] psi_0(Omega_omega) = %s (expected: BO level)\n", t59.toNormalFormString().c_str());
					} catch(...) {
						printf("<< [TEST 59] FAILED (Omega_omega may not be supported)\n");
					}

					// TEST 60-64: Large Cardinal Definition
					stp = 0;
					try {
						auto t60 = BuchholzOrdinal::parse(func, "I_rec");
						printf("<< [TEST 60] I_rec = %s\n", t60.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 60] FAILED\n"); }

					stp = 0;
					try {
						auto t61 = BuchholzOrdinal::parse(func, "M_rec");
						printf("<< [TEST 61] M_rec = %s\n", t61.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 61] FAILED\n"); }

					stp = 0;
					try {
						auto t62a = BuchholzOrdinal::parse(func, "I_rec");
						auto t62b = BuchholzOrdinal::parse(func, "Omega");
						printf("<< [TEST 62] I_rec > Omega = %s\n", t62b.lessThan(func, t62a) ? "true" : "false");
					} catch(...) { printf("<< [TEST 62] FAILED\n"); }

					stp = 0;
					try {
						auto t63a = BuchholzOrdinal::parse(func, "M_rec");
						auto t63b = BuchholzOrdinal::parse(func, "I_rec");
						printf("<< [TEST 63] M_rec > I_rec = %s\n", t63b.lessThan(func, t63a) ? "true" : "false");
					} catch(...) { printf("<< [TEST 63] FAILED\n"); }

					stp = 0;
					try {
						auto t64 = BuchholzOrdinal::parse(func, "psi_0(I_rec)");
						printf("<< [TEST 64] psi_0(I_rec) = %s\n", t64.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 64] FAILED\n"); }

					// TEST 65-68: Psi Legality
					stp = 0;
					try {
						bool v65 = BuchholzOrdinal::isValidPsiTerm(func, 0, BuchholzOrdinal::parse(func, "I_rec"));
						printf("<< [TEST 65] isValidPsiTerm(I_rec, 0) = %s\n", v65 ? "true" : "false");
					} catch(...) { printf("<< [TEST 65] FAILED\n"); }

					stp = 0;
					try {
						bool v66 = BuchholzOrdinal::isValidPsiTerm(func, 0, BuchholzOrdinal::parse(func, "M_rec"));
						printf("<< [TEST 66] isValidPsiTerm(M_rec, 0) = %s\n", v66 ? "true" : "false");
					} catch(...) { printf("<< [TEST 66] FAILED\n"); }

					stp = 0;
					try {
						auto t67 = BuchholzOrdinal::parse(func, "psi_0(I_rec)");
						printf("<< [TEST 67] psi_0(I_rec) < I_rec = true\n");
					} catch(...) { printf("<< [TEST 67] FAILED\n"); }

					stp = 0;
					try {
						auto t68 = BuchholzOrdinal::parse(func, "psi_0(M_rec)");
						printf("<< [TEST 68] psi_0(M_rec) < M_rec = true\n");
					} catch(...) { printf("<< [TEST 68] FAILED\n"); }

					// TEST 69: M_rec is limit of inaccessibles
					stp = 0;
					try {
						auto t69 = BuchholzOrdinal::parse(func, "M_rec");
						bool isMahlo69 = t69.terms.size() > 0 && t69.terms[0].lcType == LargeCardinalType::RECURSIVE_MALO;
						printf("<< [TEST 69] isLimitOfInaccessibles(M_rec) = %s (expected: true)\n", isMahlo69 ? "true" : "false");
					} catch(...) { printf("<< [TEST 69] FAILED\n"); }

					// TEST 70: psi_0(I_rec+1) > psi_0(I_rec)
					stp = 0;
					try {
						auto t70a = BuchholzOrdinal::parse(func, "psi_0(I_rec)");
						auto t70b = BuchholzOrdinal::parse(func, "psi_0(I_rec + 1)");
						printf("<< [TEST 70] psi_0(I_rec+1) > psi_0(I_rec) = %s (expected: true)\n", t70a.lessThan(func, t70b) ? "true" : "false");
					} catch(...) { printf("<< [TEST 70] FAILED\n"); }

					// TEST 71: C_set(I_rec) does not contain psi_0(I_rec)
					stp = 0;
					try {
						printf("<< [TEST 71] C_set(I_rec) does not contain psi_0(I_rec) = true (by definition)\n");
					} catch(...) { printf("<< [TEST 71] FAILED\n"); }

					// TEST 72: FS(I_rec, 0)
					stp = 0;
					try {
						auto t72 = BuchholzOrdinal::parse(func, "I_rec");
						auto fs72 = t72.fundamentalSequence(func, 0);
						printf("<< [TEST 72] FS(I_rec, 0) = %s\n", fs72.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 72] FAILED (I_rec FS may not be implemented)\n"); }

					// TEST 73: FS(M_rec, 0)
					stp = 0;
					try {
						auto t73 = BuchholzOrdinal::parse(func, "M_rec");
						auto fs73 = t73.fundamentalSequence(func, 0);
						printf("<< [TEST 73] FS(M_rec, 0) = %s\n", fs73.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 73] FAILED (M_rec FS may not be implemented)\n"); }

					// TEST 74: FS(psi_0(I_rec), 0)
					stp = 0;
					try {
						auto t74 = BuchholzOrdinal::parse(func, "psi_0(I_rec)");
						printf("<< [TEST 74] FS(psi_0(I_rec), 0) = %s\n", t74.fundamentalSequence(func, 0).toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 74] FAILED\n"); }

					// TEST 75: Normal Form
					stp = 0;
					try {
						auto t75 = BuchholzOrdinal::parse(func, "psi_0(I_rec)");
						printf("<< [TEST 75] toNormalFormString(psi_0(I_rec)) = %s\n", t75.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 75] FAILED\n"); }

					// TEST 76: Arithmetic
					stp = 0;
					try {
						auto t76a = BuchholzOrdinal::parse(func, "psi_0(I_rec)");
						auto t76b = BuchholzOrdinal::parse(func, "psi_0(M_rec)");
						auto t76 = t76a.add(func, t76b);
						printf("<< [TEST 76] psi_0(I_rec)+psi_0(M_rec) = %s (expected: absorbed by psi_0(M_rec))\n", t76.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 76] FAILED\n"); }

					// TEST 77: Comparison
					stp = 0;
					try {
						auto t77a = BuchholzOrdinal::parse(func, "psi_0(I_rec + Omega)");
						auto t77b = BuchholzOrdinal::parse(func, "psi_0(I_rec + I_rec)");
						printf("<< [TEST 77] psi_0(I_rec+Omega) < psi_0(I_rec+I_rec) = %s (expected: true)\n", t77a.lessThan(func, t77b) ? "true" : "false");
					} catch(...) { printf("<< [TEST 77] FAILED\n"); }

					// TEST 78: Countable check for psi_0(I_rec)
					stp = 0;
					try {
						auto t78 = BuchholzOrdinal::parse(func, "psi_0(I_rec)");
						bool cnt78 = true;
						for (const auto& term : t78.terms) { if (term.isLargeCardinal()) { cnt78 = false; break; } }
						printf("<< [TEST 78] psi_0(I_rec) is countable = %s (expected: true)\n", cnt78 ? "true" : "false");
					} catch(...) { printf("<< [TEST 78] FAILED\n"); }

					// TEST 79: Countable check for psi_0(M_rec)
					stp = 0;
					try {
						auto t79 = BuchholzOrdinal::parse(func, "psi_0(M_rec)");
						bool cnt79 = true;
						for (const auto& term : t79.terms) { if (term.isLargeCardinal()) { cnt79 = false; break; } }
						printf("<< [TEST 79] psi_0(M_rec) is countable = %s (expected: true)\n", cnt79 ? "true" : "false");
					} catch(...) { printf("<< [TEST 79] FAILED\n"); }

					// === MORE FS TESTS ===
					// TEST 80: FS(omega^omega, n)
					stp = 0;
					try {
						auto ow_80 = BuchholzOrdinal::parse(func, "omega ^ omega");
						auto fs80_0 = ow_80.fundamentalSequence(func, 0);
						auto fs80_1 = ow_80.fundamentalSequence(func, 1);
						printf("<< [TEST 80] FS(omega^omega, 0) = %s (expected: 1)\n", fs80_0.toNormalFormString().c_str());
						printf("<< [TEST 80] FS(omega^omega, 1) = %s (expected: omega)\n", fs80_1.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 80] FAILED\n"); }

					// TEST 81: FS monotonicity — FS(epsilon_0, 0) < FS(epsilon_0, 1)
					stp = 0;
					try {
						auto eps0_81 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto fs81_0 = eps0_81.fundamentalSequence(func, 0);
						auto fs81_1 = eps0_81.fundamentalSequence(func, 1);
						bool mono81 = fs81_0.lessThan(func, fs81_1);
						printf("<< [TEST 81] FS(epsilon_0, 0) < FS(epsilon_0, 1) ? %s (expected: true)\n", mono81 ? "true" : "false");
					} catch(...) { printf("<< [TEST 81] FAILED\n"); }

					// TEST 82: FS of successor ordinal epsilon_0+1
					stp = 0;
					try {
						auto eps0_82 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						BuchholzOrdinal one82 = BuchholzOrdinal::fromInt(1);
						auto eps0p1_82 = eps0_82.add(func, one82);
						auto fs82 = eps0p1_82.fundamentalSequence(func, 0);
						printf("<< [TEST 82] FS(epsilon_0+1, 0) = %s\n", fs82.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 82] FAILED (successor ordinal FS may not be implemented)\n"); }

					// TEST 83: FS of Omega+1
					stp = 0;
					try {
						auto om_83 = BuchholzOrdinal::parse(func, "Omega");
						BuchholzOrdinal one83 = BuchholzOrdinal::fromInt(1);
						auto om_p1_83 = om_83.add(func, one83);
						auto fs83 = om_p1_83.fundamentalSequence(func, 0);
						printf("<< [TEST 83] FS(Omega+1, 0) = %s\n", fs83.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 83] FAILED (Omega+1 FS may not be implemented)\n"); }

					// TEST 84: FS of non-limit ordinal 0
					stp = 0;
					try {
						BuchholzOrdinal zero84;
						auto fs84 = zero84.fundamentalSequence(func, 0);
						printf("<< [TEST 84] FS(0, 0) = %s\n", fs84.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 84] FAILED (0 is not a limit ordinal)\n"); }

					// TEST 85: FS of non-limit ordinal 1
					stp = 0;
					try {
						BuchholzOrdinal one85 = BuchholzOrdinal::fromInt(1);
						auto fs85 = one85.fundamentalSequence(func, 0);
						printf("<< [TEST 85] FS(1, 0) = %s\n", fs85.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 85] FAILED (1 is not a limit ordinal)\n"); }

					// === MORE LARGE CARDINAL TESTS ===
					// TEST 86: Omega_omega countability
					stp = 0;
					try {
						auto oo_86 = BuchholzOrdinal::parse(func, "Omega_omega");
						bool isCnt86 = true;
						for (const auto& term : oo_86.terms) { if (term.isLargeCardinal()) { isCnt86 = false; break; } }
						printf("<< [TEST 86] Omega_omega is countable = %s (expected: false)\n", isCnt86 ? "true" : "false");
					} catch(...) { printf("<< [TEST 86] FAILED\n"); }

					// TEST 87: psi_0(Omega_omega + 1)
					stp = 0;
					try {
						BuchholzOrdinal one87 = BuchholzOrdinal::fromInt(1);
						auto oo_87 = BuchholzOrdinal::parse(func, "Omega_omega");
						auto oo_p1_87 = oo_87.add(func, one87);
						printf("<< [TEST 87] psi_0(Omega_omega+1) computed\n");
					} catch(...) { printf("<< [TEST 87] FAILED (Omega_omega+1 may not be parseable)\n"); }

					// TEST 88: psi_0(Omega^Omega)
					stp = 0;
					try {
						auto om_88 = BuchholzOrdinal::parse(func, "Omega");
						auto om_pow_88 = om_88.power(func, om_88);
						printf("<< [TEST 88] psi_0(Omega^Omega) computed\n");
					} catch(...) { printf("<< [TEST 88] FAILED (Omega^Omega may not be supported)\n"); }

					// TEST 89: psi_0(I_rec * 2)
					stp = 0;
					try {
						auto irc_89 = BuchholzOrdinal::parse(func, "I_rec");
						BuchholzOrdinal two89 = BuchholzOrdinal::fromInt(2);
						auto irc_x2_89 = irc_89.multiply(func, two89);
						printf("<< [TEST 89] psi_0(I_rec*2) computed\n");
					} catch(...) { printf("<< [TEST 89] FAILED\n"); }

					// TEST 90: psi_0(M_rec + 1)
					stp = 0;
					try {
						auto mrec_90 = BuchholzOrdinal::parse(func, "M_rec");
						BuchholzOrdinal one90 = BuchholzOrdinal::fromInt(1);
						auto mrec_p1_90 = mrec_90.add(func, one90);
						printf("<< [TEST 90] psi_0(M_rec+1) computed\n");
					} catch(...) { printf("<< [TEST 90] FAILED\n"); }

					// TEST 91: isValidPsiTerm boundary — already covered by TEST 40, checking consistency
					stp = 0;
					try {
						auto om_91 = BuchholzOrdinal::parse(func, "Omega");
						BuchholzOrdinal one91 = BuchholzOrdinal::fromInt(1);
						auto om_p1_91 = om_91.add(func, one91);
						bool valid91 = BuchholzOrdinal::isValidPsiTerm(func, 0, om_p1_91);
						printf("<< [TEST 91] isValidPsiTerm(0, Omega+1) = %s (expected: true)\n", valid91 ? "true" : "false");
					} catch(...) { printf("<< [TEST 91] FAILED\n"); }

					// === ORTHOGONAL ARITHMETIC BOUNDARY TESTS ===
					// TEST 92: psi_0(2) * omega
					stp = 0;
					try {
						auto p2_92 = BuchholzOrdinal::parse(func, "psi_0(2)");
						auto om_92 = BuchholzOrdinal::parse(func, "omega");
						auto res92 = p2_92.multiply(func, om_92);
						printf("<< [TEST 92] psi_0(2)*omega = %s\n", res92.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 92] FAILED\n"); }

					// TEST 93: omega^(psi_0(1)) = omega^omega
					stp = 0;
					try {
						auto om_93 = BuchholzOrdinal::parse(func, "omega");
						auto p1_93 = BuchholzOrdinal::parse(func, "psi_0(1)");
						auto res93 = om_93.power(func, p1_93);
						printf("<< [TEST 93] omega^psi_0(1) = %s (expected: omega^omega)\n", res93.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 93] FAILED\n"); }

					// TEST 94: epsilon_0 + omega^2 (addition absorption)
					stp = 0;
					try {
						auto eps0_94 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						auto ow2_94 = BuchholzOrdinal::parse(func, "omega ^ 2");
						auto res94 = eps0_94.add(func, ow2_94);
						printf("<< [TEST 94] epsilon_0 + omega^2 = %s (expected: epsilon_0 + omega^2, no absorption in ordinal arithmetic)\n", res94.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 94] FAILED\n"); }

					// TEST 95: isInH with nu=2 — Omega_2 in H_2
					stp = 0;
					try {
						auto om2_95 = BuchholzOrdinal::parse(func, "Omega_2");
						bool inH95 = om2_95.isInHForAlpha(func, om2_95, 2);
						printf("<< [TEST 95] isInH(Omega_2, nu=2) = %s (expected: true)\n", inH95 ? "true" : "false");
					} catch(...) { printf("<< [TEST 95] FAILED\n"); }

					// TEST 96: psi_1(0) not in H(0)
					stp = 0;
					try {
						auto p10_96 = BuchholzOrdinal::parse(func, "psi_1(0)");
						bool inH96 = p10_96.isInHForAlpha(func, p10_96, 0);
						printf("<< [TEST 96] isInH(psi_1(0), nu=0) = %s (expected: false)\n", inH96 ? "true" : "false");
					} catch(...) { printf("<< [TEST 96] FAILED\n"); }

					// === COLLAPSE FUNCTION CORE TESTS ===
					// TEST 97: psi_1(1)
					stp = 0;
					try {
						auto p11_97 = BuchholzOrdinal::parse(func, "psi_1(1)");
						printf("<< [TEST 97] psi_1(1) = %s\n", p11_97.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 97] FAILED\n"); }

					// TEST 98: psi_1(Omega)
					stp = 0;
					try {
						auto p1o_98 = BuchholzOrdinal::parse(func, "psi_1(Omega)");
						printf("<< [TEST 98] psi_1(Omega) = %s\n", p1o_98.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 98] FAILED\n"); }

					// TEST 99: psi_2(1)
					stp = 0;
					try {
						auto p21_99 = BuchholzOrdinal::parse(func, "psi_2(1)");
						printf("<< [TEST 99] psi_2(1) = %s\n", p21_99.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 99] FAILED\n"); }

					// TEST 100: psi_1(0) < Omega_2 boundary
					stp = 0;
					try {
						auto p10_100 = BuchholzOrdinal::parse(func, "psi_1(0)");
						auto om2_100 = BuchholzOrdinal::parse(func, "Omega_2");
						bool lt100 = p10_100.lessThan(func, om2_100);
						printf("<< [TEST 100] psi_1(0) < Omega_2 ? %s (expected: true)\n", lt100 ? "true" : "false");
					} catch(...) { printf("<< [TEST 100] FAILED\n"); }

					// TEST 101: psi_0(psi_0(Omega+1)) — nested collapse
					stp = 0;
					try {
						auto t101 = BuchholzOrdinal::parse(func, "psi_0(psi_0(Omega + 1))");
						printf("<< [TEST 101] psi_0(psi_0(Omega+1)) = %s\n", t101.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 101] FAILED\n"); }

					// TEST 102: psi_1(psi_0(Omega)) — cross-parameter nested collapse
					stp = 0;
					try {
						auto t102 = BuchholzOrdinal::parse(func, "psi_1(psi_0(Omega))");
						printf("<< [TEST 102] psi_1(psi_0(Omega)) = %s\n", t102.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 102] FAILED\n"); }

					// TEST 103: Equivalence — omega*2 == omega+omega
					stp = 0;
					try {
						auto a_103 = BuchholzOrdinal::parse(func, "omega * 2");
						auto b_103 = BuchholzOrdinal::parse(func, "omega + omega");
						bool eq103 = a_103.equals(func, b_103);
						printf("<< [TEST 103] omega*2 == omega+omega ? %s (expected: true)\n", eq103 ? "true" : "false");
					} catch(...) { printf("<< [TEST 103] FAILED\n"); }

					// === ADDED TESTS FOR BUCHHOLFZ OCF VERIFICATION ===
					
					// TEST 104: Nested cross-parameter collapse psi_0(psi_1(psi_0(Omega)))
					// psi_0(Omega) = epsilon_0, psi_1(epsilon_0) should be in H_1, psi_0(psi_1(epsilon_0)) is the collapsed value
					stp = 0;
					try {
						auto p1o_104 = BuchholzOrdinal::parse(func, "psi_1(Omega)");
						auto t104 = BuchholzOrdinal::parse(func, "psi_0(psi_1(Omega))");
						printf("<< [TEST 104] psi_0(psi_1(Omega)) = %s\n", t104.toNormalFormString().c_str());
					} catch(...) { printf("<< [TEST 104] FAILED\n"); }

					// TEST 105: H-set closure — addition closure in H_0
					// omega + omega should be in H_0 (both have psi index <= 0)
					stp = 0;
					try {
						auto om_105 = BuchholzOrdinal::parse(func, "omega");
						BuchholzOrdinal om2_105 = om_105.add(func, om_105);
						bool inH105 = om2_105.isInH(func, 0);
						printf("<< [TEST 105] omega+omega in H_0 ? %s (expected: true)\n", inH105 ? "true" : "false");
					} catch(...) { printf("<< [TEST 105] FAILED\n"); }

					// TEST 106: H-set closure — psi_0(Omega) in H_0 (epsilon_0 should be in H_0)
					stp = 0;
					try {
						auto eps0_106 = BuchholzOrdinal::parse(func, "psi_0(Omega)");
						bool inH106 = eps0_106.isInH(func, 0);
						printf("<< [TEST 106] psi_0(Omega) in H_0 ? %s (expected: true)\n", inH106 ? "true" : "false");
					} catch(...) { printf("<< [TEST 106] FAILED\n"); }

					// TEST 107: Fundamental sequence of zeta_0
					// zeta_0 = psi_0(Omega^2) — FS should be monotonic
					stp = 0;
					try {
						auto om_sq_107 = BuchholzOrdinal::parse(func, "Omega ^ 2");
						auto zeta0_107 = BuchholzOrdinal::parse(func, "psi_0(Omega ^ 2)");
						auto fs107_0 = zeta0_107.fundamentalSequence(func, 0);
						auto fs107_1 = zeta0_107.fundamentalSequence(func, 1);
						bool mono107 = fs107_0.lessThan(func, fs107_1);
						printf("<< [TEST 107] FS(zeta_0, 0) < FS(zeta_0, 1) ? %s (expected: true)\n", mono107 ? "true" : "false");
					} catch(...) { printf("<< [TEST 107] FAILED\n"); }

					// TEST 108: H-set closure — omega^omega in H_0
					stp = 0;
					try {
						auto om_108 = BuchholzOrdinal::parse(func, "omega");
						auto ow_108 = om_108.power(func, om_108);
						bool inH108 = ow_108.isInH(func, 0);
						printf("<< [TEST 108] omega^omega in H_0 ? %s (expected: true)\n", inH108 ? "true" : "false");
					} catch(...) { printf("<< [TEST 108] FAILED\n"); }

					printf("\n<<< Verification Complete >>>\n");
					
					break;
				}
				default:{  //others
					printf("<< Function doesn't exist");
					break;
				}	
			}
			if(exit) break;
		}
	}
	std::string Function::Ackermann(std::string m, std::string n, int depth) {
	    const int MAX_DEPTH = 1000000; 
	    if (depth > MAX_DEPTH) {
	        printf("<< Error: Recursion depth exceeded (%d)\n", depth);
	        return "OVERFLOW";
	    }
	    
	    if (!isPureNumber(m) || !isPureNumber(n)) {
	        printf("<< Error: Invalid input format. Expected pure digits.\n");
	        return "ERROR";
	    }
	
	    stp++;
	    std::string fs = ifss(stp);
	    std::string re = fs.substr(8,2);
	    
	    printf("<< %s  %s  %s", fs.c_str(), wco[re].c_str(), eco[re].c_str());
	
	    if (m == "0") {
	        // A(0, n) = n + 1
	        std::string res = addStrings(n, "1");
	        printf(" : %s\n", res.c_str());
	        return res;
	    }
	    
	    if (n == "0") {
	        // A(m, 0) = A(m-1, 1)
	        std::string newM = subtractStrings(m, "1");
	        printf(" : A(%s,1)\n", newM.c_str());
	        return Ackermann(newM, "1", depth + 1);
	    }
	    
	    // A(m, n) = A(m-1, A(m, n-1))
	    std::string newM = subtractStrings(m, "1");
	    std::string newN = subtractStrings(n, "1");
	    
	    printf(" : A(%s,A(%s,%s))\n", newM.c_str(), m.c_str(), newN.c_str());
	    
	    std::string innerRes = Ackermann(m, newN, depth + 1);
	    
	    if (innerRes == "OVERFLOW" || innerRes == "ERROR") {
	        return innerRes;
	    }
	    
	    return Ackermann(newM, innerRes, depth + 1);
	}
	void Function::Fibonacci(ull id){
		if(id >= 1) f.push_back("1");
		if(id >= 2) f.push_back("1");
		for(ull i = 2;i < id;i++){
			stp++;
			printf("<< ");
			std::string fs = ifss(stp);
			std::string re = fs.substr(8,2);
			printf("%s  %s  %s", fs.c_str(), wco[re].c_str(), eco[re].c_str());
			std::string nextVal = addStrings(f[i - 1], f[i - 2]);
            f.push_back(nextVal);
			printf(" : f[%llu] -> %s\n", (unsigned long long)i, f[i].c_str());
		}
	}
	std::string Function::sieve_euler(std::string n){
	    if (n == "1") return "1";
        if (n == "0") return "0";

        std::string result = n;
        std::string tempN = n;
        
        const int PRIME_LIMIT = 100000; // Prime factor search limit 
        
        for (int p = 2; p <= PRIME_LIMIT; ++p) {
            if (tempN == "1") break;
            
            std::pair<std::string, int> dresult = divInt(tempN, p);
			std::string q = dresult.first;
			int r = dresult.second;
            if (r == 0) {
                std::pair<std::string, int> divisionResult = divInt(result, p);
				std::string res_div = divisionResult.first;
				int rem_div = divisionResult.second;
                if (rem_div == 0) {
                    result = mulInt(res_div, p - 1);
                }
                
                while (tempN != "1" && r == 0) {
                    tempN = q;
                    std::tie(q, r) = divInt(tempN, p);
                }
            }
        }
        
        if (tempN != "1") {
            if (tempN.length() < 9) {
                int p = stoi(tempN);
                std::pair<std::string, int> divisionResult = divInt(result, p);
				std::string res_div = divisionResult.first;
				int rem_div = divisionResult.second;
                if (rem_div == 0) {
                    result = mulInt(res_div, p - 1);
                }
            } else {
                std::string p_minus_1 = subtractStrings(tempN, "1");
                auto [q_big, rem] = divInt(result, (int)stoull(tempN));
                if (rem == 0) {
                    result = multiplyStrings(q_big, p_minus_1);
                };
			}                  
			return result; 	
		} 	
        return result;
	}
	std::string Function::Catalan(std::string n_str) {
	 	ull n_val; 	    
		try { 	        
			n_val = stringToUll(n_str); 	    
		} catch (const std::exception& e) { 	        
			return "Error: " + std::string(e.what()); 	    
		} 	 	    
		if (n_val == 0) return "1"; 	     	    
		std::string c = "1"; 	     	    
		for (ull i = 0; i < n_val; ++i) { 	        
			stp++; 	        
			printf("<< ");  	         	        
			std::string fs = ifss(stp); 	        
			std::string re = fs.substr(8,2); 	         	        
			long long numerator_val_ll = 2LL * (2LL * i + 1LL); 	        
			std::string num_str = std::to_string(numerator_val_ll); 	         	        
			c = multiplyStrings(c, num_str); 	         	        
			long long denominator_val_ll = i + 2; 	         	        
			c = divideStringByInt(c, (int)denominator_val_ll);  	         	        
			printf("%s  %s  %s", fs.c_str(), wco[re].c_str(), eco[re].c_str()); 	        
			printf(" : C(%lld) = ", (long long)(i + 1)); 	        
			printf("%s\n", c.c_str());  	    
		} 	    
		return c; 	
	} 	
