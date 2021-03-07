#include "Vector3.h"

namespace MTF
{

    Vector3::Vector3(void)
    {
	    _x = _y = _z = 0;
    }


    Vector3::Vector3(double x, double y, double z)
    {
	    _x = x;
	    _y = y;
	    _z = z;
    }

    Vector3::Vector3(double xyz[])
    {
        _x = xyz[0];
        _y = xyz[1];
        _z = xyz[2];
    }


    Vector3::~Vector3(void)
    {
    }


    // + Overload: handles "Vector3 + Vector3"
    Vector3 Vector3::operator+ (Vector3 param)
    {
	    Vector3 temp;// = new Vector3();
	    temp._x = _x + param._x;
	    temp._y = _y + param._y;
	    temp._z = _z + param._z;
	    return temp;
    }


    // + Overload: handles "Vector3 + double"
    Vector3 Vector3::operator+ (double param)
    {
	    Vector3 temp;// = new Vector3();
	    temp._x = _x + param;
	    temp._y = _y + param;
	    temp._z = _z + param;
	    return temp;
    }


    // - Overload: handles "Vector3 - Vector3"
    Vector3 Vector3::operator- (Vector3 param)
    {
	    Vector3 temp;// = new Vector3();
	    temp._x = _x - param._x;
	    temp._y = _y - param._y;
	    temp._z = _z - param._z;
	    return temp;
    }


    // - Overload: handles "Vector3 - double"
    Vector3 Vector3::operator- (double param)
    {
	    Vector3 temp;// = new Vector3();
	    temp._x = _x - param;
	    temp._y = _y - param;
	    temp._z = _z - param;
	    return temp;
    }


    // * Overload: handles "Vector3 * Vector3"
    Vector3 Vector3::operator * (Vector3 param)
    {
	    Vector3 temp;// = new Vector3();
	    temp._x = _x * param._x;
	    temp._y = _y * param._y;
	    temp._z = _z * param._z;
	    return temp;
    }


    // * Overload: handles "Vector3 * double"
    Vector3 Vector3::operator * (double param)
    {
	    Vector3 temp;// = new Vector3();
	    temp._x = _x * param;
	    temp._y = _y * param;
	    temp._z = _z * param;
	    return temp;
    }


    // / Overload: handles "Vector3 / double"
    Vector3 Vector3::operator / (double param)
    {
	    Vector3 temp;// = new Vector3();
	    temp._x = _x / param;
	    temp._y = _y / param;
	    temp._z = _z / param;
	    return temp;
    }

    // += Overload: handles "Vector3 += Vector3"
    Vector3& Vector3::operator += (const Vector3& param)
    {
	    _x += param._x;
	    _y += param._y;
	    _z += param._z;
        return *this;
    }


    // += Overload: handles "Vector3 += double"
    Vector3& Vector3::operator += (const double param)
    {
	    _x += param;
	    _y += param;
	    _z += param;
        return *this;
    }


    // -= Overload: handles "Vector3 -= Vector3"
    Vector3& Vector3::operator -= (const Vector3& param)
    {
	    _x -= param._x;
	    _y -= param._y;
	    _z -= param._z;
        return *this;
    }


    // -= Overload: handles "Vector3 -= double"
    Vector3& Vector3::operator -= (const double param)
    {
	    _x -= param;
	    _y -= param;
	    _z -= param;
        return *this;
    }


    // *= Overload: handles "Vector3 *= Vector3"
    Vector3& Vector3::operator *= (const Vector3& param)
    {
	    _x *= param._x;
	    _y *= param._y;
	    _z *= param._z;
        return *this;
    }


    // *= Overload: handles "Vector3 *= double"
    Vector3& Vector3::operator *= (const double param)
    {
	    _x *= param;
	    _y *= param;
	    _z *= param;
        return *this;
    }


    // /= Overload: handles "Vector3 /= double"
    Vector3& Vector3::operator /= (const double param)
    {
	    _x /= param;
	    _y /= param;
	    _z /= param;
        return *this;
    }


    // == Overload
    bool Vector3::operator == (const Vector3 &param)
    {
        return (fabs(_x - param._x) < 0.0001 && fabs(_y - param._y) < 0.0001 && fabs(_z - param._z) < 0.0001);
    }


    // != Overload
    bool Vector3::operator != (const Vector3 &param)
    {
        return !(*this == param);
    }


    // Sets the value for this vector
    void Vector3::Set(double x, double y, double z)
    {
	    _x = x;
	    _y = y;
	    _z = z;
    }


    // Normalizes the vector so that its magnitude is 1
    void Vector3::Normalize()
    {
	    double magnitude = GetLength();
	    _x = _x / magnitude;
	    _y = _y / magnitude;
	    _z = _z / magnitude;
    }


    // Negates each term in the vector
    void Vector3::Negate()
    {
	    _x *= -1;
	    _y *= -1;
	    _z *= -1;
    }


    // Returns the length of the vector
    double Vector3::GetLength()
    {
	    return sqrt(_x * _x + _y * _y + _z * _z);
    }


    // Returns the distance (absolute value) between two vectors
    double Vector3::GetDistance(Vector3 param)
    {
	    Vector3 difference;
	    difference._x = param.GetX() - _x;
	    difference._y = param.GetY() - _y;
	    difference._z = param.GetZ() - _z;
	    return difference.GetLength();
    }


    // Returns the dot product of this vector with a given vector
    double Vector3::DotProduct(Vector3 param)
    {
	    return _x * param.GetX() + _y * param.GetY() + _z * param.GetZ();
    }


    // Returns the absolute dot product of this vector with a given vector
    double Vector3::AbsDotProduct(Vector3 param)
    {
        return abs(_x * param.GetX() + _y * param.GetY() + _z * param.GetZ());
    }


    // Return the crossproduct of this vector with a given vector
    Vector3 Vector3::CrossProduct(Vector3 vec)
    {
	    Vector3 cross;// = new Vector3();
	    cross._x = _y * vec.GetZ() - _z * vec.GetY();
	    cross._y = _z * vec.GetX() - _x * vec.GetZ();
	    cross._z = _x * vec.GetY() - _y * vec.GetX();
	    return cross;
    }


    // Returns the normalized vector without changing the original
    Vector3 Vector3::GetNormalized()
    {
        Vector3 norm(_x, _y, _z);
        norm.Normalize();
        return norm;
    }


    std::string Vector3::ToString()
    {
        std::stringstream str("");

        str << "(" << std::setw(5) << _x << ", " << std::setw(5) << _y << ", " << std::setw(5) << _z << ")" << std::endl;

        return str.str();
    }


    double Vector3::GetX()
    {
	    return _x;
    }


    double Vector3::GetY()
    {
	    return _y;
    }


    double Vector3::GetZ()
    {
	    return _z;
    }


    Vector3 const Vector3::ZERO = Vector3(0.0, 0.0, 0.0);

    Vector3 const Vector3::UNIT_X = Vector3(1.0, 0.0, 0.0);
    Vector3 const Vector3::UNIT_Y = Vector3(0.0, 1.0, 0.0);
    Vector3 const Vector3::UNIT_Z = Vector3(0.0, 0.0, 1.0);

    Vector3 const Vector3::NEGATIVE_UNIT_X = Vector3(-1.0, 0.0, 0.0);
    Vector3 const Vector3::NEGATIVE_UNIT_Y = Vector3(0.0, -1.0, 0.0);
    Vector3 const Vector3::NEGATIVE_UNIT_Z = Vector3(0.0, 0.0, -1.0);

    Vector3 const Vector3::UNIT_SCALE = Vector3(1.0, 1.0, 1.0);

}