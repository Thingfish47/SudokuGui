/*
  ==============================================================================

    Pairs.h
    Created: 17 Jul 2024 6:12:38pm
    Author:  godfr

  ==============================================================================
*/

#pragma once


class Pair
{
public:
    Pair();
    Pair(int, int);
//    Pair(int id1 = 0, int id2 = 0) : id1(id1), id2(id2) {}    //  No ordering.

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

private:
    int     first;
    int     second;
};