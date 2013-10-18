#include "./renderer/Context.h"

int main(void)
{
	tone::renderer::Context* context = new tone::renderer::Context( glm::ivec2(1024, 768) );
	while(true)
	{
		context->swapBuffers();
	}

	return 0;
}