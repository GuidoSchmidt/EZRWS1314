//! @file Geometry.cpp
#include "Geometry.h"

namespace scene {

	Geometry::Geometry()
	{
		Geometry(-1);
	}

	Geometry::Geometry(int id)
	{
		m_id = id;
		m_type	= GEOMETRY;
		VAO_id = 0;
		VBO_id = 0;
		IBO_id = 0;
		NBO_id = 0;
		UVBO_id = 0;

		normals_VAO_id = 0;
		normals_VBO_id = 0;

		m_ErrorCheckMesh = 0;
	}

	Geometry::Geometry( const Geometry& other )
	{
		m_id = other.m_id+1;
		VAO_id = other.VAO_id;
		VBO_id = other.VBO_id;;
		IBO_id = other.IBO_id;;
		NBO_id = other.NBO_id;;
		UVBO_id = other.UVBO_id;;

		normals_VAO_id = other.normals_VAO_id;
		normals_VBO_id = normals_VBO_id;

		m_ErrorCheckMesh = 0;

		m_verticesVec3 = other.m_verticesVec3;

		bounding_box_min = other.bounding_box_min;
		bounding_box_max = other.bounding_box_max;

		index_count = other.index_count;
		m_vertexlist = other.m_vertexlist;
		m_indexlist = other.m_indexlist;
		m_normallist = other.m_normallist;
		m_uvlist = other.m_uvlist;

		m_normals_buffers = other.m_normals_buffers;
		m_normals_vertexlist = other.m_normals_vertexlist;
		m_bb_buffers = other.m_bb_buffers;
		m_bb_vertexlist = other.m_bb_vertexlist;
		m_bb_indexlist = other.m_bb_indexlist;
		bb_VAO_id = other.bb_VAO_id;
		bb_VBO_id = other.bb_VBO_id;

		createBuffers();
	}

	Geometry::~Geometry(void)
	{
	}

	void Geometry::createSinglePoint(void)
	{
		addVertex(0.0, 0.0, 0.0);
		addIndex(0);
		addNormal(0.0, 1.0, 0.0);
		addUV(0.0, 0.0);

		createBuffers();
	}

	void Geometry::createSimpleTriangle(void)
	{
		//! Vertices
		addVertex(-0.5, -0.5, 0);
		addVertex( 0.5, -0.5, 0);
		addVertex( 0.0,  0.5, 0);
		//! Indices
		addIndex(0);
		addIndex(1);
		addIndex(2);
		//! Normal
		addNormal(0.0, 0.0, 1.0);
		//! UVs
		addUV( 0.0, 0.0);
		addUV( 1.0, 0.0);
		addUV( 0.5, 1.0);

		createBuffers();
	}

	void Geometry::createSimpleQuad(void)
	{
		//! Vertices
		addVertex(-0.5, -0.5, 0);
		addVertex( 0.5, -0.5, 0);
		addVertex( 0.5,  0.5, 0);
		addVertex(-0.5,  0.5, 0);
		//! Indices
		addIndex(0);
		addIndex(1);
		addIndex(2);
		addIndex(2);
		addIndex(0);
		addIndex(3);
		//! Normal
		addNormal(0.0, 0.0, 1.0);
		//! UVs
		addUV(0.0, 0.0);
		addUV(1.0, 0.0);
		addUV(1.0, 1.0);
		addUV(0.0, 1.0);

		createBuffers();
	}

	void Geometry::setBoundingBox(glm::vec3 min, glm::vec3 max)
	{
		bounding_box_min = min;
		bounding_box_max = max;
	}

	void Geometry::createNormalsGeometry(void)
	{
		/*
		for(unsigned int face = 0; face < m_faces.size(); face++)
		{
			m_normals_vertexlist.push_back(m_faces.at(face).centroid.x);
			m_normals_vertexlist.push_back(m_faces.at(face).centroid.y);
			m_normals_vertexlist.push_back(m_faces.at(face).centroid.z);

			m_normals_vertexlist.push_back(m_faces.at(face).centroid.x + m_faces.at(face).normal.x);
			m_normals_vertexlist.push_back(m_faces.at(face).centroid.y + m_faces.at(face).normal.y);
			m_normals_vertexlist.push_back(m_faces.at(face).centroid.z + m_faces.at(face).normal.z);
		}
		*/

		createNormalsBuffers();
	}

	void Geometry::createBoundingBoxGeometry(void)
	{
		std::vector<glm::vec3> points; //= getVerticesVec3();
		if(points.empty())
			return;
		std::vector<glm::vec3>::iterator it = points.begin();
		bounding_box_min.x = (*it).x;
		bounding_box_max.x = (*it).x;
		bounding_box_min.y = (*it).y;
		bounding_box_max.y = (*it).y;
		bounding_box_min.z = (*it).z;
		bounding_box_max.z = (*it).z;
		for(0; it < points.end(); ++it){
			if((*it).x < bounding_box_min.x)
				bounding_box_min.x=(*it).x;
			if((*it).x > bounding_box_max.x)
				bounding_box_max.x=(*it).x;

			if((*it).y < bounding_box_min.y)
				bounding_box_min.y=(*it).y;
			if((*it).y > bounding_box_max.y)
				bounding_box_max.y=(*it).y;

			if((*it).z < bounding_box_min.z)
				bounding_box_min.z=(*it).z;
			if((*it).z > bounding_box_max.z)
				bounding_box_max.z=(*it).z;
		}

		//! Vertices
		//! Bottom
		m_bb_vertexlist.push_back(bounding_box_min.x);
		m_bb_vertexlist.push_back(bounding_box_min.y);
		m_bb_vertexlist.push_back(bounding_box_min.z);

		m_bb_vertexlist.push_back(bounding_box_max.x);
		m_bb_vertexlist.push_back(bounding_box_min.y);
		m_bb_vertexlist.push_back(bounding_box_min.z);

		m_bb_vertexlist.push_back(bounding_box_max.x);
		m_bb_vertexlist.push_back(bounding_box_min.y);
		m_bb_vertexlist.push_back(bounding_box_max.z);

		m_bb_vertexlist.push_back(bounding_box_min.x);
		m_bb_vertexlist.push_back(bounding_box_min.y);
		m_bb_vertexlist.push_back(bounding_box_max.z);
		//! Top
		m_bb_vertexlist.push_back(bounding_box_min.x);
		m_bb_vertexlist.push_back(bounding_box_max.y);
		m_bb_vertexlist.push_back(bounding_box_min.z);

		m_bb_vertexlist.push_back(bounding_box_max.x);
		m_bb_vertexlist.push_back(bounding_box_max.y);
		m_bb_vertexlist.push_back(bounding_box_min.z);

		m_bb_vertexlist.push_back(bounding_box_max.x);
		m_bb_vertexlist.push_back(bounding_box_max.y);
		m_bb_vertexlist.push_back(bounding_box_max.z);

		m_bb_vertexlist.push_back(bounding_box_min.x);
		m_bb_vertexlist.push_back(bounding_box_max.y);
		m_bb_vertexlist.push_back(bounding_box_max.z);

		//! Indices
		m_bb_indexlist.push_back(0);
		m_bb_indexlist.push_back(1);
		m_bb_indexlist.push_back(1);
		m_bb_indexlist.push_back(2);
		m_bb_indexlist.push_back(2);
		m_bb_indexlist.push_back(3);
		m_bb_indexlist.push_back(3);
		m_bb_indexlist.push_back(0);

		m_bb_indexlist.push_back(4);
		m_bb_indexlist.push_back(5);
		m_bb_indexlist.push_back(5);
		m_bb_indexlist.push_back(6);
		m_bb_indexlist.push_back(6);
		m_bb_indexlist.push_back(7);
		m_bb_indexlist.push_back(7);
		m_bb_indexlist.push_back(4);

		m_bb_indexlist.push_back(0);
		m_bb_indexlist.push_back(4);
		m_bb_indexlist.push_back(1);
		m_bb_indexlist.push_back(5);
		m_bb_indexlist.push_back(2);
		m_bb_indexlist.push_back(6);
		m_bb_indexlist.push_back(3);
		m_bb_indexlist.push_back(7);
			
		//! Create buffers
		createBoundingBoxBuffers();
	}

	nodetype* Geometry::getType(void)
	{
		return &m_type;
	}

	void Geometry::addVertex(GLfloat x, GLfloat y, GLfloat z)
	{
		m_vertexlist.push_back(x);
		m_vertexlist.push_back(y);
		m_vertexlist.push_back(z);
	}
		
	void Geometry::addIndex(GLuint index)
	{
		m_indexlist.push_back(index);
	}
		
	void Geometry::addNormal(GLfloat x, GLfloat y, GLfloat z)
	{
		m_normallist.push_back(x);
		m_normallist.push_back(y);
		m_normallist.push_back(z);
	}				

	void Geometry::addUV(GLfloat u, GLfloat v)
	{
		m_uvlist.push_back(u);
		m_uvlist.push_back(v);
	}

	unsigned int Geometry::vertexCount(void)
	{
		return m_vertexlist.size();
	}

	unsigned int Geometry::indexCount(void)
	{
		return m_indexlist.size();
	}

	unsigned int Geometry::normalCount(void)
	{
		return m_normallist.size();
	}

	unsigned int Geometry::uvCount(void)
	{
		return m_uvlist.size();
	}

	void Geometry::createBuffers(void)
	{
		m_ErrorCheckMesh = glGetError();

		//! Create vertex array object
		glGenVertexArrays(1, &VAO_id);
		glBindVertexArray(VAO_id);

		//! Create vertex buffer object
		glGenBuffers(1, &VBO_id);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_id);
		glBufferData(GL_ARRAY_BUFFER, m_vertexlist.size() * sizeof(GLfloat), &m_vertexlist.at(0), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//! Create index buffer
		glGenBuffers(1, &IBO_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexlist.size() * sizeof(GLint), &m_indexlist.at(0), GL_STATIC_DRAW);
		//! Create normal buffer
		glGenBuffers(1, &NBO_id);
		glBindBuffer(GL_ARRAY_BUFFER, NBO_id);
		glBufferData(GL_ARRAY_BUFFER, m_normallist.size() * sizeof(GLfloat), &m_normallist.at(0), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(1);
		if(!m_uvlist.empty())
		{
			//! Create uv buffer
			glGenBuffers(1, &UVBO_id);
			glBindBuffer(GL_ARRAY_BUFFER, UVBO_id);
			glBufferData(GL_ARRAY_BUFFER, m_uvlist.size() * sizeof(GLfloat), &m_uvlist.at(0), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
			glEnableVertexAttribArray(2);
		}

		m_ErrorCheckMesh = glGetError();
		if(m_ErrorCheckMesh != GL_NO_ERROR)
		{
			/*
			utils::log() << "\nERROR (Geometry: ";
			utils::log() << m_name;
			utils::log() << "): Buffers could not be created!\n";
			*/
		}
	}

	void Geometry::createBoundingBoxBuffers(void)
	{
		m_ErrorCheckMesh = glGetError();

		//! Create vertex array object
		glGenVertexArrays(1, &bb_VAO_id);
		glBindVertexArray(bb_VAO_id);

		//! Create vertex buffer object
		glGenBuffers(1, &bb_VBO_id);
		glBindBuffer(GL_ARRAY_BUFFER, bb_VBO_id);
		glBufferData(GL_ARRAY_BUFFER, m_bb_vertexlist.size() * sizeof(GLfloat), &m_bb_vertexlist.at(0), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//! Create index buffer
		glGenBuffers(1, &bb_IBO_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bb_IBO_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bb_indexlist.size() * sizeof(GLint), &m_bb_indexlist.at(0), GL_STATIC_DRAW);
			
		m_ErrorCheckMesh = glGetError();
		if(m_ErrorCheckMesh != GL_NO_ERROR)
		{
			/*
			utils::log() << "\nERROR (Geometry: ";
			utils::log() << m_name;
			utils::log() << "): Buffers could not be created!\n";
			*/
		}
	}

	void Geometry::createNormalsBuffers(void)
	{
		m_ErrorCheckMesh = glGetError();

		//! Create vertex array object
		glGenVertexArrays(1, &normals_VAO_id);
		glBindVertexArray(normals_VAO_id);

		//! Create vertex buffer object
		glGenBuffers(1, &normals_VBO_id);
		glBindBuffer(GL_ARRAY_BUFFER, normals_VBO_id);
		glBufferData(GL_ARRAY_BUFFER, m_normals_vertexlist.size() * sizeof(GLfloat), &m_normals_vertexlist.at(0), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);
			
		m_ErrorCheckMesh = glGetError();
		if(m_ErrorCheckMesh != GL_NO_ERROR)
		{
			/*
			utils::log() << "\nERROR (Geometry (Normals): ";
			utils::log() << m_name;
			utils::log() << "): Buffers could not be created!\n";
			*/
		}
	}

	void Geometry::drawNormals(void)
	{
		glLineWidth(2.0);
		glPointSize(5.0);
		glBindVertexArray(normals_VAO_id);
		glVertexPointer(0, GL_FLOAT, 0, &m_normals_vertexlist.at(0));
		glDrawArrays(GL_LINES, 0, 36);
		//glDrawElements(GL_LINES, m_normals_indexlist.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Geometry::drawBoundingBox(void)
	{
		glLineWidth(2.0);
		glBindVertexArray(bb_VAO_id);
		glDrawElements(GL_LINES, m_bb_indexlist.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Geometry::drawTriangles(void)
	{
		glBindVertexArray(VAO_id);
		glDrawElements(GL_TRIANGLES, m_indexlist.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Geometry::drawWireframe(void)
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glBindVertexArray(VAO_id);
		glDrawElements(GL_TRIANGLES, m_indexlist.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Geometry::drawPoints(void)
	{
		glBindVertexArray(VAO_id);
		glDrawElements(GL_POINTS, m_indexlist.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

    void Geometry::applyTransform(scene::Transform& t)
	{
		m_transform.setPosition(t.getTranslation());
		m_transform.setRotation(t.getRotation());
	}

	std::vector<GLfloat>& Geometry::getVertices()
	{
		return m_vertexlist;
	}

	glm::vec3 Geometry::getSizeBB()
	{
		return glm::vec3(0);
	}
}
