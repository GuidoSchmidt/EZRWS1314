#ifndef FINALPASS_H_
#define FINALPASS_H_

#include "ISlimRenderPass.h"

class FinalPass : public ISlimRenderPass
{
public:
	FinalPass(SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~FinalPass();

	virtual void doExecute();

	GLint gBuffer1Uniform;
	GLint sunlight1Uniform;
	GLint sunlight2Uniform;
	GLint sunlight3Uniform;
	GLint screenSizeUniform;
};

#endif /* FINALPASS_H_ */
