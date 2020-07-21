//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
//
// Health.cpp
//
// implementation of CHudHealth class
//
#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "view.h"
#include <KeyValues.h>
#include "iclientmode.h"
#include "hudelement.h"
#include "hud_numericdisplay.h"
#include "convar.h"
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/AnimationController.h>

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define INIT_HEALTH -1

//-----------------------------------------------------------------------------
// Purpose: Health panel
//-----------------------------------------------------------------------------
class CHudHealth : public CHudElement, public CHudNumericDisplay
{
	DECLARE_CLASS_SIMPLE( CHudHealth, CHudNumericDisplay );

public:
	CHudHealth( const char *pElementName );
	virtual void Init( void );
	virtual void VidInit( void );
	virtual void Reset( void );
	virtual void OnThink();
			void MsgFunc_Damage( bf_read &msg );

	virtual void Paint( void );
	virtual void PaintBackground( void );
	virtual void ApplySchemeSettings( IScheme *scheme );

private:
	// old variables
	int		m_iHealth;
	int		m_bitsDamage;
	CHudTexture *m_pHealthIcon;	

	CPanelAnimationVarAliasType( float, icon_xpos, "icon_xpos", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, icon_ypos, "icon_ypos", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, icon_tall, "icon_tall", "64", "proportional_float" );
	CPanelAnimationVarAliasType( float, icon_wide, "icon_wide", "64", "proportional_float" );
};	

DECLARE_HUDELEMENT( CHudHealth );
DECLARE_HUD_MESSAGE( CHudHealth, Damage );

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudHealth::CHudHealth( const char *pElementName ) : CHudElement( pElementName ), CHudNumericDisplay(NULL, "HudHealth")
{
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );

	SetPaintBackgroundType( 0 );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::Init()
{
	HOOK_HUD_MESSAGE( CHudHealth, Damage );
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::Reset()
{
	m_iHealth		= INIT_HEALTH;
	m_bitsDamage	= 0;

	SetDisplayValue(m_iHealth);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::VidInit()
{
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::ApplySchemeSettings( IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );

	if( !m_pHealthIcon )
		m_pHealthIcon = gHUD.GetIcon( "health_icon" );

	if( m_pHealthIcon )
	{

		icon_tall = GetTall() - YRES(2);
		float scale = icon_tall / (float)m_pHealthIcon->Height();
		icon_wide = ( scale ) * (float)m_pHealthIcon->Width();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::Paint( void )
{
	if( m_pHealthIcon )
		m_pHealthIcon->DrawSelf( icon_xpos, icon_ypos, icon_wide, icon_tall, GetFgColor() );

	//draw the health icon
	BaseClass::Paint();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::PaintBackground()
{ 
	int wide, tall;
	GetSize( wide, tall );

	//Color col = GetBgColor();

	DrawBoxFade( 0, 0, wide, tall, Color( 0, 0, 0, 200 ), 1.0f, 255, 0, true );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::OnThink()
{
	int newHealth = 0;
	C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();
	if ( local )
	{
		// Never below zero
		newHealth = MAX( local->GetHealth(), 0 );
	}

	// Only update the fade if we've changed health
	if ( newHealth == m_iHealth )
	{
		return;
	}

	m_iHealth = newHealth;

	if ( m_iHealth >= 20 )
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthIncreasedAbove20");
	}
	else if ( m_iHealth > 0 )
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthIncreasedBelow20");
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthLow");
	}

	SetDisplayValue(m_iHealth);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudHealth::MsgFunc_Damage( bf_read &msg )
{

	int armor = msg.ReadByte();	// armor
	int damageTaken = msg.ReadByte();	// health
	long bitsDamage = msg.ReadLong(); // damage bits
	bitsDamage; // variable still sent but not used

	Vector vecFrom;

	vecFrom.x = msg.ReadBitCoord();
	vecFrom.y = msg.ReadBitCoord();
	vecFrom.z = msg.ReadBitCoord();

	// Actually took damage?
	if ( damageTaken > 0 || armor > 0 )
	{
		if ( damageTaken > 0 )
		{
			// start the animation
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthDamageTaken");
		}
	}
}