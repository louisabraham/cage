# CAGE: C++ Algorithms for Genetic Evolution

CAGE is a very simple but powerful implementation of genetic algorithms in C++. In the standard (1+λ) ES, each offspring is mutatedfrom the population, whereas our offsprings are iteratively mutated, each one being the mutation ofthe previous. We apply restarts according to the Luby sequence.

It can be used for real-world applications, for example in [this paper](https://arxiv.org/abs/2008.02641).

## Usage

`cage.hpp` provides the following function:

```c++
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
```

## Example

`example.cpp` tries to find coefficients for a linear regression by using the _binary_ representation of floating-point numbers. It could optimize _any_ function by using the fact that similar inputs should give similar outputs.

Here is a sample output:

```
real coeffs 0.848 0.847 0.532
std 0.343
solution coeffs 0.843 0.808 0.555
rmse 0.0114
```
