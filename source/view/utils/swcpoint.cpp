#include "../../../include/view/utils/swcpoint.h"

namespace swc
{

template<typename T>
swcPoint<T>::swcPoint() :
    x(0),
    y(0)
{
    //ctor
}

template<typename T>
swcPoint<T>::swcPoint(T _x, T _y) :
    x(_x),
    y(_y)
{
    //ctor
}

template<typename T>
swcPoint<T>::swcPoint(const swcPoint &point)
{
    this->x = point.x;
    this->y = point.y;
}

template<typename T>
void swcPoint<T>::operator () (T x, T y)
{
    this->x = x;
    this->y = y;
}

template class swcPoint<signed int>;
template class swcPoint<unsigned int>;
template class swcPoint<float>;
template class swcPoint<double>;

}
