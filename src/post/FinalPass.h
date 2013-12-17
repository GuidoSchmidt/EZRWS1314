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
};

#endif /* FINALPASS_H_ */
