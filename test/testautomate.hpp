#ifndef TESTAUTOMATE_HPP
#define TESTAUTOMATE_HPP

#include "automate.hpp"

class TestAutomate : public Automate {
    std::stack<int> m_values;
public:
    TestAutomate();
    virtual ~TestAutomate() = default;

    // Automate interface
public:
    void onSuccess() override;
    void onFail() override;

protected:
    void initGrammar() override;
    void reset() override;
};

#endif // TESTAUTOMATE_HPP
