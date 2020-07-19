//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_medikit.h"
#include "tfc_gamerules.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
#endif

// ----------------------------------------------------------------------------- //
// CTFCMedikit tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCMedikit, DT_WeaponMedikit )

BEGIN_NETWORK_TABLE( CTFCMedikit, DT_WeaponMedikit )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCMedikit )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_medikit, CTFCMedikit );
PRECACHE_WEAPON_REGISTER( tf_weapon_medikit );

// ----------------------------------------------------------------------------- //
// CTFCMedikit implementation.
// ----------------------------------------------------------------------------- //
CTFCMedikit::CTFCMedikit()
{
}