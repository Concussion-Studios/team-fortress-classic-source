//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "in_buttons.h"
#include "basegrenade_shared.h"
#include "weapon_tfc_grenadelauncher.h"

#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "grenade_frag.h"
#endif

// ----------------------------------------------------------------------------- //
// CTFCGrenadeLauncher tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCGrenadeLauncher, DT_TFCGrenadeLauncher )

BEGIN_NETWORK_TABLE( CTFCGrenadeLauncher, DT_TFCGrenadeLauncher )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCGrenadeLauncher )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_gl, CTFCGrenadeLauncher );
PRECACHE_WEAPON_REGISTER( tf_weapon_gl );

#ifndef CLIENT_DLL
BEGIN_DATADESC( CTFCGrenadeLauncher )
END_DATADESC()
#endif

// ----------------------------------------------------------------------------- //
// CTFCGrenadeLauncher implementation.
// ----------------------------------------------------------------------------- //
CTFCGrenadeLauncher::CTFCGrenadeLauncher()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCGrenadeLauncher::Precache( void )
{
#ifndef CLIENT_DLL
	UTIL_PrecacheOther( "npc_grenade_frag" );
#endif

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFCGrenadeLauncher::PrimaryAttack( void )
{
	// Only the player fires this way so we can cast
	CHL2MP_Player *pPlayer = dynamic_cast< CHL2MP_Player* >( GetOwner() );
	if ( pPlayer == NULL )
		return;

	//Must have ammo
	if ( ( pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 ) )
	{
		SendWeaponAnim( ACT_VM_DRYFIRE );
		BaseClass::WeaponSound( EMPTY );
		m_flNextPrimaryAttack = gpGlobals->curtime + 0.5f;
		return;
	}

	if( m_bInReload )
		m_bInReload = false;

	// MUST call sound before removing a round from the clip of a CMachineGun
	BaseClass::WeaponSound( SINGLE );

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector	vecThrow;

	// Don't autoaim on grenade tosses
	AngleVectors( pPlayer->EyeAngles() + pPlayer->GetPunchAngle(), &vecThrow );
	VectorScale( vecThrow, 1000.0f, vecThrow );
	
#ifndef CLIENT_DLL
	//Create the grenade
	CBaseGrenade *pGrenade = Fraggrenade_Create( vecSrc, vec3_angle, vecThrow, AngularImpulse( 600, random->RandomInt( -1200,1200 ),0 ), pPlayer, GRENADE_TIMER, false );
	if ( pGrenade )
	{
		pGrenade->SetDamage( GRENADE_DAMAGE );
		pGrenade->SetDamageRadius( GRENADE_RADIUS );
	}
#endif

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );

	// player "shoot" animation
	pPlayer->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );

	// Decrease ammo
	pPlayer->RemoveAmmo( 1, m_iPrimaryAmmoType );

	// Can shoot again immediately
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.5f;
}