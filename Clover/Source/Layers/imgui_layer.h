#ifdef IMGUI_LAYER_H_
#define IMGUI_LAYER_H_ 1

#include <Clover.h>
#include "layer.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();


	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float delta) override;
	virtual void OnImGuiRender() override;

	void Begin();
	void End();
	void SetBlockEvents(bool block) { m_BlockEvents = block; }
	
	void SetDarkThemeColors();
protected:
	bool m_BlockEvents = true;
};

#endif
