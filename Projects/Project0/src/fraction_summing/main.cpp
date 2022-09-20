#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "fraction_toolbox.hpp"

using namespace std;

// read command line arguments
static void readcmdline(fraction & frac, int argc, char* argv[])
{
    if (argc!=3)
    {
        printf("Usage: n d\n");
        printf("  n        numerator of fraction\n");
        printf("  d        denominator of fraction\n");
        exit(1);
    }

    // read n
    frac.num = atoi(argv[1]);

    // read d
    frac.denom = atoi(argv[2]);
}

static void test23467(int argc, char* argv[])
{
    //TODO: implement function
    fraction f;
    readcmdline(f, argc, argv);
    // 2 square frac
    printf("Testing square_fraction with: ");
    print_fraction(f);
    printf("Resulting fraction: ");
    print_fraction(square_fraction(f));

    // 3 square frac inplace
    printf("\nTesting square_fraction_inline with: ");
    print_fraction(f);
    square_fraction_inline(f);
    printf("Resulting fraction: ");
    print_fraction(f);

    // 4 frac to double
    printf("\nTesting fraction2double with: ");
    print_fraction(f);
    cout<<"Resulting double: " + to_string(fraction2double(f));

    // 6 gcd overload
    printf("\n\nTesting gcd with: ");
    print_fraction(f);
    cout<<"Resulting gcd is: " + to_string(gcd(f));

    // 7 reduce frac inplace
    printf("\n\nTesting reduce_fraction_inplace with: ");
    print_fraction(f);
    printf("Resulting reduced fraction: ");
    reduce_fraction_inplace(f);
    print_fraction(f);
}

static void test5()
{
    //TODO: implement function
    cout<<"Please enter two integers, one at the time."<<endl;
    int a, b;
    cin>>a;
    cin>>b;
    cout<<"The greatest common divisor between " + to_string(a) + " and " + to_string(b) + " is " + to_string(gcd(a, b))<<endl;
}

static void test_array_functions(int n)
{
    //TODO: implement function
    fraction * fracs;
    fracs = (fraction*) malloc(n);
    fill_fraction_array(fracs, n);
    //print_fraction_array(fracs, n);

    cout<<"Sum as fraction: ";
    print_fraction(sum_fraction_array(fracs, n));
    cout<<"Sum of fractions: " + to_string(sum_fraction_array_approx(fracs, n))<<endl;
    //TODO: find n for which sum function breaks. Explain what is happening.
    /*
     * After XXX the integer will be larger than an 32-integer can represent. When this number gets exceeded
     * the bits will overflow and the intentionally large integer will suddenly start over at the smallest
     * possible number representable with an integer (-2**31).
     */
}

static void test_toolbox(int argc, char* argv[])
{
    cout << "\n===============  test23467  =============== " << endl;
    test23467(argc, argv);

    cout << "\n=================  test5  ================= " << endl;
    test5();

    cout << "\n==========  test_array_functions  ========= " << endl;
    int n = 20000;
    test_array_functions(n);
}

int main(int argc, char* argv[])
{
    test_toolbox(argc, argv);
}