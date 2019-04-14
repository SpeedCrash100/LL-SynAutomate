#include "testautomate.hpp"
#include <iostream>

TestAutomate::TestAutomate()
    : Automate()
{
}

void TestAutomate::onSuccess()
{
    Automate::onSuccess();
    if (!m_values.empty()) {
        auto value = m_values.top();
        std::cout << "Value is " << value << "\n";
    } else {
        std::cout << "ERROR: Cannot get value!";
    }
}

void TestAutomate::onFail()
{
    Automate::onFail();
}

void TestAutomate::initGrammar()
{
    m_eowSymbols = { '\0', '\n' };
    auto TERM_2 = addTerminal('2');
    auto TERM_3 = addTerminal('3');
    auto TERM_4 = addTerminal('4');
    auto TERM_SUM = addTerminal('+');
    auto TERM_MUL = addTerminal('*');

    auto NTERM_S = addNonTerminal("S");
    auto NTERM_E = addNonTerminal("E");
    auto NTERM_D = addNonTerminal("D");
    auto NTERM_R = addNonTerminal("R");
    auto NTERM_G = addNonTerminal("G");

    m_axiom = "S";

    m_productionRules = {
        std::stack<Symbol>({ NTERM_E }), ///< 0. S -> E
        std::stack<Symbol>({ NTERM_D, NTERM_R }), ///< 1. E -> DR
        std::stack<Symbol>({ NTERM_E, NTERM_G }), ///< 2. R -> EG
        std::stack<Symbol>({ TERM_SUM, NTERM_R }), ///< 3. G -> +R
        std::stack<Symbol>({ TERM_MUL, NTERM_R }), ///< 4. G -> *R
        std::stack<Symbol>({ getEmptySymbol() }), ///< 5. R -> empty
        std::stack<Symbol>({ TERM_2 }), ///< 6. D -> 2
        std::stack<Symbol>({ TERM_3 }), ///< 7. D -> 3
        std::stack<Symbol>({ TERM_4 }), ///< 8. D -> 4
    };

    m_translateTable[NTERM_S][TERM_2] = 0;
    m_translateTable[NTERM_S][TERM_3] = 0;
    m_translateTable[NTERM_S][TERM_4] = 0;
    m_translateTable[NTERM_E][TERM_2] = 1;
    m_translateTable[NTERM_E][TERM_3] = 1;
    m_translateTable[NTERM_E][TERM_4] = 1;
    m_translateTable[NTERM_D][TERM_2] = 6;
    m_translateTable[NTERM_D][TERM_3] = 7;
    m_translateTable[NTERM_D][TERM_4] = 8;
    m_translateTable[NTERM_R][TERM_2] = 2;
    m_translateTable[NTERM_R][TERM_3] = 2;
    m_translateTable[NTERM_R][TERM_4] = 2;
    m_translateTable[NTERM_R][TERM_SUM] = 5;
    m_translateTable[NTERM_R][TERM_MUL] = 5;
    m_translateTable[NTERM_R][getEOWSymbol()] = 5;
    m_translateTable[NTERM_G][TERM_SUM] = 3;
    m_translateTable[NTERM_G][TERM_MUL] = 4;

    auto addDigit = [&](int digit) {
        m_values.push(digit);
    };

    auto addDigit2 = [=]() { addDigit(2); };
    auto addDigit3 = [=]() { addDigit(3); };
    auto addDigit4 = [=]() { addDigit(4); };

    auto sum = [&]() {
        if(m_values.size() < 2)
            return;
        auto fst = m_values.top();
        m_values.pop();
        auto scnd = m_values.top();
        m_values.pop();
        m_values.push(fst + scnd); };

    auto mul = [&]() {
        if (m_values.size() < 2)
            return;
        auto fst = m_values.top();
        m_values.pop();
        auto scnd = m_values.top();
        m_values.pop();
        m_values.push(fst * scnd);
    };

    m_commandTable[TERM_2][TERM_2] = addDigit2;
    m_commandTable[TERM_3][TERM_3] = addDigit3;
    m_commandTable[TERM_4][TERM_4] = addDigit4;
    m_commandTable[TERM_SUM][TERM_SUM] = sum;
    m_commandTable[TERM_MUL][TERM_MUL] = mul;
}
void TestAutomate::reset()
{
    Automate::reset();
    while (!m_values.empty())
        m_values.pop();
}
