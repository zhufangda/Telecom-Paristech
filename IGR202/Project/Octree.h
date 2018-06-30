//#pragma once
//
//#include<memory>
//#include<vector>
//#include "Vec3.h"
//
//
//typedef struct {
//	float x_min;
//	float x_middle;
//	float x_max;
//	float y_min;
//	float y_middle;
//	float y_max;
//	float z_min;
//	float z_middle;
//	float z_max;
//} BoundingBox;
//
//
//class OctreeNode
//{
//public:
//
//
//	static OctreeNodePtr createOctree(unsigned int i, std::vector<Vec3f>& m_positions);
//	
//	~OctreeNode() {};
//
//private:
//	OctreeNode();
//	AxisAlignedBoundingBox getBoundingBoxInfo(const std::vector<unsigned int>& datas);
//
//	static OctreeNodePtr buildOctree(const std::vector<unsigned int>& datas, const BoundingBox& bbox);
//	int leavesCounter = -1;
//	int maxVertexPerLeaf = 1;
//	BoundingBox bbox;
//	int index = -1;
//	std::vector<std::shared_ptr<OctreeNode>> children;
//	std::vector<unsigned int> original_index; //data
//	
//	
//	std::vector<Vec3f>& m_positions;
//};
//
//using OctreeNodePtr = std::shared_ptr<OctreeNode>;
//
