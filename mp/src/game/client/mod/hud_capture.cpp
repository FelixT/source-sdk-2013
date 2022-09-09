#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "hudelement.h"
#include "hud_numericdisplay.h"
#include <vgui/ISurface.h>
#include "hl2mp_gamerules.h"

#include "tier0/memdbgon.h"

using namespace vgui;

class CHudCapture : public CHudNumericDisplay, public CHudElement
{
	DECLARE_CLASS_SIMPLE(CHudCapture, CHudNumericDisplay);

public:
	CHudCapture(const char *pElementName);
	void Init(void);
	void Update(void);

	void MsgFunc_CapturePercent(bf_read &msg);
protected:
	virtual void OnThink();
	int captureTeam1;
	int captureTeam2;
};

DECLARE_HUDELEMENT(CHudCapture);
DECLARE_HUD_MESSAGE(CHudCapture, CapturePercent);

CHudCapture::CHudCapture(const char *pElementName) : BaseClass(NULL, "HudCapture"), CHudElement(pElementName) {
	SetHiddenBits(HIDEHUD_PLAYERDEAD);
	SetShouldDisplaySecondaryValue(true);
	SetPaintEnabled(true);
}

void CHudCapture::Init(void) {
	captureTeam1 = -1;
	captureTeam2 = -1;

	HOOK_HUD_MESSAGE(CHudCapture, CapturePercent);
}

void CHudCapture::Update(void) {
	// CCaptureFlag *pFlag = dynamic_cast<CCaptureFlag *>(m_hEntity.Get());

	if (HL2MPRules() == nullptr || !HL2MPRules()->IsTeamplay()) {
		SetVisible(false);
		return;
	}
	SetVisible(true);

	SetDisplayValue(captureTeam1);
	SetSecondaryValue(captureTeam2);
}

//-----------------------------------------------------------------------------
// Purpose: called every frame to get capture info from the server
//-----------------------------------------------------------------------------
void CHudCapture::OnThink()
{
	Update();
}


void CHudCapture::MsgFunc_CapturePercent(bf_read &msg) {
	captureTeam1 = msg.ReadShort();
	captureTeam2 = msg.ReadShort();
}