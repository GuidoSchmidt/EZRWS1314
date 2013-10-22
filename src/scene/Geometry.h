//! @file Geometry.h
#pragma once
#ifndef H_GEOMETRY
#define H_GEOMETRY

#include "../utils/Common.h"
#include "SceneNode.h"
#include <vector>

namespace tone {
	namespace scene {
		class Geometry : public SceneNode
		{
			public:
				Geometry();
				Geometry(int id);
				~Geometry(void);
				Geometry( const Geometry& other );

				void createSinglePoint(void);
				void createSimpleTriangle(void);
				void createSimpleQuad(void);
				void createNormalsGeometry(void);
				void createBoundingBoxGeometry(void);

				nodetype* getType(void);
				void addVertex(GLfloat x, GLfloat y, GLfloat z);
				void addIndex(GLuint index);
				void addNormal(GLfloat x, GLfloat y, GLfloat z);
				void addUV(GLfloat u, GLfloat v);

				void setBoundingBox(glm::vec3 min, glm::vec3 max);

				unsigned int vertexCount(void);
				unsigned int indexCount(void);
				unsigned int normalCount(void);
				unsigned int uvCount(void);
				void createBuffers(void);
				void createNormalsBuffers(void);
				void createBoundingBoxBuffers(void);
				void drawTriangles(void);
				void drawPoints(void);
				void drawWireframe(void);
				void drawNormals(void);
				void drawBoundingBox(void);

				//! Physics
				void applyTransform(tone::scene::Transform& t);
				std::vector<GLfloat>& getVertices();
				void alignNormals();
				void cleanListOfDuplicates();

				glm::vec3 getSizeBB();
				void addFace(std::vector<GLint> indices, glm::vec3 normal, glm::vec3 centroid);
				void addEdge(GLint a, GLint b);

				void buildVerticesVec3();

			private:
				//! Physics
				std::vector<glm::vec3> m_verticesVec3;
				
				glm::vec3 bounding_box_min;
				glm::vec3 bounding_box_max;

				//! Geometry itself
				GLenum m_ErrorCheckMesh;
				unsigned int index_count;
				std::vector<GLfloat> m_vertexlist;
				std::vector<GLuint>  m_indexlist;
				std::vector<GLfloat> m_normallist;
				std::vector<GLfloat> m_uvlist;
				GLuint VAO_id;
				GLuint VBO_id, IBO_id, NBO_id, UVBO_id;
				//! Normals geometry
				bool m_normals_buffers;
				std::vector<GLfloat> m_normals_vertexlist;
				GLuint normals_VAO_id;
				GLuint normals_VBO_id;
				//! Bounding box geometry
				bool m_bb_buffers;
				std::vector<GLfloat> m_bb_vertexlist;
				std::vector<GLuint>  m_bb_indexlist;
				GLuint bb_VAO_id;
				GLuint bb_VBO_id, bb_IBO_id;
		};
	}
}
#endif //H_GEOMETRY
