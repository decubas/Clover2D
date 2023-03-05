
#include <Layers/layer_stack.h>

LayerStack::LayerStack()
{

}

LayerStack::~LayerStack()
{

}

void LayerStack::pushLayer(Ref<Layer> layer)
{
	layers.emplace(layers.begin() + layerInsert_, layer);
	layerInsert_++;
	layer->OnAttach();
}

void LayerStack::pushOverlay(Ref<Layer> overlay)
{
	layers.emplace_back(overlay);
}

void LayerStack::popLayer(Ref<Layer> layer)
{
	auto it = std::find(layers.begin(), layers.end(), layer);

	if (it != layers.end())
	{
		layer->OnDetach();
		layers.erase(it);
		layerInsert_--;
	}
}

void LayerStack::popOverlay(Ref<Layer> overlay)
{
	auto it = std::find(layers.begin(), layers.end(), overlay);

	if (it != layers.end())
	{
		layers.erase(it);
	}
}

void LayerStack::render()
{
	for (Ref<Layer>& layer : layers)
	{
		layer->OnRender();
	}
}

CVector<Ref<Layer>>::iterator LayerStack::begin()
{ 
	return layers.begin(); 
}

CVector<Ref<Layer>>::iterator LayerStack::end()
{ 
	return layers.end(); 
}
