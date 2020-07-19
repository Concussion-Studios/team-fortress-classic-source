//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_spanner.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
#endif

// ----------------------------------------------------------------------------- //
// CTFCSpanner tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCSpanner, DT_WeaponSpanner )

BEGIN_NETWORK_TABLE( CTFCSpanner, DT_WeaponSpanner )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCSpanner )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_spanner, CTFCSpanner );
PRECACHE_WEAPON_REGISTER( tf_weapon_spanner );

// ----------------------------------------------------------------------------- //
// CTFCSpanner implementation.
// ----------------------------------------------------------------------------- //
CTFCSpanner::CTFCSpanner()
{
}