/*
  ==============================================================================

    Pairs.cpp
    Created: 17 Jul 2024 6:12:38pm
    Author:  godfr

  ==============================================================================
*/

#include "Pairs.h"


Pair::Pair (int a, int b)
{
    first = 0;
    second = 0;
    if (a < b)
    {
        first = a;
        second = b;
    }
    else
    {
        first = b;
        second = a;
    }
}

Pair::Pair()
{
	first = 0;
	second = 0;
}

String Pair::toString()
{
    String txt = "{" + String(first) + " " + String(second) + "}";
    return txt;
}
