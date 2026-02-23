#pragma once

#include <string>
#include <unordered_map>

namespace vine {

// String-rewriting L-system that produces turtle-graphics command strings.
// Typical symbols for vine growth:
//   F  – move forward (draw segment)
//   +/- – yaw left / right
//   &/^ – pitch down / up
//   [/] – push / pop turtle state
class LSystem {
public:
    // A production rule maps one character to a replacement string
    using Rules = std::unordered_map<char, std::string>;

    LSystem(std::string axiom, Rules rules, float angleDeg = 25.f);

    // Rewrite the current string n times and return it
    const std::string& generate(int iterations);

    // Return the current (most recently generated) string
    const std::string& getString() const;

    // Reset to the original axiom
    void reset();

    float angle()          const;
    void  setAngle(float deg);

private:
    std::string m_axiom;
    std::string m_current;
    Rules       m_rules;
    float       m_angle;

    // Apply one round of production rules
    std::string applyRules(const std::string& s) const;
};

} // namespace vine
