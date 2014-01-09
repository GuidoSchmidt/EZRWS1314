#ifndef FINALPASS_H_
#define FINALPASS_H_

#include "ISlimRenderPass.h"

class FinalPass : public ISlimRenderPass
{
public:
	FinalPass(SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~FinalPass();

	virtual void doExecute();

	GLint topLeftUniform;
	GLint topRightUniform;
	GLint lowerLeftUniform;
	GLint lowerRightUniform;
	GLint screenSizeUniform;
	GLint minAveMaxTextureUniform;

	GLint fastExtractionUniform;

	GLint minAveMaxUniform;

	GLint minAveMaxTexture;
	float param_fastExtraction;
	glm::vec3 param_minAveMax;
};

#endif /* FINALPASS_H_ */
