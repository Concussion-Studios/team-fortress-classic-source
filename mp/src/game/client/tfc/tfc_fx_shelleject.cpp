//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "fx_impact.h"
#include "tempent.h"
#include "c_te_effect_dispatch.h"
#include "c_te_legacytempents.h"

void CS_EjectBrass( int shell, const CEffectData &data )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	tempents->CSEjectBrass( data.m_vOrigin, data.m_vAngles, data.m_fFlags, shell, pPlayer );
}

void FX_CS_EjectBrass_9mm_Callback( const CEffectData &data ) { CS_EjectBrass( CS_SHELL_9MM, data ); }
void FX_CS_EjectBrass_57_Callback( const CEffectData &data ) { CS_EjectBrass( CS_SHELL_57, data ); }
void FX_CS_EjectBrass_12gauge_Callback( const CEffectData &data ) { CS_EjectBrass( CS_SHELL_12GAUGE, data ); }
void FX_CS_EjectBrass_556_Callback( const CEffectData &data ) { CS_EjectBrass( CS_SHELL_556, data ); }
void FX_CS_EjectBrass_762nato_Callback( const CEffectData &data ) { CS_EjectBrass( CS_SHELL_762NATO, data ); }
void FX_CS_EjectBrass_338mag_Callback( const CEffectData &data ) { CS_EjectBrass( CS_SHELL_338MAG, data ); }

DECLARE_CLIENT_EFFECT( "EjectBrass_9mm", FX_CS_EjectBrass_9mm_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_57", FX_CS_EjectBrass_57_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_12gauge", FX_CS_EjectBrass_12gauge_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_556", FX_CS_EjectBrass_556_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_762nato", FX_CS_EjectBrass_762nato_Callback );
DECLARE_CLIENT_EFFECT( "EjectBrass_338mag", FX_CS_EjectBrass_338mag_Callback );