#include <iostream>

#include "fraction_toolbox.hpp"

void print_fraction(fraction frac)
{
    std::cout << frac.num << '/' << frac.denom << std::endl;
}

void print_fraction_array(fraction frac_array[], int n)
{
    std::cout << "[ " << frac_array[0].num << '/' << frac_array[0].denom << std::endl;
    for (int i = 1; i < n-1; i++)
    {
        std::cout << "  ";
        print_fraction(frac_array[i]);
    }
    std::cout << "  " << frac_array[n-1].num << '/' << frac_array[n-1].denom << " ]" << std::endl;
}

fraction square_fraction(fraction frac)
{
    //TODO: implement function 2
    fraction result;
    result.num = frac.num*frac.num;
    result.denom = frac.denom*frac.denom;
    return result;
}

//TODO: implement function 3
void square_fraction_inplace(fraction & frac)
{
    frac.num *= frac.num;
    frac.denom *= frac.denom;
}

double fraction2double(fraction frac)
{
    //TODO: implement function 4
    return (double) frac.num / (double) frac.denom;
}

int gcd(int a, int b)
{
    //TODO: implement function 5
    if (b == 0) return a;
    return gcd(b, a % b);
}

//TODO: implement function 6
int gcd(fraction frac) {
    int a = frac.num;
    int b = frac.denom;
    while (b != 0){
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

void reduce_fraction_inplace(fraction & frac)
{
    //TODO: implement function 7
    int divisor = gcd(frac);
    frac.num /= divisor;
    frac.denom /= divisor;
    //TODO: add short comment to explain which of the gcd() functions your code is calling

    // the overload with the fraction struct as argument (line 51) will be called, since the other overload
    // with two int args has a non-matching function signature.
}

fraction add_fractions(fraction frac1, fraction frac2)
{
    //TODO: implement function 8
    fraction result;
    // make sure that they share a common denominator.
    result.num = frac1.num * frac2.denom + frac2.num * frac1.denom;
    result.denom = frac1.denom * frac2.denom;
    // reduce the resulting fraction.
    reduce_fraction_inplace(result);
    return result;
}

double sum_fraction_array_approx(fraction frac_array[], int n)
{
    //TODO: implement function 9
    double r = 0;
    for (int i = 0; i < n; ++i) {
        r += fraction2double(frac_array[i]);
    }
    return r;
    
    //TODO: add short comment to explain why this function is approximate

    // because the datatype double has only 8 bytes to represent a real number.
    // this is usually not sufficient to represent a number with infinite precision,
    // however, it gets quite close which is sufficient in most cases/applications.
}

//TODO: implement function 10
fraction sum_fraction_array(fraction frac_array[], int n)
{
    fraction r; r.num = 0; r.denom = 1;
    for (int i = 0; i < n; ++i) {
        r = add_fractions(frac_array[i], r);
    }
    return r;
}

void fill_fraction_array(fraction frac_array[], int n)
{
    fraction temp_frac;
    temp_frac.num = 1;
    for (int i = 1; i <= n; i++)
    {
        temp_frac.denom = i * (i+1);
        frac_array[i-1] = temp_frac;
    }
}

