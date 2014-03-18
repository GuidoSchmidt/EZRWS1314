#include "CPUExtractionPass.h"
#include <math.h>
#include <stdlib.h>


CPUExtractionPass::CPUExtractionPass(SlimQuad* pQuad, int pWidth, int pHeight)
: ISlimRenderPass(pQuad)
{
	quad = pQuad;
	width = pWidth;
	height = pHeight;

	imageVector = new vector<float>;
	scale = 4;
	imageVector->resize(width / scale * width / scale * 4);
}


CPUExtractionPass::~CPUExtractionPass() {
	//delete renderPassShader;
}

void CPUExtractionPass::doExecute() {

	

	//blit to half size
	SlimFBO::blit(inputFBOs[0], inputFBOs[1]);


	// copy image to ram
	double time1 = glfwGetTime();

	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, inputFBOs[0]->fboPointer);
	glReadPixels(0, 0, width / scale, height / scale, GL_RGBA, GL_FLOAT, imageVector->data());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	double time2 = glfwGetTime() - time1; //ca 10ms FUUUUUUU

	
	int b;
	float max = 0;
	float min = std::numeric_limits<float>::max();
	float av = 0;
	GLfloat lum;
	//TODO pragma 
	for (b = 0; b<(width / scale)*(height / scale) * 4; b += 4)
	{
		//lum = 0.2126 * (*imageVector)[b] + 0.7152 * (*imageVector)[b + 1] + 0.0722 * (*imageVector)[b + 2];
		lum = ((*imageVector)[b] + (*imageVector)[b + 1] + (*imageVector)[b + 2])/3.0f;
		if (lum>max) 
			max = lum;
		if (lum<min) 
			min = lum;
		//		av+=log(lum+1);
		av += lum;
	}
	av /= (width / scale)*(height / scale);
	minAveMax = glm::vec3(min,av,max);
	double time3 = glfwGetTime() - time2 - time1;    //mit res /4 => 2-3ms  --- mit res /2 = > 9-10ms --- mit res /1 => 35-45ms FUUUUU
	int i = 3;
}

