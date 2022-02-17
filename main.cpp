#include <iostream>
#include "windows.h"
#include "complex"

#include "Eigen/Dense"

#include "Signal.h"

using namespace std;
using namespace Eigen;

int main()
{
    vector<double> a;
    double* b = new double[100];
    for (int i = 0; i < 100; ++i)
    {
        a.push_back(static_cast<double>(i));
        b[i] = i * 3.5;
    }

//    Signal<double> s1 = Signal<double>::randomSignal(10000000, TIME_DOMAIN);;
//    Signal<double> s2 = Signal<double>::randomSignal(10000000, TIME_DOMAIN);;


    Signal<double> result1;
    Signal<double> result2;
    Signal<double> result3;
    Signal<double> result4;
    Signal<double> result5;
    Signal<double> result6;
    Signal<double> result7;
    Signal<double> result8;
    Signal<double> result9;
    Signal<double> result10;
    Signal<double> s1 = Signal<double>::randomSignal(10000000, TIME_DOMAIN);
    Signal<double> s2 = Signal<double>::randomSignal(10000000, TIME_DOMAIN);
    try
    {
        long startTime = GetTickCount();
        result1 = s1 + s2;
        result2 = s2 + s1;
        result3 = s1 - s2;
        result4 = s2 - s1;
        result5 = s1 * s2;
        result6 = s2 * s1;
        result7 = s1 * 0.28;
        result8 = 0.28 * s1;
        result9 = s2 * 0.28;
        result10 = 0.28 * s2;

        long endTime = GetTickCount();
        long runTime = endTime - startTime;
        printf("%d ms\n", runTime);
    }
    catch (const char* error)
    {
        cout << error;
    }
    cout << result1[1234567];

    return 0;
}
