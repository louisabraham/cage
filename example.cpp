#include <math.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
#include "cage.hpp"

using namespace std;

static float_t drand()
{
    return (float_t)rand() / RAND_MAX;
}

int main()
{
    srand(time(NULL));

    const int n = 1000;
    const int dim = 2;

    vector<vector<float_t>> X(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < dim; j++)
            X[i].push_back(drand());

    using elem_t = vector<float_t>;

    const function<elem_t()> init = []() {
        elem_t ans;
        for (int i = 0; i < dim + 1; i++)
            ans.push_back(drand());
        return ans;
    };

    const function<float_t(elem_t, elem_t)> eval = [&](elem_t coefs, elem_t x) {
        float_t ans = coefs[dim];
        for (int i = 0; i < dim; i++)
            ans += coefs[i] * x[i];
        return ans;
    };

    vector<float_t> y(n);
    auto true_coefs = init();
    cout << "real coeffs ";
    for (auto c : true_coefs)
        cout << setprecision(3) << c << ' ';
    cout << endl;
    for (int i = 0; i < n; i++)
        y[i] = eval(true_coefs, X[i]);

    float mean = 0;
    for (int i = 0; i < n; i++)
        mean += y[i];
    mean /= n;
    float var = 0;
    for (int i = 0; i < n; i++)
    {
        float tmp = y[i] - mean;
        var += tmp * tmp;
    }
    var /= n;

    cout << "std " << sqrt(var) << endl;

    const function<float_t(const elem_t &)> score = [&](const elem_t &coefs) {
        float_t error = 0;
        for (int i = 0; i < n; i++)
        {
            float_t e = eval(coefs, X[i]) - y[i];
            error += e * e;
        }
        error /= n;
        if (error != error)
            return (float)1e6;
        return error;
    };

    const function<void(elem_t &)> mutate = [&](elem_t &coefs) {
        float_t &x = coefs[rand() % (dim + 1)];
        auto ptr = reinterpret_cast<uint32_t *>(&x);
        *ptr ^= (1 << (rand() % 32));
    };

    auto ans = genetic_algorithm(
        init,   // generate a new item
        score,  // score function
        true,   // minimize or maximize
        mutate, // in-place mutation function
        10,     // population size
        100000, // number of optimization steps
        50      // if not -1, use optimal Las Vegas restarts
    );

    cout << "solution coeffs ";
    for (auto c : ans.second)
        cout << setprecision(3) << c << ' ';
    cout << endl;

    cout << "rmse " << sqrt(ans.first) << endl;
}
