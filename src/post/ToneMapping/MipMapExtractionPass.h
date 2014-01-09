#ifndef MIPMAPEXTRACTIONPASS_H_
#define MIPMAPEXTRACTIONPASS_H_

#include "../ISlimRenderPass.h"
#include "../../utils/Importer.h"

class MipMapExtractionPass : public ISlimRenderPass
{
public:
	MipMapExtractionPass(SlimQuad* pQuad, int pWidth, int pHeight);
	~MipMapExtractionPass();

	void doExecute();

	std::vector<SlimFBO*> fbos;
	int nPasses;

	GLuint inputTexture;
	GLuint outputTexture;
	GLuint inputUniform;

	GLint screenSizeUniform;
	GLint firstPassUniform;


	GLfloat image_average;
	GLfloat image_max;
	GLfloat image_min;
};

#endif /* MIPMAPEXTRACTIONPASS_H_ */
