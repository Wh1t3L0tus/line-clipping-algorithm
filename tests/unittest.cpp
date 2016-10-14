//
// Created by theo on 10/12/16.
//
#include <iostream>
#include <cfloat>

#define EPSILON FLT_EPSILON
using namespace std;

void printFailure(const string& testName, const string& msg) {
    cerr << "Test failure [" << testName << "] : " << msg << endl;
}

void printSuccess(const string& testName) {
    cout << "Test success [" << testName << "]" << endl;
}

void isTrue(bool actual, const string& testName, const string& msg) {
    if (actual) {
        printSuccess(testName);
    }
    else {
        printFailure(testName, msg);
    }
}

void isFalse(bool actual, const string& testName, const string& msg) {
    isTrue(!actual, testName, msg);
}

void equals(float actual, float expected, const std::string& testName, const std::string& msg) {
    if (actual >= expected - EPSILON && actual <= actual + EPSILON) {
        printSuccess(testName);
    }
    else {
        printFailure(testName, msg);
    }
}