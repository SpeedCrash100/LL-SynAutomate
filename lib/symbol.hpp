#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>

class Symbol {
    std::string m_ID;
    std::string m_printedName;
    bool m_isTerminal;

public:
    Symbol(std::string id = "", std::string printedName = "", bool isTerm = false);
    Symbol(const Symbol&) = default;
    ~Symbol() = default;

	auto printedName() const { return m_printedName; }
    auto ID() const { return m_ID; }
    auto isTerm() const { return m_isTerminal; }

	bool operator==(const Symbol& other) const;
    bool operator!=(const Symbol& other) const;
    inline bool operator<(const Symbol& other) const { return m_ID < other.m_ID; }
    inline bool operator>(const Symbol& other) const { return m_ID > other.m_ID; }

};



#endif // SYMBOL_HPP