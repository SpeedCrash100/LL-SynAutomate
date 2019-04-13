#include "automate.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

Automate::Automate()
    : m_axiom("S")
    , m_empty("EMPTY")
    , m_eow("EOW")
{

    m_symbolsMap[m_empty] = Symbol(m_empty, "<" + m_empty + ">", true);
    m_symbolsMap[m_eow] = Symbol(m_eow, "<" + m_eow + ">", true);

    initGrammar();
    reset();
}

bool Automate::Analyze(std::basic_istream<char>& input)
{
    if (input.fail())
        return false;
    reset();

    std::cout << "Used rules: ";

    Symbol symbol = Terminal(static_cast<char>(input.peek()));

    while (!m_symbols.empty()) {
        auto topStack = m_symbols.top();

        symbol = Terminal(static_cast<char>(input.peek()));
        if (isEOW(symbol))
            symbol = m_symbolsMap[m_eow];

        auto cmd = getCommand(topStack, symbol);
        processCommand(cmd);

        if (topStack == symbol) {
            m_symbols.pop();
            input.get();
        } else if (isTerm(topStack) || !isValid(symbol)) {
            break;
        } else {
            int prodRule = getProductionRuleID(topStack, symbol);
            if (prodRule == -1) {
                break;
            }

            std::cout << prodRule << ", ";

            m_symbols.pop();
            auto prodStack = m_productionRules[static_cast<size_t>(prodRule)];
            while (!prodStack.empty()) {
                auto sym = prodStack.top();
                if (sym != m_symbolsMap[m_empty])
                    m_symbols.push(sym);
                prodStack.pop();
            }
        }
    }

    std::cout << "END\n";

    std::cout << "Readed : " << m_readed << "\n";

    if (m_symbols.empty() && symbol == m_symbolsMap[m_eow]) {
        onSuccess();
        return true;
    }

    onFail();
    return false;
}

void Automate::onSuccess()
{
    std::cout << "Success\n";
}

void Automate::onFail()
{
    std::cout << "Invalid word.\n";
}

void Automate::initGrammar()
{
    m_eowSymbols = { '\0', '\n' };
}

void Automate::reset()
{
    while (!m_symbols.empty())
        m_symbols.pop();
    m_symbols.push(m_symbolsMap[m_axiom]);

    while (!m_values.empty())
        m_values.pop();
}

bool Automate::isTerm(Symbol sym) const
{
    auto it = m_symbolsMap.find(sym.ID());
    if (it != m_symbolsMap.end()) {
        if ((*it).second.isTerm())
            return true;
    }
    return false;
}

bool Automate::isNonTerm(Symbol sym) const
{
    auto it = m_symbolsMap.find(sym.ID());
    if (it != m_symbolsMap.end()) {
        if (!(*it).second.isTerm())
            return true;
    }
    return false;
}

bool Automate::isValid(Symbol sym) const
{
    return isTerm(sym) || isNonTerm(sym);
}

bool Automate::isEOW(const Symbol& sym) const
{
    for (auto eow : m_eowSymbols) {
        if (eow == sym)
            return true;
    }
    return false;
}

void Automate::addTerminal(char ch)
{
    if (m_symbolsMap.find({ ch }) != m_symbolsMap.end()) {
        throw std::runtime_error("The symbol with ID \"" + std::string({ ch }) + "\" is already exist");
    }

    Symbol sym = Symbol({ ch }, { ch }, true);
    m_symbolsMap[sym.ID()] = sym;
}

void Automate::addNonTerminal(std::string ID)
{
    if (m_symbolsMap.find(ID) != m_symbolsMap.end()) {
        throw std::runtime_error("The symbol with ID \"" + ID + "\" is already exist");
    }

    Symbol sym = Symbol(ID, "<" + ID + ">", false);
    m_symbolsMap[ID] = sym;
}

int Automate::getProductionRuleID(Symbol topStack, Symbol currentSymbol) const
{
    int rule = -1;
    try {
        rule = m_translateTable.at(topStack).at(currentSymbol);
    } catch (...) {
        return -1;
    }
    return rule;
}

Automate::Command Automate::getCommand(Symbol topStack, Symbol currentSymbol)
{
    Automate::Command emptyCommand = []() { return; };
    if (!isValid(topStack) || !isValid(currentSymbol))
        return emptyCommand;

    auto cmd = emptyCommand;
    try {
        cmd = m_commandTable.at(topStack).at(currentSymbol);
    } catch (...) {
        return emptyCommand;
    }
    return cmd;
}

void Automate::processCommand(Automate::Command cmd)
{
    cmd();
}
