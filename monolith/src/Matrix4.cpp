#include "Matrix4.h"

namespace MTF
{

    Matrix4::Matrix4(void)
    {
	    LoadIdentity();
    }


    Matrix4::Matrix4(double mat[4][4])
    {
	    for (int i = 0; i < 4; i++)
		    for (int j = 0; j < 4; j++)
			    _mat[i][j] = mat[i][j];
    }


    Matrix4::Matrix4(double m00, double m01, double m02, double m03,
		             double m10, double m11, double m12, double m13,
		             double m20, double m21, double m22, double m23,
		             double m30, double m31, double m32, double m33)
    {
	    _mat[0][0] = m00;  _mat[0][1] = m01;  _mat[0][2] = m02;  _mat[0][3] = m03;
	    _mat[1][0] = m10;  _mat[1][1] = m11;  _mat[1][2] = m12;  _mat[1][3] = m13;
	    _mat[2][0] = m20;  _mat[2][1] = m21;  _mat[2][2] = m22;  _mat[2][3] = m23;
	    _mat[3][0] = m30;  _mat[3][1] = m31;  _mat[3][2] = m32;  _mat[3][3] = m33;
    }


    Matrix4::Matrix4(Vector3 position, Vector3 view, Vector3 right, Vector3 up)
    {
        MakeRotationMatrix(view, right, up);

        _mat[0][3] = position.GetX();
        _mat[1][3] = position.GetY();
        _mat[2][3] = position.GetZ();
    }


    Matrix4::Matrix4(Vector3 view, Vector3 right, Vector3 up)
    {
        MakeRotationMatrix(view, right, up);
    }


    Matrix4::~Matrix4(void)
    {
    }


    Matrix4 Matrix4::operator + (Matrix4 param)
    {
        Matrix4 temp;
    
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp._mat[i][j] = _mat[i][j] + param._mat[i][j];

        return temp;
    }


    Matrix4 Matrix4::operator - (Matrix4 param)
    {
        Matrix4 temp;
    
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp._mat[i][j] = _mat[i][j] - param._mat[i][j];

        return temp;
    }

	
    Matrix4 Matrix4::operator * (Matrix4 param)
    {
	    Matrix4 mult;
	
	    for (int i = 0; i < 4; i++)
	    {
		    for (int j = 0; j < 4; j++) 
		    {
			    mult._mat[i][j] = 0;
			    for (int k = 0; k < 4; k++) 
				    mult._mat[i][j] +=  _mat[i][k] * param._mat[k][j];
		    }
	    }

	    return mult;
    }


    Matrix4 Matrix4::operator * (double param)
    {
	    Matrix4 mult;// = new Matrix4();
	
	    for (int i = 0; i < 4; i++)
	    {
		    for (int j = 0; j < 4; j++) 
		    {
			    mult._mat[i][j] *= param;
		    }
	    }

	    return mult;
    }


    Vector3 Matrix4::operator * (Vector3 param)
    {
	    double vec[4];

	    for (int i = 0; i < 4; i++)
	    {
		    vec[i] = _mat[i][0] * param.GetX() + _mat[i][1] * param.GetY() + _mat[i][2] * param.GetZ() + _mat[i][3];
	    }

	    // All well and good, except we have a vector4 instead of a vector3, so we
	    // need to scale the x, y, and z components by the value of w, unless w=0.
	    // A 'w=0' indicates a point at infinity along the other three components
	    // which is something we are not going to handle here, and will instead
	    // return a zero vector
	    if (vec[3] != 0)
	    {
		    vec[0] = vec[0] / vec[3];
		    vec[1] = vec[1] / vec[3];
		    vec[2] = vec[2] / vec[3];
	    }
	    else
	    {
		    vec[0] = vec[1] = vec[2] = 0;
	    }

	    return Vector3(vec[0], vec[1], vec[2]);
    }


    Matrix4& Matrix4::operator += (const Matrix4& param)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                _mat[i][j] += param._mat[i][j];
        return *this;
    }


    Matrix4& Matrix4::operator -= (const Matrix4& param)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                _mat[i][j] -= param._mat[i][j];
        return *this;
    }


    Matrix4& Matrix4::operator *= (const Matrix4& param)
    {
	    Matrix4 mult;

	    for (int i = 0; i < 4; i++)
	    {
		    for (int j = 0; j < 4; j++) 
		    {
			    mult._mat[i][j] = 0;
			    for (int k = 0; k < 4; k++) 
				    mult._mat[i][j] +=  _mat[i][k] * param._mat[k][j];
		    }
	    }

	    for (int i = 0; i < 4; i++)
	    {
		    for (int j = 0; j < 4; j++)
		    {
			    _mat[i][j] = mult._mat[i][j];
		    }
	    }

        return *this;
    }


    Matrix4& Matrix4::operator *= (const double param)
    {
	    for (int i = 0; i < 4; i++)
	    {
		    for (int j = 0; j < 4; j++) 
		    {
			    _mat[i][j] *= param;
		    }
	    }

        return *this;
    }


    void Matrix4::MakeScaleMatrix(Vector3 scale)
    {
        LoadIdentity();

        _mat[0][0] = scale.GetX();
        _mat[1][1] = scale.GetY();
        _mat[2][2] = scale.GetZ();
    }


    void Matrix4::MakeTranslationMatrix(Vector3 trans)
    {
        LoadIdentity();

	    _mat[0][3] = trans.GetX();
	    _mat[1][3] = trans.GetY();
	    _mat[2][3] = trans.GetZ();
    }


    void Matrix4::MakeRotationMatrix(Vector3 view, Vector3 right, Vector3 up)
    {
        LoadIdentity();

        _mat[0][0] =  right.GetX();
        _mat[1][0] =  right.GetY();
        _mat[2][0] =  right.GetZ();

        _mat[0][1] =  up.GetX();
        _mat[1][1] =  up.GetY();
        _mat[2][1] =  up.GetZ();

        _mat[0][2] = view.GetX();
        _mat[1][2] = view.GetY();
        _mat[2][2] = view.GetZ();
    }


    void Matrix4::MakeViewRotationMatrix(Vector3 view, Vector3 right, Vector3 up)
    {
        LoadIdentity();

        view *= -1;

        _mat[0][0] = right.GetX();
        _mat[0][1] = right.GetY();
        _mat[0][2] = right.GetZ();

        _mat[1][0] = up.GetX();
        _mat[1][1] = up.GetY();
        _mat[1][2] = up.GetZ();

        _mat[2][0] = view.GetX();
        _mat[2][1] = view.GetY();
        _mat[2][2] = view.GetZ();
    }


    void Matrix4::MakeViewMatrix(Vector3 position, Vector3 view, Vector3 right, Vector3 up)
    {
        LoadIdentity();

        view *= -1;

        _mat[0][0] =  right.GetX();
        _mat[0][1] =  right.GetY();
        _mat[0][2] =  right.GetZ();

        _mat[1][0] =  up.GetX();
        _mat[1][1] =  up.GetY();
        _mat[1][2] =  up.GetZ();

        _mat[2][0] = view.GetX();
        _mat[2][1] = view.GetY();
        _mat[2][2] = view.GetZ();

        _mat[0][3] = -position.DotProduct(right);
        _mat[1][3] = -position.DotProduct(up);
        _mat[2][3] = -position.DotProduct(view);
    }


    void Matrix4::MakeYawRotationMatrix(double yaw)
    {
        LoadIdentity();

        _mat[0][0] = cos(yaw);
        _mat[0][2] = sin(yaw);
        _mat[2][0] = -sin(yaw);
        _mat[2][2] = cos(yaw);
    }


    void Matrix4::MakePitchRotationMatrix(double pitch)
    {
        LoadIdentity();

        _mat[1][1] = cos(pitch);
        _mat[1][2] = -sin(pitch);
        _mat[2][1] = sin(pitch);
        _mat[2][2] = cos(pitch);
    }


    void Matrix4::MakeRollRotationMatrix(double roll)
    {
        LoadIdentity();

        _mat[0][0] = cos(roll);
        _mat[0][1] = -sin(roll);
        _mat[1][0] = sin(roll);
        _mat[1][1] = cos(roll);
    }


    Matrix4 Matrix4::GetTranspose()
    {
        Matrix4 transpose;

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                transpose._mat[i][j] = _mat[j][i];

        return transpose;
    }


    Matrix4 Matrix4::GetInversion()
    {
        Matrix4 inv(*this);
        inv.Invert();
        return inv;
    }


    void Matrix4::Invert()
    {
        double a0 = _mat[0][0]*_mat[1][1] - _mat[0][1]*_mat[1][0];
        double a1 = _mat[0][0]*_mat[1][2] - _mat[0][2]*_mat[1][0];
        double a2 = _mat[0][0]*_mat[1][3] - _mat[0][3]*_mat[1][0];
        double a3 = _mat[0][1]*_mat[1][2] - _mat[0][2]*_mat[1][1];
        double a4 = _mat[0][1]*_mat[1][3] - _mat[0][3]*_mat[1][1];
        double a5 = _mat[0][2]*_mat[1][3] - _mat[0][3]*_mat[1][2];
        double b0 = _mat[2][0]*_mat[3][1] - _mat[2][1]*_mat[3][0];
        double b1 = _mat[2][0]*_mat[3][2] - _mat[2][2]*_mat[3][0];
        double b2 = _mat[2][0]*_mat[3][3] - _mat[2][3]*_mat[3][0];
        double b3 = _mat[2][1]*_mat[3][2] - _mat[2][2]*_mat[3][1];
        double b4 = _mat[2][1]*_mat[3][3] - _mat[2][3]*_mat[3][1];
        double b5 = _mat[2][2]*_mat[3][3] - _mat[2][3]*_mat[3][2];

        double det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
        if (fabs(det) < 0.00001)
        {
            LoadIdentity();
        }
        else
        {
            Matrix4 inverse;
            inverse._mat[0][0] = + _mat[1][1]*b5 - _mat[1][2]*b4 + _mat[1][3]*b3;
            inverse._mat[1][0] = - _mat[1][0]*b5 + _mat[1][2]*b2 - _mat[1][3]*b1;
            inverse._mat[2][0] = + _mat[1][0]*b4 - _mat[1][1]*b2 + _mat[1][3]*b0;
            inverse._mat[3][0] = - _mat[1][0]*b3 + _mat[1][1]*b1 - _mat[1][2]*b0;
            inverse._mat[0][1] = - _mat[0][1]*b5 + _mat[0][2]*b4 - _mat[0][3]*b3;
            inverse._mat[1][1] = + _mat[0][0]*b5 - _mat[0][2]*b2 + _mat[0][3]*b1;
            inverse._mat[2][1] = - _mat[0][0]*b4 + _mat[0][1]*b2 - _mat[0][3]*b0;
            inverse._mat[3][1] = + _mat[0][0]*b3 - _mat[0][1]*b1 + _mat[0][2]*b0;
            inverse._mat[0][2] = + _mat[3][1]*a5 - _mat[3][2]*a4 + _mat[3][3]*a3;
            inverse._mat[1][2] = - _mat[3][0]*a5 + _mat[3][2]*a2 - _mat[3][3]*a1;
            inverse._mat[2][2] = + _mat[3][0]*a4 - _mat[3][1]*a2 + _mat[3][3]*a0;
            inverse._mat[3][2] = - _mat[3][0]*a3 + _mat[3][1]*a1 - _mat[3][2]*a0;
            inverse._mat[0][3] = - _mat[2][1]*a5 + _mat[2][2]*a4 - _mat[2][3]*a3;
            inverse._mat[1][3] = + _mat[2][0]*a5 - _mat[2][2]*a2 + _mat[2][3]*a1;
            inverse._mat[2][3] = - _mat[2][0]*a4 + _mat[2][1]*a2 - _mat[2][3]*a0;
            inverse._mat[3][3] = + _mat[2][0]*a3 - _mat[2][1]*a1 + _mat[2][2]*a0;

            double invDet = 1/det;

            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    _mat[i][j] = inverse._mat[i][j] * invDet;
        }
    }


    void Matrix4::LoadIdentity() 
    {
	    for (int i = 0; i < 4; i++) {
		    for (int j = 0; j < 4; j++) {
			    _mat[i][j] = (i == j ? 1 : 0);
		    }
	    }
    }


    Vector3 Matrix4::GetPosition()
    {
        Matrix4 temp = GetInversion();
        return Vector3(_mat[0][3], _mat[1][3], _mat[2][3]);
    }


    Vector3 Matrix4::GetViewVector()
    {
        return *this * Vector3(0.0, 0.0, 1.0);
    }


    Vector3 Matrix4::GetRightVector()
    {
        return *this * Vector3(1.0, 0.0, 0.0);
    }


    Vector3 Matrix4::GetUpVector()
    {
        return *this * Vector3(0.0, 1.0, 0.0);
    }


    // Sets the passed in array to this _matrix's values (column-major ordering)
    void Matrix4::GetMatrixArray(double (&matArray)[16])
    {
        int index = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                matArray[index++] = _mat[j][i];
            }
        }
    }


    std::string Matrix4::ToString()
    {
        std::stringstream str("");

        for (int i = 0; i < 4; i++) 
        {
            for (int j = 0; j < 4; j++)
            {
                str << std::setw(5) << _mat[i][j] << "  ";
            }
            str << std::endl;
        }

        return str.str();
    }


    Matrix4 const Matrix4::IDENTITY = Matrix4(1.0, 0.0, 0.0, 0.0,
                                              0.0, 1.0, 0.0, 0.0,
                                              0.0, 0.0, 1.0, 0.0,
                                              0.0, 0.0, 0.0, 1.0);

    Matrix4 const Matrix4::ZERO     = Matrix4(0.0, 0.0, 0.0, 0.0,
                                              0.0, 0.0, 0.0, 0.0,
                                              0.0, 0.0, 0.0, 0.0,
                                              0.0, 0.0, 0.0, 0.0);

    Matrix4 const Matrix4::ONE      = Matrix4(1.0, 1.0, 1.0, 1.0,
                                              1.0, 1.0, 1.0, 1.0,
                                              1.0, 1.0, 1.0, 1.0,
                                              1.0, 1.0, 1.0, 1.0);

}