//
// Created by theo on 10/12/16.
//

#ifndef LINE_CLIPPING_ALGORITHM_UNITTEST_H
#define LINE_CLIPPING_ALGORITHM_UNITTEST_H

#include <string>

void isTrue(bool actual, const std::string& testName, const std::string& msg);
void isFalse(bool actual, const std::string& testName, const std::string& msg);

void equals(float actual, float expected, const std::string& testName, const std::string& msg);
#endif //LINE_CLIPPING_ALGORITHM_UNITTEST_H
