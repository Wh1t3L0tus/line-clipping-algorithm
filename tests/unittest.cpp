//
// Created by theo on 10/12/16.
//
#include <iostream>

using namespace std;

void printFailure(const string& testName, const string& msg) {
    cerr << "Test failure [" << testName << "] : " << msg << endl;
}

void printSuccess(const string& testName, const string& msg) {
    cout << "Test success [" << testName << "]" << endl;
}

void isTrue(bool actual, const string& testName, const string& msg) {
    if (actual) {
        printSuccess(testName, msg);
    }
    else {
        printFailure(testName, msg);
    }
}

void isFalse(bool actual, const string& testName, const string& msg) {
    isTrue(!actual, testName, msg);
}

