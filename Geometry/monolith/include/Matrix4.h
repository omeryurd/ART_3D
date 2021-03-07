#ifndef _MATRIX4_H
#define _MATRIX4_H
///
///  \file Matrix4.h
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::Matrix4 Matrix4.h "Matrix4.h"
///  \brief This class provides for a 4 by 4 matrix and its associated operations
///
///  This class stores a 4x4 double matrix.  Common functions, such as matrix addition
///  and multiplication is provided through overload operators.  Less common functions
///  such as matrix inversion and multiplying a vector by a matrix are also included.
///

#include "Vector3.h"
#include <string>
#include <sstream>
#include <iomanip>

namespace MTF
{

    class Matrix4
    {

    public:
        ///
        ///  \brief Default Matrix4 Constructor
        ///
        ///  Creates a Matrix4 set to the identity matrix.
        ///
        Matrix4();

        ///
        ///  \brief Matrix4 Constructor
        ///
        ///  Creates a Matrix4 from the pased in 2 dimensional double array.
        ///
        ///  @param mat                     2D double array with a length of 4 in each dimension, containing a matrix to be assigned
        ///
        Matrix4(double mat[4][4]);

        ///
        ///  \brief Matrix4 Constructor
        ///
        ///  Creates a Matrix4 from the pased in 16 double values
        ///
        ///  @param m00                     double value at the first row and first column in the matrix
        ///  @param m01                     double value at the first row and second column in the matrix
        ///  @param m02                     double value at the first row and third column in the matrix
        ///  @param m03                     double value at the first row and fourth column in the matrix
        ///  @param m10                     double value at the second row and first column in the matrix
        ///  @param m11                     double value at the second row and second column in the matrix
        ///  @param m12                     double value at the second row and third column in the matrix
        ///  @param m13                     double value at the second row and fourth column in the matrix
        ///  @param m20                     double value at the third row and first column in the matrix
        ///  @param m21                     double value at the third row and second column in the matrix
        ///  @param m22                     double value at the third row and third column in the matrix
        ///  @param m23                     double value at the third row and fourth column in the matrix
        ///  @param m30                     double value at the fourth row and first column in the matrix
        ///  @param m31                     double value at the fourth row and second column in the matrix
        ///  @param m32                     double value at the fourth row and third column in the matrix
        ///  @param m33                     double value at the fourth row and fourth column in the matrix
        ///
        Matrix4(double m00, double m01, double m02, double m03,
                double m10, double m11, double m12, double m13,
                double m20, double m21, double m22, double m23,
                double m30, double m31, double m32, double m33);

        ///
        ///  \brief Matrix4 Constructor
        ///
        ///  Creates a transform Matrix4 from the passed in values.
        ///
        ///  @param position                Vector3 containing the position to be translated to
        ///  @param view                    Vector3 containing the view vector to be used in the rotation
        ///  @param right                   Vector3 containing the right vector to be used in the rotation
        ///  @param up                      Vector3 containing the up vector to be used in the rotation
        ///
        Matrix4(Vector3 position, Vector3 view, Vector3 right, Vector3 up);

        ///
        ///  \brief Matrix4 Constructor
        ///
        ///  Creates a rotation Matrix4 from the passed in values.
        ///
        ///  @param view                    Vector3 containing the view vector to be used in the rotation
        ///  @param right                   Vector3 containing the right vector to be used in the rotation
        ///  @param up                      Vector3 containing the up vector to be used in the rotation
        ///
        Matrix4(Vector3 view, Vector3 right, Vector3 up);

        ///
        ///  \brief Matrix4 Destructor
        ///
        ~Matrix4();

        ///
        ///  \brief Overload Operator for: Matrix4 + Matrix4
        ///
        Matrix4 operator + (Matrix4);

        ///
        ///  \brief Overload Operator for: Matrix4 - Matrix4
        ///
        Matrix4 operator - (Matrix4);

        ///
        ///  \brief Overload Operator for: Matrix4 * Matrix4
        ///
        Matrix4 operator * (Matrix4);

        ///
        ///  \brief Overload Operator for: Matrix4 * double
        ///
        ///  The result is that each element of the matrix is multiplied by the double value.
        ///
        Matrix4 operator * (double);

        ///
        ///  \brief Overload Operator for: Matrix4 * Vector3
        ///
        ///  Returns the result of a Vector3 multiplied by a Matrix4, which is Vector3, after the w component has been dropped.
        ///
        Vector3 operator * (Vector3);

        ///
        ///  \brief Overload Operator for: Matrix4 += Matrix4
        ///
        Matrix4& operator += (const Matrix4&);

        ///
        ///  \brief Overload Operator for: Matrix4 -= Matrix4
        ///
        Matrix4& operator -= (const Matrix4&);

        ///
        ///  \brief Overload Operator for: Matrix4 *= Matrix4
        ///
        Matrix4& operator *= (const Matrix4&);

        ///
        ///  \brief Overload Operator for: Matrix4 *= double
        ///
        ///  Each element of this Matrix4 will be multiplied by the double value
        ///
        Matrix4& operator *= (const double);

        ///
        ///  \brief Makes this Matrix4 a scaling matrix
        ///
        ///  \param scale                   Vector3 containing the scaling values in the x, y, and z dimensions
        ///
        void MakeScaleMatrix(Vector3 scale);

        ///
        ///  \brief Makes this Matrix4 a translation matrix
        ///
        ///  \param trans                   Vector3 containing the amount to translate in the x, y, and z directions
        ///
        void MakeTranslationMatrix(Vector3 trans);

        ///
        ///  \brief Makes this Matrix4 a rotation matrix
        ///
        ///  \param view                    A normalized Vector3 pointing in the view direction for the rotation
        ///  \param right                   A normalized Vector3 pointing in the right direction for the rotation
        ///  \param up                      A normalized Vector3 pointing in the up direction for the rotation
        ///
        void MakeRotationMatrix(Vector3 view, Vector3 right, Vector3 up);

        ///
        ///  \brief Makes this Matrix4 an identity matrix
        ///
        void LoadIdentity();

        ///
        ///  \brief Inverts this Matrix4
        ///
        ///  Assigns the inversion of this Matrix4 to this Matrix4.  If the matrix is invertible, then it is assigned to the identity matrix
        ///
        void Invert();

        ///
        ///  \brief Makes this Matrix4 a rotation matrix based on the amount of yaw provided
        ///
        ///  \param yaw                     A double value in radians for the amount of yaw rotation (around the +y axis)
        ///
        void MakeYawRotationMatrix(double yaw);

        ///
        ///  \brief Makes this Matrix4 a rotation matrix based on the amount of pitch provided
        ///
        ///  \param pitch                   A double value in radians for the amount of pitch rotation (around the +x axis)
        ///
        void MakePitchRotationMatrix(double pitch);

        ///
        ///  \brief Makes this Matrix4 a rotation matrix based on the amount of roll provided
        ///
        ///  \param roll                    A double value in radians for the amount of roll rotation (around the +z axis)
        ///
        void MakeRollRotationMatrix(double roll);

        ///
        ///  \brief Makes this Matrix4 a view (camera) rotation matrix
        ///
        ///  This is different than a traditional rotation matrix, since it is designed for the camera.  This should usually only be used in the creation of a view matrix.
        ///
        ///  \param view                    A normalized Vector3 pointing in the view direction for the rotation
        ///  \param right                   A normalized Vector3 pointing in the right direction for the rotation
        ///  \param up                      A normalized Vector3 pointing in the up direction for the rotation
        ///
        void MakeViewRotationMatrix(Vector3 view, Vector3 right, Vector3 up);

        ///
        ///  \brief Makes this Matrix4 a view (camera) transform matrix
        ///
        ///  This is different than a traditional transformation matrix, since it is designed for the camera.  This should usually only be used in the creation of a view matrix.
        ///
        ///  \param position                A Vector3 containing the amount of translation in the x, y, and z directions
        ///  \param view                    A normalized Vector3 pointing in the view direction for the rotation
        ///  \param right                   A normalized Vector3 pointing in the right direction for the rotation
        ///  \param up                      A normalized Vector3 pointing in the up direction for the rotation
        ///
        void MakeViewMatrix(Vector3 position, Vector3 view, Vector3 right, Vector3 up);

        ///
        ///  \brief Returns the transpose of this Matrix4
        ///
        ///  \return                        Matrix4 containing the transpose of this Matrix4
        ///
        Matrix4 GetTranspose();

        ///
        ///  \brief Returns the inverse of this Matrix4
        ///
        ///  \return                        Matrix4 containing the inversion of this Matrix4
        ///
        Matrix4 GetInversion();

        ///
        ///  \brief Returns the position component of this Matrix4
        ///
        ///  \return                        Vector3 containing the position component of this Matrix4
        ///
        Vector3 GetPosition();

        ///
        ///  \brief Returns the view vector component of this Matrix4
        ///
        ///  \return                        Vector3 containing the view component of this Matrix4 (matrix transform of the (0,0,1) vector)
        ///
        Vector3 GetViewVector();

        ///
        ///  \brief Returns the right vector component of this Matrix4
        ///
        ///  \return                        Vector3 containing the right component of this Matrix4 (matrix transform of the (1,0,0) vector)
        ///
        Vector3 GetRightVector();

        ///
        ///  \brief Returns the up vector component of this Matrix4
        ///
        ///  \return                        Vector3 containing the up component of this Matrix4 (matrix transform of the (0,1,0) vector)
        ///
        Vector3 GetUpVector();

        ///
        ///  \brief Provides a single dimension double array representation of the Matrix4.
        ///
        ///  A double array is passed in as the parameter by reference, which is then assigned the values from this Matrix4.
        ///  These values are assigned in a column-wise order, so it can be directly used with OpenGL.
        ///
        ///  \param matArray                The double array that will be assigned the values of the Matrix4
        ///
        void GetMatrixArray(double (&matArray)[16]);

        ///
        ///  \brief Provides a human readable string representing the matrix
        ///
        ///  \return                        A std::string with containing the Matrix4 in a human readable form (4 row & 4 column)
        ///
        std::string ToString();

        ///
        ///  \brief The identity matrix (1 on the diagonal)
        ///
        static const Matrix4 IDENTITY;

        ///
        ///  \brief A matrix consisting of all zeros
        ///
        static const Matrix4 ZERO;

        ///
        ///  \brief A matrix consisting of all ones
        ///
        static const Matrix4 ONE;

    private:
        ///
        ///  \brief A multi-dimensional double array containing the data for our matrix
        ///
        double _mat[4][4];
    };

}

#endif
