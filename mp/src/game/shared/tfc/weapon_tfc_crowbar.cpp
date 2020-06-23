//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_crowbar.h"

#if defined( CLIENT_DLL )
	#include "c_tfc_player.h"
#else
	#include "tfc_player.h"
#endif

static ConVar tfc_crowbar_damage_first( "tfc_crowbar_damage_first", "25", 0, "First crowbar hit damage." );
static ConVar tfc_crowbar_damage_next( "tfc_crowbar_damage_next", "12.5", 0, "Crowbar hit damage after first hit." );

// ----------------------------------------------------------------------------- //
// CTFCCrowbar tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCCrowbar, DT_WeaponCrowbar )

BEGIN_NETWORK_TABLE( CTFCCrowbar, DT_WeaponCrowbar )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCCrowbar )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_crowbar, CTFCCrowbar );
PRECACHE_WEAPON_REGISTER( weapon_crowbar );

// ----------------------------------------------------------------------------- //
// CTFCCrowbar implementation.
// ----------------------------------------------------------------------------- //
CTFCCrowbar::CTFCCrowbar()
{
}

#ifdef CLIENT_DLL
	// ------------------------------------------------------------------------------------------------ //
	// ------------------------------------------------------------------------------------------------ //
	// CLIENT DLL SPECIFIC CODE
	// ------------------------------------------------------------------------------------------------ //
	// ------------------------------------------------------------------------------------------------ //
#else
	// ------------------------------------------------------------------------------------------------ //
	// ------------------------------------------------------------------------------------------------ //
	// GAME DLL SPECIFIC CODE
	// ------------------------------------------------------------------------------------------------ //
	// ------------------------------------------------------------------------------------------------ //
	void CTFCCrowbar::AxeHit( CBaseEntity *pHit, bool bFirstSwing, trace_t &tr, float *flDamage, bool *bDoEffects )
	{
		if ( bFirstSwing )
			*flDamage = tfc_crowbar_damage_first.GetFloat();
		else
			*flDamage = tfc_crowbar_damage_next.GetFloat();
	}
#endif
