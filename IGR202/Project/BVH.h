#pragma once
#include<vector>
#include<cfloat>
#include "Triangle.h"
#include "Vec3.h"
#include "AxisAlignedBoundingBox.h"
#include <cstdlib>
#include <vector>
#include "Mesh.h"
class BVH
{
public:
	static int total_nbr_tri;
	int tri_nb = 0;
	static const int X_AXIS = 0;
	static const int Y_AXIS = 1;
	static const int Z_AXIS = 2;

	BVH();
	static BVH* buildBVH2(const Mesh & mesh, const std::vector<Triangle*>& triangles, int deep);
	static BVH* buildBVH(const Mesh& mesh, const std::vector<Triangle*>& traingles, int deep);
	virtual ~BVH();

	void drawBVH(const float width, const int deep) const;

	static float findMax(const Mesh & mesh, const Triangle & triangle, int axis);

	static void findExtreme(const Mesh & mesh, const std::vector<Triangle*>& triangles, float & xMin, float & xMax, float & yMin, float & yMax, float & zMin, float & zMax);


	bool rayCollision(const Mesh & mesh, const Ray & ray) const;

	static float findMin(const Mesh & mesh, const Triangle & triangle, const int axis);

	static bool findDuplicatedElement(int * index, int size);

	/** Cette fonction met en ordre le triangles par un aix indiqué par paramètre et retourné un tableaux de indices par ordre
	* @param triangles list de traingles à traiter
	* @param index tableaux de indice
	* @param left la borne inférieur de tableau
	* @param right la borne supérieur de tableau
	* @param axial axial à comparere
	**/
	void static qsort(const std::vector<Vec3f>& barycentres, int * const index, const int axial);



	static Vec3f getPartition(const std::vector<Triangle*>& triangles,
		const std::vector<Vec3f>& barycentres,
		std::vector<Triangle*>& upperList,
		std::vector<Triangle*>& lowerList,
		const int* const index,
		const int axis);
	static void insertsort(int * const index, const int left, const int right, const std::vector<Vec3f>& barycentre, int aix);
	static int* genIndexArray(const std::vector<Triangle*>& container);
	static std::vector<Vec3f>* genBarycentre(const std::vector<Vec3f>& positions, const std::vector<Triangle*>& triangles);

	AxisAlignedBoundingBox& getAABB();




private:

	AxisAlignedBoundingBox bbox;
	BVH* leftChild = nullptr;
	BVH* rightChild = nullptr;
	int aixs;
	Vec3f barycentre_median;
	Triangle* triangle = nullptr;
	int deep = 0;
	static int partition(int * const index, int left, int right, const std::vector<Vec3f>& barycentre, int aix);
	static void swap(int & a, int & b);

};


typedef struct Region {
	int left;
	int right;
}Region;



