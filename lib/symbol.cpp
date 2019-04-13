#include "symbol.hpp"

Symbol::Symbol(std::string id, std::string printedName, bool isTerm)
    : m_ID(id)
    , m_printedName(printedName)
    , m_isTerminal(isTerm)
{
}

bool Symbol::operator==(const Symbol& other) const
{
    return this->m_ID == other.m_ID && this->m_isTerminal == other.m_isTerminal;
}

bool Symbol::operator!=(const Symbol& other) const
{
    return !(*this == other);
}

Symbol& Symbol::operator=(const Symbol& other)
{
    this->m_ID = other.m_ID;
    this->m_isTerminal = other.m_isTerminal;
    this->m_printedName = other.m_printedName;
    return *this;
}
