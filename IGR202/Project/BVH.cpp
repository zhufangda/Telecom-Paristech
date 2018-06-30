#include "BVH.h"
#include<vector>
#include "Mesh.h"
#include <cstdlib>
#include <ctime>
#include <GL/glew.h>
#include <GL/glut.h>
#include "AxisAlignedBoundingBox.h"
#include <cassert>
BVH::BVH()
{
}

int BVH::total_nbr_tri = 0;

BVH* BVH::buildBVH2(const Mesh& mesh, const std::vector<Triangle*>& triangles, int deep = 0)
{
	if (deep == 0) total_nbr_tri = 0;

	srand((unsigned int)time(NULL));
	BVH* node = new  BVH();

	std::vector<Vec3f> *barycentre = BVH::genBarycentre(mesh.positions(), triangles);

	int* x_index = genIndexArray(triangles);
	int* y_index = genIndexArray(triangles);
	int* z_index = genIndexArray(triangles);

	qsort(*barycentre, x_index, X_AXIS);
	qsort(*barycentre, z_index, Z_AXIS);
	qsort(*barycentre, y_index, Y_AXIS);


	float xMin = 100, xMax=-100, yMin=100, yMax=-100, zMin=100, zMax=-100;
	findExtreme(mesh, triangles, xMin, xMax, yMin, yMax, zMin, zMax);


	node->bbox.init(xMax, xMin, yMax, yMin, zMax, zMin);
	

	node->deep = deep;
	for (auto triangle_ptr : triangles) {
		for (int j = 0; j < 3; j++) {
			Vec3f position = mesh.positions()[(*triangle_ptr)[j]];
			if (!node->bbox.contain(position)) {
				std::cout << "BVH2: " << position << " for \t";
				std::cout << node->bbox << std::endl;

			}
		}

	}
	
	float theta_x = xMax - xMin;
	float theta_y = yMax - yMin;
	float theta_z = zMax - zMin;
	
	if (triangles.size() == 1) {
		total_nbr_tri++;
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		node->aixs = -1;
		node->triangle = triangles.back();
		return node;
	}

	std::vector<Triangle*> upperTriangleList, lowerTriangleList;

	if (theta_x >= theta_y && theta_x >= theta_z) {
		node->aixs = X_AXIS;
		node->barycentre_median = getPartition(triangles,
										*barycentre,
			upperTriangleList,
			lowerTriangleList,
			x_index,
			X_AXIS);



	}else if (theta_y >= theta_x && theta_y >= theta_z) {
		node->aixs = Y_AXIS;
		node->barycentre_median = getPartition(triangles,
			*barycentre,
			upperTriangleList,
			lowerTriangleList,
			y_index,
			Y_AXIS);
	}else {
		node->aixs = Z_AXIS;
		node->barycentre_median = getPartition(triangles,
			*barycentre,
			upperTriangleList,
			lowerTriangleList,
			z_index,
			Z_AXIS);
	}

	node->leftChild = buildBVH2(mesh, upperTriangleList, deep+1);
	node->rightChild = buildBVH2(mesh, lowerTriangleList, deep+1);

	delete barycentre;
	delete[] x_index;
	delete[] y_index;
	delete[] z_index;

	return node;
}


BVH* BVH::buildBVH(const Mesh& mesh, const std::vector<Triangle*>& triangles, int deep = 0)
{
	srand(time(NULL));
	BVH* node = new  BVH();

	std::vector<Vec3f> *barycentre = BVH::genBarycentre(mesh.positions(), triangles);

	int* x_index = genIndexArray(triangles);
	int* y_index = genIndexArray(triangles);
	int* z_index = genIndexArray(triangles);

	qsort(*barycentre, x_index, X_AXIS);
	qsort(*barycentre, z_index, Z_AXIS);
	qsort(*barycentre, y_index, Y_AXIS);


	int size = triangles.size();
	float xMin = findMin(mesh, *triangles[x_index[0]], X_AXIS);
	float xMax = findMax(mesh, *triangles[x_index[size - 1]], X_AXIS);
	float yMin = findMin(mesh, *triangles[y_index[0]], Y_AXIS);
	float yMax = findMax(mesh, *triangles[y_index[size - 1]], Y_AXIS);
	float zMin = findMin(mesh, *triangles[z_index[0]], Z_AXIS);
	float zMax = findMax(mesh, *triangles[z_index[size - 1]], Z_AXIS);


	node->bbox.init(xMax, xMin, yMax, yMin, zMax, zMin);
	node->deep = deep;
	for (auto triangle_ptr : triangles) {
		for (int j = 0; j < 3; j++) {
			Vec3f position = mesh.positions()[(*triangle_ptr)[j]];
			if (!node->bbox.contain(position)) {
				std::cout << "BVH: " << position << " for \t";
				std::cout << node->bbox << std::endl;

			}
		}

	}

	float theta_x = xMax - xMin;
	float theta_y = yMax - yMin;
	float theta_z = zMax - zMin;

	if (triangles.size() == 1) {
		node->leftChild = nullptr;
		node->rightChild = nullptr;
		node->aixs = -1;
		node->triangle = triangles.back();
		return node;
	}

	std::vector<Triangle*> upperTriangleList, lowerTriangleList;

	if (theta_x >= theta_y && theta_x >= theta_z) {
		node->aixs = X_AXIS;
		node->barycentre_median = getPartition(triangles,
			*barycentre,
			upperTriangleList,
			lowerTriangleList,
			x_index,
			X_AXIS);



	}
	else if (theta_y >= theta_x && theta_y >= theta_z) {
		node->aixs = Y_AXIS;
		node->barycentre_median = getPartition(triangles,
			*barycentre,
			upperTriangleList,
			lowerTriangleList,
			y_index,
			Y_AXIS);
	}
	else {
		node->aixs = Z_AXIS;
		node->barycentre_median = getPartition(triangles,
			*barycentre,
			upperTriangleList,
			lowerTriangleList,
			z_index,
			Z_AXIS);
	}

	node->leftChild = buildBVH(mesh, upperTriangleList, deep + 1);
	node->rightChild = buildBVH(mesh, lowerTriangleList, deep + 1);

	delete barycentre;
	delete[] x_index;
	delete[] y_index;
	delete[] z_index;

	return node;
}

Vec3f BVH::getPartition(const std::vector<Triangle*>& triangles,
										   const std::vector<Vec3f>& barycentres,
										   std::vector<Triangle*>& upperList,
										   std::vector<Triangle*>& lowerList,
											const int* const index, 
											const int axis) {
	auto size = triangles.size();
	int median1 = (size-1) / 2, median2 = (size) / 2;
	Vec3f barycentre_median = (barycentres.at(median1) + barycentres.at(median2))/2;
	for (int i = 0; i <= median1; i++) {
		upperList.push_back(triangles[index[i]]);
	}

	for (unsigned int i = median1 + 1; i < size; i++) {
		lowerList.push_back(triangles[index[i]]);
	}



	return barycentre_median;
}


void BVH::qsort(const std::vector<Vec3f>& barycentres, int* const index, const int aix){
	std::vector<Region> regions;

	Region region;
	region.left = 0;
	region.right = barycentres.size()-1;
	regions.push_back(region);

	while (!regions.empty()) {
		region = regions.back();
		regions.pop_back();

		if (region.left + 20 < region.right) {
			int p = partition(index, region.left, region.right, barycentres, aix);
			if (p - 1 > region.left) {
				Region regionlow;
				regionlow.left = region.left;
				regionlow.right = p - 1;
				regions.push_back(regionlow);
			}

			if (p + 1 < region.right) {
				Region regionhigh;
				regionhigh.left = p + 1;
				regionhigh.right = region.right;
				regions.push_back(regionhigh);
			}
		}
		else {
			insertsort(index, region.left, region.right, barycentres, aix);

		}

	}		
	regions.clear();


}

void BVH::insertsort(int* const index, const int left, const int right, const std::vector<Vec3f>& barycentre, int aix) {
	int i, j, tmp;
	for (i = left; i < right; i++) {
		tmp = index[i];
		for (j = i; j > 0 && barycentre[index[j - 1]][aix] > barycentre[tmp][aix]; j--) {
			index[j] = index[j - 1];
		}
		index[j] = tmp;
	}
	//BVH::findDuplicatedElement(index, right - left + 1);
}
int* BVH::genIndexArray(const std::vector<Triangle*>& container) {
	int* index = new int[container.size()];
	for (unsigned int i = 0; i < container.size(); i++) {
		index[i] = i;
	}

	return index;
}

std::vector<Vec3f>* BVH::genBarycentre(const std::vector<Vec3f>& positions, const std::vector<Triangle*>& triangles) {
	std::vector<Vec3f>* barycentre = new std::vector<Vec3f>();
	float x, y, z;
	const Vec3f *point1, *point2, *point3;
	for (Triangle* triangle : triangles) {
		point1 = &positions[(*triangle)[0]];
		point2 = &positions[(*triangle)[1]];
		point3 = &positions[(*triangle)[2]];

		x = ((*point1)[0] + (*point2)[0] + (*point3)[0]) / 3;
		y = ((*point1)[1] + (*point2)[1] + (*point3)[1]) / 3;
		z = ((*point1)[2] + (*point2)[2] + (*point3)[2]) / 3;
		barycentre->push_back(Vec3f(x,y,z));

	}
	
	return barycentre;

}

AxisAlignedBoundingBox & BVH::getAABB()
{
	return this->bbox;
}



int BVH::partition(int *const index, int left, int right, const std::vector<Vec3f>& barycentre, int aix) {
	assert(left < right);
	
	//int pivot = rand() % (right - left + 1) + left;
	int pivot = (left + right) / 2;
	float pivot_value = barycentre.at(index[pivot])[aix];
	if (barycentre.at(left)[aix] > pivot_value) swap(index[pivot], index[left]);
	if (barycentre.at(left)[aix] > barycentre.at(right)[aix]) swap(index[left], index[right]);
	if (barycentre.at(right)[aix] < pivot_value) swap(index[pivot], index[right]);
	
	
	swap(index[pivot], index[right - 1]);

	pivot_value = barycentre.at(index[right-1])[aix];
	int i = left, j = right-1;

	while (i<j) {
			while (barycentre.at(index[++i])[aix] < pivot_value && i<right-1) ;
			while (barycentre.at(index[--j])[aix] > pivot_value && j>left) ;
			if (i < j) {
				swap(index[i], index[j]);
			}
			else {
				break;
			}
	}

	swap(index[i], index[right - 1]);
	
	//bool flag = findDuplicatedElement(index, barycentre.size());
	
	return i;

}
void BVH::swap(int &a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}


BVH::~BVH() {
	delete this->leftChild;
	delete this->rightChild;
}


void BVH::drawBVH(const float width, const int deep) const{


	if (this->deep == deep) {
		this->bbox.drawBox(width);
	}
	else {
		if (leftChild != nullptr) {
			leftChild->drawBVH(width, deep);
		}
		if (rightChild != nullptr){
			rightChild->drawBVH(width, deep);
		}
	}
	
}

float BVH::findMax(const Mesh& mesh, const Triangle& triangle, int axis) {
	float tmp[3];
	float max = -100;
	for (int i = 0; i < 3; i++) {
		tmp[i] = mesh.positions()[triangle[i]][axis];
		if (max < tmp[i]) max = tmp[i];
	}

	return max;
}

void BVH::findExtreme(const Mesh& mesh, const std::vector<Triangle*>& triangles, float& xMin, float& xMax, float& yMin, float& yMax, float& zMin, float& zMax) {

	for (auto triangle : triangles) {
			if (xMin > findMin(mesh, *triangle, X_AXIS)) {
				xMin = findMin(mesh, *triangle, X_AXIS);
			}

			if (xMax < findMax(mesh, *triangle, X_AXIS)) {
				xMax = findMax(mesh, *triangle, X_AXIS);
			}

			if ( yMin > findMin(mesh, *triangle, Y_AXIS)) {
				yMin = findMin(mesh, *triangle, Y_AXIS);
			}
			
			if (yMax < findMax(mesh, *triangle, Y_AXIS)) {
				yMax = findMax(mesh, *triangle, Y_AXIS);
			}
				
			if (zMin > findMin(mesh, *triangle, Z_AXIS)) {
				zMin = findMin(mesh, *triangle, Z_AXIS);
			}
			
			if (zMax < findMax(mesh, *triangle, Z_AXIS)) {
				zMax = findMax(mesh, *triangle, Z_AXIS);
			}
	}
}

bool BVH::rayCollision(const Mesh& mesh, const Ray& ray) const{
	if (this->bbox.intersection(ray)) {

		bool flag1 = false;
		bool flag2 = false;
		if (this->triangle != nullptr && ray.intersectionRayonTriangle(mesh, *(this->triangle))) {
			//std::cout << "Find triangle:" << static_cast<const void*>(triangle) << " in box " << this->bbox;
			return true;
		}

		if (this->rightChild != nullptr && bbox.intersection(ray)) {
			//std::cout << "deep-l\t" << this->deep << std::endl;
			flag1 = rightChild->rayCollision(mesh, ray);
		}


		if (this->leftChild != nullptr && bbox.intersection(ray)) {
			//std::cout << "deep_r" << this->deep << std::endl;
			flag2 = leftChild->rayCollision(mesh, ray);
		}

		if (flag1 == true || flag2 == true) 
			return true;
		

	}

	return false;


}
float BVH::findMin(const Mesh& mesh, const Triangle& triangle, const int axis) {
	float tmp[3];
	float min = FLT_MAX;
	for (int i = 0; i <3; i++) {
		tmp[i] = mesh.positions()[triangle[i]][axis];
		if (min > tmp[i]) min = tmp[i];
	}

	return min;
}

bool BVH::findDuplicatedElement(int* index, int size){
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (index[i] == index[j]) {
				std::cout << i << "and" << j << " duplicated!"<<std::endl;
				return true;
			}

		}
	}

	return false;
}