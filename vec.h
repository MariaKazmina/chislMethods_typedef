//
// Created by anatoly_yakimov on 30.10.2019.
//

#ifndef MASHALAB_VEC_H
#define MASHALAB_VEC_H

static const int limit = 10;
static const int start = 1;

#include <cassert>
/**
 * Обёртка над стандартным массивом
 * @tparam N
 */
template<unsigned int N>
class vec {
public:
    double array[N];

    //Конструктор по умолчанию
    vec() = default;
    //Конструктор копирования
    vec(const vec<N> &v);

    static vec<N> generate_random();
    static vec<N> generate_unit();

    /**
     * Переопределение оператора [] позволяет нам вместо vec.array[i] писать просто vec[i] и использовать его как массив
     * Этот вариант возвращает ссылку на переменную, следовательно, она изменяема.
     * например
     * vec<3> v;
     * v[1] = 0; //Всё ок, v[1] - double&
     * @param idx
     * @return
     */
    double& operator[](unsigned int idx) {
        /* проверка на то что не выходим за границы.
        * Если обратиться к неверному индексу то благодаря assert код даже не скомпилируется*/
        assert(idx < N);
        return array[idx];
    }
    /**
     * Обрати внимание на const после объявления функции. Это const-qualifier (можешь погуглить)
     * Данный метод вызывается для const объектов. Возвращает const double&, т.е. неизменяемое значение
     * const vec<3> v;
     * v[1] = 0; //Ошибка! Попытка присвоить новое значение const переменной! v[1] - CONST double &
     * @param idx
     * @return
     */
    const double& operator[](unsigned int idx) const {
        assert(idx < N);
        return array[idx];
    }

};

template<unsigned int N>
vec<N> vec<N>::generate_random() {
    vec<N> res;
    for (int i = 0; i < N; i++)
    {
        res[i] = (double) start +  rand() % limit;
    }
    return res;
}

template<unsigned int N>
vec<N> vec<N>::generate_unit() {
    vec<N> res;
    for (int i = 0; i < N; i++)
    {
        res[i] = 1;
    }
    return res;
}

template<unsigned int N>
vec<N>::vec(const vec<N> &v) {
    for (int i = 0; i < N; ++i) {
        array[i] = v[i];
    }
}

#endif //MASHALAB_VEC_H
