#include "automate.hpp"
#include <iostream>

Automate::Automate() : m_axiom(NTERM_BEGIN)
{
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

        auto cmd = getCommand(topStack, m_charToTerm[symbol]);
        processCommand(cmd);

        if (topStack == m_charToTerm[symbol]) {
            m_symbols.pop();
            input.get();
        } else if (isTerm(topStack) || !isValid(m_charToTerm[symbol])) {
            break;
        } else {
            int prodRule = getProductionRuleID(topStack, m_charToTerm[symbol]);
            if (prodRule == -1) {
                break;
            }

            std::cout << prodRule << ", ";

            m_symbols.pop();
            auto prodStack = m_productionRules[static_cast<size_t>(prodRule)];
            while (!prodStack.empty()) {
                auto sym = prodStack.top();
                if (sym != TERM_EMPTY)
                    m_symbols.push(sym);
                prodStack.pop();
            }
        }
    }

    std::cout << "END\n";

	std::cout << "Readed : " << m_readed << "\n";

    if (m_symbols.empty() && m_charToTerm[symbol] == TERM_EOW) {
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
		{ '\0', TERM_EOW },
		{ '\n', TERM_EOW }
	};
}

void Automate::reset()
{
	while (!m_symbols.empty())
		m_symbols.pop();
	m_symbols.push(m_axiom); 

	while (!m_values.empty())
		m_values.pop();
}

bool Automate::isTerm(Automate::Symbols sym) const
{
    return TERM_BEGIN < sym && sym < TERM_END;
}

bool Automate::isNonTerm(Automate::Symbols sym) const
{
    return NTERM_BEGIN < sym && sym < NTERM_END;
}

bool Automate::isValid(Automate::Symbols sym) const
{
    return isTerm(sym) || isNonTerm(sym);
}

int Automate::getProductionRuleID(Automate::Symbols nterm, Automate::Symbols term) const
{
    int rule = -1;
    try {
        rule = m_translateTable.at(nterm).at(term);
    } catch (...) {
        return -1;
    }
    return rule;
}

Automate::Command Automate::getCommand(Automate::Symbols topStack, Automate::Symbols currentSymbol)
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
