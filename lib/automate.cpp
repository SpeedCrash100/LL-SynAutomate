#include "automate.hpp"
#include <iostream>
#include <stdexcept>

Automate::Automate()
{

	m_symbolsMap["EMPTY"] = Symbol("EMPTY", "<EMPTY>", true);
    m_symbolsMap["EOW"] = Symbol("EOW", "<EOW>", true);

    initGrammar();
    reset();
}

bool Automate::Analyze(std::basic_istream<char>& input)
{
    if (input.fail())
        return false;
    reset();

    std::cout << "Used rules: ";

    char symbol = input.peek();

    while (!m_symbols.empty()) {
        auto topStack = m_symbols.top();
        symbol = input.peek();
        auto symbolAsTerminal = m_symbolsMap[m_charToTerm[symbol]];

        auto cmd = getCommand(topStack, symbolAsTerminal);
        processCommand(cmd);

        if (topStack == symbolAsTerminal) {
            m_symbols.pop();
            input.get();
        } else if (isTerm(topStack) || !isValid(symbolAsTerminal)) {
            break;
        } else {
            int prodRule = getProductionRuleID(topStack, symbolAsTerminal);
            if (prodRule == -1) {
                break;
            }

            std::cout << prodRule << ", ";

            m_symbols.pop();
            auto prodStack = m_productionRules[static_cast<size_t>(prodRule)];
            while (!prodStack.empty()) {
                auto sym = prodStack.top();
                if (sym != m_symbolsMap["EMPTY"])
                    m_symbols.push(sym);
                prodStack.pop();
            }
        }
    }

    std::cout << "END\n";

    std::cout << "Readed : " << m_readed << "\n";

	auto curTerminal = m_symbolsMap[m_charToTerm[symbol]];

    if (m_symbols.empty() && curTerminal == m_symbolsMap["EOW"]) {
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
    m_charToTerm = {
        { '\0', "EOW" },
        { '\n', "EOW" }
    };
}

void Automate::reset()
{
    while (!m_symbols.empty())
        m_symbols.pop();
    m_symbols.push(m_symbolsMap[ m_axiom ]);

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
