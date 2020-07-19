//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_crowbar.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// CTFCCrowbar Tables
//-----------------------------------------------------------------------------
IMPLEMENT_NETWORKCLASS_ALIASED( TFCCrowbar, DT_TFCCrowbar )

BEGIN_NETWORK_TABLE( CTFCCrowbar, DT_TFCCrowbar )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCCrowbar )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_crowbar, CTFCCrowbar );
PRECACHE_WEAPON_REGISTER( tf_weapon_crowbar );

//-----------------------------------------------------------------------------
// CTFCCrowbar Implementation
//-----------------------------------------------------------------------------
CTFCCrowbar::CTFCCrowbar( void )
{
}
//-----------------------------------------------------------------------------
// Purpose: Add in a view kick for this weapon
//-----------------------------------------------------------------------------
void CTFCCrowbar::AddViewKick( void )
{
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle punchAng;

	punchAng.x = SharedRandomFloat( "crowbarpax", 1.0f, 2.0f );
	punchAng.y = SharedRandomFloat( "crowbarpay", -2.0f, -1.0f );
	punchAng.z = 0.0f;
	
	pPlayer->ViewPunch( punchAng ); 
}