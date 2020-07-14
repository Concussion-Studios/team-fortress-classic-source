//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_minigun.h"
#include "decals.h"
#include "in_buttons.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
#endif

// ----------------------------------------------------------------------------- //
// CTFCMinigun tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCMinigun, DT_WeaponMinigun )

BEGIN_NETWORK_TABLE( CTFCMinigun, DT_WeaponMinigun )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCMinigun )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_ac, CTFCMinigun );
PRECACHE_WEAPON_REGISTER( tf_weapon_ac );

#ifndef CLIENT_DLL

	BEGIN_DATADESC( CTFCMinigun )
	END_DATADESC()

#endif

// ----------------------------------------------------------------------------- //
// CTFCMinigun implementation.
// ----------------------------------------------------------------------------- //

CTFCMinigun::CTFCMinigun()
{
	m_iWeaponState = AC_STATE_IDLE;
}

void CTFCMinigun::Precache()
{
	BaseClass::Precache();

	PrecacheScriptSound( "Weapon_Minigun.Deploy" );
}

bool CTFCMinigun::Deploy()
{
	CPASAttenuationFilter filter( this );
	filter.UsePredictionRules();
	EmitSound( filter, entindex(), "Weapon_Minigun.Deploy" );

	return BaseClass::Deploy();
}

void CTFCMinigun::WindUp( )
{
	CHL2MP_Player *pOwner = GetPlayerOwner();
	if ( !pOwner )
		return;

	if ( !(pOwner->m_nButtons & IN_ATTACK) )
		return;

	WeaponSound( SPECIAL1 );	// "special1" = wind up

	m_iWeaponState = AC_STATE_STARTFIRING;
	//pOwner->m_Shared.AddStateFlags( TFSTATE_AIMING );

	// Play the "spin down" animation.
	SendWeaponAnim( ACT_VM_PULLBACK_HIGH );

#ifdef GAME_DLL
	//pOwner->TeamFortress_SetSpeed();
#endif
}

void CTFCMinigun::WindDown( bool bFromHolster )
{
	CHL2MP_Player *pOwner = GetPlayerOwner();
	if ( !pOwner )
		return;

	WeaponSound( SPECIAL2 );

	m_iWeaponState = AC_STATE_IDLE;
	//pOwner->m_Shared.RemoveStateFlags( TFSTATE_AIMING );
	m_flTimeWeaponIdle = gpGlobals->curtime + 2.0;
	
	// Play the "spin down" animation.
	SendWeaponAnim( ACT_VM_PULLBACK_LOW );

#ifdef GAME_DLL
	//pOwner->TeamFortress_SetSpeed();
#endif
}

void CTFCMinigun::Spin()
{
	CHL2MP_Player *pOwner = GetPlayerOwner();
	if ( !pOwner )
		return;

	WeaponSound( SPECIAL3 );	// "special3" = spin

	pOwner->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );
}

void CTFCMinigun::Fire()
{
	if ( m_flNextPrimaryAttack > gpGlobals->curtime )
		return;

	CHL2MP_Player *pOwner = GetPlayerOwner();
	if ( !pOwner )
		return;

	pOwner->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );

	BaseClass::PrimaryAttack();	// Use the base HL2 system to fire.
}

void CTFCMinigun::StartSpin()
{
	CHL2MP_Player *pOwner = GetPlayerOwner();
	if ( !pOwner )
		return;

	WeaponSound( SPECIAL1 );	// "special1" = wind up

	// Rotate barrel without firing for fun
	m_iWeaponState = AC_STATE_SPINNING;

	pOwner->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );
}

void CTFCMinigun::PrimaryAttack()
{
	CHL2MP_Player *pOwner = GetPlayerOwner();
	if ( !pOwner )
		return;

	switch ( m_iWeaponState )
	{
	default:
	case AC_STATE_IDLE:
		// Removed the need for cells to powerup the AC
		WindUp();
		m_flNextPrimaryAttack = gpGlobals->curtime + 0.5;
		m_flTimeWeaponIdle = gpGlobals->curtime + 0.6;
		break;

	case AC_STATE_STARTFIRING:
		// Start playing the looping fire sound
		if ( m_flNextPrimaryAttack <= gpGlobals->curtime )
		{
			WeaponSound( SINGLE );	// "single" = the looping fire sound
			m_iWeaponState = AC_STATE_FIRING;
			m_flNextPrimaryAttack = m_flTimeWeaponIdle = gpGlobals->curtime + 0.1;
		}
		break;

	case AC_STATE_FIRING:
		Fire();
		m_flNextPrimaryAttack = m_flTimeWeaponIdle = gpGlobals->curtime + 0.1;
		break;
	}
}

void CTFCMinigun::HandleFireOnEmpty()
{
	if ( m_iWeaponState == AC_STATE_FIRING )
	{
		StartSpin();
		m_flNextPrimaryAttack = m_flTimeWeaponIdle = gpGlobals->curtime + 0.1;
	}
	else if ( m_iWeaponState == AC_STATE_SPINNING )
	{
		if ( HasPrimaryAmmo() )
			Spin();
		else
			m_iWeaponState = AC_STATE_STARTFIRING;	

		m_flNextPrimaryAttack = m_flTimeWeaponIdle = gpGlobals->curtime + 0.1;
	}
	else
	{
		BaseClass::HandleFireOnEmpty();
	}
}

void CTFCMinigun::WeaponIdle()
{
	//ResetEmptySound( );

	if ( gpGlobals->curtime < m_flTimeWeaponIdle )
		return;

	// Always wind down if we've hit here, because it only happens when the player has stopped firing/spinning
	if ( m_iWeaponState != AC_STATE_IDLE )// && !(m_pPlayer->pev->button & IN_ATTACK) )
	{
		WindDown( FALSE );
		return;
	}

	SendWeaponAnim( ACT_IDLE );

	m_flTimeWeaponIdle = gpGlobals->curtime + 12.5;// how long till we do this again.
}

bool CTFCMinigun::SendWeaponAnim( int iActivity )
{
	if ( GetActivity() == ACT_VM_PRIMARYATTACK && iActivity == ACT_VM_PRIMARYATTACK )
	{
		// The minigun's fire animation loops, so we don't want it to reset each time
		// they fire a bullet.
		return true;
	}
	else
	{
		return BaseClass::SendWeaponAnim( iActivity );
	}
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
