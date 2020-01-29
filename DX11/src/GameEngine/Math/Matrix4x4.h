#pragma once

#include "Vector3D.h"
#include "Vector4D.h"

#include <iostream>
#include <memory>

class Matrix4x4
{

public:

	Matrix4x4()
	{
		setIdentity();
	}

	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setTranslation(const Vector3D& translation)
	{
		Matrix4x4 mat_in;
		Matrix4x4 mat_trans;

		copy(*this, mat_in);

		mat_trans.setIdentity();

		mat_trans.m_mat[3][0] = translation.m_x;
		mat_trans.m_mat[3][1] = translation.m_y;
		mat_trans.m_mat[3][2] = translation.m_z;

		mat_in *= mat_trans;

		setMatrix(mat_in);
	}

	void setRotationX(float x)
	{
		Matrix4x4 mat_in;
		Matrix4x4 mat_rot;

		copy(*this, mat_in);

		mat_rot.setIdentity();

		mat_rot.m_mat[1][1] =  cos(x);
		mat_rot.m_mat[1][2] =  sin(x);
		mat_rot.m_mat[2][1] = -sin(x);
		mat_rot.m_mat[2][2] =  cos(x);

		mat_in *= mat_rot;

		setMatrix(mat_in);
	}

	void setRotationY(float y)
	{
		Matrix4x4 mat_in;
		Matrix4x4 mat_rot;

		copy(*this, mat_in);

		mat_rot.setIdentity();

		mat_rot.m_mat[0][0] =  cos(y);
		mat_rot.m_mat[0][2] = -sin(y);
		mat_rot.m_mat[2][0] =  sin(y);
		mat_rot.m_mat[2][2] =  cos(y);

		mat_in *= mat_rot;

		setMatrix(mat_in);
	}

	void setRotationZ(float z)
	{
		Matrix4x4 mat_in;
		Matrix4x4 mat_rot;

		copy(*this, mat_in);

		mat_rot.setIdentity();

		mat_rot.m_mat[0][0] =  cos(z);
		mat_rot.m_mat[0][1] =  sin(z);
		mat_rot.m_mat[1][0] = -sin(z);
		mat_rot.m_mat[1][1] =  cos(z);

		mat_in *= mat_rot;

		setMatrix(mat_in);
	}

	void setRotation(const Vector3D& rot)
	{
		setRotation(rot.m_x, rot.m_y, rot.m_z);
	}

	void setRotation(float x, float y, float z)
	{
		setRotationX(x);
		setRotationY(y);
		setRotationZ(z);
	}

	void setScale(const Vector3D& scale)
	{
		Matrix4x4 mat_in;
		Matrix4x4 mat_scale;

		copy(*this, mat_in);

		mat_scale.setIdentity();

		mat_scale.m_mat[0][0] = scale.m_x;
		mat_scale.m_mat[1][1] = scale.m_y;
		mat_scale.m_mat[2][2] = scale.m_z;

		mat_in *= mat_scale;

		setMatrix(mat_in);
	}

	float getDeterminant()
	{
		Vector4D minor, v1, v2, v3;
		float det;

		v1 = Vector4D(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = Vector4D(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = Vector4D(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);

		minor.cross(v1, v2, v3);
		det = -(
			this->m_mat[0][3] * minor.m_x +
			this->m_mat[1][3] * minor.m_y +
			this->m_mat[2][3] * minor.m_z +
			this->m_mat[3][3] * minor.m_w);
		return det;
	}

	void inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		Vector4D v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].m_x = (this->m_mat[j][0]);
					vec[a].m_y = (this->m_mat[j][1]);
					vec[a].m_z = (this->m_mat[j][2]);
					vec[a].m_w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = pow(-1.0f, i) * v.m_x / det;
			out.m_mat[1][i] = pow(-1.0f, i) * v.m_y / det;
			out.m_mat[2][i] = pow(-1.0f, i) * v.m_z / det;
			out.m_mat[3][i] = pow(-1.0f, i) * v.m_w / det;
		}

		this->setMatrix(out);
	}

	void operator *=(const Matrix4x4& other)
	{
		Matrix4x4 out;
		for (short i = 0; i < 4; i++)
		{
			for (short j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m_mat[i][0] * other.m_mat[0][j] +
					m_mat[i][1] * other.m_mat[1][j] +
					m_mat[i][2] * other.m_mat[2][j] +
					m_mat[i][3] * other.m_mat[3][j];
			}
		}
		::memcpy(m_mat, out.m_mat, sizeof(float) * 16);
	}

	Vector3D getXDirection()
	{
		return Vector3D(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}

	Vector3D getYDirection()
	{
		return Vector3D(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}

	Vector3D getZDirection()
	{
		return Vector3D(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}

	Vector3D getTranslation()
	{
		return Vector3D(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	void setPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		setIdentity();
		float yscale = 1.0f / tan(fov / 2.0f);
		float xscale = yscale / aspect;

		m_mat[0][0] = xscale;
		m_mat[1][1] = yscale;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
	}

	void setOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

	void setMatrix(const Matrix4x4& matrix)
	{
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}

	void copy(const Matrix4x4& souce, Matrix4x4& dest)
	{
		::memcpy(dest.m_mat, souce.m_mat, sizeof(float) * 16);
	}

	void print()
	{
		std::cout << "[00]: " << m_mat[0][0] << " [10]: " << m_mat[1][0] << " [20]: " << m_mat[2][0] << " [30]: " << m_mat[3][0] << std::endl;
		std::cout << "[01]: " << m_mat[0][1] << " [12]: " << m_mat[1][1] << " [21]: " << m_mat[2][1] << " [31]: " << m_mat[3][1] << std::endl;
		std::cout << "[02]: " << m_mat[0][2] << " [13]: " << m_mat[1][2] << " [22]: " << m_mat[2][2] << " [32]: " << m_mat[3][2] << std::endl;
		std::cout << "[03]: " << m_mat[0][3] << " [14]: " << m_mat[1][3] << " [23]: " << m_mat[2][3] << " [33]: " << m_mat[3][3] << std::endl;
	}

	~Matrix4x4()
	{

	}

private:

	float m_mat[4][4] = {};

};
