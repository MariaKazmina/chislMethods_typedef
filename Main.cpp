#include"vec.h"
#include "matrix.h"
using namespace std;

int main()
{
    constexpr int N = 5;
    matrix<N> A = matrix<N>::generate();
    vec<N> x1 = vec<N>::generate_unit();
    vec<N> x2 = vec<N>::generate_random();
    auto f1 = A.multiply(x1);
    auto f2 = A.multiply(x2);

    pair<vec<N>, vec<N>> res = matrix<N>::solve(A, f1, f2);
    /**
     * res.first = решение для f1
     * res.second = решение для f2
     * Решает неверно, дальше пока не смотрел.
     * Но теперь это всё вышлядит намного аккуратнее и проще отлаживать.
     * После применения метода Гаусса твои матрица и вектора остаются неизменными
     */
}