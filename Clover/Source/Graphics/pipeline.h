#pragma once

#include <Clover.h>

#include "shader.h"

	class Pipeline : public Cl_Object
	{
	public:
		Pipeline(Ref<Shader>& shader)
			: m_Shader(shader) {}

	protected:
		Ref<Shader> m_Shader;

	};

