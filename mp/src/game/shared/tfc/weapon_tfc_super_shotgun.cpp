//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_super_shotgun.h"
#include "decals.h"
#include "in_buttons.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
#endif

// ----------------------------------------------------------------------------- //
// CTFCSuperShotgun tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCSuperShotgun, DT_WeaponSuperShotgun )

BEGIN_NETWORK_TABLE( CTFCSuperShotgun, DT_WeaponSuperShotgun )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCSuperShotgun )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_supershotgun, CTFCSuperShotgun );
PRECACHE_WEAPON_REGISTER( tf_weapon_supershotgun );

#ifndef CLIENT_DLL
	BEGIN_DATADESC( CTFCSuperShotgun )
	END_DATADESC()
#endif

// ----------------------------------------------------------------------------- //
// CTFCSuperShotgun implementation.
// ----------------------------------------------------------------------------- //
CTFCSuperShotgun::CTFCSuperShotgun()
{
	m_iShellsReloaded = 2;
}

void CTFCSuperShotgun::PrimaryAttack()
{
	CHL2MP_Player *pOwner = GetPlayerOwner();
	if ( !pOwner )
		return;

	Assert( Clip1() > 0 );

	// If we've only 1 shell left, fire a single shot
	if ( Clip1() == 1 )
	{
		BaseClass::PrimaryAttack();
		return;
	}

	WeaponSound( SINGLE );
	SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	pOwner->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );

	Vector vecSrc = pOwner->Weapon_ShootPosition();
	Vector vecAiming = ToBasePlayer( pOwner )->GetAutoaimVector( AUTOAIM_5DEGREES );
	
	// Shoot!	
	FireBulletsInfo_t info;
	info.m_vecSrc = vecSrc;
	info.m_vecDirShooting = vecAiming;
	info.m_iShots = 14;
	info.m_flDistance = 2048;
	info.m_iAmmoType = GetPrimaryAmmoType();
	info.m_vecSpread = VECTOR_CONE_TF_SHOTGUN;
	info.m_iTracerFreq = 4;
	info.m_flDamage = 4;
	pOwner->FireBullets( info );

	m_iClip1 -= 2;
	m_flTimeWeaponIdle = gpGlobals->curtime + 5.0;
	m_fInSpecialReload = 0;

	// Setup fire delays
	if ( Clip1() != 0 )
		m_flPumpTime = gpGlobals->curtime + 0.7;
	
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.7;
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
