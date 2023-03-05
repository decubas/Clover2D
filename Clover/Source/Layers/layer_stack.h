#ifndef _LAYER_STACK_H_
#define _LAYER_STACK_H_ 1

#include <Clover.h>

#include <Layers/layer.h>

// Layer Management
class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void pushLayer(Ref<Layer> layer);
	void pushOverlay(Ref<Layer> overlay);

	void popLayer(Ref<Layer> layer);
	void popOverlay(Ref<Layer> overlay);

	void render();

	CVector<Ref<Layer>>::iterator begin();
	CVector<Ref<Layer>>::iterator end();

protected:
	std::vector<Ref<Layer>> layers;
	u32 layerInsert_ = 0;
};

#endif
