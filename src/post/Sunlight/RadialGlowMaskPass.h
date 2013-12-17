#ifndef RADIAL_GLOW_H_
#define RADIAL_GLOW_H_

#include "../ISlimRenderPass.h"
#include "../../utils/Importer.h"

class RadialGlowMaskPass : public ISlimRenderPass
{
public:
	RadialGlowMaskPass(SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~RadialGlowMaskPass();

	virtual void doExecute();

	GLint blurUniform;
	GLint maskUniform;

	GLuint maskTexture;

	GLuint ssSunPosUniform; 
	GLint screenSizeUniform;

	glm::vec4 param_ssSunPos;
};

#endif /* RADIAL_GLOW_H_ */
