//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_nailgun.h"
#include "decals.h"
#include "in_buttons.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "nailgun_nail.h"
#endif

// ----------------------------------------------------------------------------- //
// CTFCNailgun tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCNailgun, DT_WeaponNailgun )

BEGIN_NETWORK_TABLE( CTFCNailgun, DT_WeaponNailgun )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCNailgun )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_ng, CTFCNailgun );
PRECACHE_WEAPON_REGISTER( tf_weapon_ng );

#ifndef CLIENT_DLL
BEGIN_DATADESC( CTFCNailgun )
END_DATADESC()
#endif

// ----------------------------------------------------------------------------- //
// CTFCNailgun implementation.
// ----------------------------------------------------------------------------- //
CTFCNailgun::CTFCNailgun()
{
}

void CTFCNailgun::PrimaryAttack()
{
	CHL2MP_Player *pOwner = GetPlayerOwner();
	if ( !pOwner )
		return;

	Assert( HasPrimaryAmmo() );

	WeaponSound( SINGLE );
	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pOwner->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );

	// Fire the nail
#ifdef GAME_DLL
	Vector vecSrc = pOwner->Weapon_ShootPosition();
	CTFNailgunNail *pNail = CTFNailgunNail::CreateNail( 
		false, 
		vecSrc, 
		pOwner->EyeAngles(), 
		pOwner, 
		this, 
		true );
	pNail=pNail; // avoid compiler warning..
#endif

	pOwner->RemoveAmmo( 1, GetPrimaryAmmoType() );
	
	// Setup fire delays
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.1;
	m_flTimeWeaponIdle = gpGlobals->curtime + 10;
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
#endif
