#ifndef TESTAUTOMATE_HPP
#define TESTAUTOMATE_HPP

#include "automate.hpp"

class TestAutomate : public Automate {
public:
    TestAutomate();
    virtual ~TestAutomate() = default;

    // Automate interface
public:
    void onSuccess() override;
    void onFail() override;

protected:
    void initGrammar() override;
};

#endif // TESTAUTOMATE_HPP
