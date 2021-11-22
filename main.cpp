/*
 *  UCF COP3330 Fall 2021 Assignment 5 Solution
 *  Copyright 2021 Michael Vuolo
 */

// Q: Can we declare a non-reference function argument const (e.g., void
// f(const int);)? What might that mean? Why might we want to do that?
// Why don’t people do that often?

// A: Yes we can declare a non-reference function argument const.
// That means in the function, the argument will not update.
// We might want to do this to ensure the argument will not update when the function is called.
// People don't do this often because it is simple to not update the argument anyway, especially since functions don't update the original value of the argument.

#include <iostream>
#include <cmath>

using namespace std;

// function prototypes
int pow3(const int number);
int pow5(const int number);
int pow7(const int number);

int main() {
    // init number variable
    int number;

    // read number from user
    cout << "Enter a number: ";
    cin >> number;

    // display number to the powers of 3, 5, and 7
    cout << "The number " << number << " to the power of 3 is " << pow3(number) << ".\n";
    cout << "The number " << number << " to the power of 5 is " << pow5(number) << ".\n";
    cout << "The number " << number << " to the power of 7 is " << pow7(number) << ".\n";

    return 0;
}

// take a number and put it to the power of 3
int pow3(const int number) {
    return pow(number, 3);
}

// take a number and put it to the power of 5
int pow5(const int number) {
    return pow(number, 5);
}

// take a number and put it to the power of 7
int pow7(const int number) {
    return pow(number, 7);
}