#include <functional>

using namespace std;
/////
// Genetic algorithms
/////

// luby() function copied from MiniSat 2
// Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
// Copyright (c) 2007-2010, Niklas Sorensson
long long luby(int x)
{
    // Find the finite subsequence that contains index 'x', and the
    // size of that subsequence:
    int size, seq;
    for (size = 1, seq = 0; size < x + 1; seq++, size = 2 * size + 1)
        ;

    while (size - 1 != x)
    {
        size = (size - 1) >> 1;
        seq--;
        x = x % size;
    }

    return 1LL << seq;
}

template <typename T>
auto maxi(T a, T b)
{
    return a > b;
}

template <typename T>
auto mini(T a, T b)
{
    return a < b;
}

template <typename T, typename S> // T are elements, S are scores
pair<S, T> genetic_algorithm(
    function<T()> init,           // generate a new item
    function<S(const T &)> score, // score function
    bool minimize,                // minimize or maximize
    function<void(T &)> mutate,   // in-place mutation function
    int pop_size,                 // population size
    int steps,                    // number of optimization steps
    int luby_basis = -1           // if not -1, use optimal Las Vegas restarts
                                  // heuristic: diameter of the search space in number of mutations
)
{
    srand(time(NULL));

    // restarts

    int cur_luby = 1;
    int luby_count = 0;
    int cur_count = 0;
    auto restart = [&]() {
        if (luby_basis == -1)
            return false;
        if (cur_count++ < cur_luby)
            return false;
        cur_count = 0;
        cur_luby = luby_basis * luby(luby_count++);
        return true;
    };

    using pair_t = pair<double, T>;

    auto comp = minimize ? mini<pair_t> : maxi<pair_t>;

    auto rank = [&](pair_t &e) {
        e.first = score(e.second);
    };

    pair_t global_best, best;

    auto reset = [&]() {
        if (comp(best, global_best))
            global_best = best;
        best.second = init();
        rank(best);
        //        cerr << best.first << endl;
    };

    reset();
    global_best = best;

    while (true)
    {
        auto cand = best;
        for (int i = 0; i < pop_size; i++)
        {
            mutate(cand.second);
            rank(cand);
            if (comp(cand, best))
                best = cand;
            steps--;
            if (!steps)
            {
                reset();
                return global_best;
            }
            if (restart())
                reset();
        }
    }
}
