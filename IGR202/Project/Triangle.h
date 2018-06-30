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
#include <vector>
#include "Vec3.h"

/// A Triangle class expressed as a triplet of indices (unsigned integer over an external vertex list)
class Triangle {
public:
    inline Triangle () {
        m_v[0] = m_v[1] = m_v[2] = 0;
    }

    inline Triangle (const Triangle & t) {
        m_v[0] = t.m_v[0];
        m_v[1] = t.m_v[1];
        m_v[2] = t.m_v[2];
    }
    
    inline Triangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        m_v[0] = v0;
        m_v[1] = v1;
        m_v[2] = v2;
    }
    
    inline ~Triangle () {}

    inline Triangle & operator= (const Triangle & t) {
        m_v[0] = t.m_v[0];
        m_v[1] = t.m_v[1];
        m_v[2] = t.m_v[2];
        return (*this);
    }

    inline unsigned int & operator[] (unsigned int i) { return m_v[i]; }
	inline bool contains(unsigned int i) const  { 
		return m_v[0] == i || m_v[1] == i||  m_v[2] == i; }

	/**
	* Get index of position index in the triangle
	* @param i the position index
	* @return the index in the triangle
	*/
	inline int getIndex(unsigned int position) {
		for (int i = 0; i < 3; i++) {
			if (m_v[i] == position) return i;
		}
	}

    inline unsigned int operator[] (unsigned int i) const { return m_v[i]; }

	inline bool swap(unsigned int i, unsigned int j) {
		if (i < 2 || j < 2 || i == j) return false;

		unsigned int tmp = m_v[i];
		m_v[i] = m_v[j];
		m_v[j] = tmp;
		
		return true;
	}

private:
    unsigned int m_v[3];
};