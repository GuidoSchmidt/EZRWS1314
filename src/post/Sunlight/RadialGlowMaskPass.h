#ifndef RADIAL_GLOW_H_
#define RADIAL_GLOW_H_

#include "../ISlimRenderPass.h"

class RadialGlowMaskPass : public ISlimRenderPass
{
public:
	RadialGlowMaskPass(SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~RadialGlowMaskPass();

	virtual void doExecute();

	GLint blurUniform;
	GLint maskUniform;

	GLint screenSizeUniform;
};

#endif /* RADIAL_GLOW_H_ */
