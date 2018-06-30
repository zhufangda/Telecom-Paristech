//#include "Octree.h"
//#include "AxisAlignedBoundingBox.h"
//
//OctreeNodePtr OctreeNode::createOctree(unsigned int n, std::vector<Vec3f>& m_positions)
//{
//	
//
//
//AxisAlignedBoundingBox OctreeNode::getBoundingBoxInfo(const std::vector<unsigned int>& datas)
//{
//	BoundingBox bbox;
//
//	if (datas.size() <= 0) {
//		throw std::invalid_argument("Datas size can not be 0!");
//	}
//
//	bbox.x_max = m_positions[datas[0]][0];
//	bbox.x_min = m_positions[datas[0]][0];
//	bbox.y_max = m_positions[datas[0]][1];
//	bbox.y_min = m_positions[datas[0]][1];
//	bbox.z_max = m_positions[datas[0]][2];
//	bbox.z_min = m_positions[datas[0]][2];
//
//	for (const auto data : datas) {
//		bbox.x_min = bbox.x_min > m_positions[data][0] ? m_positions[data][0] : bbox.x_min;
//		bbox.x_max = bbox.x_max < m_positions[data][0] ? m_positions[data][0] : bbox.x_max;
//		bbox.y_min = bbox.y_min > m_positions[data][1] ? m_positions[data][1] : bbox.y_min;
//		bbox.y_max = bbox.y_max < m_positions[data][1] ? m_positions[data][1] : bbox.y_max;
//		bbox.z_min = bbox.z_min > m_positions[data][2] ? m_positions[data][2] : bbox.z_min;
//		bbox.z_max = bbox.z_max < m_positions[data][2] ? m_positions[data][2] : bbox.z_max;
//	}
//
//	bbox.x_min -= (bbox.x_max - bbox.x_min) / 1000.0f;
//	bbox.x_max += (bbox.x_max - bbox.x_min) / 1001.0f;
//	bbox.y_min -= (bbox.y_max - bbox.y_min) / 1000.0f;
//	bbox.y_max += (bbox.y_max - bbox.y_min) / 1001.0f;
//	bbox.z_min -= (bbox.z_max - bbox.z_min) / 1000.0f;
//	bbox.z_max += (bbox.z_max - bbox.z_min) / 1001.0f;
//
//	bbox.x_middle = (bbox.x_max + bbox.x_min) / 2.0f;
//	bbox.y_middle = (bbox.y_max + bbox.y_min) / 2.0f;
//	bbox.z_middle = (bbox.z_max + bbox.z_min) / 2.0f;
//
//	return bbox;
//}
//
//OctreeNodePtr OctreeNode::buildOctree(const std::vector<unsigned int>& datas, const BoundingBox & bbox)
//{
//	return OctreeNodePtr();
//}
