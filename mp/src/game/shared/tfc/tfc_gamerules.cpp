//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "tfc_gamerules.h"
#include "viewport_panel_names.h"
#include "gameeventdefs.h"
#include <KeyValues.h>
#include "ammodef.h"

#ifdef CLIENT_DLL
	#include "c_tfc_player.h"
#else

	#include "eventqueue.h"
	#include "player.h"
	#include "gamerules.h"
	#include "game.h"
	#include "items.h"
	#include "entitylist.h"
	#include "mapentities.h"
	#include "in_buttons.h"
	#include <ctype.h>
	#include "voice_gamemgr.h"
	#include "iscorer.h"
	#include "tfc_player.h"
	#include "weapon_hl2mpbasehlmpcombatweapon.h"
	#include "team.h"
	#include "voice_gamemgr.h"
	#include "gameinterface.h"
#endif

REGISTER_GAMERULES_CLASS( CTFCGameRules );

BEGIN_NETWORK_TABLE_NOBASE( CTFCGameRules, DT_TFCGameRules )
END_NETWORK_TABLE()

LINK_ENTITY_TO_CLASS( tfc_gamerules, CTFCGameRulesProxy );
IMPLEMENT_NETWORKCLASS_ALIASED( TFCGameRulesProxy, DT_TFCGameRulesProxy )

#ifdef CLIENT_DLL
void RecvProxy_TFCGameRules( const RecvProp *pProp, void **pOut, void *pData, int objectID )
{
	CTFCGameRules *pRules = TFCGameRules();
	Assert( pRules );
	*pOut = pRules;
}

BEGIN_RECV_TABLE( CTFCGameRulesProxy, DT_TFCGameRulesProxy )
	RecvPropDataTable( "tfc_gamerules_data", 0, 0, &REFERENCE_RECV_TABLE( DT_TFCGameRules ), RecvProxy_TFCGameRules )
END_RECV_TABLE()
#else
void* SendProxy_TFCGameRules( const SendProp *pProp, const void *pStructBase, const void *pData, CSendProxyRecipients *pRecipients, int objectID )
{
	CTFCGameRules *pRules = TFCGameRules();
	Assert( pRules );
	return pRules;
}

BEGIN_SEND_TABLE( CTFCGameRulesProxy, DT_TFCGameRulesProxy )
	SendPropDataTable( "tfc_gamerules_data", 0, &REFERENCE_SEND_TABLE( DT_TFCGameRules ), SendProxy_TFCGameRules )
END_SEND_TABLE()
#endif

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTFCGameRules::CTFCGameRules()
{
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTFCGameRules::~CTFCGameRules( void )
{
#ifndef CLIENT_DLL
	// Note, don't delete each team since they are in the gEntList and will 
	// automatically be deleted from there, instead.
	g_Teams.Purge();
#endif
}

// shared ammo definition
// JAY: Trying to make a more physical bullet response
#define BULLET_MASS_GRAINS_TO_LB(grains)	(0.002285*(grains)/16.0f)
#define BULLET_MASS_GRAINS_TO_KG(grains)	lbs2kg(BULLET_MASS_GRAINS_TO_LB(grains))

// exaggerate all of the forces, but use real numbers to keep them consistent
#define BULLET_IMPULSE_EXAGGERATION			3.5
// convert a velocity in ft/sec and a mass in grains to an impulse in kg in/s
#define BULLET_IMPULSE(grains, ftpersec)	((ftpersec)*12*BULLET_MASS_GRAINS_TO_KG(grains)*BULLET_IMPULSE_EXAGGERATION)

CAmmoDef *GetAmmoDef()
{
	static CAmmoDef def;
	static bool bInitted = false;
	
	if ( !bInitted )
	{
		bInitted = true;
		
		// Start at 1 here and skip the dummy ammo type to make CAmmoDef use the same indices
		// as our #defines.
		for ( int i=1; i < AMMO_LAST; i++ )
		{
			def.AddAmmoType( g_aAmmoNames[i], DMG_BULLET, TRACER_LINE, 0, 0, "ammo_max", 2400, 10, 14 );
			Assert( def.Index( g_aAmmoNames[i] ) == i );
		}
	}

	return &def;
}