//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_umbrella.h"

#if defined( CLIENT_DLL )
	#include "c_tfc_player.h"
#else
	#include "tfc_player.h"
#endif

// ----------------------------------------------------------------------------- //
// CTFCUmbrella tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCUmbrella, DT_WeaponUmbrella )

BEGIN_NETWORK_TABLE( CTFCUmbrella, DT_WeaponUmbrella )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCUmbrella )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_umbrella, CTFCUmbrella );
PRECACHE_WEAPON_REGISTER( tf_weapon_umbrella );

// ----------------------------------------------------------------------------- //
// CTFCUmbrella implementation.
// ----------------------------------------------------------------------------- //
CTFCUmbrella::CTFCUmbrella()
{
}