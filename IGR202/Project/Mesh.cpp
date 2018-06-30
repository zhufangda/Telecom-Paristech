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
#define _USE_MATH_DEFINES

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <utility> 
#include <cmath>
using namespace std;





void Mesh::clear() {
	m_positions.clear();
	m_normals.clear();
	m_triangles.clear();
}

void Mesh::loadOFF(const std::string & filename) {
	clear();
	ifstream in(filename.c_str());
	if (!in)
		throw std::runtime_error("File " + filename + " not found!");
	string offString;
	unsigned int sizeV, sizeT, tmp;
	in >> offString >> sizeV >> sizeT >> tmp;
	m_positions.resize(sizeV);
	m_triangles.resize(sizeT);
	for (unsigned int i = 0; i < sizeV; i++)
		in >> m_positions[i];
	int s;
	for (unsigned int i = 0; i < sizeT; i++) {
		in >> s;
		for (unsigned int j = 0; j < 3; j++)
			in >> m_triangles[i][j];
	}
	in.close();
	centerAndScaleToUnit();
	recomputeNormals();
}

void Mesh::exportOFF(const std::string & filename) {
	ofstream out(filename.c_str());
	if (!out)
		exit(1);
	string offString;
	auto sizeV = this->m_positions.size();
	auto sizeT = this->m_triangles.size();

	out << "OFF" << endl;
	out << sizeV << " " << sizeT << " " << 0 << endl;

	for (unsigned int i = 0; i < sizeV; i++) {
		out << m_positions[i] << endl;
	}

	for (unsigned int i = 0; i < sizeT; i++) {
		out << 3 << " ";
		for (unsigned int j = 0; j < 3; j++) {
			out << m_triangles[i][j] << " ";
		}
		out << endl;
	}

	out.close();

}

void Mesh::recomputeNormals() {
	m_normals.clear();
	m_normals.resize(m_positions.size(), Vec3f(0.f, 0.f, 0.f));
	for (unsigned int i = 0; i < m_triangles.size(); i++) {
		Vec3f e01 = m_positions[m_triangles[i][1]] - m_positions[m_triangles[i][0]];
		Vec3f e02 = m_positions[m_triangles[i][2]] - m_positions[m_triangles[i][0]];
		Vec3f n = cross(e01, e02);
		n.normalize();
		for (unsigned int j = 0; j < 3; j++)
			m_normals[m_triangles[i][j]] += n;
	}
	for (unsigned int i = 0; i < m_normals.size(); i++)
		m_normals[i].normalize();
}

void Mesh::centerAndScaleToUnit() {
	Vec3f c;
	for (unsigned int i = 0; i < m_positions.size(); i++)
		c += m_positions[i];
	c /= m_positions.size();
	float maxD = dist(m_positions[0], c);
	for (unsigned int i = 0; i < m_positions.size(); i++) {
		float m = dist(m_positions[i], c);
		if (m > maxD)
			maxD = m;
	}
	for (unsigned int i = 0; i < m_positions.size(); i++)
		m_positions[i] = (m_positions[i] - c) / maxD;
}


/*********************** Mesh Filter ********************************/


double Mesh::cot(const Vec3f& vec1, const Vec3f& vec2) {
	double cos_v = dot(vec1, vec2) / (vec1.length() * vec2.length());
	return cos_v / sqrt(1 - cos_v * cos_v);
}



void Mesh::topoFilter() {
	createAdjacencyMatrix();

	if (!Lt.empty()) Lt.clear();

	Lt.resize(m_positions.size(), Vec3f(0.0f, 0.0f, 0.0f));

	/** get barycentreList**/
	for (unsigned int i = 0; i < m_adjacency_list.size(); i++) {
		if (m_adjacency_list[i].empty()) continue;
		Vec3f barycentre(0.0f, 0.0f, 0.0f);
		for (auto indice : m_adjacency_list[i]) {
			Lt[i] += m_positions[indice] - m_positions[i];
		}
		Lt[i] /= m_adjacency_list[i].size();
	}

	//m_positions_filtrage.resize(m_positions.size());
	
	for (unsigned int i = 0; i < m_positions.size(); i++) {
		m_positions[i] += Lt[i];
	}

	recomputeNormals();

}

void Mesh::geoFilter(float alpha) {
	createAdjacencyMatrix();
	if (!Lg.empty()) {
		Lg.clear();
	}

	Lg.resize(m_positions.size(), Vec3f(0, 0, 0));

	for (unsigned int i = 0; i < m_positions.size(); i++) {
		float weight = 0.0f;
		auto v_i = m_positions[i];
		for (int unsigned j = 0; j < m_adjacency_list[i].size(); j++) {
			auto v_j = m_positions[m_adjacency_list[i][j]];
			auto v_k = m_positions[m_adjacency_list[i][++j]];

			auto edge_1 =  v_j - v_i;
			auto edge_2 =  v_k - v_i;

			float cot_1 = cot(edge_1, v_j - v_k);
			float cot_2 = cot(edge_2, v_k - v_j);

			weight += cot_2 + cot_1;

			//surface += cross(-edge_1, -edge_2).length() / 3.0f;
			Lg[i] += cot_2 * edge_1 + cot_1 * edge_2;
		}

		Lg[i] = Lg[i] * 0.5 / weight;
	}

	for (unsigned int i = 0; i < m_positions.size(); i++) {
		m_positions[i] +=  alpha * Lg[i];
	}

	
	recomputeNormals();

}


void Mesh::createAdjacencyMatrix() {
	if (m_adjacency_list.size() != 0) {
		m_adjacency_list.clear();
	}

	m_adjacency_list.resize(m_positions.size());

	for (Triangle triangle : m_triangles) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (triangle[i] != triangle[j]) {
					m_adjacency_list[triangle[i]].push_back(triangle[j]);
				}
			}
		}
	}

}

void Mesh::createAdjacencyTrianglesMatrix()
{
	if (!m_adjacency_triangles_list.empty()) {
		m_adjacency_list.clear();
	}

	m_adjacency_triangles_list.resize(m_positions.size());

	for (auto triangle : m_triangles) {
		m_adjacency_triangles_list[triangle[0]].push_back(triangle);

		triangle.swap(0, 1);
		m_adjacency_triangles_list[triangle[0]].push_back(triangle);

		triangle.swap(0, 2);
		m_adjacency_triangles_list[triangle[2]].push_back(triangle);
	}
}

float Mesh::voronoiRegionSurface(std::vector<Triangle> t_list, int x)
{

	return 0.0f;
}

float Mesh::meanCurvatureOperator(int position_index)
{
	const std::vector<Triangle>& t_list
		= m_adjacency_triangles_list[position_index];

	for (auto triangle : t_list) {


	}

	return 0.0f;
}



//const std::vector<Vec3f>& Mesh::getFilterPositions() const {
//	if (this->m_positions_filtrage.size() == 0) {
//		throw logic_error("You have to use filter function before!");
//	}
//
//	return m_positions_filtrage;
//
//
//}


/************************ Simplification ********************************/

// Get bounding box
Mesh::BoundingBox Mesh::getBoundingCube(const std::vector<Vec3f>& positions) {
	BoundingBox bbox;
	if(positions.size() <=0){
		throw invalid_argument("Nomber of vertex have to be bigger than 1!");
	}


	bbox.x_max = positions[0][0];
	bbox.x_min = positions[0][0];
	bbox.y_max = positions[0][1];
	bbox.y_min = positions[0][1];
	bbox.z_max = positions[0][2];
	bbox.z_min = positions[0][2];

	for (const auto& position : positions) {
		bbox.x_min = bbox.x_min > position[0] ? position[0] : bbox.x_min;
		bbox.x_max = bbox.x_max < position[0] ? position[0] : bbox.x_max;
		bbox.y_min = bbox.y_min > position[1] ? position[1] : bbox.y_min;
		bbox.y_max = bbox.y_max < position[1] ? position[1] : bbox.y_max;
		bbox.z_min = bbox.z_min > position[2] ? position[2] : bbox.z_min;
		bbox.z_max = bbox.z_max < position[2] ? position[2] : bbox.z_max;
	}
	float x_len = bbox.x_max - bbox.x_min;
	float y_len = bbox.y_max - bbox.y_min;
	float z_len = bbox.z_max - bbox.z_min;
	float len_max = x_len > y_len ? (x_len > z_len ? x_len : z_len) : (y_len > z_len ? y_len : z_len);
	len_max *= 1.00001;


	bbox.x_middle = (bbox.x_max + bbox.x_min) * 0.5f;
	bbox.y_middle = (bbox.y_max + bbox.y_min) * 0.5f;
	bbox.z_middle = (bbox.z_max + bbox.z_min) * 0.5f;

	bbox.x_min = bbox.x_middle - 0.5 * len_max;
	bbox.x_max = bbox.x_middle + 0.5 * len_max;
	bbox.y_min = bbox.x_middle - 0.5 * len_max;
	bbox.y_max = bbox.x_middle + 0.5 * len_max;
	bbox.z_min = bbox.x_middle - 0.5 * len_max;
	bbox.z_max = bbox.x_middle + 0.5 * len_max;


	return bbox;
}

void Mesh::simplify(unsigned resolution) {
	std::vector<Vec3f> simplify_normals;
	std::vector<Vec3f> simplify_positions;
	std::vector<Triangle> simplify_triangles;

	// step 1
	BoundingBox bbox = getBoundingCube(m_positions);

	this->resolution = resolution;
	// step 2
	std::vector<GrillCelle> grillCelles(resolution * resolution * resolution);

    float celle_length = (bbox.x_max - bbox.x_min) / (resolution - 1);
	

	//step 3
	for (unsigned int i = 0; i < m_positions.size(); i++) {
		int cellIndex = getCell(i, bbox, celle_length, grillCelles);
		grillCelles[cellIndex].position += m_positions[i];
		grillCelles[cellIndex].normal += m_normals[i];
		grillCelles[cellIndex].weight++;
	}


	// step 5
	int vecIndex = 0;
	for (unsigned int i = 0; i < grillCelles.size(); i++) {
		grillCelles[i].celleIndex = i; 
		if (grillCelles[i].weight <= 0) continue;
		grillCelles[i].vecIndex = simplify_positions.size();
		simplify_positions.push_back(grillCelles[i].position / (grillCelles[i].weight*1.0));
		grillCelles[i].normal.normalize();
		
		simplify_normals.push_back(grillCelles[i].normal);
	}

	//step 4

	for (auto triangle : m_triangles) {
		unsigned int p1_index = getCell(triangle[0], bbox, celle_length, grillCelles);
		unsigned int p2_index = getCell(triangle[1], bbox, celle_length, grillCelles);
		unsigned int p3_index = getCell(triangle[2], bbox, celle_length, grillCelles);

		if (p1_index != p2_index && p2_index != p3_index && p3_index != p1_index) {
			Vec3f normal = simplify_normals[p1_index] + simplify_normals[p2_index] + simplify_normals[p3_index];
			Vec3f test_normal = cross(simplify_positions[p2_index] - simplify_positions[p1_index]
				, simplify_positions[p3_index] - simplify_positions[p2_index]);

			if (dot(normal, test_normal) < 0.0f) {
				simplify_triangles.push_back(Triangle(p1_index, p3_index, p2_index));
			}
			else {
				simplify_triangles.push_back(Triangle(p1_index, p2_index, p3_index));
			}

		}
	}

	m_positions.clear();
	m_triangles.clear();
	m_normals.clear();

	m_positions = simplify_positions;
	m_triangles = simplify_triangles;
	m_normals = simplify_normals;

}


unsigned int Mesh::getCell(unsigned int index, const BoundingBox& bbox, float celle_length, std::vector<GrillCelle>& celleList) {
	int x_index = (m_positions[index][0] - bbox.x_min) / celle_length;
	int y_index = (m_positions[index][1] - bbox.y_min) / celle_length;
	int z_index = (m_positions[index][2] - bbox.z_min) / celle_length;
	int res = x_index * resolution * resolution + y_index * resolution + z_index;
	if (res >= resolution * resolution * resolution) {
		throw overflow_error(" Over range of grill");
	}

	return celleList[res].vecIndex == -1 ? res : celleList[res].vecIndex;
}


//const std::vector<Vec3f>& Mesh::getSimplifyPositions() const {
//	if (this->simplify_positions.size() == 0) {
//		throw std::logic_error("You have to use simply() function before!");
//	}
//
//	return simplify_positions;
//
//}
//
//const std::vector<Vec3f>& Mesh::getSimplifyNormals() const {
//	if (this->simplify_normals.size() == 0) {
//		throw std::logic_error("You have to use simply() function before!");
//	}
//
//	return simplify_normals;
//
//}
//
//const std::vector<Triangle>& Mesh::getSimplifyTriangles() const {
//	if (this->simplify_triangles.size() == 0) {
//		throw std::logic_error("You have to use simply() function before!");
//	}
//
//	return simplify_triangles;
//
//}



/************************* Simplification with octree  **************/
void Mesh::initNode(OctreeNodePtr nodePtr, const std::vector<unsigned int>& data)
{
	nodePtr->original_index = data;
	nodePtr->index = ++leavesCounter;
}

void Mesh::dataSpatiaSplite(const std::vector<unsigned int>& datas, std::vector<unsigned int> childrenData[], const BoundingBox& bbox)
{

	for (int i = 0; i < 8; i++) {
		childrenData[i].clear();
	}


	for (const auto& data : datas) {
		float x = m_positions[data][0], y = m_positions[data][1], z = m_positions[data][2];
		if (x > bbox.x_max || x < bbox.x_min || y > bbox.y_max || y < bbox.y_min || z > bbox.z_max || z < bbox.z_min) {
			throw invalid_argument("Data positions is out of bound!");
		}

		if (x >= bbox.x_middle && y >= bbox.y_middle && z >= bbox.z_middle) {
			childrenData[0].push_back(data);
		}
		else if (x < bbox.x_middle && y >= bbox.y_middle && z >= bbox.z_middle) {
			childrenData[1].push_back(data);
		}
		else if (x < bbox.x_middle && y < bbox.y_middle && z >= bbox.z_middle) {
			childrenData[2].push_back(data);
		}
		else if (x >= bbox.x_middle && y < bbox.y_middle && z >= bbox.z_middle) {
			childrenData[3].push_back(data);
		}
		else if (x >= bbox.x_middle && y >= bbox.y_middle && z < bbox.z_middle) {
			childrenData[4].push_back(data);
		}
		else if (x < bbox.x_middle && y >= bbox.y_middle && z < bbox.z_middle) {
			childrenData[5].push_back(data);
		}
		else if (x < bbox.x_middle && y < bbox.y_middle && z < bbox.z_middle) {
			childrenData[6].push_back(data);
		}
		else {
			childrenData[7].push_back(data);
		}

	}
}

Mesh::BoundingBox Mesh::getBoundingBoxInfo(const std::vector<unsigned int>& datas) {
	BoundingBox bbox;

	if (datas.size() <= 0) {
		throw std::invalid_argument("Datas size can not be 0!");
	}

	bbox.x_max = m_positions[datas[0]][0];
	bbox.x_min = m_positions[datas[0]][0];
	bbox.y_max = m_positions[datas[0]][1];
	bbox.y_min = m_positions[datas[0]][1];
	bbox.z_max = m_positions[datas[0]][2];
	bbox.z_min = m_positions[datas[0]][2];

	for (const auto data : datas) {
		bbox.x_min = bbox.x_min > m_positions[data][0] ? m_positions[data][0] : bbox.x_min;
		bbox.x_max = bbox.x_max < m_positions[data][0] ? m_positions[data][0] : bbox.x_max;
		bbox.y_min = bbox.y_min > m_positions[data][1] ? m_positions[data][1] : bbox.y_min;
		bbox.y_max = bbox.y_max < m_positions[data][1] ? m_positions[data][1] : bbox.y_max;
		bbox.z_min = bbox.z_min > m_positions[data][2] ? m_positions[data][2] : bbox.z_min;
		bbox.z_max = bbox.z_max < m_positions[data][2] ? m_positions[data][2] : bbox.z_max;
	}

	bbox.x_min -= (bbox.x_max - bbox.x_min) / 1000.0f;
	bbox.x_max += (bbox.x_max - bbox.x_min) / 1001.0f;
	bbox.y_min -= (bbox.y_max - bbox.y_min) / 1000.0f;
	bbox.y_max += (bbox.y_max - bbox.y_min) / 1001.0f;
	bbox.z_min -= (bbox.z_max - bbox.z_min) / 1000.0f;
	bbox.z_max += (bbox.z_max - bbox.z_min) / 1001.0f;

	bbox.x_middle = (bbox.x_max + bbox.x_min) / 2.0f;
	bbox.y_middle = (bbox.y_max + bbox.y_min) / 2.0f;
	bbox.z_middle = (bbox.z_max + bbox.z_min) / 2.0f;

	return bbox;
}



Mesh::OctreeNodePtr Mesh::buildOctree(const std::vector<unsigned int>& datas, const BoundingBox& bbox)
{
	OctreeNodePtr nodePtr = std::make_shared<OctreeNode>();
	nodePtr->bbox = bbox;

	if (stopCriteria(datas)) {
		initNode(nodePtr, datas);
	}
	else {

		std::vector<unsigned int> childrenData[8];
		dataSpatiaSplite(datas, childrenData, bbox);

		BoundingBox bboxs[8];
		setChildrenBBoxesList(bbox, bboxs);

		nodePtr->children.clear();
		for (int i = 0; i < 8; i++) {
			if (childrenData[i].size() <= 0) continue;
			nodePtr->children.push_back(buildOctree(childrenData[i], bboxs[i]));
			//nodePtr->children.push_back(buildOctree(childrenData[i], getBoundingBoxInfo(childrenData[i])));
		}

	}

	return nodePtr;
}

void Mesh::setChildrenBBoxesList(const Mesh::BoundingBox& parentBBox, Mesh::BoundingBox childrenBBoxes[]) {

	childrenBBoxes[0] = { parentBBox.x_middle, (parentBBox.x_middle + parentBBox.x_max) / 2.0f , parentBBox.x_max,
		parentBBox.y_middle,	(parentBBox.y_middle + parentBBox.y_max) / 2.0f ,parentBBox.y_max,
		parentBBox.z_middle,	(parentBBox.z_middle + parentBBox.z_max) / 2.0f	,parentBBox.z_max };

	childrenBBoxes[1] = { parentBBox.x_min, (parentBBox.x_min + parentBBox.x_middle) / 2.0f , parentBBox.x_middle,
		parentBBox.y_middle,	(parentBBox.y_middle + parentBBox.y_max) / 2.0f ,parentBBox.y_max,
		parentBBox.z_middle,	(parentBBox.z_middle + parentBBox.z_max) / 2.0f	,parentBBox.z_max };

	childrenBBoxes[2] = { parentBBox.x_min, (parentBBox.x_min + parentBBox.x_middle) / 2.0f , parentBBox.x_middle,
		parentBBox.y_min,	(parentBBox.y_min + parentBBox.y_middle) / 2.0f ,parentBBox.y_middle,
		parentBBox.z_middle,	(parentBBox.z_middle + parentBBox.z_max) / 2.0f	,parentBBox.z_max };

	childrenBBoxes[3] = { parentBBox.x_middle, (parentBBox.x_middle + parentBBox.x_max) / 2.0f , parentBBox.x_max,
		parentBBox.y_min,	(parentBBox.y_min + parentBBox.y_middle) / 2.0f ,parentBBox.y_middle,
		parentBBox.z_middle,	(parentBBox.z_middle + parentBBox.z_max) / 2.0f	,parentBBox.z_max };

	childrenBBoxes[4] = { parentBBox.x_middle, (parentBBox.x_middle + parentBBox.x_max) / 2.0f , parentBBox.x_max,
		parentBBox.y_middle,	(parentBBox.y_middle + parentBBox.y_max) / 2.0f ,parentBBox.y_max,
		parentBBox.z_min,	(parentBBox.z_min + parentBBox.z_middle) / 2.0f	,parentBBox.z_middle };

	childrenBBoxes[5] = { parentBBox.x_min, (parentBBox.x_min + parentBBox.x_middle) / 2.0f , parentBBox.x_middle,
		parentBBox.y_middle,	(parentBBox.y_middle + parentBBox.y_max) / 2.0f ,parentBBox.y_max,
		parentBBox.z_min,	(parentBBox.z_min + parentBBox.z_middle) / 2.0f	,parentBBox.z_middle };

	childrenBBoxes[6] = { parentBBox.x_min, (parentBBox.x_min + parentBBox.x_middle) / 2.0f , parentBBox.x_middle,
		parentBBox.y_min,	(parentBBox.y_min + parentBBox.y_middle) / 2.0f ,parentBBox.y_middle,
		parentBBox.z_min,	(parentBBox.z_min + parentBBox.z_middle) / 2.0f	,parentBBox.z_middle };

	childrenBBoxes[7] = { parentBBox.x_middle, (parentBBox.x_middle + parentBBox.x_max) / 2.0f , parentBBox.x_max,
		parentBBox.y_min,	(parentBBox.y_min + parentBBox.y_middle) / 2.0f ,parentBBox.y_middle,
		parentBBox.z_min,	(parentBBox.z_min + parentBBox.z_middle) / 2.0f	,parentBBox.z_middle };

}

bool Mesh::containsPtr(const OctreeNodePtr& nodePtr, const Vec3f& vertex) {
	BoundingBox& bbox = nodePtr->bbox;

	if (vertex[0] < bbox.x_min || vertex[0] > bbox.x_max
		|| vertex[1] < bbox.y_min || vertex[1] > bbox.y_max
		|| vertex[2] < bbox.z_min || vertex[2] > bbox.z_max) {
		return false;
	}

	return true;
}

unsigned int Mesh::findIndexByPosition(const OctreeNodePtr& nodePtr, const Vec3f& vertex) {

	if (nodePtr->children.size() == 0) {
		return nodePtr->index;
	}

	for (auto child : nodePtr->children) {
		if (containsPtr(child, vertex)) {
			return findIndexByPosition(child, vertex);
		}
	}
}

void Mesh::simplifyAdaptiveMesh(unsigned int n)
{
	this->maxChildrenNbr = n;
	std::vector<unsigned int> dataIndex;
	for (unsigned int i = 0; i < m_positions.size(); i++) {
		dataIndex.push_back(i);
	}

	// Step 1 Get bounding box
	BoundingBox bbox = getBoundingBoxInfo(dataIndex);

	// Step 2 Create octree
	this->leavesCounter = -1;
	OctreeNodePtr octree = buildOctree(dataIndex, bbox);

	//Step 3 
	std::vector<OctreeNodePtr> leaves;
	getLeaves(octree, leaves);

	std::vector<Vec3f> simplify_normals(leaves.size());
	std::vector<Vec3f> simplify_positions(leaves.size());
	std::vector<Triangle> simplify_triangles;
	
	for (auto leaf : leaves) {
		Vec3f position(0.0f, 0.0f, 0.0f);
		Vec3f normal(0.0f, 0.0f, 0.0f);
		for (auto index : leaf->original_index) {
			position += m_positions[index];
			normal += m_normals[index];
		}

		position /= leaf->original_index.size();
		normal.normalize();
		simplify_normals[leaf->index] = normal;
		simplify_positions[leaf->index] = position;

	}
	// Step 4 
	for (auto triangle : m_triangles) {
		unsigned int p1_index = findIndexByPosition(octree, m_positions[triangle[0]]);
		unsigned int p2_index = findIndexByPosition(octree, m_positions[triangle[1]]);
		unsigned int p3_index = findIndexByPosition(octree, m_positions[triangle[2]]);

		if (p1_index != p2_index && p2_index != p3_index && p3_index != p1_index) {
			simplify_triangles.push_back(Triangle(p1_index, p2_index, p3_index));
		}
	}

	m_positions.clear();
	m_triangles.clear();
	m_normals.clear();

	m_positions = simplify_positions;
	m_triangles = simplify_triangles;
	m_normals = simplify_normals;

}

void Mesh::getLeaves(const OctreeNodePtr & nodePtr, std::vector<OctreeNodePtr>& leaves)
{
	if (nodePtr->children.size() == 0) {
		leaves.push_back(nodePtr);
		return;
	}

	for (auto child : nodePtr->children) {
		getLeaves(child, leaves);
	}
}

/***********************   Subdivision   *************************************/
void Mesh::subdivide() {
	std::map<Edge, unsigned int> midPointMap = insertMidPoint();
	updateTriangle(midPointMap);
	averagingPass();
}

void Mesh::averagingPass() {
	std::vector<Vec3f> newVertex(m_positions.size());
	std::vector<int> dim(m_positions.size(), 0);
	std::vector<float> totalWeight(m_positions.size(), 0);
	std::vector<int> nbTri(m_positions.size(), 0);

	for (auto triangle : m_triangles) {
		Vec3f cent0 = 0.25f * m_positions[triangle[0]]
			+ 0.375f * (m_positions[triangle[1]] + m_positions[triangle[2]]);
		totalWeight[triangle[0]] += M_PI / 3.0f;
		newVertex[triangle[0]] += cent0 * M_PI / 3.0f;
		nbTri[triangle[0]] ++;


		Vec3f cent1 = 0.25f * m_positions[triangle[1]]
			+ 0.375f * (m_positions[triangle[0]] + m_positions[triangle[2]]);
 		totalWeight[triangle[1]] += M_PI / 3.0f;
		newVertex[triangle[1]] += cent1 * M_PI / 3.0f;
		nbTri[triangle[1]] ++;


		Vec3f cent2 = 0.25f * m_positions[triangle[2]]
			+ 0.375f * (m_positions[triangle[1]] + m_positions[triangle[0]]);
		totalWeight[triangle[2]] += M_PI / 3.0f;
		newVertex[triangle[2]] += cent2 * M_PI / 3.0f;
		nbTri[triangle[2]] ++;
	}

	for (unsigned int i = 0; i < m_positions.size(); i++) {
		newVertex[i] /= totalWeight[i];
		newVertex[i] = m_positions[i] + corrFact(nbTri[i]) * (newVertex[i] - m_positions[i]);
	}

	m_positions.clear();
	m_positions = newVertex;
	recomputeNormals();

}

float Mesh::corrFact(int n) {
	if (n == 3) return 1.5f;
	else return 6.0f / n;
}

void Mesh::updateTriangle(std::map<Edge, unsigned int> midPointMap) {
	std::vector<Triangle> newTriangles;
	for (auto triangle : m_triangles) {
		auto pt0 = triangle[0];
		auto pt1 = triangle[1];
		auto pt2 = triangle[2];

		int midPt01 = midPointMap[makeEdge(pt0, pt1)];
		int midPt12 = midPointMap[makeEdge(pt1, pt2)];
		int midPt20 = midPointMap[makeEdge(pt2, pt0)];
		newTriangles.push_back(Triangle(pt0, midPt01, midPt20));
		newTriangles.push_back(Triangle(midPt01, pt1, midPt12));
		newTriangles.push_back(Triangle(midPt20, midPt12, pt2));
		newTriangles.push_back(Triangle(midPt01, midPt12, midPt20));
	}
	m_triangles.clear();
	m_triangles = newTriangles;
	recomputeNormals();

}


std::map<Edge, unsigned int> Mesh::insertMidPoint() {
	std::map<Edge, unsigned int> midPointMap;
	for (auto triangle : m_triangles) {
		for (int i = 0; i < 3; i++) {
			for (int j = i + 1; j < 3; j++) {
				Edge edge = makeEdge(triangle[i], triangle[j]);

				if (midPointMap.count(edge) <= 0 ) {
					Vec3f midPoint = (m_positions[triangle[i]] + m_positions[triangle[j]])*0.5f;
					midPointMap[edge] = m_positions.size();
					m_positions.push_back(midPoint);
				}
			}
		}

	}

	
	return midPointMap;
}

Edge Mesh::makeEdge(int i, int j) {
	Edge edge;
	edge.first = i < j ? i : j;
	edge.second = i >= j ? i : j;
	return edge;
}