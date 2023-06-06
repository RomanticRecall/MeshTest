#ifndef POINT_H
#define POINT_H

#include <Real.hpp>

class Point
{
    public:
    Point();
    Point(double x, double y);
    Point(double x, double y , double z);
    Point(Real x, Real y);
    Point(Real x, Real y , Real z);
    Real &X();
    Real &Y();  
    Real X() const;
    Real Y() const;
    void Info();
    Point operator+(Point const& v) const
    {
        return Point(this->coord[0] + v.X(), this->coord[1] + v.Y());
    }

    Point operator-(Point const& v) const
    {
        return Point(this->coord[0] - v.X(), this->coord[1] - v.Y());
    }

    Point operator*(double const& s) const
    {
        return Point(this->coord[0] * s, this->coord[1] * s);
    }

    Point operator/(double const& s) const
    {
        return Point(this->coord[0] / s, this->coord[1] / s);
    }

    Point operator*(Real const& s) const
    {
        return Point(this->coord[0] * s, this->coord[1] * s);
    }

    Point operator/(Real const& s) const
    {
        return Point(this->coord[0] / s, this->coord[1] / s);
    }

    Real operator*(Point const& v) const
    {
        return this->coord[0] * v.X() + this->coord[1] * v.Y();
    }

    bool operator==(Point const& v) const
    {
        if(this->coord[0] < v.X() - 0.000001 || this->coord[0] > v.X() + 0.000001 )
        return false;
        if(this->coord[1] < v.Y() - 0.000001 || this->coord[1] > v.Y() + 0.000001 )
        return false;
        return true;
        // return (this->coord[0] == v.X() && this->coord[1] == v.Y());
    }

    bool operator!=(Point const& v) const
    {
        if(this->coord[0] < v.X() - 0.000001 || this->coord[0] > v.X() + 0.000001 )
        return true;
        if(this->coord[1] < v.Y() - 0.000001 || this->coord[1] > v.Y() + 0.000001 )
        return true;
        return false;
    }
    protected:
    private:
    Real coord[3];
};

#endif