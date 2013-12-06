#include "./utils/Common.h"
#include "./renderer/Context.h"
#include "./renderer/Renderer.h"

int main(void)
{
	renderer::Context* context = new renderer::Context( glm::ivec2(1024, 768) );
    utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/simple_cube.dae");
    renderer::Renderer* main_renderer;
    main_renderer = renderer::Renderer::instance();
    main_renderer->setRenderContext(*context);
    main_renderer->renderloop();

	return 0;
}
