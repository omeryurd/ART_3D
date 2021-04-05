#ifndef _VECTOR3_H
#define _VECTOR3_H
///
///  \file Vector3.h
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::Vector3 Vector3.h "Vector3.h"
///  \brief This class provides for a 3D vector and its associated operations.
///
///  This class stores a three dimensional vector.  Common functions, such as the dot 
///  product, cross product, addition, and multiplication with vectors are provided.
///  Many overload operators are used to make the use of Vector3 easy to use.
///

#include <math.h>
#include <sstream>
#include <iomanip>
#include <string>

namespace MTF
{

    class Vector3
    {

    public:

        ///
        ///  \brief Vector3 Constructor
        ///
        ///  Creates a vector initialized as the zero vector (0,0,0)
        ///
        Vector3();

        ///
        ///  \brief Vector3 Constructor
        ///
        ///  Creates a vector initialized with the passed in values
        ///
        ///  @param x                       Value for the X component of the vector
        ///  @param y                       Value for the Y component of the vector
        ///  @param z                       Value for the Z component of the vector
        ///
        Vector3(double x, double y, double z);

        ///
        ///  \brief Vector3 Constructor
        ///
        ///  Creates a vector initialized with the passed in values
        ///
        ///  @param xyz                     Double array of length 3 with the X, Y, and Z components for the vector
        ///
        Vector3(double xyz[]);

        ///
        ///  \brief Vector3 Deconstructor
        ///
        ~Vector3();

        ///
        ///  \brief Overload Operator for: Vector3 + Vector3
        ///
        Vector3 operator + (Vector3);

        ///
        ///  \brief Overload Operator for: Vector3 + double
        ///
        ///  Returns the vector with the double value added to each component
        ///
        Vector3 operator + (double);

        ///
        ///  \brief Overload Operator for: Vector3 - Vector3
        ///
        Vector3 operator - (Vector3);

        ///
        ///  \brief Overload Operator for: Vector3 - double
        ///
        ///  Returns the vector with the double value subtracted from each component
        ///
        Vector3 operator - (double);

        ///
        ///  \brief Overload Operator for: Vector3 * Vector3
        ///
        Vector3 operator * (Vector3);

        ///
        ///  \brief Overload Operator for: Vector3 * double
        ///
        ///  Returns the vector with each component multiplied by the double value
        ///
        Vector3 operator * (double);

        ///
        ///  \brief Overload Operator for: Vector3 / double
        ///
        ///  Returns the vector with each component divided by the double value
        ///
        Vector3 operator / (double);

        ///
        ///  \brief Overload Operator for: Vector3 += Vector3
        ///
        Vector3& operator += (const Vector3&);

        ///
        ///  \brief Overload Operator for: Vector3 += double
        ///
        ///  Each component of this Vector3 is increased by the double value
        ///
        Vector3& operator += (const double);

        ///
        ///  \brief Overload Operator for: Vector3 -= Vector3
        ///
        Vector3& operator -= (const Vector3&);

        ///
        ///  \brief Overload Operator for: Vector3 -= double
        ///
        ///  Each component of this Vector3 is decreased by the double value
        ///
        Vector3& operator -= (const double);

        ///
        ///  \brief Overload Operator for: Vector3 *= Vector3
        ///
        Vector3& operator *= (const Vector3&);

        ///
        ///  \brief Overload Operator for: Vector3 *= double
        ///
        ///  Multiplies each component of this Vector3 by the double value
        ///  
        Vector3& operator *= (const double);

        ///
        ///  \brief Overload Operator for: Vector3 \= double
        ///
        ///  Divides each component of this Vector3 by the double value
        ///  
        Vector3& operator /= (const double);

        ///
        ///  \brief Overload Operator for: Vector3 == Vector3
        ///
        ///  Provides a way to test if two vectors are the same by comparing their components
        ///
        bool operator == (const Vector3&);

        ///
        ///  \brief Overload Operator for: Vector3 != Vector3
        ///
        ///  Provides a way to test if two vectors are different by comparing their components
        ///
        bool operator != (const Vector3&);

        ///
        ///  \brief Method to set the components of the Vector3
        ///
        ///  \param x                       The X component of the vector
        ///  \param y                       The Y component of the vector
        ///  \param z                       The Z component of the vector
        ///
        void Set(double x, double y, double z);

        ///
        ///  \brief Makes this Vector3 a normalized vector
        ///
        void Normalize();

        ///
        ///  \brief Negates all three components of this Vector3
        ///
        void Negate();

        ///
        ///  \brief Returns the lenght of this Vector3
        ///
        ///  \return                        A double value containing the length of the vector
        ///
        double GetLength();

        ///
        ///  \brief Returns the distance between this Vector3 and a passed in Vector3 (treating them as points)
        ///
        ///  \param param                   Passed in Vector3 used in calculating the distance
        ///  \return                        A double value containing the distance between two vectors
        ///
        double GetDistance(Vector3 param);

        ///
        ///  \brief Returns the dot product between this Vector3 and a passed in Vector3
        ///
        ///  \param param                   Passed in Vector3 used in calculating the dot product
        ///  \return                        A double value containing the dot product of two vectors
        ///
        double DotProduct(Vector3 param);

        ///
        ///  \brief Returns the absolute dot product between this Vector3 and a passed in Vector3
        ///
        ///  \param param                   Passed in Vector3 used in calculating the dot product
        ///  \return                        A double value containing the absolute dot product of two vectors
        ///
        double AbsDotProduct(Vector3 param);

        ///
        ///  \brief Returns the cross product between this Vector3 and a passed in Vector3
        ///
        ///  \param param                   Passed in Vector3 used in calculating the cross product
        ///  \return                        A double value containing the cross product of two vectors
        ///
        Vector3 CrossProduct(Vector3 param);

        ///
        ///  \brief Returns a normalized copy of this Vector3
        ///
        ///  \return                        A normalized Vector3 copy of this Vector3
        ///
        Vector3 GetNormalized();

        ///
        ///  \brief Returns a human readable string representing the Vector3
        ///
        ///  \return                        A string with the components of the Vector3 terminated in a new line
        ///
        std::string ToString();

        ///
        ///  \brief Returns the X component of this Vector3
        ///
        ///  \return                        A double value containing the X component
        ///
        double GetX();

        ///
        ///  \brief Returns the Y component of this Vector3
        ///
        ///  \return                        A double value containing the Y component
        ///
        double GetY();

        ///
        ///  \brief Returns the Z component of this Vector3
        ///
        ///  \return                        A double value containing the Z component
        ///
        double GetZ();

        ///
        ///  \brief A Vector3 with zeros for each component
        ///
        static const Vector3 ZERO;

        ///
        ///  \brief A Vector3 pointing in the positive X direction
        ///
        static const Vector3 UNIT_X;

        ///
        ///  \brief A Vector3 pointing in the positive Y direction
        ///
        static const Vector3 UNIT_Y;

        ///
        ///  \brief A Vector3 pointing in the positive Z direction
        ///
        static const Vector3 UNIT_Z;

        ///
        ///  \brief A Vector3 pointing in the negative X direction
        ///
        static const Vector3 NEGATIVE_UNIT_X;

        ///
        ///  \brief A Vector3 pointing in the negative Y direction
        ///
        static const Vector3 NEGATIVE_UNIT_Y;

        ///
        ///  \brief A Vector3 pointing in the negative Z direction
        ///
        static const Vector3 NEGATIVE_UNIT_Z;

        ///
        ///  \brief A unit scale Vector3
        ///
        static const Vector3 UNIT_SCALE;

    private:
        double _x;
        double _y;
        double _z;
    };

}

#endif
