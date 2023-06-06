#include <Point.h>

Point::Point(){}

Point::Point(double x, double y)
{
    coord[0] = x;
    coord[1] = y;
}

Point::Point(double x, double y , double z)
{
    coord[0] = x;
    coord[1] = y;
    coord[2] = 0.0;
}

Point::Point(Real x , Real y)
{
    coord[0] = x;
    coord[1] = y;
}

Point::Point(Real x, Real y , Real z)
{
    coord[0] = x;
    coord[1] = y;
    coord[2] = 0.0;
}

Real &Point::X()
{
    return coord[0];
}

Real &Point::Y()
{
    return coord[1];
}

void Point::Info()
{
    std::cout << coord[0] << " " << coord[1] << std::endl;
}

Real Point::X() const
{
    return coord[0];
}

Real Point::Y() const
{
    return coord[1];
}