//
// Created by anatoly_yakimov on 30.10.2019.
//

#include <ctime>
#include <utility>
#include <iostream>
#include "vec.h"

#ifndef MASHALAB_MATRIX_H
#define MASHALAB_MATRIX_H

/**
 * Класс матрицы. Вместо обычных массивов я использую свою обёртку над массивом: vec, олицетворяющую вектор
 * Ты можешь работать с ними так же как и с обычными массивами, но удобнее т.к. тебе не придётся бороться с указателями
 * и передавать размер массива в каждую функцию.
 * @tparam N - наш размер матрицы. передаём его как шаблонный параметр
 */
template<unsigned int N>
class matrix {
public:
    vec<N - 1> a;
    vec<N> b;
    vec<N - 1> c;
    vec<N> d;
    vec<N> e;

    //Конструктор по умолчанию
    matrix() = default;

    //Конструктор копирования
    //Передаётся const-ссылка на объект типа matrix

    matrix(const matrix &m);

    vec<N> multiply(const vec<N> &v) const;  //перемножение матрицы на вектор

    /**
     * @return случайно сгенерированную матрицу
     */
    static matrix generate();

    /**
 * Твой метод гаусса, но настроенный на работу с классами matrix и vec.
 * std::pair - очень удобный класс, позволяющий обернуть в него 2 любых объект.
 * В данном случае оба объекта в std::pair будут типа vec<N>.
 * Но можно использовать по разному: например std::pair<float, char*> - абсолютно любые типы.
 * @tparam N - размерность
 * @param A - матрица. Передаётся не по ссылке, поэтому неявно вызывается конструктор копирования.
 * @param f - вектор f. Также передаётся не по ссылке, поэтому неявно вызывается конструктор копирования.
 * @return - вектор x являющийся решением данной системы.
 */
    static std::pair<vec<N>, vec<N>> solve(matrix<N> M, vec<N> f1, vec<N> f2);
};

template<unsigned int N>
matrix<N>::matrix(const matrix &m) {
    for (int i = 0; i < N - 1; ++i) {
        a[i] = m.a[i];
        b[i] = m.b[i];
        c[i] = m.c[i];
        d[i] = m.d[i];
        e[i] = m.e[i];
    }
    b[N - 1] = m.b[N - 1];
    d[N - 1] = m.d[N - 1];
    e[N - 1] = m.e[N - 1];
}

template<unsigned int N>
matrix<N> matrix<N>::generate() {
    matrix<N> res;
    srand(time(NULL));
    res.a = vec<N - 1>::generate_random();
    res.b = vec<N>::generate_random();
    res.c = vec<N - 1>::generate_random();
    res.d = vec<N>::generate_random();
    res.e = vec<N>::generate_random();

    //не проверял, не знаю какая схема у тебя у матрицы
    res.a[N - 2] = res.d[N - 1];
    res.b[N - 2] = res.d[N - 2];
    res.b[N - 1] = res.e[N - 1];
    res.c[N - 2] = res.e[N - 2];
    res.c[N - 3] = res.d[N - 3];
    return res;
}

template<unsigned int N>
vec<N> matrix<N>::multiply(const vec<N> &v) const {
    vec<N> res;
    for (int i = 0; i < N; i++) {
        if (i == 0) {
            res[i] = v[i] * b[i] + v[i + 1] * c[i] + v[N - 2] * d[i] + v[N - 1] * e[i];

        }
        if (i > 0 && i < N - 3) {
            res[i] = a[i - 1] * v[i - 1] + b[i] * v[i] + c[i] * v[i + 1] + v[N - 2] * d[i] + v[N - 1] * e[i];

        }
        if (i == N - 3) {
            res[i] = a[i] * v[i - 1] + b[i] * v[i] + d[i] * v[N - 2] + e[i] * v[N - 1];

        }
        if (i == N - 2) {
            res[i] = a[i - 1] * v[i - 1] + b[i] * v[i] + e[i] * v[N - 1];

        }
        if (i == N - 1) {
            res[i] = a[i - 1] * v[i - 1] + e[i] * v[i];
        }
    }
    return res;
}

template<unsigned int N>
std::pair<vec<N>, vec<N>> matrix<N>::solve(matrix<N> M, vec<N> f1, vec<N> f2) {
    //В алгоритме я пока ничего не менял
    for (int i = 0; i < N - 1; i++) {

        long double R;
        try {
            if (M.b[i] != 0)
                R = 1 / M.b[i];
            else throw M.b[i];
        }
        catch (long double &value) {
            std::cout << "Division by zero" << std::endl;
            throw "Division by zero";
        }
        M.b[i] = 1;
        M.c[i] = R * M.c[i];

        M.d[i] = R * M.d[i];
        M.e[i] = R * M.e[i];

        f1[i] = R * f1[i];
        f2[i] = R * f2[i];
        R = M.a[i];

        M.a[i] = 0;
        M.b[i + 1] = M.b[i + 1] - R * M.c[i];


        M.d[i + 1] = M.d[i + 1] - R * M.d[i];
        M.e[i + 1] = M.e[i + 1] - R * M.e[i];

        f1[i + 1] = f1[i + 1] - R * f1[i];
        f2[i + 1] = f2[i + 1] - R * f2[i];
        if (i == N - 4) M.c[i + 1] = M.d[i + 1];
        if (i == N - 3) {
            M.b[i + 1] = M.d[i + 1];
            M.c[i + 1] = M.e[i + 1];
        }
    }

    //Обратный ход

    vec<N> x1;
    vec<N> x2;

    try {
        if (M.e[N - 1] != 0) {
            x1[N - 1] = f1[N - 1] / M.e[N - 1];
        } else throw M.e[N - 1];
    }
    catch (long double &value) {
        std::cout << "Division by zero" << std::endl;
        throw "Division by zero";
    }

    x1[N - 2] = f1[N - 2] - M.e[N - 2] * x1[N - 1];
    x1[N - 3] = f1[N - 3] - x1[N - 1] * M.e[N - 3] - x1[N - 2] * M.d[N - 3];

    try {
        if (M.e[N - 1] != 0) {
            x2[N - 1] = f2[N - 1] / M.e[N - 1];
        } else throw M.e[N - 1];

    }
    catch (long double &value) {
        std::cout << "Division by zero" << std::endl;
        throw "Division by zero";
    }
    x2[N - 2] = f2[N - 2] - M.e[N - 2] * x2[N - 1];
    x2[N - 3] = f2[N - 3] - x2[N - 1] * M.e[N - 3] - x2[N - 2] * M.d[N - 3];

    for (int k = N - 4; k >= 0; k--) {
        x1[k] = f1[k] - M.e[k] * x1[N - 1] - M.d[k] * x1[N - 2] - M.c[k] * x1[k + 1];
        x2[k] = f2[k] - M.e[k] * x2[N - 1] - M.d[k] * x2[N - 2] - M.c[k] * x2[k + 1];
    }
    //Специальный метод который создаёт объет std::pair из двух любых объектов
    return std::make_pair(x1, x2);
}

#endif //MASHALAB_MATRIX_H
