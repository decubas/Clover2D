#include "Minigame.h"
#include "../PrototypeMain.h"
#include "Card.h"


Minigame::Minigame()
{
	PrototypeEngine::GetEngine()->m_Tick.AddFunction(this, &Minigame::OnUpdate);

}

Minigame::~Minigame()
{
}

void Minigame::OnActivate()
{
	m_bIsActive = true;
}

void Minigame::ResetCooldown()
{
	m_bIsActive = false;
	PrototypeEngine::GetEngine()->m_TimerManager.AddTimer(this, &Minigame::OnActivate, Random::frand(m_MinCooldown, m_MaxCooldown));
}
