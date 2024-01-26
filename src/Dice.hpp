#pragma once


#include <cassert>
#include <random>

class DiceI {
public:
    virtual ~DiceI() = default;
    virtual unsigned roll() const noexcept = 0;
};

template <typename Strategy>
class Dice : public DiceI {
public:
    explicit Dice(unsigned count) noexcept
        : m_count(count) {
        assert(m_count > 0);
    }

    unsigned roll() const noexcept override {
        auto tmp = m_count - 1;
        unsigned out = rollOne();
        while (tmp--) {
            out = Strategy::merge(out, rollOne());
        }
        return out;
    }
private:
    static unsigned rollOne() noexcept {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned> distrib(0, 6);
        return distrib(gen);
    }

    unsigned m_count;
};

namespace details {

struct Sum {
    static unsigned merge(unsigned lhs, unsigned rhs) noexcept{
        return lhs + rhs;
    }
};

struct Max {
    static unsigned merge(unsigned lhs, unsigned rhs) noexcept{
        return std::max(lhs, rhs);
    }
};

struct Min {
    static unsigned merge(unsigned lhs, unsigned rhs) noexcept{
        return std::min(lhs, rhs);
    }
};

}
