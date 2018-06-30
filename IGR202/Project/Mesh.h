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
#include "Triangle.h"
#include <memory>
#include <map>

using Edge = std::pair<int, int>;

/// A Mesh class, storing a list of vertices and a list of triangles indexed over it.
class Mesh {
public:
    inline Mesh () {}
    inline virtual ~Mesh () {}

    inline std::vector<Vec3f> & positions () { 
		return m_positions;
	}

    inline const std::vector<Vec3f> & positions () const {
		return m_positions;
	}

    inline  std::vector<Vec3f> & normals () { return m_normals; }
    inline const std::vector<Vec3f> & normals () const { return m_normals; }
    inline std::vector<Triangle> &triangles () { return m_triangles; }
    inline const std::vector<Triangle> & triangles () const { return m_triangles; }

    /// Empty the positions, normals and triangles arrays.
    void clear ();

	/// Loads the mesh from a <file>.off
	void loadOFF (const std::string & filename);
    
	void exportOFF(const std::string & filename);

	/// Compute smooth per-vertex normals
    void recomputeNormals ();

    /// scale to the unit cube and center at original
    void centerAndScaleToUnit ();

	/****************** Filtrage **********************************/

	const std::vector<Vec3f>& getFilterPositions() const;

	/***
	* Filter the mesh by the laplacien topological graph
	*/
	void topoFilter();

	/***
	* Filter the mesh by the filtrage of geometrical laplacian 
	*/
	void geoFilter(float alpha);


	/************** Simplification ***************************/
	//const std::vector<Vec3f>& getSimplifyPositions() const;
	//const std::vector<Vec3f>& getSimplifyNormals() const;
	//const std::vector<Triangle>& getSimplifyTriangles() const;

	/** Simplify the mesh by uniform grill with the resolution
	* specified by arguments.
	* @param resolution the resolution of grill
	**/
	void simplify(unsigned resolition);
	
	/** Simplify the mesh by octree with the max vertex
	* specified by arguments.
	* @param resolution the resolution of grill
	**/
	void simplifyAdaptiveMesh(unsigned int n);

	/****************** Subdivision*******************************/
	/*** 
	* Implementation of subdivision. 
	***/
	void subdivide();
private:
    std::vector<Vec3f> m_positions;
	//std::vector<Vec3f> m_positions_filtrage;
    std::vector<Vec3f> m_normals;
    std::vector<Triangle> m_triangles;
	std::vector< std::vector<int>> m_adjacency_list;
	std::vector< std::vector<Triangle>> m_adjacency_triangles_list;

	//  save the results of filter by laplacian topo
	std::vector<Vec3f> Lt;
	
	//  save the results of filter by laplacian geo
	std::vector<Vec3f> Lg;

	/******************** Filtrage ******************************/
	/** Create the adjacencyMatrix, if the adjacencyMatrix is not
	* empty, this function will clear the old matrix and recreate.
	**/
	void createAdjacencyMatrix();
	void createAdjacencyTrianglesMatrix();
	float voronoiRegionSurface(std::vector<Triangle> t_list, int x);
	float meanCurvatureOperator(int position_index);
	/** Calcule the cot of angle AOB*/
	static double cot(const Vec3f& a, const Vec3f& vec2);


	/******************** Simplification *************************/
	typedef struct {
		int celleIndex = -1;
		int vecIndex = -1;
		Vec3f position;
		Vec3f normal;
		int weight = 0;
	} GrillCelle;

	typedef struct {
		float x_min;
		float x_middle;
		float x_max;
		float y_min;
		float y_middle;
		float y_max;
		float z_min;
		float z_middle;
		float z_max;
	} BoundingBox;



	int resolution;


	// Get Cube bounding box
	static BoundingBox getBoundingCube(const std::vector<Vec3f>& positions);

	/** get reprensentive cells of position specified by argement
	* @param index index of position
	* @return index of grill celle
	*/
	unsigned int getCell(unsigned int index, const BoundingBox& bbox, float celle_lenght, std::vector<GrillCelle>& celleList);


	/************** Simplification with octree  ************************/


	typedef struct OctreeNode {
		BoundingBox bbox;
		int index = -1; 
		std::vector<std::shared_ptr<OctreeNode>> children;
		std::vector<unsigned int> original_index; //data
	} OctreeNode;


	using OctreeNodePtr = std::shared_ptr<OctreeNode>;
	
	int leavesCounter = -1;
	unsigned maxChildrenNbr = 1;

	/** Returns a vector contains all info about bounding Box
	* {x_min, x_mean, x_max, y_min, y_mean, y_max, z_min, z_mean, z_max}
	**/
	BoundingBox getBoundingBoxInfo(const std::vector<unsigned int>& datas);
	
	/*** According the bounding box, set the children's bounding boxes ****/
	void setChildrenBBoxesList(const BoundingBox& bbox, BoundingBox bboxs[]);


	/** Create a octree**/
	OctreeNodePtr buildOctree(const std::vector<unsigned int>& datas, const BoundingBox& bbox);
	
	/**
	* Returns the new index of the position in the octree 
	* @param vertex the coordinates of vertex 
	* @param nodePtr the shared_pointer of the octree node
	* @returns the index of the vertex in the octree
	*/
	static unsigned int findIndexByPosition(const OctreeNodePtr& nodePtr, const Vec3f& vertex);


	/**
	* Test whether the vertex contained in a node specified by argument.
	* @param nodePtr a smart pointer of the node
	* @param vertex a coordinates of a vertex
	* @return true if the nodePtre containing the vertex, false otherwise.
	**/
	static bool containsPtr(const OctreeNodePtr& nodePtr, const Vec3f& vertex);
	
	/**
	* Get a list cotaining all the leaves in the list and passe it to
	* to the vector specified by arguments.
	* @param nodePtr the shared_ptr of octree node
	* @param leaves the list containing all the leaves
	**/
	static void getLeaves(const OctreeNodePtr& nodePtr, std::vector<OctreeNodePtr>& leaves);

	void initNode(OctreeNodePtr nodePtr, const std::vector<unsigned int>& data);
	void dataSpatiaSplite(const std::vector<unsigned int>& datas, std::vector<unsigned int> childrenData[], const BoundingBox& bbox);

	/** Returns ture if the data conformes to stop criteria, otherwise false **/
	inline bool stopCriteria(const std::vector<unsigned int>& data) {
		return data.size() < maxChildrenNbr;  
	}


	/*************************  Subdivision ****************************/
	std::map<Edge, unsigned int>insertMidPoint();
	void updateTriangle(std::map<Edge,unsigned int> midPointMap);
	Edge makeEdge(int i, int j);
	void averagingPass();
	static float corrFact(int n);
};


