#if 0
#include "engine/scene/entity.h"
#include "engine/components/base_components.h"

namespace Coffee
{
	CID& Entity::getCID()
	{
		return getComponent<IDComponent>().ID;
	}

}
#endif