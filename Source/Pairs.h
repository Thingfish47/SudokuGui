/*
  ==============================================================================

    Pairs.h
    Created: 17 Jul 2024 6:12:38pm
    Author:  godfr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class Pair
{
public:
    Pair();
    Pair(int, int);

    // Copy Constructor
    Pair(const Pair& other) : first(other.first), second(other.second) {}

    //     ~Pair() {};

    Pair& operator=(const Pair& other) {
        if (this != &other) { // Self-assignment check
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    bool operator==(const Pair& other) const {
        return (first == other.first) && (second == other.second);
    }
    String toString();
    const int get1st() { return first; }
    const int get2nd() { return second; }
private:
    int     first;
    int     second;
};