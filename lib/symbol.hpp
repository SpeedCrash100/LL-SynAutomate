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

    virtual bool operator==(const Symbol& other) const;
    virtual bool operator!=(const Symbol& other) const;
    virtual Symbol& operator=(const Symbol& other);
    virtual inline bool operator<(const Symbol& other) const { return m_ID < other.m_ID; }
    virtual inline bool operator>(const Symbol& other) const { return m_ID > other.m_ID; }
};

class Terminal : public Symbol {
public:
    Terminal(char ch = '\0')
        : Symbol({ ch }, { ch }, true)
    {
    }
};

class NonTerminal : public Symbol {
public:
    NonTerminal(std::string id)
        : Symbol(id, "<" + id + ">", false)
    {
    }
};

#endif // SYMBOL_HPP
