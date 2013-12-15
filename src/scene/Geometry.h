//! @file Geometry.h
#pragma once
#ifndef H_GEOMETRY
#define H_GEOMETRY

#include "../utils/Common.h"
#include "SceneNode.h"
#include <vector>

namespace scene {
	//! @class Geometry
	class Geometry : public SceneNode
	{
		private:
			//! Geometry itself
			GLenum m_ErrorCheckMesh;
			unsigned int index_count;
			std::vector<GLfloat> m_vertexlist;
			std::vector<GLuint>  m_indexlist;
			std::vector<GLfloat> m_normallist;
			std::vector<GLfloat> m_uvlist;
			GLuint VAO_handle;
			GLuint VBO_handle, IBO_handle, NBO_handle, UVBO_handle;
			
			//! Normals geometry
			//! \todo old - rework
			bool m_normals_buffers;
			std::vector<GLfloat> m_normals_vertexlist;
			GLuint normals_VAO_id;
			GLuint normals_VBO_id;

			//! Bounding box geometry
			//! \todo old - rework
			bool m_bb_buffers;
			std::vector<GLfloat> m_bb_vertexlist;
			std::vector<GLuint>  m_bb_indexlist;
			GLuint bb_VAO_id;
			GLuint bb_VBO_id, bb_IBO_id;
			
			//! \brief Constructor: default
			Geometry();

		public:

			//! \brief Constructor
			Geometry(int id, std::string name);
			
			//! \brief Destructor
			~Geometry(void);
			
			//! \brief Creates a single point
			void createSinglePoint(void);

			//! \brief Creates a single triangle
			void createSimpleTriangle(void);

			//! \brief Creates a single quad
			void createSimpleQuad(void);

			//! \brief Creates normals
			void createNormalsGeometry(void);
			
			//! \brief Creates renderable bounding box geometry
			void createBoundingBoxGeometry(void);

			//! \brief Returns the node type
			nodetype getType(void);

			//! \brief Adds a vertex
			void addVertex(GLfloat x, GLfloat y, GLfloat z);

			//! \brief Adds an index point
			void addIndex(GLuint index);

			//! \brief Adds a normal
			void addNormal(GLfloat x, GLfloat y, GLfloat z);
			
			//! \brief Adds a uv-/texture coordinate
			void addUV(GLfloat u, GLfloat v);

			//! \brief Sets the bounding box
			void setBoundingBox(glm::vec3 min, glm::vec3 max);

			//! \brief Returns the count of vertices
			unsigned int vertexCount(void);

			//! \brief Returns the count of index points
			unsigned int indexCount(void);

			//! \brief Returns the count of normals
			unsigned int normalCount(void);

			//! \brief Returns the count of uv-/texture-coordinates
			unsigned int uvCount(void);

			//! \brief Creates the needed vertex buffers
			void createBuffers(void);

			//! \brief Creates the vertex buffers for the normals
			void createNormalsBuffers(void);

			//! \brief Creates the vertex buffers for the bounding box
			void createBoundingBoxBuffers(void);
			
			//! \brief Draws the geometry using triangles
			void drawTriangles(void);

			//! \brief Draws the geometry using points
			void drawPoints(void);

			//! \brief Draws the geomety as wireframe
			void drawWireframe(void);

			//! \brief Draws the normals
			void drawNormals(void);

			//! \brief Draws the boundign box
			void drawBoundingBox(void);

			//! Physics
            void applyTransform(scene::Transform& t);
			std::vector<GLfloat>& getVertices();
			void alignNormals();
			void cleanListOfDuplicates();

			glm::vec3 getSizeBB();
			void addFace(std::vector<GLint> indices, glm::vec3 normal, glm::vec3 centroid);
			void addEdge(GLint a, GLint b);

			void buildVerticesVec3();
	};
}
#endif //H_GEOMETRY
