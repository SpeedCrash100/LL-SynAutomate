#ifndef AUTOMATE_HPP
#define AUTOMATE_HPP

#include <map>
#include <stack>
#include <vector>
#include <istream>
#include <string>
#include <functional>
#include "symbol.hpp"

class Automate {
private:
	typedef std::function<void(void)> Command;

	std::map<std::string, Symbol> m_symbolsMap;

    std::map<char, std::string> m_charToTerm;
    std::vector<std::stack<Symbol>> m_productionRules;
    std::map<Symbol, std::map<Symbol, int>> m_translateTable;

    std::map<Symbol, std::map<Symbol, Command>> m_commandTable;

    std::stack<Symbol> m_symbols;
    std::stack<int> m_values;

	std::basic_string<char> m_readed;

	std::string m_axiom;

public:
    Automate();
    Automate(const Automate&) = delete;

    bool Analyze(std::basic_istream<char>& input);

	virtual void onSuccess();
	virtual void onFail();

protected:

	virtual void initGrammar();
	virtual void reset();

    bool isTerm(Symbol sym) const;
    bool isNonTerm(Symbol sym) const;
    bool isValid(Symbol sym) const;

	void addTerminal(char ch); 
    void addNonTerminal(std::string ID); 

private:
    int getProductionRuleID(Symbol topStack, Symbol currentSymbol) const;
    Automate::Command getCommand(Symbol topStack, Symbol currentSymbol);

    void processCommand(Command cmd);
};

#endif // AUTOMATE_HPP
