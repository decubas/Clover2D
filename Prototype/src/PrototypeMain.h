#pragma once

#define CL_USE_IMGUI

#include <CloverEngine.h>
#include "imgui.h"
#include "Graphics/UI.h"
#include "Tools/Serializer.h"
#include "Prot_Camera.h"
#include "Entities/Minigame.h"


class PrototypeEngine : public CloverEngine
{

public:
	Ref<Texture2D> ButtonAtlas;
	Ref<Texture2D> ButtonPressedAtlas;
	TexRenderable Floor;
	PrototypeCamera MainCamera = PrototypeCamera(1000, -1, 1);

	std::vector<Minigame*> m_ActiveMinigames;


	void EnableEditor(s32 key);

	void OnUserConstruct() override;
	void OnUserUpdate(float fDeltaTime) override;
	void OnUserDestroy() override;

	void OnUserUI(float fDeltaTime) override;
	void OnUserImGuiUpdate(float fDeltaTime) override;

};
