#ifndef AUTOMATE_HPP
#define AUTOMATE_HPP

#include "symbol.hpp"
#include <functional>
#include <istream>
#include <map>
#include <stack>
#include <string>
#include <vector>

class Automate {
protected:
    typedef std::function<void(void)> Command;

    std::vector<Terminal> m_eowSymbols;
    std::vector<std::stack<Symbol>> m_productionRules;
    std::map<Symbol, std::map<Symbol, int>> m_translateTable;
    std::map<Symbol, std::map<Symbol, Command>> m_commandTable;

    std::string m_axiom;
    std::string m_empty;
    std::string m_eow;

    std::stack<Symbol> m_symbols;

private:
    std::map<std::string, Symbol> m_symbolsMap;

    //std::basic_string<char> m_readed;

public:
    Automate();
    Automate(const Automate&) = delete;

    void Init();

    bool Analyze(std::basic_istream<char>& input);

    virtual void onSuccess();
    virtual void onFail();

protected:
    virtual void initGrammar();
    virtual void reset();

    bool isTerm(Symbol sym) const;
    bool isNonTerm(Symbol sym) const;
    bool isValid(Symbol sym) const;
    bool isEOW(const Symbol& sym) const;

    Symbol getEmptySymbol() const;
    Symbol getEOWSymbol() const;

    const Symbol& addTerminal(char ch);
    const Symbol& addNonTerminal(std::string ID);

private:
    int getProductionRuleID(Symbol topStack, Symbol currentSymbol) const;
    Automate::Command getCommand(Symbol topStack, Symbol currentSymbol);

    void processCommand(Command cmd);
};

#endif // AUTOMATE_HPP
