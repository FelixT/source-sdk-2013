#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "hudelement.h"
#include "hud_numericdisplay.h"
#include <vgui/ISurface.h>
#include "c_team.h"
#include "hl2mp_gamerules.h"

#include "tier0/memdbgon.h"

using namespace vgui;

class CHudTeamScores : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE(CHudTeamScores, CHudNumericDisplay);

public:
	CHudTeamScores(const char *pElementName);
	void Init(void);
	void Update(void);

protected:
	virtual void OnThink();
	int captureTeam1;
	int captureTeam2;
};

DECLARE_HUDELEMENT(CHudTeamScores);

CHudTeamScores::CHudTeamScores(const char *pElementName) : BaseClass(NULL, "HudTeamScores"), CHudElement(pElementName) {
	SetHiddenBits(HIDEHUD_PLAYERDEAD);
	SetShouldDisplaySecondaryValue(true);
	
	SetPaintEnabled(true);
}

void CHudTeamScores::Init(void) {
	captureTeam1 = -1;
	captureTeam2 = -1;
}

void CHudTeamScores::Update(void) {
	// CCaptureFlag *pFlag = dynamic_cast<CCaptureFlag *>(m_hEntity.Get());
	//C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	//C_Team *team = player->GetTeam();

	if (HL2MPRules() == nullptr || !HL2MPRules()->IsTeamplay()) {
		SetVisible(false);
		return;
	}
	SetVisible(true);
	
	C_Team *team1 = GetGlobalTeam(TEAM_COMBINE);
	C_Team *team2 = GetGlobalTeam(TEAM_REBELS);

	SetDisplayValue(team1->Get_Score());
	SetSecondaryValue(team2->Get_Score());
}

//-----------------------------------------------------------------------------
// Purpose: called every frame to get capture info from the server
//-----------------------------------------------------------------------------
void CHudTeamScores::OnThink()
{
	Update();
}