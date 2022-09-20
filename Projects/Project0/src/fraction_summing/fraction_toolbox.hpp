#include <iostream>

//TODO: implement fraction datatype
struct fraction {
    int num;
    int denom;
};

void print_fraction(fraction frac);

void print_fraction_array(fraction frac_array[], int n);

fraction square_fraction(fraction frac);

//TODO: add function declaration for function 3
void square_fraction_inline(fraction & frac);

double fraction2double(fraction frac);

int gcd(int a, int b);

int gcd(fraction frac);

//TODO: add function declaration for function 7
void reduce_fraction_inplace(fraction &frac);

fraction add_fractions(fraction frac1, fraction frac2);

double sum_fraction_array_approx(fraction frac_array[], int n);

fraction sum_fraction_array(fraction frac_array[], int n);

void fill_fraction_array(fraction frac_array[], int n);