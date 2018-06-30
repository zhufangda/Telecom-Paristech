// --------------------------------------------------------------------------
// Copyright(C) 2009-2016
// Tamy Boubekeur
// 
// Permission granted to use this code only for teaching projects and 
// private practice.
//
// Do not distribute this code outside the teaching assignements.                                                                           
// All rights reserved.                                                       
// --------------------------------------------------------------------------
#pragma once

#include <cmath>
#include <iostream>
#include <algorithm>

#include "Vec3.h"
/**
 * A colomn-major 4x4 transformation matrix.
 */
template <class T>
class Mat4 {
public:
    class Exception {
    public:
        inline Exception (const std::string & msg) : m_msg ("Blade Mat4 Exception: " + msg) {}
        inline const std::string & msg () const { return m_msg; }
    protected:
        std::string m_msg;
    };

    /// Careful, the basic constructor set the matrix to identity by default.
    inline Mat4 (void)	{ loadIdentity (); }
    inline Mat4 (const Mat4 & mat) {
        for (unsigned int i = 0; i < 16; i++)
            m_m[i] = mat[i];
    }
    ~Mat4() {}
    inline Mat4 (T a00, T a01, T a02, T a03,
       T a10, T a11, T a12, T a13,
       T a20, T a21, T a22, T a23,
       T a30, T a31, T a32, T a33) {
        set (a00, a01, a02, a03,
           a10, a11, a12, a13,
           a20, a21, a22, a23,
           a30, a31, a32, a33);
    }
    inline void set (T a00, T a01, T a02, T a03,
       T a10, T a11, T a12, T a13,
       T a20, T a21, T a22, T a23,
       T a30, T a31, T a32, T a33) {
        m_m[0] = a00; m_m[1] = a01; m_m[2] = a02; m_m[3] = a03;
        m_m[4] = a10; m_m[5] = a11; m_m[6] = a12; m_m[7] = a13;
        m_m[8] = a20; m_m[9] = a21; m_m[10] = a22; m_m[11] = a23;
        m_m[12] = a30; m_m[13] = a31; m_m[14] = a32; m_m[15] = a33;
    }
    inline void set (const T * a) { for (unsigned int i = 0; i < 16; i++) m_m[i] = a[i]; }
    inline void setNull () {  set (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); }
    inline void loadIdentity () { setNull (); m_m[0] = m_m[5] = m_m[10] = m_m[15] = 1.0; }
    inline T& operator[] (int index) { return (m_m[index]); }
    inline const T& operator[] (int index) const { return (m_m[index]); }
    inline T& operator() (int i, int j) { return (m_m[4*i+j]); }
    inline const T& operator() (int i, int j) const { return (m_m[4*i+j]); }
    inline Mat4& operator= (const Mat4 & mat) {
        for (unsigned int i = 0; i < 16; i++)
            m_m[i] = mat[i];
        return (*this);
    }
    inline Mat4 operator+ (const Mat4 & mat) const {
        Mat4 res;
        for (unsigned int i = 0; i < 16; i++)
            res[i] = m_m[i] + mat[i];
        return res;
    }
    inline Mat4 operator- (const Mat4 & mat) const {
        Mat4 res;
        for (unsigned int i = 0; i < 16; i++)
            res[i] = m_m[i] - mat[i];
        return res;
    }
    inline Mat4 operator- () const {
        Mat4 res;
        for (unsigned int i = 0; i < 16; i++)
            res[i] = -m_m[i];
        return res;
    }
    inline Mat4 operator* (const Mat4 & mat) const {
        Mat4 tmp;
        tmp.setNull ();
        for (unsigned int i = 0; i < 4; i++)
            for (unsigned int j = 0; j < 4; j++)
                for (unsigned int k = 0; k < 4; k++)
                    tmp(i, j) += (*this)(k, j)*mat(i,k);
                return tmp;
            }
            inline Mat4 operator* (T s) const {
                Mat4 res;
                for (unsigned int i = 0; i < 16; i++)
                    res[i] = s*m_m[i];
                return res;
            }
            inline Mat4 operator/ (T s) const {
                Mat4 res;
                for (unsigned int i = 0; i < 16; i++)
                    res[i] = m_m[i]/s;
                return res;
            }
            inline Vec3<T> operator* (const Vec3<T> & v) const {
                T tmp[4];
                tmp[0] = v[0];
                tmp[1] = v[1];
                tmp[2] = v[2];
                tmp[3] = 1.0;
                T res[4];
                res[0] = 0;
                res[1] = 0;
                res[2] = 0;
                res[3] = 0;
                for (unsigned int i = 0; i < 4; i++)
                    for (unsigned int j = 0; j < 4; j++)
                        res[i] += tmp[j]*(*this)(j, i);
                    return Vec3<T> (res[0], res[1], res[2])/res[3];
                }
                inline Mat4& operator+= (const Mat4 & mat) {
                    Mat4 tmp = (*this) + mat;
                    (*this) = tmp;
                    return (*this);
                }
                inline Mat4& operator-= (const Mat4 & mat) {
                    Mat4 tmp = (*this) - mat;
                    (*this) = tmp;
                    return (*this);
                }
                inline Mat4& operator*= (T & s) {
                    Mat4 tmp = (*this) * s;
                    (*this) = tmp;
                    return (*this);
                }
                inline Mat4& operator*= (const Mat4 & mat) {
                    Mat4 tmp = (*this) * mat;
                    (*this) = tmp;
                    return (*this);
                }
                inline bool operator == (const Mat4 & mat) const {
                    for (unsigned int i = 0; i < 16; i++)
                        if (m_m[i] != mat.m_m[i])
                            return false;
                        return true;
                    }
                    inline bool operator != (const Mat4 & mat) const {
                        return !((*this)==mat);
                    }
                    inline T * data () { return m_m; }
                    inline const T * data () const { return m_m; }
                    inline Mat4 & transpose () {
                        for (unsigned int i = 0; i < 4; i++)
                            for (unsigned int j = i+1; j < 4; j++)
                                std::swap ((*this) (i, j), (*this) (j, i));
                            return (*this);
                        }
                        inline Mat4 & invert () {
                            double inv[16], det;
                            int i;
                            inv[0] = m_m[5]  * m_m[10] * m_m[15] -
                            m_m[5]  * m_m[11] * m_m[14] -
                            m_m[9]  * m_m[6]  * m_m[15] +
                            m_m[9]  * m_m[7]  * m_m[14] +
                            m_m[13] * m_m[6]  * m_m[11] -
                            m_m[13] * m_m[7]  * m_m[10];

                            inv[4] = -m_m[4]  * m_m[10] * m_m[15] +
                            m_m[4]  * m_m[11] * m_m[14] +
                            m_m[8]  * m_m[6]  * m_m[15] -
                            m_m[8]  * m_m[7]  * m_m[14] -
                            m_m[12] * m_m[6]  * m_m[11] +
                            m_m[12] * m_m[7]  * m_m[10];

                            inv[8] = m_m[4]  * m_m[9] * m_m[15] -
                            m_m[4]  * m_m[11] * m_m[13] -
                            m_m[8]  * m_m[5] * m_m[15] +
                            m_m[8]  * m_m[7] * m_m[13] +
                            m_m[12] * m_m[5] * m_m[11] -
                            m_m[12] * m_m[7] * m_m[9];

                            inv[12] = -m_m[4]  * m_m[9] * m_m[14] +
                            m_m[4]  * m_m[10] * m_m[13] +
                            m_m[8]  * m_m[5] * m_m[14] -
                            m_m[8]  * m_m[6] * m_m[13] -
                            m_m[12] * m_m[5] * m_m[10] +
                            m_m[12] * m_m[6] * m_m[9];

                            inv[1] = -m_m[1]  * m_m[10] * m_m[15] +
                            m_m[1]  * m_m[11] * m_m[14] +
                            m_m[9]  * m_m[2] * m_m[15] -
                            m_m[9]  * m_m[3] * m_m[14] -
                            m_m[13] * m_m[2] * m_m[11] +
                            m_m[13] * m_m[3] * m_m[10];

                            inv[5] = m_m[0]  * m_m[10] * m_m[15] -
                            m_m[0]  * m_m[11] * m_m[14] -
                            m_m[8]  * m_m[2] * m_m[15] +
                            m_m[8]  * m_m[3] * m_m[14] +
                            m_m[12] * m_m[2] * m_m[11] -
                            m_m[12] * m_m[3] * m_m[10];

                            inv[9] = -m_m[0]  * m_m[9] * m_m[15] +
                            m_m[0]  * m_m[11] * m_m[13] +
                            m_m[8]  * m_m[1] * m_m[15] -
                            m_m[8]  * m_m[3] * m_m[13] -
                            m_m[12] * m_m[1] * m_m[11] +
                            m_m[12] * m_m[3] * m_m[9];

                            inv[13] = m_m[0]  * m_m[9] * m_m[14] -
                            m_m[0]  * m_m[10] * m_m[13] -
                            m_m[8]  * m_m[1] * m_m[14] +
                            m_m[8]  * m_m[2] * m_m[13] +
                            m_m[12] * m_m[1] * m_m[10] -
                            m_m[12] * m_m[2] * m_m[9];

                            inv[2] = m_m[1]  * m_m[6] * m_m[15] -
                            m_m[1]  * m_m[7] * m_m[14] -
                            m_m[5]  * m_m[2] * m_m[15] +
                            m_m[5]  * m_m[3] * m_m[14] +
                            m_m[13] * m_m[2] * m_m[7] -
                            m_m[13] * m_m[3] * m_m[6];

                            inv[6] = -m_m[0]  * m_m[6] * m_m[15] +
                            m_m[0]  * m_m[7] * m_m[14] +
                            m_m[4]  * m_m[2] * m_m[15] -
                            m_m[4]  * m_m[3] * m_m[14] -
                            m_m[12] * m_m[2] * m_m[7] +
                            m_m[12] * m_m[3] * m_m[6];

                            inv[10] = m_m[0]  * m_m[5] * m_m[15] -
                            m_m[0]  * m_m[7] * m_m[13] -
                            m_m[4]  * m_m[1] * m_m[15] +
                            m_m[4]  * m_m[3] * m_m[13] +
                            m_m[12] * m_m[1] * m_m[7] -
                            m_m[12] * m_m[3] * m_m[5];

                            inv[14] = -m_m[0]  * m_m[5] * m_m[14] +
                            m_m[0]  * m_m[6] * m_m[13] +
                            m_m[4]  * m_m[1] * m_m[14] -
                            m_m[4]  * m_m[2] * m_m[13] -
                            m_m[12] * m_m[1] * m_m[6] +
                            m_m[12] * m_m[2] * m_m[5];

                            inv[3] = -m_m[1] * m_m[6] * m_m[11] +
                            m_m[1] * m_m[7] * m_m[10] +
                            m_m[5] * m_m[2] * m_m[11] -
                            m_m[5] * m_m[3] * m_m[10] -
                            m_m[9] * m_m[2] * m_m[7] +
                            m_m[9] * m_m[3] * m_m[6];

                            inv[7] = m_m[0] * m_m[6] * m_m[11] -
                            m_m[0] * m_m[7] * m_m[10] -
                            m_m[4] * m_m[2] * m_m[11] +
                            m_m[4] * m_m[3] * m_m[10] +
                            m_m[8] * m_m[2] * m_m[7] -
                            m_m[8] * m_m[3] * m_m[6];

                            inv[11] = -m_m[0] * m_m[5] * m_m[11] +
                            m_m[0] * m_m[7] * m_m[9] +
                            m_m[4] * m_m[1] * m_m[11] -
                            m_m[4] * m_m[3] * m_m[9] -
                            m_m[8] * m_m[1] * m_m[7] +
                            m_m[8] * m_m[3] * m_m[5];

                            inv[15] = m_m[0] * m_m[5] * m_m[10] -
                            m_m[0] * m_m[6] * m_m[9] -
                            m_m[4] * m_m[1] * m_m[10] +
                            m_m[4] * m_m[2] * m_m[9] +
                            m_m[8] * m_m[1] * m_m[6] -
                            m_m[8] * m_m[2] * m_m[5];

                            det = m_m[0] * inv[0] + m_m[1] * inv[4] + m_m[2] * inv[8] + m_m[3] * inv[12];
                            if (det == 0)
                                throw Exception ("Matrix non-invertible (null determinant).");
                            det = 1.0 / det;
                            for (i = 0; i < 16; i++)
                                m_m[i] = inv[i] * det;
                            return (*this);
                        }

        /// Left axis
                        inline void setAxis (unsigned int axis, const Vec3<T> & x) { for (unsigned int i = 0; i < 3; i++) (*this)(axis, i) = x[i]; }
                        inline Vec3<T> getAxis (unsigned int axis) const { return Vec3<T> (m_m[4*axis], m_m[4*axis+1], m_m[4*axis+2]); }
                        inline Vec3<T> getTranslation () const { return Vec3<T> (m_m[12], m_m[13], m_m[14]); }
                        inline static Mat4 translation (const Vec3<T> & t) {
                            Mat4 m;
                            m.loadIdentity ();
                            m[12] = t[0];
                            m[13] = t[1];
                            m[14] = t[2];
                            return m;
                        }
                        inline static Mat4 rotate (const Vec3<T> & axis, T angle) {
                            Vec3<T> a = normalize (axis);
                            T s = sin (angle);
                            T c = cos (angle);
                            T oc = 1.0 - c;
                            return Mat4 (oc * a[0] * a[0] + c,           oc * a[0] * a[1] - a[2] * s,  oc * a[2] * a[0] + a[1] * s,  0.0,
                                oc * a[0] * a[1] + a[2] * s,  oc * a[1] * a[1] + c,           oc * a[1] * a[2] - a[0] * s,  0.0,
                                oc * a[2] * a[0] - a[1] * s,  oc * a[1] * a[2] + a[0] * s,  oc * a[2] * a[2] + c,           0.0,
                                0.0,                                0.0,                                0.0,                                1.0);
                        }
                        inline static Mat4 scale (const Vec3<T> & s) {
                            Mat4 m;
                            m.loadIdentity ();
                            m[0] = s[0];
                            m[5] = s[1];
                            m[10] = s[2];
                            return m;
                        }
                        inline static Mat4 lookAt(const Vec3<T> & eye, const Vec3<T> & center, const Vec3<T> & up) {
                            Mat4 m;
                            m.loadIdentity();
                            Vec3<T> f (normalize (center - eye));
                            Vec3<T> s = normalize (cross (f, up));
                            Vec3<T> u = normalize (cross (s, f));
                            m[0] = s[0];
                            m[4] = s[1];
                            m[8] = s[2];
                            m[1] = u[0];
                            m[5] = u[1];
                            m[9] = u[2];
                            m[2] = -f[0];
                            m[6] = -f[1];
                            m[10] = -f[2];
                            m[12] = -dot(s, eye);
                            m[13] = -dot(u, eye);
                            m[14] = dot(f, eye);
                            return m;
                        }
                        inline static Mat4 perspective (float fovy, float aspectRatio, float n, float f) {
                            Mat4 m;
                            const float deg2rad = M_PI/180.0;
                            float fovyRad = deg2rad*fovy;
                            float tanHalfFovy = tan (fovyRad/2.0);
                            for (unsigned int i = 0; i < 16; i++)
                                m[i] = 0.0;
                            m[0] = 1.0 / (aspectRatio * tanHalfFovy);
                            m[5] = 1.0 / tanHalfFovy;
                            m[10] = -(f + n) / (f - n);
                            m[11] = -1.0;
                            m[14] = -2.0*(f * n) / (f - n);
                            return m;
                        }
                    protected:
                        T m_m[16];
                    };

    template <class T> Mat4<T> operator* (const T &s, const Mat4<T> &M) {
                    return (M * s);
                }

    template <class T> Mat4<T> transpose (Mat4<T> & M) {
                return M.transpose ();
            }

    template <class T> Mat4<T> inverse (Mat4<T> & M) {
            return M.invert ();
        }

    template <class T> std::ostream & operator<< (std::ostream & output, const Mat4<T> & m) {
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++)
                output << m(j,i) << (j < 3 ? " " : "");
            output << std::endl;
        }
        return output;
    }

    template <class T> std::istream & operator>> (std::istream & input, Mat4<T> & m) {
    for (unsigned int i = 0; i < 4; i++)
        for (unsigned int j = 0; j < 4; j++)
            input >> m (j, i);
        return input;
    }

    typedef Mat4<float> Mat4f;
    typedef Mat4<double> Mat4d;
