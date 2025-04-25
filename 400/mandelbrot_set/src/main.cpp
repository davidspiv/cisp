// abs complex example
#include <iostream>     // std::cout
#include <complex>      // std::complex, std::abs
using namespace std;

void testConvergence(complex<double> c)
{
    cout << "c = " << c << endl;
    cout << "Real component of c: " << c.real() << endl;
    cout << "Imaginary component of c: " << c.imag() << endl;
    complex<double> z  = c;
    int i = 0;
    while(abs(z) < 2.0 && i < 64)
    {
        z = z*z + c;
        //cout << "z_" << i << "= " << z << endl;
        //cout << "|z| = " << abs(c) << endl;
        i++;
    }
    if(i == 64) cout << c << " converges!" << endl;
    else cout << c << " escapes after " << i << " iterations" << endl;
}

int main ()
{
    double re = -0.77568377;
    double im = 0.13646737;
    complex<double> c (re, im);
    testConvergence(c);
    cout << endl;
    c = {-0.77568377, 0.23646737};
    testConvergence(c);
    return 0;
}
