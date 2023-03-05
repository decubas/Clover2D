#ifndef _LAYER_H_
#define _LAYER_H_ 1

#include <Clover.h>
#include "Graphics/renderer2D.h"

class Layer : public Cl_Object
{
public:
	Layer() = default;
	Layer(std::string name) : name_(name){}

	virtual void OnAttach() {};
	virtual void OnDetach() {};

	virtual void OnUpdate(float delta) {};
	
	virtual void OnRender() {};
	virtual void OnImGuiRender() {};

	CString toString() const { return name_; }
protected:
	CString name_ = "UnNamed Layer";
};

#endif
