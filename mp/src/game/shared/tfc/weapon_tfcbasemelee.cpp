//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfcbasemelee.h"
#include "decals.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "ilagcompensationmanager.h"
	#include "util.h"
#endif

static Vector head_hull_mins(-16, -16, -18);
static Vector head_hull_maxs(16, 16, 18);

// ----------------------------------------------------------------------------- //
// CWeaponTFCBaseMelee tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( WeaponTFCBaseMelee, DT_WeaponTFCBaseMelee )

BEGIN_NETWORK_TABLE_NOBASE( CWeaponTFCBaseMelee, DT_LocalActiveWeaponBaseMeleeData )
END_NETWORK_TABLE()

BEGIN_NETWORK_TABLE( CWeaponTFCBaseMelee, DT_WeaponTFCBaseMelee )
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponTFCBaseMelee )
END_PREDICTION_DATA()
#endif

#ifndef CLIENT_DLL
BEGIN_DATADESC( CWeaponTFCBaseMelee )
	DEFINE_FUNCTION( Smack )
END_DATADESC()
#endif

// ----------------------------------------------------------------------------- //
// CWeaponTFCBaseMelee implementation.
// ----------------------------------------------------------------------------- //
CWeaponTFCBaseMelee::CWeaponTFCBaseMelee()
{
}

void FindHullIntersection( const Vector &vecSrc, trace_t &tr, const Vector &mins, const Vector &maxs, CBaseEntity *pEntity )
{
	int			i, j, k;
	float		distance;
	Vector minmaxs[2] = {mins, maxs};
	trace_t tmpTrace;
	Vector		vecHullEnd = tr.endpos;
	Vector		vecEnd;

	distance = 1e6f;

	vecHullEnd = vecSrc + ((vecHullEnd - vecSrc)*2);
	UTIL_TraceLine( vecSrc, vecHullEnd, MASK_SOLID, pEntity, COLLISION_GROUP_NONE, &tmpTrace );
	if ( tmpTrace.fraction < 1.0 )
	{
		tr = tmpTrace;
		return;
	}

	for ( i = 0; i < 2; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			for ( k = 0; k < 2; k++ )
			{
				vecEnd.x = vecHullEnd.x + minmaxs[i][0];
				vecEnd.y = vecHullEnd.y + minmaxs[j][1];
				vecEnd.z = vecHullEnd.z + minmaxs[k][2];

				UTIL_TraceLine( vecSrc, vecEnd, MASK_SOLID, pEntity, COLLISION_GROUP_NONE, &tmpTrace );
				if ( tmpTrace.fraction < 1.0 )
				{
					float thisDistance = (tmpTrace.endpos - vecSrc).Length();
					if ( thisDistance < distance )
					{
						tr = tmpTrace;
						distance = thisDistance;
					}
				}
			}
		}
	}
}


void CWeaponTFCBaseMelee::Spawn()
{
	m_iClip1 = -1;
	BaseClass::Spawn();
}

bool CWeaponTFCBaseMelee::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	if ( GetPlayerOwner() )
		GetPlayerOwner()->m_flNextAttack = gpGlobals->curtime + 0.5;

	return BaseClass::Holster( pSwitchingTo );
}

void CWeaponTFCBaseMelee::ItemPostFrame()
{
	// Store this off so we can detect if it's our first swing or not later on.
	m_flStoredPrimaryAttack = m_flNextPrimaryAttack;

	BaseClass::ItemPostFrame();
}

void CWeaponTFCBaseMelee::WeaponIdle( void )
{
	CHL2MP_Player *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	if ( m_flTimeWeaponIdle > gpGlobals->curtime )
		return;

	m_flTimeWeaponIdle = gpGlobals->curtime + 20;

	// only idle if the slid isn't back
	SendWeaponAnim( ACT_VM_IDLE );
}

void CWeaponTFCBaseMelee::PrimaryAttack()
{
	CHL2MP_Player *pPlayer = GetPlayerOwner();

#if !defined (CLIENT_DLL)
	// Move other players back to history positions based on local player's lag
	lagcompensation->StartLagCompensation( pPlayer, pPlayer->GetCurrentCommand() );
#endif

	Vector vForward;
	AngleVectors( pPlayer->EyeAngles(), &vForward );
	Vector vecSrc	= pPlayer->Weapon_ShootPosition();
	Vector vecEnd	= vecSrc + vForward * 32;

	trace_t tr;
	UTIL_TraceLine( vecSrc, vecEnd, MASK_SOLID, pPlayer, COLLISION_GROUP_NONE, &tr );

	if ( tr.fraction >= 1.0 )
	{
		UTIL_TraceHull( vecSrc, vecEnd, head_hull_mins, head_hull_maxs, MASK_SOLID, pPlayer, COLLISION_GROUP_NONE, &tr );
		if ( tr.fraction < 1.0 )
		{
			// Calculate the point of intersection of the line (or hull) and the object we hit
			// This is and approximation of the "best" intersection
			CBaseEntity *pHit = tr.m_pEnt;
			if ( !pHit || pHit->IsBSPModel() )
				FindHullIntersection( vecSrc, tr, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX, pPlayer );
			vecEnd = tr.endpos;	// This is the point on the actual surface (the hull could have hit space)
		}
	}

	bool bDidHit = tr.fraction < 1.0f;

#ifndef CLIENT_DLL
	bool bFirstSwing = (gpGlobals->curtime - m_flStoredPrimaryAttack) >= 1;
#endif

	pPlayer->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );

	m_flTimeWeaponIdle = gpGlobals->curtime + 2;
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.4f;

	if ( bDidHit )
	{
		SendWeaponAnim( ACT_VM_HITCENTER );
	}
	else
	{
		// Allow for there only being hit activities.
		if ( !SendWeaponAnim( ACT_VM_MISSCENTER ) )
			SendWeaponAnim( ACT_VM_HITCENTER );

		// play wiff or swish sound
		WeaponSound( MELEE_MISS );
	}

	bool bPlayImpactEffect = false;
	
#ifndef CLIENT_DLL

	if ( bDidHit )
	{
		CBaseEntity *pEntity = tr.m_pEnt;
		
		ClearMultiDamage();

		float flDamage = 0;
		bool bDoEffects = true;
		AxeHit( pEntity, bFirstSwing, tr, &flDamage, &bDoEffects );
		if ( flDamage != 0 )
		{
			CTakeDamageInfo info( pPlayer, pPlayer, flDamage, DMG_CLUB | DMG_NEVERGIB );

			CalculateMeleeDamageForce( &info, vForward, tr.endpos, 1.0f/flDamage );
			pEntity->DispatchTraceAttack( info, vForward, &tr ); 
			ApplyMultiDamage();
		}

		if ( bDoEffects )
		{
			if ( pEntity && pEntity->IsPlayer() )
			{
				WeaponSound( MELEE_HIT );

				if ( pEntity->IsAlive() )
					bPlayImpactEffect = true; // no blood effect on dead bodies
			}
			else
			{
				bPlayImpactEffect = true; // always show impact effects on world objects
			}
		}
		else
		{
			bDoEffects = false;
		}
	}


#endif

	if ( bPlayImpactEffect )
	{
		// delay the decal a bit
		m_trHit = tr;
		
		// Store the ent in an EHANDLE, just in case it goes away by the time we get into our think function.
		m_pTraceHitEnt = tr.m_pEnt; 

		SetThink( &CWeaponTFCBaseMelee::Smack );
		SetNextThink( gpGlobals->curtime + 0.2f );
	}

#ifndef CLIENT_DLL
	lagcompensation->FinishLagCompensation( pPlayer );
#endif	//CLIENT_DLL
}

void CWeaponTFCBaseMelee::Smack()
{
	if ( !GetPlayerOwner() )
		return;

	m_trHit.m_pEnt = m_pTraceHitEnt;

	if ( !m_trHit.m_pEnt || ( m_trHit.surface.flags & SURF_SKY ) )
		return;

	if ( m_trHit.fraction == 1.0 )
		return;

	UTIL_ImpactTrace( &m_trHit, DMG_CLUB );
	
	surfacedata_t *psurf = physprops->GetSurfaceData( m_trHit.surface.surfaceProps );
	if ( psurf->game.material != CHAR_TEX_FLESH && psurf->game.material != CHAR_TEX_BLOODYFLESH )
		WeaponSound( MELEE_HIT_WORLD );
}