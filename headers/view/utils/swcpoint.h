#ifndef SWCPOINT_H
#define SWCPOINT_H

namespace swc
{

template<typename T>
class swcPoint
{
public:
    swcPoint();
    swcPoint(T x, T y);
    swcPoint(const swcPoint &point);

    inline bool operator == (const swcPoint<T> other) const {
        return x == other.x && y == other.y;
    }

    inline bool operator == (T point) const {
        return x == point && y == point;
    }

    inline bool operator != (T point) const {
        return x != point && y != point;
    }

    virtual void operator () (T x, T y);

    T x;
    T y;
};

}

#endif // SWCPOINT_H
