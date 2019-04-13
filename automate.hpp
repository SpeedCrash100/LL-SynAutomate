#ifndef AUTOMATE_HPP
#define AUTOMATE_HPP

#include <map>
#include <stack>
#include <vector>
#include <istream>
#include <string>

class Automate {
private:
    enum Symbols {
        TERM_INVALID = 0,

        TERM_BEGIN,
        TERM_EOW,
        TERM_EMPTY,
        TERM_END,

        NTERM_BEGIN,
        NTERM_END
    };

    enum Commands {
        CMD_NO = 0
    };

    std::map<char, Symbols> m_charToTerm;
    std::vector<std::stack<Symbols>> m_productionRules;
    std::map<Symbols, std::map<Symbols, int>> m_translateTable;
    std::map<Symbols, std::map<Symbols, Commands>> m_commandTable;

    std::stack<Symbols> m_symbols;
    std::stack<int> m_values;

	std::basic_string<char> m_readed;
	Symbols m_axiom;

public:
    Automate();
    Automate(const Automate&) = delete;

    bool Analyze(std::basic_istream<char>& input);

	void onSuccess();
	void onFail();

private:

	void initGrammar();
	void reset();

    bool isTerm(Symbols sym) const;
    bool isNonTerm(Symbols sym) const;
    bool isValid(Symbols sym) const;
    int getProductionRuleID(Automate::Symbols nterm, Automate::Symbols term) const;
    Commands getCommand(Automate::Symbols topStack, Automate::Symbols currentSymbol);

    void processCommand(Commands cmd);
};

#endif // AUTOMATE_HPP