//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_super_nailgun.h"
#include "decals.h"
#include "in_buttons.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "nailgun_nail.h"
#endif

// ----------------------------------------------------------------------------- //
// CTFCSuperNailgun tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCSuperNailgun, DT_WeaponSuperNailgun )

BEGIN_NETWORK_TABLE( CTFCSuperNailgun, DT_WeaponSuperNailgun )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCSuperNailgun )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_superng, CTFCSuperNailgun );
PRECACHE_WEAPON_REGISTER( tf_weapon_superng );

#ifndef CLIENT_DLL
BEGIN_DATADESC( CTFCSuperNailgun )
END_DATADESC()
#endif

// ----------------------------------------------------------------------------- //
// CTFCSuperNailgun implementation.
// ----------------------------------------------------------------------------- //
CTFCSuperNailgun::CTFCSuperNailgun()
{
}

void CTFCSuperNailgun::PrimaryAttack()
{
	CHL2MP_Player *pOwner = GetPlayerOwner();
	if ( !pOwner )
		return;

	Assert( HasPrimaryAmmo() );

	// Effects.
	WeaponSound( SINGLE );
	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pOwner->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );

	// Create the nail.
	int iCurrentAmmoCount = pOwner->GetAmmoCount( GetPrimaryAmmoType() );

#ifdef GAME_DLL // TFCTODO: predict this
	Vector vecSrc = pOwner->Weapon_ShootPosition();
	CTFNailgunNail *pNail = NULL;
	if ( iCurrentAmmoCount < 4 )
		 pNail = CTFNailgunNail::CreateNail( false, vecSrc, pOwner->EyeAngles(), pOwner, this, true );
	else
		pNail = CTFNailgunNail::CreateSuperNail( vecSrc, pOwner->EyeAngles(), pOwner, this );
#endif

	// Uses 2 nails if it can
	pOwner->RemoveAmmo( min( 2, iCurrentAmmoCount ), GetPrimaryAmmoType() );
	
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