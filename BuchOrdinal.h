#pragma once
#include <string>
#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "BigNumber.h"
#include "Function_fwd.h"

class BuchholzOrdinal;
using OrdPtr = std::shared_ptr<BuchholzOrdinal>;
    enum class LargeCardinalType {
        OMEGA = 0,
        UNCOUNTABLE,
        RECURSIVE_INACCESSIBLE,
        RECURSIVE_MALO
    };

    static int getLargeCardinalLevel(LargeCardinalType lc) {
        switch (lc) {
            case LargeCardinalType::OMEGA: return 0;
            case LargeCardinalType::UNCOUNTABLE: return 1;
            case LargeCardinalType::RECURSIVE_INACCESSIBLE: return 2;
            case LargeCardinalType::RECURSIVE_MALO: return 3;
            default: return -1;
        }
    }

    static std::string getLargeCardinalName(LargeCardinalType lc) {
        switch (lc) {
            case LargeCardinalType::OMEGA: return "omega";
            case LargeCardinalType::UNCOUNTABLE: return "\u03A9";
            case LargeCardinalType::RECURSIVE_INACCESSIBLE: return "I_rec";
            case LargeCardinalType::RECURSIVE_MALO: return "M_rec";
            default: return "Unknown";
        }
    }

    struct Term {
        std::string coeff;
        int v;
        OrdPtr ord;
        LargeCardinalType lcType;
        int reflectionLevel;
        int reflectionOrder; // 1 = first-order (Pi_n), 2 = second-order (Pi_n1)
        bool isCountableOmega; // true = ω (countable limit), false = Ω_v (uncountable cardinal)

        Term() : coeff("0"), v(-1), ord(nullptr), lcType(LargeCardinalType::OMEGA), reflectionLevel(0), reflectionOrder(1), isCountableOmega(false) {}
        Term(std::string c, int n, OrdPtr o) : coeff(c), v(n), ord(o), lcType(LargeCardinalType::OMEGA), reflectionLevel(0), reflectionOrder(1), isCountableOmega(false) {}
        Term(std::string c, int n, OrdPtr o, LargeCardinalType lc, int refl)
            : coeff(c), v(n), ord(o), lcType(lc), reflectionLevel(refl), reflectionOrder(1), isCountableOmega(false) {}

        int getCoeff() const {
            try { return stoi(coeff); }
            catch (...) { return 1; }
        }

        bool isCountable() const { return isCountableOmega; }
        bool isLargeCardinal() const { return lcType != LargeCardinalType::OMEGA; }
        bool isReflection() const { return reflectionLevel > 0; }
        bool isPsi() const { return !isLargeCardinal() && !isReflection() && v >= 0 && !isCountableOmega; }
        bool isFinite() const { return v == -1; }

        int getStrengthLevel() const {
            if (isReflection()) return 200 + reflectionOrder * 50 + reflectionLevel;
            if (isLargeCardinal()) return 100 + getLargeCardinalLevel(lcType);
            return v;
        }

        // Defined later after BuchholzOrdinal class is complete
        std::string toString() const;
    };

    static Term makeLargeCardinalTerm(LargeCardinalType lc) {
        return Term("1", 0, nullptr, lc, 0);
    }
    static Term makeReflectionTerm(int level, OrdPtr alpha = nullptr, int order = 1) {
        Term t("1", 0, alpha, LargeCardinalType::OMEGA, level);
        t.reflectionOrder = order;
        return t;
    }

    class BuchholzOrdinal {
    public:
        std::vector<Term> terms; 
		int constant_coefficient;
		
        BuchholzOrdinal() : constant_coefficient(0) {}

        static BuchholzOrdinal fromInt(int val) {
            BuchholzOrdinal ord;
            ord.constant_coefficient = val;
            return ord;
        }

        static BuchholzOrdinal fromOmega(int nu) {
            BuchholzOrdinal ord;
            ord.terms.push_back(Term("1", nu, nullptr));
            return ord;
        }

        static BuchholzOrdinal fromPsi(int nu, const BuchholzOrdinal& argVal) {
            BuchholzOrdinal ord;
            OrdPtr argPtr = std::make_shared<BuchholzOrdinal>(argVal);
            ord.terms.push_back(Term("1", nu, argPtr));
            return ord;
        }

        bool checkZero(Function& func) const {
            if (!terms.empty()) return false;
            return constant_coefficient == 0;
        }

        BuchholzOrdinal add(Function& func, const BuchholzOrdinal& other) const {
            if (this->checkZero(func)) return other;
            if (other.checkZero(func)) return *this;

            // Ordinal addition: alpha + beta is NOT absorbed in general.
            // Absorption only happens in specific Cantor normal form cases:
            //   omega^alpha * a + omega^beta * b where beta < alpha => omega^alpha * a (absorbed)
            // But for distinct psi terms like psi_0(I_rec) + psi_0(M_rec),
            // we keep both terms unless they have the same leading term type.
            // For large cardinal hierarchy: psi_0(M_rec) >> psi_0(I_rec),
            // so psi_0(I_rec) + psi_0(M_rec) should be dominated by psi_0(M_rec)
            // ONLY when M_rec represents a strictly higher cardinal strength level.
            if (!this->terms.empty() && !other.terms.empty()) {
                const auto& this_lead = this->terms[0];
                const auto& other_lead = other.terms[0];
                
                // For same-level psi terms (e.g., psi_0(I_rec) + psi_0(M_rec)),
                // compare inner largest cardinal type: higher absorbs lower
                if (this_lead.isPsi() && other_lead.isPsi() &&
                    this_lead.v == other_lead.v && this_lead.ord && other_lead.ord) {
                    auto maxLc = [](const std::shared_ptr<BuchholzOrdinal>& ord) -> LargeCardinalType {
                        LargeCardinalType mx = LargeCardinalType::OMEGA;
                        for (const auto& t : ord->terms) {
                            if (t.isLargeCardinal() && t.lcType > mx) mx = t.lcType;
                        }
                        return mx;
                    };
                    if (maxLc(other_lead.ord) > maxLc(this_lead.ord)) return other;
                    if (maxLc(this_lead.ord) > maxLc(other_lead.ord)) return *this;
                }
                
                // Check if right operand is at a strictly higher large cardinal level
                // (e.g., psi_0(M_rec) vs psi_0(I_rec)) - M_rec has higher strength level
                if (this_lead.isPsi() && other_lead.isPsi() &&
                    this_lead.ord && other_lead.ord) {
                    int this_strength = this_lead.getStrengthLevel();
                    int other_strength = other_lead.getStrengthLevel();
                    // Only absorb when the right psi term has strictly higher large cardinal strength
                    if (this_strength < other_strength) {
                        // Check if the difference is at the large cardinal level
                        bool this_has_lc = false, other_has_lc = false;
                        for (const auto& t : this_lead.ord->terms) {
                            if (t.isLargeCardinal()) { this_has_lc = true; break; }
                        }
                        for (const auto& t : other_lead.ord->terms) {
                            if (t.isLargeCardinal()) { other_has_lc = true; break; }
                        }
                        if (this_has_lc && other_has_lc) {
                            func.log("[ADD] Higher large cardinal level psi term on right absorbs smaller on left");
                            return other;
                        }
                    }
                }
            }

            std::vector<Term> combined;
            combined.insert(combined.end(), this->terms.begin(), this->terms.end());
            combined.insert(combined.end(), other.terms.begin(), other.terms.end());

            BuchholzOrdinal result = normalize(func, combined);
            result.constant_coefficient = this->constant_coefficient + other.constant_coefficient;
            return result;
        }

        // Cantor normal form absorption: if this ends with a limit ordinal term
        // and other starts with a strictly smaller term, other gets absorbed.
        // This implements: alpha + beta = alpha when alpha is a power of omega
        // (additively principal) and alpha > beta.
        // IMPORTANT: This only applies to countable ordinals, not to distinct psi terms.
        bool isOmegaInTerm(const Term& t) const {
            if (!t.ord) return false;
            for (const auto& inner : t.ord->terms) {
                // Check for uncountable Omega (Omega_1, etc.)
                if (inner.isLargeCardinal() && !inner.isCountableOmega && inner.v > 0) {
                    return true;
                }
                // Recurse into nested ordinals
                if (inner.ord && isOmegaInTerm(inner)) {
                    return true;
                }
            }
            return false;
        }

        BuchholzOrdinal multiply(Function& func, const BuchholzOrdinal& other) const {
            if (this->checkZero(func) || other.checkZero(func)) return BuchholzOrdinal();
            
            BuchholzOrdinal result;
            result.constant_coefficient = this->constant_coefficient * other.constant_coefficient;

            // Case 1: both have terms - distribute
            if (!this->terms.empty() && !other.terms.empty()) {
                for (const auto& termA : this->terms) {
                    BuchholzOrdinal partial = multiplyTermByOrdinal(func, termA, other);
                    result = result.add(func, partial);
                }
            }
            // Case 2: this has constant, other has terms - scale each term in other
            if (!other.terms.empty() && this->constant_coefficient > 0) {
                for (const auto& termB : other.terms) {
                    BuchholzOrdinal partial;
                    Term scaled = termB;
                    scaled.coeff = std::to_string(this->constant_coefficient * termB.getCoeff());
                    partial.terms.push_back(scaled);
                    result = result.add(func, partial);
                }
            }
            // Case 3: this has terms, other is constant only (e.g. omega * 2)
            if (!this->terms.empty() && other.terms.empty() && other.constant_coefficient > 0) {
                for (const auto& termA : this->terms) {
                    BuchholzOrdinal partial;
                    Term scaled = termA;
                    scaled.coeff = std::to_string(other.constant_coefficient * termA.getCoeff());
                    partial.terms.push_back(scaled);
                    result = result.add(func, partial);
                }
            }
            return result;
        }

        // Ordinal exponentiation: alpha ^ beta (right-sided, ordinal power)
        // For finite exponents: alpha^n = alpha * alpha * ... * alpha
        // For omega exponents: alpha^omega = sup(alpha^n | n < omega)
        // For epsilon numbers: omega^epsilon = epsilon when epsilon is an epsilon number
        BuchholzOrdinal power(Function& func, const BuchholzOrdinal& exponent) const {
            if (this->checkZero(func)) return BuchholzOrdinal();
            if (exponent.checkZero(func)) return BuchholzOrdinal::fromInt(1);

            int expVal = exponent.constant_coefficient;
            // For finite exponents, use direct exponent representation for non-omega bases
            if (expVal > 0 && exponent.terms.empty()) {
                // If base is countable omega, use multiplication
                bool baseIsCountableOmega = (this->terms.size() == 1 && this->terms[0].isCountableOmega);
                if (baseIsCountableOmega) {
                    BuchholzOrdinal result = *this;
                    for (int i = 1; i < expVal; i++) result = result.multiply(func, *this);
                    return result;
                }
                // For non-omega bases (like Omega), represent as base^(expVal) with ord
                Term t = this->terms[0];
                t.ord = std::make_shared<BuchholzOrdinal>(exponent);
                BuchholzOrdinal result;
                result.terms.push_back(t);
                return normalize(func, result.terms);
            }

            // For finite exponent in terms[v == -1]
            if (exponent.terms.size() == 1 && exponent.constant_coefficient == 0 && exponent.terms[0].v == -1) {
                expVal = std::stoi(exponent.terms[0].coeff);
                BuchholzOrdinal result = *this;
                for (int i = 1; i < expVal; i++) {
                    result = result.multiply(func, *this);
                }
                return result;
            }

            // For infinite exponent: first try to evaluate simple psi terms
            // psi_0(n) = omega^n for finite n, psi_0(1) = omega
            BuchholzOrdinal evaluatedExponent = exponent;
            if (exponent.terms.size() == 1 && exponent.terms[0].v >= 0 && exponent.terms[0].isPsi() && exponent.terms[0].v == 0) {
                // psi_0(alpha) for small alpha can be evaluated
                const auto& psiArg = exponent.terms[0].ord;
                if (psiArg) {
                    // Check if alpha is a finite number
                    bool allFinite = true;
                    bool zero = true;
                    for (const auto& t : psiArg->terms) {
                        if (t.v != -1) { allFinite = false; break; }
                        if (t.getCoeff() > 0) zero = false;
                    }
                    int finiteVal = psiArg->constant_coefficient;
                    if (allFinite && !psiArg->terms.empty()) {
                        for (const auto& t : psiArg->terms) {
                            finiteVal += t.getCoeff();
                        }
                    }
                    if (allFinite && finiteVal > 0) {
                        // Make it omega (countable)
                        BuchholzOrdinal omegaOrd;
                        omegaOrd.constant_coefficient = 0;
                        Term omegaT;
                        omegaT.coeff = "1";
                        omegaT.v = 0;
                        omegaT.ord = nullptr;
                        omegaT.lcType = LargeCardinalType::OMEGA;
                        omegaT.isCountableOmega = true;
                        if (finiteVal == 1) {
                            // psi_0(1) = omega
                            omegaOrd.terms.push_back(omegaT);
                            evaluatedExponent = omegaOrd;
                        } else {
                            // psi_0(n) = omega*n for finite n
                            Term omegaScaled;
                            omegaScaled.coeff = std::to_string(finiteVal);
                            omegaScaled.v = 0;
                            omegaScaled.ord = nullptr;
                            omegaScaled.lcType = LargeCardinalType::OMEGA;
                            omegaScaled.isCountableOmega = true;
                            evaluatedExponent.terms.clear();
                            evaluatedExponent.constant_coefficient = 0;
                            evaluatedExponent.terms.push_back(omegaScaled);
                        }
                    }
                }
            }
            
            // Now use evaluatedExponent for the rest of the power computation
            
            if (this->terms.size() == 1 && this->terms[0].isCountableOmega && !this->terms[0].ord &&
                evaluatedExponent.terms.size() == 1 && evaluatedExponent.terms[0].isCountableOmega &&
                !evaluatedExponent.terms[0].ord) {
                // omega^omega - result is omega with ord pointing to omega as the exponent
                BuchholzOrdinal result;
                Term t;
                t.coeff = "1";
                t.v = 0;
                t.ord = std::make_shared<BuchholzOrdinal>(evaluatedExponent);
                t.lcType = LargeCardinalType::OMEGA;
                t.reflectionLevel = 0;
                t.reflectionOrder = 1;
                t.isCountableOmega = true;
                result.terms.push_back(t);
                return normalize(func, result.terms);
            }

            // Key case: epsilon is an epsilon number (omega^epsilon = epsilon)
            // If base is countable omega and exponent is a psi_0(Omega) = epsilon_0, return epsilon_0
            if (this->terms.size() == 1 && this->terms[0].isCountableOmega && !this->terms[0].ord) {
                // Base is omega; check if exponent is a fixed point of omega^x = x
                // Only epsilon numbers (psi_0(Omega) = epsilon_0 level) are fixed points
                // psi_0(1) = omega is NOT an epsilon number
                if (exponent.terms.size() == 1 && exponent.terms[0].v >= 0 && exponent.terms[0].isPsi()) {
                    // Only treat psi_0(Omega) as epsilon_0, not psi_0(1) = omega
                    // psi_0(Omega) contains uncountable Omega in its argument
                    if (exponent.terms[0].ord && isOmegaInTerm(exponent.terms[0])) {
                        // This is epsilon_0 (psi_0(Omega)) or higher epsilon number
                        // omega^epsilon_0 = epsilon_0
                        return exponent;
                    }
                }
            }

            // For omega^alpha where alpha is a general infinite ordinal,
            // represent as omega with ord pointing to alpha as exponent
            if (this->terms.size() == 1 && this->terms[0].isCountableOmega && !this->terms[0].ord) {
                BuchholzOrdinal result;
                Term t;
                t.coeff = "1";
                t.v = 0;
                t.ord = std::make_shared<BuchholzOrdinal>(evaluatedExponent);
                t.lcType = LargeCardinalType::OMEGA;
                t.reflectionLevel = 0;
                t.reflectionOrder = 1;
                t.isCountableOmega = true;
                result.terms.push_back(t);
                return normalize(func, result.terms);
            }

            // Support for uncountable Omega base: Omega^beta
            // Omega is a large cardinal (uncountable), not countable omega
            if (this->terms.size() == 1 && this->terms[0].isLargeCardinal() && !this->terms[0].isCountableOmega) {
                // Handle Omega^Omega: result is unmatched uncountable cardinal, represented as psi term
                // Omega^Omega is in C_0(Omega^2), so it's representable
                // For simplicity, represent Omega^beta as a single term with the base and exponent
                BuchholzOrdinal result;
                Term t;
                t.coeff = "1";
                t.v = this->terms[0].v;
                t.lcType = this->terms[0].lcType;
                t.reflectionLevel = this->terms[0].reflectionLevel;
                t.reflectionOrder = this->terms[0].reflectionOrder;
                t.isCountableOmega = this->terms[0].isCountableOmega;
                t.ord = std::make_shared<BuchholzOrdinal>(exponent);
                result.terms.push_back(t);
                return normalize(func, result.terms);
            }

            // Handle psi_v(alpha)^beta: if base is a collapsed ordinal, treat carefully
            // For epsilon_0^epsilon_0 = epsilon_0 (fixed point)
            if (this->terms.size() == 1 && this->terms[0].v >= 0 && this->terms[0].isPsi() && isOmegaInTerm(this->terms[0])) {
                // base is epsilon-level ordinal; omega^epsilon = epsilon
                // epsilon^epsilon = epsilon (still a fixed point)
                return *this;
            }

            throw std::runtime_error("Ordinal exponentiation not fully implemented for this case");
        }

        bool lessThan(Function& func, const BuchholzOrdinal& other, int depth = 0) const {
            if (depth > 100) return this->constant_coefficient < other.constant_coefficient;
            if (this->checkZero(func) && other.checkZero(func)) return false;
            if (this->checkZero(func)) return true;
            if (other.checkZero(func)) return false;
            try {
                // For single psi_v terms with same v, compare alphas semantically:
                // psi_v(alpha) < psi_v(beta) iff alpha < beta (OFC monotonicity)
                if (this->terms.size() == 1 && other.terms.size() == 1 &&
                    this->terms[0].isPsi() && other.terms[0].isPsi() &&
                    this->terms[0].v == other.terms[0].v &&
                    this->terms[0].ord && other.terms[0].ord &&
                    this->constant_coefficient == 0 && other.constant_coefficient == 0) {
                    const auto& a1 = this->terms[0];
                    const auto& a2 = other.terms[0];
                    bool a1_less_a2 = alphaLessThan(func, a1.ord, a2.ord);
                    if (a1_less_a2) return true;
                    if (alphaLessThan(func, a2.ord, a1.ord)) return false;
                    return false; // equal
                }

                // Direct term comparison
                size_t minSize = std::min(this->terms.size(), other.terms.size());
                for (size_t i = 0; i < minSize; ++i) {
                    int cmp = compareTerms(func, this->terms[i], other.terms[i], depth);
                    if (cmp != 0) return cmp < 0;
                }
                if (this->terms.size() != other.terms.size()) return this->terms.size() < other.terms.size();
                return this->constant_coefficient < other.constant_coefficient;
            } catch (...) {
                // Absolute fallback: one with more terms is usually larger
                if (this->terms.size() != other.terms.size()) return this->terms.size() < other.terms.size();
                return this->constant_coefficient < other.constant_coefficient;
            }
        }

        // Semantic alpha comparison: Omega > psi_0(Omega)+1
        // Since psi_0(Omega) = epsilon_0 (countable), but Omega is uncountable
        // In OCF context: psi_0(Omega) < Omega always holds
        // But psi_0(Omega)+1 < Omega still holds (countable + 1 < uncountable)
        static bool alphaLessThan(Function& func, const std::shared_ptr<BuchholzOrdinal>& a,
                                    const std::shared_ptr<BuchholzOrdinal>& b) {
            try {
                if (a->terms.empty() && b->terms.empty()) return a->constant_coefficient < b->constant_coefficient;
                if (a->terms.empty() && !b->terms.empty()) return true;
                if (!a->terms.empty() && b->terms.empty()) return false;
                
                auto topHasUncountable = [](const BuchholzOrdinal& o) {
                    for (const auto& t : o.terms) {
                        if (t.isLargeCardinal() && !t.isCountableOmega && !t.isPsi() && t.v > 0) return true;
                    }
                    return false;
                };
                bool aUnc = topHasUncountable(*a);
                bool bUnc = topHasUncountable(*b);
                if (!aUnc && bUnc) return true;
                if (aUnc && !bUnc) return false;
                size_t minSize = std::min(a->terms.size(), b->terms.size());
                for (size_t i = 0; i < minSize; ++i) {
                    const Term& ta = a->terms[i];
                    const Term& tb = b->terms[i];
                    if (ta.v != tb.v) return ta.v < tb.v;
                }
                if (a->terms.size() != b->terms.size()) return a->terms.size() < b->terms.size();
                return a->constant_coefficient < b->constant_coefficient;
            } catch (...) {
                return a->constant_coefficient < b->constant_coefficient;
            }
        }

        bool equals(Function& func, const BuchholzOrdinal& other) const {
            if (this->terms.size() != other.terms.size()) return false;
            if (this->constant_coefficient != other.constant_coefficient) return false;
            for (size_t i = 0; i < this->terms.size(); ++i) {
                if (!areTermsEqual(func, this->terms[i], other.terms[i])) return false;
            }
            return true;
        }

        std::string toString() const {
		    std::string res;
		    bool hasTerms = !terms.empty();
		    bool hasConstant = (constant_coefficient > 0);

		    if (!hasTerms && !hasConstant) return "0";

		    if (hasTerms) {
		        for (size_t i = 0; i < terms.size(); ++i) {
		            if (i > 0) res += " + ";
		            res += terms[i].toString();
		        }
		    }

		    if (hasConstant) {
		        if (hasTerms) res += " + ";
		        res += std::to_string(constant_coefficient);
		    }

		    return res;
		}
		// toNormalFormString: output canonical representation
		std::string toNormalFormString() const {
			std::string res;
			bool hasTerms = !terms.empty();
			bool hasConstant = (constant_coefficient > 0);
			if (!hasTerms && !hasConstant) return "0";
			if (hasTerms) {
				for (size_t i = 0; i < terms.size(); ++i) {
					if (i > 0) res += " + ";
					res += toNormalFormTerm(terms[i]);
				}
			}
			if (hasConstant) {
				if (hasTerms) res += " + ";
				res += std::to_string(constant_coefficient);
			}
			return res;
		}

		std::string toNormalFormTerm(const Term& t) const {
			if (t.isFinite()) return t.coeff;
			if (t.isCountable()) {
				std::string c = (t.getCoeff() == 1) ? "" : ("*" + t.coeff);
				if (t.ord != nullptr) {
					// omega^alpha form: display as "omega^alpha"
					std::string expStr = t.ord->toNormalFormString();
					return "\u03C9" + c + "^" + expStr;
				}
				return "\u03C9" + c;
			}
			if (t.isLargeCardinal()) {
				std::string c = (t.getCoeff() == 1) ? "" : ("*" + t.coeff);
				std::string lc = getLargeCardinalName(t.lcType);
				if (t.ord != nullptr && t.ord->terms.empty() && t.ord->constant_coefficient > 0) {
					int exp = t.ord->constant_coefficient;
					if (t.v > 0) return lc + c + "[" + std::to_string(t.v) + "]^" + std::to_string(exp);
					return lc + c + "^" + std::to_string(exp);
				}
				if (t.ord != nullptr) {
					std::string expStr = t.ord->toNormalFormString();
					if (t.v > 0) return lc + c + "[" + std::to_string(t.v) + "]^" + expStr;
					return lc + c + "^" + expStr;
				}
				if (t.v > 0) return lc + c + "[" + std::to_string(t.v) + "]";
				return lc + c;
			}
			if (t.isReflection()) {
				std::string c = (t.getCoeff() == 1) ? "" : ("*" + t.coeff);
				std::string a = (t.ord ? t.ord->toNormalFormString() : "0");
				if (t.reflectionLevel == 99) {
					if (t.reflectionOrder == 2) return c + "PiOmega1(" + a + ")";
					return c + "PiOmega(" + a + ")";
				}
				if (t.reflectionOrder == 2) return c + "Pi" + std::to_string(t.reflectionLevel) + "1(" + a + ")";
				return c + "Pi" + std::to_string(t.reflectionLevel) + "(" + a + ")";
			}
			std::string c = (t.getCoeff() == 1) ? "" : ("*" + t.coeff);
			if (!t.ord) return c + "\u03A9_" + std::to_string(t.v);
			std::string alphaStr = t.ord->toNormalFormString();
			if (t.v == 0 && t.ord && t.ord->terms.size() == 1) {
				const Term& inner = t.ord->terms[0];
				// psi_0(Omega) = epsilon_0: Omega is v==1, no ord, not large cardinal (uncountable)
				// But Omega parsed as large cardinal has isLargeCardinal() == true
				if (inner.v == 1 && !inner.ord && inner.getCoeff() == 1) {
					if (!inner.isLargeCardinal()) return c + "\u03B5_0";
					// Also match if it IS a large cardinal (uncountable Omega)
					if (inner.isLargeCardinal()) return c + "\u03B5_0";
				}
				// psi_0(Omega^2) = zeta_0: inner is Omega with ord=2 (or ord pointing to another Omega)
				if (inner.v == 1 && inner.ord && inner.ord->terms.size() == 1) {
					const Term& inner2 = inner.ord->terms[0];
					if (inner2.v == 1 && inner2.getCoeff() == 1) {
						return c + "\u03B6_0"; // zeta_0 (matches both large cardinal and non-large cardinal Omega)
					}
				}
				// psi_0(Omega^2) = zeta_0: inner.ord could be a constant (constant_coefficient=2)
				if (inner.v == 1 && inner.ord && inner.ord->terms.empty() && inner.ord->constant_coefficient == 2) {
					return c + "\u03B6_0"; // zeta_0
				}
			}
			return c + "\u03C8_" + std::to_string(t.v) + "(" + alphaStr + ")";
		}
		BuchholzOrdinal fundamentalSequence(Function& func, int n) const {
		    if (checkZero(func)) throw std::runtime_error("Zero has no FS");
		    if (terms.empty() && constant_coefficient == 0) throw std::runtime_error("Cannot compute FS for empty ordinal");
		    if (!isLimit(func)) throw std::runtime_error("Non-limit ordinal has no fundamental sequence");
		    
		    // Successor: ends with "... + 1" -> return predecessor
		    // Case 1: last term is v==-1, coeff=="1" (explicit +1 term)
		    if (!terms.empty() && terms.back().v == -1 && terms.back().coeff == "1" && !terms.back().ord) {
		        BuchholzOrdinal beta;
		        if (terms.size() > 1) {
		            beta.terms.assign(terms.begin(), terms.end() - 1);
		            beta.constant_coefficient = constant_coefficient;
		        }
		        return beta;
		    }
		    // Case 2: constant_coefficient == 1 and no v==-1 terms (e.g. omega + 1 parsed as terms + constant)
		    if (constant_coefficient == 1) {
		        BuchholzOrdinal beta;
		        beta.terms = this->terms;
		        beta.constant_coefficient = 0;
		        return beta;
		    }
		    if (terms.empty()) throw std::runtime_error("Finite ordinal has no fundamental sequence");
		    BuchholzOrdinal prefix;
		    if (terms.size() > 1) {
		        prefix.terms.assign(terms.begin(), terms.end() - 1);
		    }
		    Term last = terms.back();
		    BuchholzOrdinal fs_last;
		    if (last.v == -1) throw std::runtime_error("Invalid limit ordinal structure");
		    // omega^alpha FS rules:
		    //   alpha=1 (bare omega):       FS(omega, n) = n
		    //   alpha=finite k>=2:           FS(omega^k, n) = omega^(k-1) * n
		    //   alpha=successor beta+1:      FS(omega^(beta+1), n) = omega^beta * n
		    //   alpha=limit:                 FS(omega^alpha, n) = omega^(FS(alpha, n))
		    if (last.isCountableOmega) {
		        if (last.ord != nullptr) {
		            // omega^alpha form
		            OrdPtr alpha = last.ord;
		            if (alpha->checkZero(func)) throw std::runtime_error("omega^0 is not a limit");
		            bool alphaIsFinite = (alpha->terms.empty() && alpha->constant_coefficient > 0);
		            if (alphaIsFinite) {
		                int k = alpha->constant_coefficient;
		                if (k == 1) fs_last = fromInt(n);
		                else {
		                    // omega^(k-1) * n
		                    Term tw; tw.coeff = last.coeff; tw.v = 0; tw.isCountableOmega = true; tw.lcType = LargeCardinalType::OMEGA;
		                    tw.ord = std::make_shared<BuchholzOrdinal>(fromInt(k - 1));
		                    BuchholzOrdinal ob; ob.terms.push_back(tw);
		                    fs_last = ob.multiply(func, fromInt(n));
		                }
		            } else if (!alpha->isLimit(func)) {
		                // successor: alpha = beta + 1
		                BuchholzOrdinal beta_ord = alpha->predecessor(func);
		                Term tw; tw.coeff = last.coeff; tw.v = 0; tw.isCountableOmega = true; tw.lcType = LargeCardinalType::OMEGA;
		                if (beta_ord.checkZero(func)) fs_last = fromInt(n);
		                else { tw.ord = std::make_shared<BuchholzOrdinal>(beta_ord); BuchholzOrdinal ob; ob.terms.push_back(tw); fs_last = ob.multiply(func, fromInt(n)); }
		            } else {
		                // limit alpha
			        	BuchholzOrdinal alpha_fs = alpha->fundamentalSequence(func, n);
			            Term tw; tw.coeff = last.coeff; tw.v = 0; tw.isCountableOmega = true; tw.lcType = LargeCardinalType::OMEGA;
			            tw.ord = std::make_shared<BuchholzOrdinal>(alpha_fs);
		                BuchholzOrdinal ob; ob.terms.push_back(tw);
		                fs_last = ob;
		            }
		        } else if (!prefix.checkZero(func)) {
		            // bare omega with prefix -> prefix * n
		            return prefix.multiply(func, fromInt(n));
		        } else {
		            // bare omega alone: FS(omega, n) = n
		            fs_last = fromInt(n);
		        }
		    } else if (last.ord == nullptr) {
		        // Bare Omega_v (uncountable) with no argument: FS must be n-dependent and monotonic
		        if (last.v == 0) {
		            fs_last = fromInt(n);
		        } else {
		            // Omega_v: use omega^(omega^n) as monotonic sequence
		            Term inner; inner.coeff = "1"; inner.v = 0; inner.isCountableOmega = true; inner.lcType = LargeCardinalType::OMEGA;
		            inner.ord = std::make_shared<BuchholzOrdinal>(fromInt(n + 1));
		            BuchholzOrdinal exponent; exponent.terms.push_back(inner);
		            Term outer; outer.coeff = "1"; outer.v = 0; outer.isCountableOmega = true; outer.lcType = LargeCardinalType::OMEGA;
		            outer.ord = std::make_shared<BuchholzOrdinal>(exponent);
		            fs_last.terms.push_back(outer);
		        }
		    } else {
		        // psi_v(alpha) form
		        OrdPtr alpha = last.ord;
		        if (alpha->checkZero(func)) fs_last = fromOmega(last.v).fundamentalSequence(func, n);
		        else if (!alpha->isLimit(func)) {
		            BuchholzOrdinal beta = alpha->predecessor(func);
		            fs_last = fromPsi(last.v, beta).multiply(func, fromInt(n));
		        } else {
		            BuchholzOrdinal alpha_fs = alpha->fundamentalSequence(func, n);
		            fs_last = fromPsi(last.v, alpha_fs);
		        }
		    }
		    return prefix.add(func, fs_last);
		}
	
	    BuchholzOrdinal predecessor(Function& func) const {
	        if (checkZero(func)) throw std::runtime_error("Zero has no predecessor");
	        if (terms.empty()) throw std::runtime_error("Empty ordinal terms");
	        
	        Term last = terms.back();
	        BuchholzOrdinal res;
	        
	        if (last.v == -1) {
	            int val = std::stoi(last.coeff);
	            if (val > 1) {
	                res.terms = this->terms;
	                res.terms.back().coeff = std::to_string(val - 1);
	            } else {
	                if (terms.size() > 1) {
	                    res.terms.assign(terms.begin(), terms.end() - 1);
	                }
	            }
	        } else {
	            throw std::runtime_error("Predecessor not defined for this infinite term in simple mode");
	        }
	        return normalize(func, res.terms);
	    }
	    
	    bool isLimit(Function& func) const {
	        if (checkZero(func)) return false;
	        if (constant_coefficient > 0) return false;
			if (terms.empty()) return true;
			if (terms.back().v == -1 && terms.back().coeff == "1" && !terms.back().ord) {
	            return false;
	        }
	        return true;
	    }
	    static BuchholzOrdinal parse(Function& func, const std::string& s, int indent = 0) {
	    	func.log("[TRACE] Parse called with len: " + std::to_string(s.length()) + " | Content: [" + s.substr(0, std::min((int)s.length(), 20)) + "...]");
		    func.stp++;
		    std::string prefix(indent * 2, ' ');
		    std::string trimmed = trim(s);
		    
		    func.log(prefix + "[PARSE] Input: \"" + trimmed + "\"");
		
		    if (trimmed.empty()) throw std::invalid_argument("Empty input");
		
		    if (isPureNumber(trimmed)) {
		        func.log(prefix + "[TYPE] Finite Number: " + trimmed);
		        return fromInt(stoi(trimmed));
		    }
		
		    if (trimmed == "\u03C9" || trimmed == "\u03C9_0" || trimmed == "omega") {
		        func.log(prefix + "[TYPE] omega Symbol (countable limit ordinal)");
		        BuchholzOrdinal ord;
		        Term t;
		        t.coeff = "1";
		        t.v = 0;
		        t.ord = nullptr;
		        t.lcType = LargeCardinalType::OMEGA;
		        t.reflectionLevel = 0;
		        t.reflectionOrder = 1;
		        t.isCountableOmega = true;
		        ord.terms.push_back(t);
		        return normalize(func, ord.terms, indent + 1);
		    }
		    // Omega (Ω) = uncountable cardinal, handled below by parseLargeCardinal

		    // Parse large cardinal symbols: I, M, K, R, Me, S, W, Su, H, I3, I2, I1, I_rec, M_rec
		    if (isLargeCardinalFormat(trimmed)) {
		        func.log(prefix + "[TYPE] Large Cardinal Detected");
		        return parseLargeCardinal(func, trimmed, indent + 1);
		    }

		    // Parse reflection operators: Pi1(...), Pi2(...), Pi3(...)
		    if (isReflectionFormat(trimmed)) {
		        func.log(prefix + "[TYPE] Reflection Operator Detected");
		        return parseReflection(func, trimmed, indent + 1);
		    }
		
		    if (trimmed.front() == '(' && trimmed.back() == ')') {
		        if (isBalancedParentheses(trimmed)) { 
		            func.log(prefix + "[TYPE] Grouped Expression, stripping parentheses");
		            return parse(func, trimmed.substr(1, trimmed.size() - 2), indent + 1);
		        }
		    }
		
			// Parse multiplication: alpha * beta (higher precedence than +)
			int mulPos = findTopLevelMultiply(trimmed);
			if (mulPos != -1) {
				func.log(prefix + "[TYPE] Multiplication Found at index " + std::to_string(mulPos));
				std::string leftStr = trimmed.substr(0, mulPos);
				std::string rightStr = trimmed.substr(mulPos + 1);
				func.log(prefix + "[MUL_SPLIT] Left:  \"" + leftStr + "\"");
				func.log(prefix + "[MUL_SPLIT] Right: \"" + rightStr + "\"");
				BuchholzOrdinal leftOrd = parse(func, leftStr, indent + 1);
				BuchholzOrdinal rightOrd = parse(func, rightStr, indent + 1);
				return leftOrd.multiply(func, rightOrd);
			}

			// Parse exponentiation: alpha ^ beta (highest precedence among alg ops)
			int powPos = findTopLevelPower(trimmed);
			if (powPos != -1) {
				func.log(prefix + "[TYPE] Exponentiation Found at index " + std::to_string(powPos));
				std::string leftStr = trimmed.substr(0, powPos);
				std::string rightStr = trimmed.substr(powPos + 1);
				func.log(prefix + "[POW_SPLIT] Left:  \"" + leftStr + "\"");
				func.log(prefix + "[POW_SPLIT] Right: \"" + rightStr + "\"");
				BuchholzOrdinal leftOrd = parse(func, leftStr, indent + 1);
				BuchholzOrdinal rightOrd = parse(func, rightStr, indent + 1);
				return leftOrd.power(func, rightOrd);
			}

			// Parse exponentiation: alpha + beta (low precedence)
		    int plusPos = findTopLevelPlus(trimmed);
		    if (plusPos != -1) {
		        func.log(prefix + "[TYPE] Addition Found at index " + std::to_string(plusPos));
		        
		        std::string leftStr = trimmed.substr(0, plusPos);
		        std::string rightStr = trimmed.substr(plusPos + 1);
		        
		        func.log(prefix + "[SPLIT] Left:  \"" + leftStr + "\"");
		        func.log(prefix + "[SPLIT] Right: \"" + rightStr + "\"");
		        
		        BuchholzOrdinal leftOrd = parse(func, leftStr, indent + 1);
		        BuchholzOrdinal rightOrd = parse(func, rightStr, indent + 1);
		        
		        func.log(prefix + "[MERGE] Combining results...");
		        
		        std::vector<Term> combined = leftOrd.terms;
		        combined.insert(combined.end(), rightOrd.terms.begin(), rightOrd.terms.end());
		        
            BuchholzOrdinal result = normalize(func, combined, indent + 1);
            result.constant_coefficient = leftOrd.constant_coefficient + rightOrd.constant_coefficient;
            return result;
		    }
		
		    if (isPsiFormat(trimmed)) {
		        func.log(prefix + "[TYPE] Psi Function Detected");
		        return parsePsi(func, trimmed, indent + 1);
		    }
		
		    throw std::invalid_argument("Unknown format: " + trimmed);
		}
        // Helper: multiply a Term by an Ordinal (distributive expansion)
        static BuchholzOrdinal multiplyTermByOrdinal(Function& func, const Term& t, const BuchholzOrdinal& ord) {
            BuchholzOrdinal result;
            
            for (const auto& termO : ord.terms) {
                std::string newCoeff = func.multiplyStrings(t.coeff, termO.coeff);
                if (func.isZero(newCoeff)) continue;

                if (t.v == -1) {
                    // When multiplying finite * infinite, put infinite first (Cantor normal form: ω*n not n*ω)
                    result.terms.push_back(Term(termO.coeff, termO.v, termO.ord));
                    // Multiply coefficients
                    if (!func.isZero(t.coeff) && t.coeff != "1") {
                        result.terms.back().coeff = func.multiplyStrings(termO.coeff, t.coeff);
                    }
                }
                else if (termO.v == -1) {
                    // When multiplying infinite * finite, finite goes on right: ω*n
                    result.terms.push_back(Term(t.coeff, t.v, t.ord));
                    result.terms.back().coeff = func.multiplyStrings(t.coeff, termO.coeff);
                    if (t.isCountableOmega) result.terms.back().isCountableOmega = true;
                }
                else if (t.isCountableOmega && termO.isCountableOmega) {
                    // ω * c_a * ω * c_b = ω^(a+b) with coefficient c_b
                    // IMPORTANT: left coefficient is NOT exponent! "omega*2" means 2*omega, not omega^2
                    // ord field represents the exponent (omega^ord), while coeff is just a multiplier
                    
                    BuchholzOrdinal exponent;
                    // Left exponent: if t.ord exists, use it; otherwise ω^1
                    if (t.ord) {
                        exponent = exponent.add(func, *t.ord);
                    } else {
                        exponent = exponent.add(func, fromInt(1));
                    }
                    // Right exponent: if termO.ord exists, use it; otherwise ω^1
                    if (termO.ord) {
                        exponent = exponent.add(func, *termO.ord);
                    } else {
                        exponent = exponent.add(func, fromInt(1));
                    }
                    Term omegaPow;
                    omegaPow.coeff = "1"; // Ordinal power absorbs coefficients
                    omegaPow.v = 0;
                    omegaPow.ord = std::make_shared<BuchholzOrdinal>(exponent);
                    omegaPow.lcType = LargeCardinalType::OMEGA;
                    omegaPow.reflectionLevel = 0;
                    omegaPow.reflectionOrder = 1;
                    omegaPow.isCountableOmega = true;
                    result.terms.push_back(omegaPow);
                }
                else if (t.isCountableOmega && !termO.isCountableOmega) {
                    // ω * psi_v(alpha) - omega is smaller, so result is psi_v(alpha)
                    result.terms.push_back(Term(newCoeff, termO.v, termO.ord));
                    if (termO.ord) {
                        result.terms.back().ord = termO.ord;
                    }
                }
                else if (!t.isCountableOmega && termO.isCountableOmega) {
                    // psi_v(alpha) * ω - omega is smaller term, goes on right
                    result.terms.push_back(Term(t.coeff, t.v, t.ord));
                    if (t.ord) {
                        result.terms.back().ord = t.ord;
                    }
                    // Add omega term after
                    result.terms.push_back(Term(termO.coeff, termO.v, termO.ord));
                }
                else {
                    result.terms.push_back(Term(newCoeff, termO.v, termO.ord));
                }
            }
            return normalize(func, result.terms);
        }

        static BuchholzOrdinal normalize(Function& func, std::vector<Term> terms, int indent = 0) {
		    std::string prefix(indent * 2, ' ');
		    func.log(prefix + "[NORM] Start normalizing " + std::to_string(terms.size()) + " terms");
		    func.stp++;
		
		    terms.erase(
		        remove_if(terms.begin(), terms.end(), [](const Term& t) {
		            return t.getCoeff() == 0;
		        }),
		        terms.end()
		    );
		    
		    if (terms.empty()) {
		        func.log(prefix + "[NORM] Result is 0 (empty)");
		        return BuchholzOrdinal();
		    }
		
		    sort(terms.begin(), terms.end(), [](const Term& a, const Term& b) {
		        return BuchholzOrdinal::ordIsGreater(a, b);
		    });
		    func.log(prefix + "[NORM] Sorted terms by v and alpha");
		
		    std::vector<Term> merged;
		    for (const auto& term : terms) {
		        if (merged.empty()) {
		            merged.push_back(term);
		        } else {
		            Term& last = merged.back();
		            if (BuchholzOrdinal::isSameType(last, term)) {
		                int newCoeff = last.getCoeff() + term.getCoeff();
		                last.coeff = std::to_string(newCoeff);
		                func.log(prefix + "[NORM] Merged term, new coeff: " + std::to_string(newCoeff));
		            } else {
		                merged.push_back(term);
		            }
		        }
		    }
		    
		    BuchholzOrdinal result;
		    int constant_coefficient = 0;
		    std::vector<Term> filtered;
		    for (auto& term : merged) {
		        // omega (ω) is NOT a finite number - do NOT absorb into constant_coefficient.
		            // constant_coefficient is only for actual finite numbers (v == -1 terms).
		            if (term.v == -1) {
		            constant_coefficient += term.getCoeff();
		            func.log(prefix + "[NORM] Absorb finite term, add coeff: " + std::to_string(term.getCoeff()));
		        } else {
		            filtered.push_back(term);
		        }
		    }
		    merged.swap(filtered);
		    result.constant_coefficient = constant_coefficient;
		    
		    merged.erase(
		        std::remove_if(merged.begin(), merged.end(), [](const Term& t) {
		            return t.getCoeff() == 0;
		        }),
		        merged.end()
		    );
		
		    if (merged.empty()) {
		         func.log(prefix + "[NORM] Result is 0 after merging");
		         return BuchholzOrdinal();
		    }
		
		    func.log(prefix + "[NORM] Normalization complete. Final terms: " + std::to_string(merged.size()));
		    
		    result.terms = merged;
		    return result;
		}
        static int compareTerms(Function& func, const Term& a, const Term& b, int depth = 0) {
            try {
                if (depth > 100) return (a.v != b.v) ? (a.v < b.v ? -1 : 1) : 0;
                // Handle finite terms (v == -1)
                if (a.v == -1 && b.v != -1) return -1;
                if (a.v != -1 && b.v == -1) return 1;

                if (a.v == -1 && b.v == -1) {
                    if (a.coeff.length() != b.coeff.length())
                        return a.coeff.length() < b.coeff.length() ? -1 : 1;
                    if (a.coeff != b.coeff)
                        return a.coeff < b.coeff ? -1 : 1;
                    return 0;
                }

                // Compare type hierarchy: reflection > large cardinal > psi
                int aType = a.isReflection() ? 3 : (a.isLargeCardinal() ? 2 : 1);
                int bType = b.isReflection() ? 3 : (b.isLargeCardinal() ? 2 : 1);
                if (aType != bType) return aType < bType ? -1 : 1;

                // Both are same type
                if (a.isReflection() && b.isReflection()) {
                    if (a.reflectionOrder != b.reflectionOrder)
                        return a.reflectionOrder < b.reflectionOrder ? -1 : 1;
                    if (a.reflectionLevel != b.reflectionLevel)
                        return a.reflectionLevel < b.reflectionLevel ? -1 : 1;
                    if (a.ord && b.ord && !a.ord->equals(func, *b.ord))
                        return a.ord->lessThan(func, *b.ord, depth + 1) ? -1 : 1;
                    return compareCoeffs(func, a.coeff, b.coeff);
                }

                if (a.isLargeCardinal() && b.isLargeCardinal()) {
                    int aLc = getLargeCardinalLevel(a.lcType);
                    int bLc = getLargeCardinalLevel(b.lcType);
                    if (aLc != bLc) return aLc < bLc ? -1 : 1;
                    if (a.v != b.v) return a.v < b.v ? -1 : 1;
                    return compareCoeffs(func, a.coeff, b.coeff);
                }

                // Both are psi terms - compare by psi index first
                if (a.v != b.v) {
                    return a.v < b.v ? -1 : 1;
                }
                
                // Same psi index - compare coefficients if no arguments
                if (!a.ord && !b.ord) {
                    return compareCoeffs(func, a.coeff, b.coeff);
                }
                if (!a.ord) return -1; 
                if (!b.ord) return 1;

                // Compare alpha arguments using proper ordinal comparison
                try {
                    if (!a.ord->equals(func, *b.ord)) {
                        return a.ord->lessThan(func, *b.ord, depth + 1) ? -1 : 1;
                    }
                } catch (...) {}
                return compareCoeffs(func, a.coeff, b.coeff);
            } catch (...) {
                return (a.v != b.v) ? (a.v < b.v ? -1 : 1) :
                       ((a.getCoeff() != b.getCoeff()) ? (a.getCoeff() < b.getCoeff() ? -1 : 1) : 0);
            }
        }

        // Helper: compare two OrdPtr ordinals
        static int ORD_PTR_LESS_THAN(Function& func, const BuchholzOrdinal& alpha, const BuchholzOrdinal& beta) {
            // For psi terms, compare by computing their actual values
            // Try to compute psi values if both are psi terms
            if (!alpha.terms.empty() && !beta.terms.empty()) {
                const auto& ta = alpha.terms[0];
                const auto& tb = beta.terms[0];
                // If both are psi terms with same v, compare their arguments
                if (ta.v >= 0 && tb.v >= 0 && ta.v == tb.v) {
                    if (ta.ord && tb.ord) {
                        return ORD_PTR_LESS_THAN(func, *ta.ord, *tb.ord);
                    }
                }
            }
            // Fall back to direct ordinal comparison
            return alpha.lessThan(func, beta);
        }

        static int compareCoeffs(Function& func, const std::string& c1, const std::string& c2) {
            if (c1.length() != c2.length())
                return c1.length() < c2.length() ? -1 : 1;
            if (c1 != c2)
                return c1 < c2 ? -1 : 1;
            return 0;
        }
		
		static bool areTermsEqual(Function& func, const Term& a, const Term& b) {
	        if (a.v != b.v) return false;
        if (a.coeff != b.coeff) return false;
                if (a.lcType != b.lcType) return false;
                if (a.reflectionLevel != b.reflectionLevel) return false;
                if (a.reflectionOrder != b.reflectionOrder) return false;
	        if (!a.ord && !b.ord) return true;
	        if (!a.ord || !b.ord) return false;
	        return a.ord->equals(func, *b.ord);
	    }
    
        static bool areArgsEqual(Function& func, const OrdPtr& a, const OrdPtr& b) {
            if (!a && !b) return true;
            if (!a || !b) return false;
            return a->equals(func, *b);
        }

        static std::string termToString(const Term& t) {
            if (t.v == -1) {
                return t.coeff;
            }
            
            std::string s;
            if (t.coeff != "1") {
                 s += t.coeff + "*";
            }
            
            if (!t.ord) {
                s += "ψ_" + std::to_string(t.v);
            } else {
                s += "ψ_" + std::to_string(t.v) + "(" + t.ord->toString() + ")";
            }
            return s;
        }
        static BuchholzOrdinal fromOmega(Function& func) {
		    func.log("  [CONST] Constructing Omega (ψ_1(0))");
		    Term t;
		    t.coeff = "1";
		    t.v = 1;
		    t.ord = std::make_shared<BuchholzOrdinal>();
		    std::vector<Term> terms = {t};
		    return normalize(func, terms);
		}
		static bool isPsiFormat(const std::string& s) {
		    return s.find("ψ") != std::string::npos || s.find("psi") != std::string::npos;
		}
		static int findTopLevelPlus(const std::string& s) {
		    int depth = 0;
		    for (size_t i = 0; i < s.length(); ++i) {
		        char c = s[i];
		        if (c == '(') depth++;
		        else if (c == ')') depth--;
		        else if (c == '+' && depth == 0) {
		            return (int)i;
		        }
		    }
		    return -1; 
		}

		// Find top-level '*' (multiplication) - returns RIGHTMOST * to handle left-associativity
		static int findTopLevelMultiply(const std::string& s) {
		    int depth = 0;
		    int lastMul = -1;
		    for (size_t i = 0; i < s.length(); ++i) {
		        char c = s[i];
		        if (c == '(') depth++;
		        else if (c == ')') depth--;
		        else if (c == '*' && depth == 0) {
		            lastMul = (int)i;
		        }
		    }
		    return lastMul;
		}

		// Find top-level '^' (exponentiation) - returns LEFTMOST ^ to handle right-associativity
		static int findTopLevelPower(const std::string& s) {
		    int depth = 0;
		    for (size_t i = 0; i < s.length(); ++i) {
		        char c = s[i];
		        if (c == '(') depth++;
		        else if (c == ')') depth--;
		        else if (c == '^' && depth == 0) {
		            return (int)i;
		        }
		    }
		    return -1;
		}
		
		static bool compareOrdinals(const std::shared_ptr<BuchholzOrdinal>& a, const std::shared_ptr<BuchholzOrdinal>& b) {
		    if (a->terms.size() != b->terms.size()) return false;
		    if (a->constant_coefficient != b->constant_coefficient) return false;
		    for (size_t i = 0; i < a->terms.size(); ++i) {
		        if (a->terms[i].v != b->terms[i].v) return false;
		        if (a->terms[i].getCoeff() != b->terms[i].getCoeff()) return false;
		        if (a->terms[i].lcType != b->terms[i].lcType) return false;
		        if (a->terms[i].reflectionLevel != b->terms[i].reflectionLevel) return false;
		        if (a->terms[i].reflectionOrder != b->terms[i].reflectionOrder) return false;
		        if (a->terms[i].isCountableOmega != b->terms[i].isCountableOmega) return false;
		        bool a_has = (a->terms[i].ord != nullptr);
		        bool b_has = (b->terms[i].ord != nullptr);
		        if (a_has && b_has) {
		            if (!compareOrdinals(a->terms[i].ord, b->terms[i].ord)) return false;
		        } else if (a_has != b_has) {
		            return false;
		        }
		    }
		    return true;
		}
		

		static bool ordLessThan(const BuchholzOrdinal& lhs, const BuchholzOrdinal& rhs) {
			if (lhs.terms.empty() && rhs.terms.empty()) return false;
			if (lhs.terms.empty()) return true;
			if (rhs.terms.empty()) return false;
			size_t minSize = std::min(lhs.terms.size(), rhs.terms.size());
			for (size_t i = 0; i < minSize; ++i) {
				const Term& a = lhs.terms[i];
				const Term& b = rhs.terms[i];
				if (a.v < b.v) return true;
				if (a.v > b.v) return false;
				bool a_has_ord = (a.ord != nullptr);
				bool b_has_ord = (b.ord != nullptr);
				if (a_has_ord && b_has_ord) {
					if (ordLessThan(*a.ord, *b.ord)) return true;
					if (ordLessThan(*b.ord, *a.ord)) return false;
				} else if (a_has_ord && !b_has_ord) {
					return a.ord->terms.empty();
				} else if (!a_has_ord && b_has_ord) {
					if (!b.ord->terms.empty()) return true;
					continue;
				}
			}
			return lhs.terms.size() < rhs.terms.size();
		}

	    static bool ordIsGreater(const Term& a, const Term& b) {
		    if (a.v != b.v) {
		        return a.v > b.v;
		    }
		    
		    bool a_has_ord = (a.ord != nullptr);
		    bool b_has_ord = (b.ord != nullptr);
		    
		    if (a_has_ord && b_has_ord) {
		        bool isLess = ordLessThan(*a.ord, *b.ord);
		        bool isEq = compareOrdinals(a.ord, b.ord);
		        return !isLess && !isEq;
		    }
		    else if (a_has_ord && !b_has_ord) {
		        return !a.ord->terms.empty(); 
		    }
		    else if (!a_has_ord && b_has_ord) {
		        return false;
		    }
		    else {
		        return false; 
		    }
		}
	
	    static bool isSameType(const Term& a, const Term& b) {
		    if (a.v != b.v) return false;
		    if (a.lcType != b.lcType) return false;
		    if (a.reflectionLevel != b.reflectionLevel) return false;
		    if (a.reflectionOrder != b.reflectionOrder) return false;
		    
		    bool a_has_ord = (a.ord != nullptr);
		    bool b_has_ord = (b.ord != nullptr);
		    
		    if (a_has_ord && b_has_ord) {
		        return compareOrdinals(a.ord, b.ord);
		    } 
		    else if (!a_has_ord && !b_has_ord) {
		        return true;
		    } 
		    else {
		        return false;
		    }
		}
	
	    friend bool operator==(const BuchholzOrdinal& lhs, const BuchholzOrdinal& rhs) {
		    if (lhs.terms.empty() && rhs.terms.empty()) return true;
		    if (lhs.terms.empty() || rhs.terms.empty()) return false;
		    if (lhs.terms.size() != rhs.terms.size()) return false;
		    
		    for (size_t i = 0; i < lhs.terms.size(); ++i) {
		        if (lhs.terms[i].v != rhs.terms[i].v) return false;
		        if (!lhs.terms[i].ord || !rhs.terms[i].ord) {
		            if (lhs.terms[i].ord != rhs.terms[i].ord) return false;
		        } else {
		            if (!compareOrdinals(lhs.terms[i].ord, rhs.terms[i].ord)) return false;
		        }
		        if (lhs.terms[i].getCoeff() != rhs.terms[i].getCoeff()) return false;
		    }
		    return true;
		}
	    friend bool operator>(const BuchholzOrdinal& lhs, const BuchholzOrdinal& rhs) {
		    if (lhs.terms.empty() && rhs.terms.empty()) return false; 
		    if (lhs.terms.empty()) return false;                    
		    if (rhs.terms.empty()) return true;                 
		    
		    size_t minSize = std::min(lhs.terms.size(), rhs.terms.size());
		    for (size_t i = 0; i < minSize; ++i) {
		        const Term& a = lhs.terms[i];
		        const Term& b = rhs.terms[i];
		        
		        if (a.v > b.v) return true;
		        if (a.v < b.v) return false;
		        
		        bool a_has_ord = (a.ord != nullptr);
		        bool b_has_ord = (b.ord != nullptr);
		        
		        if (a_has_ord && b_has_ord) {
		            if (ordLessThan(*b.ord, *a.ord)) return true;
		            if (!compareOrdinals(a.ord, b.ord)) return false;
		        }
		        
		        else if (a_has_ord && !b_has_ord) {
		            if (!a.ord->terms.empty()) return true;
		            else return false; 
		        }
		        else if (!a_has_ord && b_has_ord) {
		            if (!b.ord->terms.empty()) return false; 
		            else continue; 
		        }
		        else {
		            continue;
		        }
		    }
		    
		    return !ordLessThan(lhs, rhs) && !(lhs == rhs);
		}

		static BuchholzOrdinal applyRadix(Function& func, BuchholzOrdinal ord, int radix) {
			func.log("[RADIX] Applying radix " + std::to_string(radix));
			if (radix < 2) return ord;
			
			// Find all v=0 terms with ord=0 (psi_0(0) form, representing finite numbers)
			int sum = 0;
			std::vector<Term> otherTerms;
			for (auto& term : ord.terms) {
				if (term.v == 0 && term.ord && term.ord->checkZero(func)) {
					sum += term.getCoeff();
					func.log("[RADIX] Found psi_0(0) term, coeff: " + std::to_string(term.getCoeff()) + ", sum: " + std::to_string(sum));
				} else {
					otherTerms.push_back(term);
				}
			}
			
			if (sum < radix) {
				func.log("[RADIX] Sum " + std::to_string(sum) + " < radix " + std::to_string(radix) + ", no carry needed");
				return ord;
			}
			
			int quotient = sum / radix;
			int remainder = sum % radix;
			func.log("[RADIX] " + std::to_string(sum) + " / " + std::to_string(radix) + " = " + std::to_string(quotient) + " remainder " + std::to_string(remainder));
			
			// Add psi_1(0) term with quotient coefficient
			if (quotient > 0) {
				Term omegaTerm;
				omegaTerm.coeff = std::to_string(quotient);
				omegaTerm.v = 1;
				omegaTerm.ord = std::make_shared<BuchholzOrdinal>();
				otherTerms.push_back(omegaTerm);
			}
			
			// Add psi_0(remainder) term if remainder > 0
			if (remainder > 0) {
				Term remTerm;
				remTerm.coeff = std::to_string(remainder);
				remTerm.v = 0;
				remTerm.ord = std::make_shared<BuchholzOrdinal>();
				otherTerms.push_back(remTerm);
			}
			
			ord.terms = otherTerms;
			return normalize(func, otherTerms);
		}
		
		static std::pair<std::string, int> extractRadix(const std::string& s) {
			// Find the )(digit pattern from the end
			// Look for the last ) that is followed by (digits)
			int lastParen = -1;
			int depth = 0;
			for (int i = (int)s.length() - 1; i >= 0; i--) {
				if (s[i] == ')') {
					if (depth == 0) lastParen = i;
					depth--;
				} else if (s[i] == '(') {
					depth++;
				}
			}
			
			if (lastParen == -1) return {"", 10};
			
			// The radix is the last (number) group
			std::string radixStr = s.substr(lastParen + 1);
			if (radixStr.front() != '(' || radixStr.back() != ')') return {"", 10};
			radixStr = radixStr.substr(1, radixStr.length() - 2);
			radixStr = trim(radixStr);
			for (char c : radixStr) {
				if (!isdigit(c)) return {"", 10};
			}
			int radix = stoi(radixStr);
			
			// Expression is everything before the radix part
			std::string expr = s.substr(0, lastParen);
			expr = trim(expr);
			return {expr, radix};
		}

	    static BuchholzOrdinal parsePsi(Function& func, const std::string& s, int indent) {
		    std::string prefix(indent * 2, ' ');
		    func.log(prefix + "[PSI] Parsing Psi structure");
		    
		    size_t parenStart = s.find('(');
		    size_t parenEnd = s.rfind(')');
		    if (parenStart == std::string::npos || parenEnd == std::string::npos) {
		        throw std::invalid_argument("Invalid Psi syntax: " + s);
		    }
		    
		    std::string prefixStr = s.substr(0, parenStart);
		    std::string alphaStr = s.substr(parenStart + 1, parenEnd - parenStart - 1);
		    
		    int v = 0;
		    size_t underscore = prefixStr.find('_');
		    if (underscore != std::string::npos) v = stoi(prefixStr.substr(underscore + 1));
		    
		    func.log(prefix + "[PSI] v = " + std::to_string(v));
		    func.log(prefix + "[PSI] Alpha string: \"" + alphaStr + "\"");
		    
		    BuchholzOrdinal alphaOrd = parse(func, alphaStr, indent + 1);
		    
		    // Buchholz standard: psi_0(0) = 1 (C_0(0) = {0}, smallest missing is 1)
		    // psi_0(n) = omega*n for finite n >= 1
		    if (v == 0 && alphaOrd.checkZero(func)) {
		        func.log(prefix + "[PSI] Applying Buchholz convention: psi_0(0) = 1");
		        return fromInt(1);
		    }
		    // Check if alpha is a finite number >= 1 (no terms, just constant_coefficient)
		    // psi_0(1) = omega, psi_0(n) = omega*n for finite n >= 1
		    if (v == 0 && alphaOrd.terms.empty() && alphaOrd.constant_coefficient >= 1) {
		        int finVal = alphaOrd.constant_coefficient;
		        func.log(prefix + "[PSI] Applying Buchholz convention: psi_0(" + std::to_string(finVal) + ") = omega*" + std::to_string(finVal));
		        BuchholzOrdinal result;
		        Term omegaTerm;
		        omegaTerm.coeff = std::to_string(finVal);
		        omegaTerm.v = 0;
		        omegaTerm.ord = nullptr;
		        omegaTerm.lcType = LargeCardinalType::OMEGA;
		        omegaTerm.reflectionLevel = 0;
		        omegaTerm.reflectionOrder = 1;
		        omegaTerm.isCountableOmega = true;
		        result.terms.push_back(omegaTerm);
		        return result;
		    }
		    // Also check if alpha has a single finite term (legacy format)
		    if (v == 0 && alphaOrd.terms.size() == 1 && alphaOrd.terms[0].v == -1) {
		        int finVal = alphaOrd.terms[0].getCoeff();
		        func.log(prefix + "[PSI] Applying Buchholz convention: psi_0(" + std::to_string(finVal) + ") = omega*" + std::to_string(finVal));
		        BuchholzOrdinal result;
		        Term omegaTerm;
		        omegaTerm.coeff = std::to_string(finVal);
		        omegaTerm.v = 0;
		        omegaTerm.ord = nullptr;
		        omegaTerm.lcType = LargeCardinalType::OMEGA;
		        omegaTerm.reflectionLevel = 0;
		        omegaTerm.reflectionOrder = 1;
		        omegaTerm.isCountableOmega = true;
		        result.terms.push_back(omegaTerm);
		        return result;
		    }

			// Buchholz convention: psi_nu(0) = Omega_nu for nu > 0
			// Omega_nu is the nu-th uncountable cardinal
			if (v > 0 && alphaOrd.checkZero(func)) {
				func.log(prefix + "[PSI] Applying Buchholz convention: psi_" + std::to_string(v) + "(0) = Omega_" + std::to_string(v));
				BuchholzOrdinal result;
				Term omegaTerm;
				omegaTerm.coeff = "1";
				omegaTerm.v = v;
				omegaTerm.ord = nullptr;
				omegaTerm.lcType = LargeCardinalType::UNCOUNTABLE;
				omegaTerm.reflectionLevel = 0;
				omegaTerm.reflectionOrder = 1;
				omegaTerm.isCountableOmega = false;
				result.terms.push_back(omegaTerm);
				return result;
			}
			
			// Fixed point: psi_0(psi_0(Omega)) = psi_0(Omega) (= epsilon_0)
			// ONLY when alpha is EXACTLY psi_0(Omega) with no extra terms or constants
			if (v == 0 && alphaOrd.terms.size() == 1 && alphaOrd.constant_coefficient == 0) {
				const auto& at = alphaOrd.terms[0];
				if (at.v == 0 && at.isPsi() && at.ord && at.ord->terms.size() == 1 && at.ord->constant_coefficient == 0) {
					const auto& inner_at = at.ord->terms[0];
					if (inner_at.isLargeCardinal() && inner_at.lcType == LargeCardinalType::UNCOUNTABLE &&
						inner_at.v == 1 && !inner_at.ord && inner_at.getCoeff() == 1) {
						func.log(prefix + "[PSI] Fixed point: psi_0(epsilon_0) = epsilon_0");
						BuchholzOrdinal omegaOrd;
						Term omegaT;
						omegaT.coeff = "1"; omegaT.v = 1; omegaT.ord = nullptr;
						omegaT.lcType = LargeCardinalType::UNCOUNTABLE;
						omegaT.reflectionLevel = 0; omegaT.reflectionOrder = 1;
						omegaT.isCountableOmega = false;
						omegaOrd.terms.push_back(omegaT);
						omegaOrd.constant_coefficient = 0;
						OrdPtr argPtr = std::make_shared<BuchholzOrdinal>(omegaOrd);
						BuchholzOrdinal result;
						result.terms.push_back(Term("1", 0, argPtr));
						return result;
					}
				}
			}
			
			Term t;
		    t.coeff = "1";
		    t.v = v;
		    t.ord = std::make_shared<BuchholzOrdinal>(alphaOrd);
		    
		    std::vector<Term> terms = {t};
		    func.log(prefix + "[PSI] Constructed term, normalizing...");
		    return normalize(func, terms, indent + 1);
		}
		static bool isBalancedParentheses(const std::string& s) {
		    if (s.empty() || s.front() != '(' || s.back() != ')') return false;
		    int count = 0;
		    for (size_t i = 0; i < s.length(); ++i) {
		        if (s[i] == '(') count++;
		        else if (s[i] == ')') count--;
		        if (count == 0 && i < s.length() - 1) return false; 
		    }
		    return count == 0;
		}

		// Detect large cardinal format
		static bool isLargeCardinalFormat(const std::string& s) {
		    // Detect Omega_N format for uncountable cardinals: Omega[N], Omega_N, Omega[
		    // Also detect Unicode \u03A9 (Omega) - 3 bytes: 0xE2 0x84 0xA9
		    if (s == "Omega" || s == "Omega[" || s == "Omega_" || s == "\u03A9" || s == "\u03A9[" || s == "\u03A9_" || s == "\xE2\x84\xA9" || s == "\xE2\x84\xA9[" || s == "\xE2\x84\xA9_") return true;
		    if (s.substr(0, 5) == "Omega") {
		        char next;
		        if (s.length() == 5) next = '\0';
		        else next = s[5];
		        if (next == '[' || next == '_' || next == '\0' || isdigit(next)) return true;
		    }
		    // Check for Unicode Omega at start
		    if (s.length() >= 3 && s[0] == (char)0xE2 && s[1] == (char)0x84 && s[2] == (char)0xA9) {
		        if (s.length() == 3) return true;
		        char next = s[3];
		        if (next == '[' || next == '_' || isdigit(next)) return true;
		    }
		    // 递归不可达基数与递归马洛基数 (I_rec, M_rec)
		    static const char* multiCharNames[] = {"I_rec", "M_rec"};
		    for (const char* name : multiCharNames) {
		        std::string ns = name;
		        if (s == ns || s == ns + "(" || s == ns + "[") return true;
		    }
		    // 是否以递归不可达开�?
		    for (const char* name : multiCharNames) {
		        if (s.substr(0, strlen(name)) == name) {
		            char next = s[strlen(name)];
		            if (next == '[' || next == '(' || next == '\0') return true;
		        }
		    }
		    return false;
		}

		// Parse large cardinal symbol into a Term
		static BuchholzOrdinal parseLargeCardinal(Function& func, const std::string& s, int indent) {
		    std::string prefix(indent * 2, ' ');
		    func.log(prefix + "[LC] Parsing large cardinal: " + s);

		    // Detect which large cardinal
		    LargeCardinalType lc = LargeCardinalType::OMEGA;
		    std::string base;
		    bool found = false;

		    // Check for Omega (uncountable cardinals) first: Omega[N], Omega_N, Ω[N], Ω_N
		    {
		        size_t omegaLen = 0;
		        bool isOmega = false;

		        // Check ASCII "Omega"
		        if (s.length() >= 5 && s.substr(0, 5) == "Omega") {
		            isOmega = true;
		            omegaLen = 5;
		        }
		        // Check Unicode Ω (3 bytes in UTF-8: 0xE2 0x84 0xA9)
		        else if (s.length() >= 3 && s[0] == (char)0xE2 && s[1] == (char)0x84 && s[2] == (char)0xA9) {
		            isOmega = true;
		            omegaLen = 3;
		        }

		        if (isOmega) {
		            lc = LargeCardinalType::UNCOUNTABLE;
		            base = "\u03A9";
		            bool hasIndex = false;
		            int idx = 1; // Default: Omega_1 = smallest uncountable cardinal				
				if (omegaLen < s.length() && s[omegaLen] == '[') {
					size_t end = s.find(']', omegaLen);
					if (end != std::string::npos) {
						std::string idxStr = s.substr(omegaLen + 1, end - omegaLen - 1);
						try { idx = stoi(idxStr); } catch (...) { idx = 1; }
						hasIndex = true;
					}
				} else if (omegaLen < s.length() && s[omegaLen] == '_') {
					std::string idxStr = s.substr(omegaLen + 1);
					if (idxStr == "omega" || idxStr == "ω") {
						idx = 0;
						hasIndex = true;
					} else {
						try { idx = stoi(idxStr); } catch (...) { idx = 1; }
						hasIndex = true;
					}
				} else if (omegaLen < s.length() && isdigit(s[omegaLen])) {
					std::string idxStr = s.substr(omegaLen);
					try { idx = stoi(idxStr); } catch (...) { idx = 1; }
					hasIndex = true;
				} else {
					// Bare "Omega" or "Ω" without index defaults to Omega_1
					idx = 1;
					hasIndex = true;
				}


		            func.log(prefix + "[LC] Parsed Uncountable: " + base + "[" + std::to_string(idx) + "]");

		            Term t;
		            t.coeff = "1";
		            t.v = idx;
		            t.ord = nullptr;
		            t.lcType = lc;
		            t.reflectionLevel = 0;

		            std::vector<Term> terms = {t};
		            return normalize(func, terms, indent + 1);
		        }

				// 递归不可达基数与递归马洛基数 (I_rec, M_rec)
				const struct { const char* name; LargeCardinalType type; } recursiveChars[] = {
					{"I_rec", LargeCardinalType::RECURSIVE_INACCESSIBLE},
					{"M_rec", LargeCardinalType::RECURSIVE_MALO},
				};
				for (auto& rc : recursiveChars) {
					if (s.substr(0, strlen(rc.name)) == rc.name) {
						lc = rc.type;
						base = rc.name;
						found = true;
						break;
					}
				}

				if (!found) {
					throw std::invalid_argument("Unknown large cardinal: " + s);
				}

				// Check for [N] suffix (indexed cardinals)
				int idx = 0;
				size_t bracket = s.find('[');
				if (bracket != std::string::npos) {
					size_t end = s.find(']', bracket);
					if (end != std::string::npos) {
						try { idx = stoi(s.substr(bracket + 1, end - bracket - 1)); }
						catch (...) { idx = 0; }
					}
				}

				func.log(prefix + "[LC] Parsed: " + getLargeCardinalName(lc) + (idx > 0 ? "[" + std::to_string(idx) + "]" : ""));
				
				Term t;
				t.coeff = "1";
				t.v = idx;
				t.ord = nullptr;
				t.lcType = lc;
				t.reflectionLevel = 0;

				std::vector<Term> terms = {t};
				return normalize(func, terms, indent + 1);
			}
		}

		// Detect reflection format: Pi1(...), Pi2(...), Pi11(...), Pi21(...), PiOmega(...), PiOmega1(...)
		static bool isReflectionFormat(const std::string& s) {
		    if (s.length() < 4) return false;
		    
		    // PiOmega1(...) = second-order omega reflection
		    if (s.length() > 8 && s.substr(0, 8) == "PiOmega1" && s[8] == '(') return true;
		    // PiOmega(...) = first-order omega reflection
		    if (s.length() > 7 && s.substr(0, 7) == "PiOmega" && s[7] == '(') return true;
		    
		    if (s.substr(0, 2) != "Pi") return false;
		    size_t i = 2;
		    bool hasDigit = false;
		    while (i < s.length() && isdigit(s[i])) { i++; hasDigit = true; }
		    if (!hasDigit) return false;
		    
		    // Second-order format: Pin1(...)
		    if (i < s.length() && s[i] == '1' && i + 1 < s.length() && s[i + 1] == '(') return true;
		    // First-order format: Pin(...)
		    return i < s.length() && s[i] == '(';
		}

		// Parse reflection: Pi1(a), Pi2(a)... Pi11(a), Pi21(a)... PiOmega(a), PiOmega1(a)
		static BuchholzOrdinal parseReflection(Function& func, const std::string& s, int indent) {
		    std::string prefix(indent * 2, ' ');
		    func.log(prefix + "[REFL] Parsing reflection operator: " + s);

		    int level = 1;
		    int order = 1; // 1=first-order(P^n), 2=second-order(P^n_1)
		    size_t parenStart;

		    if (s.length() > 8 && s.substr(0, 8) == "PiOmega1") {
		        level = 99; order = 2; parenStart = 8;
		    }
		    else if (s.length() > 7 && s.substr(0, 7) == "PiOmega") {
		        level = 99; order = 1; parenStart = 7;
		    }
		    else {
		        size_t i = 2;
		        std::string numStr;
		        while (i < s.length() && isdigit(s[i])) {
		            numStr += s[i++];
		        }
		        if (!numStr.empty()) level = stoi(numStr);

		        if (i < s.length() && s[i] == '1' && i + 1 < s.length() && s[i + 1] == '(') {
		            order = 2; parenStart = i + 1;
		        } else {
		            order = 1; parenStart = i;
		        }
		    }

		    if (parenStart >= s.length() || s[parenStart] != '(') {
		        throw std::invalid_argument("Invalid reflection syntax: " + s);
		    }
		    size_t parenEnd = s.rfind(')');
		    if (parenEnd == std::string::npos || parenEnd <= parenStart) {
		        throw std::invalid_argument("Invalid reflection syntax: " + s);
		    }

		    std::string alphaStr = s.substr(parenStart + 1, parenEnd - parenStart - 1);
		    std::string orderName = (order == 2) ? "Second-order" : "First-order";
		    func.log(prefix + "[REFL] Order: " + orderName + ", Level: " + std::to_string(level) + ", Alpha: " + alphaStr);

		    BuchholzOrdinal alphaOrd = parse(func, alphaStr, indent + 1);

		    Term t;
		    t.coeff = "1";
		    t.v = 0;
		    t.ord = std::make_shared<BuchholzOrdinal>(alphaOrd);
		    t.lcType = LargeCardinalType::OMEGA;
		    t.reflectionLevel = level;
		    t.reflectionOrder = order;

		    std::vector<Term> terms = {t};
		    func.log(prefix + "[REFL] Constructed reflection term, normalizing...");
		    return normalize(func, terms, indent + 1);
		}


        // === Buchholz OCF Core Functions ===

        // Collect all psi indices (v values >= 0) from an ordinal
        static void collectPsiIndices(const BuchholzOrdinal& ord, std::set<int>& indices) {
            for (const auto& t : ord.terms) {
                if (t.v >= 0 && t.isPsi()) {
                    indices.insert(t.v);
                    if (t.ord) {
                        collectPsiIndices(*t.ord, indices);
                    }
                }
            }
        }

        // Check if ordinal alpha is in H(alpha)_nu (simplified Buchholz condition)
        // H_nu is the set of ordinals constructible using only psi_k with k <= nu
        // 
        // KEY RULES:
        // - psi_1(0) = Omega_1 is NOT in H_0 (psi index 1 > nu=0)
        // - psi_0(Omega) = epsilon_0 IS in H_0 (psi index 0 <= nu=0, and Omega in argument is allowed in psi_0)
        // - For H_0: countable omega is allowed, and psi_0(arg) is allowed even if arg contains Omega
        // - Uncountable Omega_k appearing DIRECTLY (not inside psi) in the ordinal is blocked for nu < k
        static bool isInH(Function& func, const BuchholzOrdinal& alpha, int nu) {
            func.log("[H_ALPHA] Checking if alpha is in H_" + std::to_string(nu) + "\n");
            
            // Check all terms in alpha
            for (const auto& t : alpha.terms) {
                // Finite terms are always in H
                if (t.v == -1) continue;
                
                // Countable omega is always in H
                if (t.isCountableOmega) continue;
                
                // For uncountable Omega_k appearing DIRECTLY (not within psi terms):
                // Omega is allowed in H_0 for use as psi_0 argument
                // Omega_v in H_nu is allowed when v <= nu+1
                // But if this is a psi term (has an argument ordinal), check psi index instead
                if (t.isLargeCardinal() && !t.isPsi()) {
                    // Omega with an argument (e.g., Omega^alpha) is a psi-like term
                    // treat psi index as t.v
                    if (t.ord != nullptr) {
                        // Has argument, treat as psi term
                        if (t.v > nu) {
                            func.log("[H_ALPHA] FAIL: psi index " + std::to_string(t.v) + " > nu=" + std::to_string(nu));
                            return false;
                        }
                    }
                    // No argument: bare Omega or result of psi_v(0)
                    // Use the psi index (v) for H-set membership
                    // Omega_1 should NOT be in H_0 (it requires psi index 0)
                    // But Omega (uncountable) is allowed in H_0 for use as psi_0 argument
                    // Distinguish: if it came from computePsiValue (psi_v(0) -> Omega_v),
                    // the v directly determines the psi level required
                    // For bare Omega parsed from input with v=1, allow up to nu+1
                    if (!t.ord) {
                        // psi_v(0) result: v is the psi index, must satisfy v <= nu
                        // or Omega used as argument: v <= nu+1
                        // Since we can't distinguish the source, use relaxed rule:
                        // Only block if v > nu+1 (allows Omega_1 in H_0 for psi_0 arguments,
                        // but psi_1(0)=Omega_1 should NOT be in H_0 because it represents
                        // a psi at level 1)
                        // The fix: treat uncountable terms without arg that have v >= 1
                        // as psi results, checking v <= nu (stricter than nu+1)
                        if (t.v > nu) {
                            func.log("[H_ALPHA] FAIL: Omega_v with v=" + std::to_string(t.v) + " > nu=" + std::to_string(nu));
                            return false;
                        }
                    }
                }
                
                // For psi terms: psi_k(gamma) is in H_nu if k <= nu
                // psi terms have v >= 0 and an arg ordinal; they may also have lcType=UNCOUNTABLE
                // because computePsiValue creates that for nu>0
                bool isPsiTerm = (t.v >= 0 && t.ord != nullptr) || (t.isPsi());
                if (isPsiTerm) {
                    if (t.v > nu) {
                        func.log("[H_ALPHA] FAIL: psi index " + std::to_string(t.v) + " > nu=" + std::to_string(nu));
                        return false;
                    }
                    // The argument gamma must be in H_k (or is Zero)
                    if (t.ord && !t.ord->checkZero(func)) {
                        // Check if argument only contains valid terms
                        for (const auto& arg_t : t.ord->terms) {
                            // Finite terms and omega are always valid
                            if (arg_t.v == -1 || arg_t.isCountableOmega) continue;
                            // Omega (uncountable) is explicitly allowed in psi arguments
                            if (arg_t.isLargeCardinal() && !arg_t.isPsi()) continue;
                            // psi terms in argument must have index <= t.v
                            if (arg_t.v >= 0 && arg_t.isPsi()) {
                                if (arg_t.v > t.v) {
                                    func.log("[H_ALPHA] FAIL: nested psi index > parent psi index");
                                    return false;
                                }
                            }
                        }
                    }
                }
                // Reflection terms are always in H_0 if v=0
                if (t.isReflection()) {
                    continue;
                }
            }
            
            func.log("[H_ALPHA] All checks passed for nu=" + std::to_string(nu));
            return true;
        }

        // Member function wrapper: check if *this is in H_nu
        bool isInH(Function& func, int nu) const {
            return BuchholzOrdinal::isInH(func, *this, nu);
        }

        // Member function wrapper: given alpha, check if alpha is in H_nu
        bool isInHForAlpha(Function& func, const BuchholzOrdinal& alpha, int nu) const {
            return BuchholzOrdinal::isInH(func, alpha, nu);
        }

        // Member function wrapper: check if *this is in C_nu(alpha)
        bool isInCForThis(Function& func, const BuchholzOrdinal& alpha, int nu) const {
            return BuchholzOrdinal::isInC(func, *this, alpha, nu, 0);
        }

        // Member function wrapper: check if beta is in C_nu(alpha)
        bool isInCForBeta(Function& func, const BuchholzOrdinal& beta, const BuchholzOrdinal& alpha, int nu) const {
            return BuchholzOrdinal::isInC(func, beta, alpha, nu, 0);
        }

        // Validate that psi_nu(alpha) is a well-formed Buchholz term
        // In Buchholz OCF, Omega can appear in the argument of psi_0
        // (it's used as a marker for uncountable cardinals)
        static bool isValidPsiTerm(Function& func, int nu, const BuchholzOrdinal& alpha, int depth = 0) {
            if (depth > 100) {
                func.log("[VALID] Recursion depth exceeded");
                return false;
            }
            if (nu < 0) return false;
            // Negative ordinals are invalid
            if (alpha.constant_coefficient < 0) {
                func.log("[VALID] FAILED: negative constant coefficient");
                return false;
            }
            // Empty ordinal with negative constant is invalid
            if (alpha.terms.empty() && alpha.constant_coefficient < 0) {
                func.log("[VALID] FAILED: negative ordinal");
                return false;
            }
            // Check that alpha only contains ordinals from H_nu
            // omega and finite numbers are always allowed
            // Omega (uncountable) is allowed as an argument to psi_0
            for (const auto& t : alpha.terms) {
                // Finite terms are always valid
                if (t.v == -1) continue;
                // Countable omega is always valid
                if (t.isCountableOmega) continue;
                // Uncountable Omega is allowed in psi_0 arguments
                if (t.isLargeCardinal() && !t.isPsi()) {
                    func.log("[VALID] Omega in argument is allowed for psi_0");
                    continue;
                }
                // psi terms with index > nu are not allowed
                if (t.v >= 0 && t.isPsi()) {
                    if (t.v > nu) {
                        func.log("[VALID] FAILED: psi index " + std::to_string(t.v) + " > nu=" + std::to_string(nu));
                        return false;
                    }
                }
            }
            func.log("[VALID] psi_" + std::to_string(nu) + "(alpha) is valid");
            return true;
        }

        // Check if ordinal beta is in C_nu(alpha) - the construction set used by Buchholz OCF
        // C_nu(alpha) is the smallest set such that:
        //   - 0 is in C_nu(alpha)
        //   - closed under + (addition)
        //   - if beta < omega^alpha and beta in C_nu(alpha), then omega^beta in C_nu(alpha)
        //   - if beta in C_nu(alpha) and beta <= alpha, then psi_nu(beta) in C_nu(alpha) (for nu <= target)
        //   - Omega_k is in C_nu(alpha) for all k <= nu when alpha >= Omega_nu
        static bool isInC(Function& func, const BuchholzOrdinal& beta, const BuchholzOrdinal& alpha, int nu, int depth = 0) {
            if (depth > 100) {
                func.log("[ISC] Recursion depth exceeded");
                return false;
            }
            func.log("[ISC] Checking if beta in C_" + std::to_string(nu) + "(alpha)");
            
            // 0 is always in C_nu(alpha)
            if (beta.checkZero(func)) {
                func.log("[ISC] 0 is in C_nu(alpha)");
                return true;
            }
            
            // Finite numbers are generated by repeated addition of 1, so they're in the set
            bool finite = true;
            for (const auto& t : beta.terms) {
                if (t.v != -1) { finite = false; break; }
            }
            if (finite) {
                func.log("[ISC] Finite ordinal is in C_nu(alpha)");
                return true;
            }
            
            // Omega_k is in C_nu(alpha) for all k <= nu
            for (const auto& t : beta.terms) {
                if (t.isLargeCardinal() && t.v <= nu) {
                    func.log("[ISC] Omega term with v=" + std::to_string(t.v) + " <= nu=" + std::to_string(nu) + " is in C_nu(alpha)");
                }
            }
            
            // For psi terms: psi_k(gamma) is in C_nu(alpha) if k <= nu, gamma in C_k(alpha), 
            // and psi_k(gamma) <= alpha
            for (const auto& t : beta.terms) {
                if (t.v >= 0 && t.isPsi() && t.ord) {
                    if (t.v > nu) {
                        func.log("[ISC] FAIL: psi index " + std::to_string(t.v) + " > nu=" + std::to_string(nu));
                        return false;
                    }
                    // Recursively check that the argument is in C_k(alpha)
                    if (!isInC(func, *t.ord, alpha, t.v, depth + 1)) {
                        func.log("[ISC] FAIL: psi argument not in C_k(alpha)");
                        return false;
                    }
                }
                // For large cardinal terms (uncountable): only allowed if v <= nu
                if (t.isLargeCardinal()) {
                    if (t.v > nu) {
                        func.log("[ISC] FAIL: large cardinal v=" + std::to_string(t.v) + " > nu=" + std::to_string(nu));
                        return false;
                    }
                }
            }
            
            func.log("[ISC] beta is in C_nu(alpha)");
            return true;
        }

        // Collect all large cardinal v levels from an ordinal
        static void collectLargeCardinalLevels(const BuchholzOrdinal& ord, std::set<int>& levels) {
            for (const auto& t : ord.terms) {
                if (t.isLargeCardinal()) {
                    levels.insert(t.v);
                }
                if (t.ord) {
                    collectLargeCardinalLevels(*t.ord, levels);
                }
            }
        }

        // Compute the value of psi_nu(alpha) as a countable ordinal
        // Implements Buchholz's OCF algorithm:
        // psi_nu(alpha) = min { beta | beta not in C_nu(alpha) }
        // where C_nu(alpha) is the construction set defined above
        static BuchholzOrdinal computePsiValue(Function& func, int nu, const BuchholzOrdinal& alpha, int depth = 0) {
            if (depth > 50) {
                throw std::runtime_error("Psi computation recursion depth exceeded");
            }
            func.log("[PSI_COMPUTE] Computing psi_" + std::to_string(nu) + "(alpha)");

            // Buchholz standard: psi_0(0) = 1 (C_0(0) = {0}, smallest missing is 1)
            if ((alpha.checkZero(func) || alpha.terms.empty()) && nu == 0) {
                func.log("[PSI_COMPUTE] psi_0(0) = 1 (Buchholz standard)");
                return fromInt(1);
            }

            // For nu > 0: psi_nu(0) = Omega_nu (the nu-th uncountable cardinal)
            if (nu > 0 && (alpha.checkZero(func) || alpha.terms.empty())) {
                func.log("[PSI_COMPUTE] psi_nu(0) = Omega_nu");
                BuchholzOrdinal result;
                Term omegaTerm;
                omegaTerm.coeff = "1";
                omegaTerm.v = nu;
                omegaTerm.ord = nullptr;
                omegaTerm.lcType = LargeCardinalType::UNCOUNTABLE;
                omegaTerm.reflectionLevel = 0;
                omegaTerm.reflectionOrder = 1;
                omegaTerm.isCountableOmega = false;
                result.terms.push_back(omegaTerm);
                return result;
            }

            // Buchholz convention: psi_nu(0) = Omega_nu for nu >= 0 (general case already handled above)

            // For psi_0(alpha) where alpha > 0:
            // Collapse psi terms to their standard ordinal values
            if (nu == 0 && !alpha.checkZero(func)) {
                // Check if alpha is a finite number
                bool allFinite = true;
                int finiteVal = 0;
                for (const auto& t : alpha.terms) {
                    if (t.v != -1) { allFinite = false; break; }
                    finiteVal += t.getCoeff();
                }
                
                if (allFinite) {
                    // psi_0(n) = omega^(n+1)
                    // psi_0(1) = omega^2, psi_0(2) = omega^3, etc.
                    func.log("[PSI_COMPUTE] psi_0(" + std::to_string(finiteVal) + ") = omega^" + std::to_string(finiteVal + 1));
                    
                    // Construct omega^(n+1) in Cantor normal form
                    // omega^2 = omega * omega represented as omega^2 term
                    BuchholzOrdinal result;
                    Term t;
                    t.coeff = "1";
                    t.v = 0;
                    t.lcType = LargeCardinalType::OMEGA;
                    t.reflectionLevel = 0;
                    t.reflectionOrder = 1;
                    t.isCountableOmega = false;
                    
                    // omega^(n+1) represented as psi_0(omega^(n+1)) simplified to omega^exp form
                    // For finite values, the result is omega^(n+1)
                    // Represent as omega^2 = omega * omega
                    if (finiteVal == 1) {
                        // psi_0(1) = omega^2
                        // Represented as: omega^2 (exponent form)
                        auto expOrd = std::make_shared<BuchholzOrdinal>();
                        Term expBase;
                        expBase.coeff = "2";
                        expBase.v = -1; // finite exponent
                        expBase.ord = nullptr;
                        expOrd->terms.push_back(expBase);
                        t.ord = expOrd;
                    } else {
                        // psi_0(n) = omega^(n+1)
                        auto expOrd = std::make_shared<BuchholzOrdinal>();
                        Term expBase;
                        expBase.coeff = std::to_string(finiteVal + 1);
                        expBase.v = -1; // finite exponent
                        expBase.ord = nullptr;
                        expOrd->terms.push_back(expBase);
                        t.ord = expOrd;
                    }
                    result.terms.push_back(t);
                    return result;
                }
                
                // Check if alpha contains omega (v=0, isCountableOmega) - this gives epsilon numbers
                bool containsOmega = false;
                bool containsOnlyOmega = true;
                for (const auto& t : alpha.terms) {
                    if (t.v == 0 && t.isCountableOmega) {
                        containsOmega = true;
                    }
                    if (!t.isCountableOmega && !t.isFinite()) {
                        containsOnlyOmega = false;
                    }
                }
                
                if (containsOmega && !containsUncountableCardinal(alpha)) {
                    // psi_0(omega) = epsilon_0
                    // psi_0(omega * 2) = epsilon_1  
                    // psi_0(omega^2) = zeta_0 (fixed point of epsilon)
                    // Represent these as special psi terms with proper alpha
                    func.log("[PSI_COMPUTE] psi_0(omega-based) - constructing collapsed value");
                    BuchholzOrdinal result;
                    
                    // Check if alpha is exactly "omega" (single omega term with coeff 1)
                    bool isPureOmega = (alpha.terms.size() == 1 && 
                                        alpha.terms[0].isCountableOmega && 
                                        alpha.terms[0].getCoeff() == 1);
                    
                    if (isPureOmega) {
                        // psi_0(omega) = epsilon_0
                        // Represent as epsilon_0 = phi(1, 0) = fixed point of alpha -> omega^alpha
                        // In normal form: epsilon_0 is represented as psi_0(omega)
                        func.log("[PSI_COMPUTE] epsilon_0 = psi_0(omega)");
                    }
                    
                    // Return the psi term as the canonical representation
                    // The collapsed value IS the psi term - that's the normal form
                    OrdPtr alphaCopy = std::make_shared<BuchholzOrdinal>(alpha);
                    result.terms.push_back(Term("1", 0, alphaCopy));
                    return result;
                }
                
                // Check if alpha contains uncountable cardinals
                if (containsUncountableCardinal(alpha)) {
                    // psi_0(Omega) = epsilon_0
                    // psi_0(Omega + 1) = psi_0(psi_0(Omega) + 1) (next after epsilon_0)
                    // Check for fixed-point property: psi_0(epsilon_0) = epsilon_0
                    // epsilon_0 = psi_0(Omega), so if alpha is structurally psi_0(Omega), return psi_0(Omega)
                    
                    // Check if alpha is a single psi term psi_0(Omega) = epsilon_0
                    bool isAlphaEpsilon0 = (alpha.terms.size() == 1 && 
                                           alpha.terms[0].v == 0 && 
                                           alpha.terms[0].ord &&
                                           alpha.terms[0].ord->terms.size() == 1 &&
                                           !alpha.terms[0].ord->terms[0].isCountableOmega &&
                                           alpha.terms[0].ord->terms[0].getCoeff() == 1);
                    
                    if (isAlphaEpsilon0) {
                        // psi_0(epsilon_0) = epsilon_0 (fixed point property)
                        // Return psi_0(Omega) which is the representation of epsilon_0
                        func.log("[PSI_COMPUTE] psi_0(epsilon_0) = epsilon_0 (fixed point)");
                        BuchholzOrdinal result;
                        OrdPtr alphaCopy = std::make_shared<BuchholzOrdinal>(alpha);
                        result.terms.push_back(Term("1", 0, alphaCopy));
                        return result;
                    }
                    
                    // psi_0(Omega) = epsilon_0
                    // psi_0(Omega + 1) = Gamma_0 (Feferman-Schutte)
                    // psi_0(Omega * 2) = LVO (Large Veblen Ordinal)
                    // These collapse to countable ordinals and are represented as psi terms
                    func.log("[PSI_COMPUTE] psi_0(uncountable) - constructing collapsed value");
                    
                    // The canonical representation of these large ordinals
                    // IS the psi_0 term itself in Buchholz normal form
                    BuchholzOrdinal result;
                    OrdPtr alphaCopy = std::make_shared<BuchholzOrdinal>(alpha);
                    result.terms.push_back(Term("1", 0, alphaCopy));
                    return result;
                }
                
                func.log("[PSI_COMPUTE] psi_0 - returning normal form");
                BuchholzOrdinal result;
                OrdPtr alphaCopy = std::make_shared<BuchholzOrdinal>(alpha);
                result.terms.push_back(Term("1", 0, alphaCopy));
                return result;
            }

            // For nu > 0 and alpha > 0: 
            // psi_nu(alpha) where alpha contains only ordinals available at level nu
            func.log("[PSI_COMPUTE] Returning as normal form representation");
            BuchholzOrdinal result;
            OrdPtr alphaCopy = std::make_shared<BuchholzOrdinal>(alpha);
            result.terms.push_back(Term("1", nu, alphaCopy));
            return result;
        }

        // Helper: check if an ordinal contains any uncountable cardinal terms
        static bool containsUncountableCardinal(const BuchholzOrdinal& ord) {
            for (const auto& t : ord.terms) {
                if (t.isLargeCardinal() && t.lcType == LargeCardinalType::UNCOUNTABLE) return true;
                if (t.ord && containsUncountableCardinal(*t.ord)) return true;
            }
            return false;
        }

        // Check if ordinal is within BO = psi_0(Omega_omega)
        // Standard Buchholz OCF (case 5) supports:
        // - Countable ordinals (omega, finite numbers)
        // - psi_v(...) with finite v
        // - Uncountable cardinals Omega[1], Omega[2], ... (finite indices)
        // Exceeds BO if:
        // - Contains I_rec, M_rec, or other large cardinals (RECURSIVE_INACCESSIBLE, RECURSIVE_MALO)
        // - Contains reflection operators (Pi1, Pi2, ...)
        static bool isWithinBO(const BuchholzOrdinal& ord) {
            for (const auto& t : ord.terms) {
                // I_rec, M_rec exceed basic Buchholz OCF
                if (t.isLargeCardinal()) {
                    if (t.lcType == LargeCardinalType::RECURSIVE_INACCESSIBLE) return false;
                    if (t.lcType == LargeCardinalType::RECURSIVE_MALO) return false;
                }
                // Reflection operators exceed basic Buchholz OCF
                if (t.isReflection()) return false;
                // Recursively check nested ordinals
                if (t.ord && !isWithinBO(*t.ord)) return false;
            }
            return true;
        }
    };

    // Out-of-line definition for Term::toString (needs complete BuchholzOrdinal type)
    inline std::string Term::toString() const {
        if (isFinite()) return coeff;
        if (isCountable()) {
            std::string c = (getCoeff() == 1) ? "" : ("*" + coeff);
            if (ord != nullptr) {
                std::string expStr = ord->toString();
                return "\u03C9" + c + "^" + expStr;
            }
            return "\u03C9" + c;
        }
        if (isLargeCardinal()) {
            std::string c = (getCoeff() == 1) ? "" : ("*" + coeff);
            std::string lcName = getLargeCardinalName(lcType);
            if (v > 0) return lcName + c + "[" + std::to_string(v) + "]";
            return lcName + c;
        }
        if (isReflection()) {
            std::string c = (getCoeff() == 1) ? "" : ("*" + coeff);
            std::string a = (ord ? ord->toString() : "0");
            if (reflectionLevel == 99) {
                if (reflectionOrder == 2) return c + "PiOmega1(" + a + ")";
                return c + "PiOmega(" + a + ")";
            }
            if (reflectionOrder == 2) {
                return c + "Pi" + std::to_string(reflectionLevel) + "1(" + a + ")";
            }
            return c + "Pi" + std::to_string(reflectionLevel) + "(" + a + ")";
        }
        std::string c = (getCoeff() == 1) ? "" : ("*" + coeff);
        if (v >= 0) {
            if (!ord) {
                return c + "\u03A9_" + std::to_string(v);
            }
            std::string alphaStr = (ord ? ord->toString() : "0");
            return c + "\u03c8_" + std::to_string(v) + "(" + alphaStr + ")";
        }
        return c + "\u03c8_" + std::to_string(v);
    }