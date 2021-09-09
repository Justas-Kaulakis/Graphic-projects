#ifndef RANDOMGEN_H_INCLUDED
#define RANDOMGEN_H_INCLUDED
#include "HSV_HSL.h"
#include <time.h>
#include <boost/random.hpp>
#include <boost/nondet_random.hpp>

class RandomGen {
private:
    std::random_device rand_dev;
    boost::random::mt19937 generator;
    //boost::random::uniform_int_distribution<T> distr(range_from, range_to);
public:
    RandomGen()
    : rand_dev(), generator(rand_dev()) {}

    template<typename T>
    T getInRange(T range_from, T range_to) {
        boost::random::uniform_int_distribution<T>    distr(range_from, range_to);
        return distr(generator);
    }
    sf::Color getRandColor() {
        //float hue = (float)(rand() % 360);
        float hue = getInRange(1, 360);
        cout << hue << endl;

        return HSVtoRGB(hue, 1.f, 1.f);
    }
};


#endif // RANDOMGEN_H_INCLUDED
