#include <iostream>
#include <iomanip>

using namespace std;

int sumY(const int y[], int n) {
    int s = 0;
    for (int i = 0; i < n; i++) s += y[i];
    return s;
}

double sumWeighted(const int y[], const double x[], int n) {
    double s = 0.0;
    for (int i = 0; i < n; i++) s += y[i] * x[i];
    return s;
}

// Xкр = (Σ(y_i * X_i)) / (Σ y_i)
double calcCriticalValue(const int y[], const double x[], int n) {
    int sy = sumY(y, n);
    if (sy == 0) return 0.0;
    return sumWeighted(y, x, n) / sy;
}

int main() {
    const int N = 9;

    int Y[N] = { 4, 1, 8, 5, 6, 3, 7, 5, 4 };
    double Tkr[N] = { 70, 60, 80, 65, 50, 45, 85, 44, 68 };
    double Pkr[N] = { 0, 12, 15, 11, 14, 10, 13, 16, 19 };

    double T_mix = calcCriticalValue(Y, Tkr, N);
    double P_mix = calcCriticalValue(Y, Pkr, N);

    cout << fixed << setprecision(4);
    cout << "SumY = " << sumY(Y, N) << "\n";
    cout << "Tkr = " << T_mix << "\n";
    cout << "Pkr = " << P_mix << "\n";

    return 0;
}