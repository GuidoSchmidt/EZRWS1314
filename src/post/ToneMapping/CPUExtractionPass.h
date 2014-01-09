#ifndef CPUEXTRACTIONPASS_H_
#define CPUEXTRACTIONPASS_H_

#include "../ISlimRenderPass.h"
#include "../../utils/Importer.h"

class CPUExtractionPass : public ISlimRenderPass
{
public:
	CPUExtractionPass(SlimQuad* pQuad, int pWidth, int pHeight);
	virtual ~CPUExtractionPass();

	virtual void doExecute();


	//GLfloat* imageArray[];
	vector<GLfloat>* imageVector;

	GLfloat image_average;
	GLfloat image_max;
	GLfloat image_min;

	int scale;


};

#endif /* CPUEXTRACTIONPASS_H_ */
