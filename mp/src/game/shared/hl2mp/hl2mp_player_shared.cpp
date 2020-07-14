//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "hl2mp_gamerules.h"
#include "takedamageinfo.h"

#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
	#include "prediction.h"
#else
	#include "hl2mp_player.h"
	#include "ai_basenpc.h"

	void TE_PlayerAnimEvent( CBasePlayer* pPlayer, PlayerAnimEvent_t playerAnim, int nData );
#endif

void SpawnBlood ( Vector vecSpot, const Vector &vecDir, int bloodColor, float flDamage );

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Consider the weapon's built-in accuracy, this character's proficiency with
// the weapon, and the status of the target. Use this information to determine
// how accurately to shoot at the target.
//-----------------------------------------------------------------------------
Vector CHL2MP_Player::GetAttackSpread( CBaseCombatWeapon *pWeapon, CBaseEntity *pTarget )
{
	if ( pWeapon )
		return pWeapon->GetBulletSpread( WEAPON_PROFICIENCY_PERFECT );
	
	return VECTOR_CONE_15DEGREES;
}

//-----------------------------------------------------------------------------
// Purpose: multiplayer does not do autoaiming.
//-----------------------------------------------------------------------------
Vector CHL2MP_Player::GetAutoaimVector( float flScale )
{
	//No Autoaim
	Vector	forward;
	AngleVectors( EyeAngles() + m_Local.m_vecPunchAngle, &forward );
	return	forward;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CHL2MP_Player::TraceAttack( const CTakeDamageInfo &inputInfo, const Vector &vecDir, trace_t *ptr, CDmgAccumulator *pAccumulator )
{
	if ( m_takedamage )
	{
		CTakeDamageInfo info = inputInfo;

		if ( info.GetAttacker() )
		{
#ifdef GAME_DLL
			// --------------------------------------------------
			//  If an NPC check if friendly fire is disallowed
			// --------------------------------------------------
			CAI_BaseNPC *pNPC = info.GetAttacker()->MyNPCPointer();
			if ( pNPC && (pNPC->CapabilitiesGet() & bits_CAP_NO_HIT_PLAYER) && pNPC->IRelationType( this ) != D_HT )
				return;

			// --------------------------------------------------
			//  Prevent damage here so blood doesn't appear
			// --------------------------------------------------
			if ( info.GetAttacker()->IsPlayer() )
			{
				if ( !g_pGameRules->FPlayerCanTakeDamage( this, info.GetAttacker(), info ) )
					return;
			}
#endif // GAME_DLL

			// --------------------------------------------------
			// Don't allow blood to appear on teamplay
			// --------------------------------------------------
			if ( HL2MPRules()->IsTeamplay() && info.GetAttacker()->InSameTeam( this ) == true )
				return;
		}

#ifdef GAME_DLL
		SetLastHitGroup( ptr->hitgroup );
#endif  // GAME_DLL

		ConVarRef sk_player_head( "sk_player_head" );
		ConVarRef sk_player_chest( "sk_player_chest" );
		ConVarRef sk_player_stomach( "sk_player_stomach" );
		ConVarRef sk_player_arm( "sk_player_arm" );
		ConVarRef sk_player_leg( "sk_player_leg" );

		switch ( ptr->hitgroup )
		{
		case HITGROUP_GENERIC:
			break;
		case HITGROUP_HEAD:
			info.ScaleDamage( sk_player_head.GetFloat() );
			break;
		case HITGROUP_CHEST:
			info.ScaleDamage( sk_player_chest.GetFloat() );
			break;
		case HITGROUP_STOMACH:
			info.ScaleDamage( sk_player_stomach.GetFloat() );
			break;
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			info.ScaleDamage( sk_player_arm.GetFloat() );
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			info.ScaleDamage( sk_player_leg.GetFloat() );
			break;
		default:
			break;
		}

		// If this damage type makes us bleed, then do so
		bool bShouldBleed = !g_pGameRules->Damage_ShouldNotBleed( info.GetDamageType() ) || BloodColor() != DONT_BLEED;
		if ( bShouldBleed )
		{
			SpawnBlood( ptr->endpos, vecDir, BloodColor(), info.GetDamage() );// a little surface blood.
			TraceBleed( info.GetDamage(), vecDir, ptr, info.GetDamageType() );
		}

		AddMultiDamage( info, this );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CHL2MP_Player::DoAnimationEvent( PlayerAnimEvent_t event, int nData )
{
#ifdef CLIENT_DLL
	if ( IsLocalPlayer() )
	{
		if ( ( prediction->InPrediction() && !prediction->IsFirstTimePredicted() ) )
			return;
	}

	MDLCACHE_CRITICAL_SECTION();
#endif // CLIENT_DLL

	m_PlayerAnimState->DoAnimationEvent( event, nData );

#ifndef CLIENT_DLL
    TE_PlayerAnimEvent( this, event, nData );
#endif // CLIENT_DLL
}

//-----------------------------------------------------------------------------
// Purpose: Do nothing multiplayer_animstate takes care of animation.
// Input  : playerAnim - 
//-----------------------------------------------------------------------------
void CHL2MP_Player::SetAnimation( PLAYER_ANIM playerAnim )
{
	if ( playerAnim == PLAYER_WALK || playerAnim == PLAYER_IDLE ) 
		return;

    if ( playerAnim == PLAYER_RELOAD )
        DoAnimationEvent( PLAYERANIMEVENT_RELOAD );
    else if ( playerAnim == PLAYER_JUMP )
        DoAnimationEvent( PLAYERANIMEVENT_JUMP );
    else
        Assert( !"CHL2MP_Player::SetAnimation OBSOLETE!" );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CStudioHdr *CHL2MP_Player::OnNewModel( void )
{
	CStudioHdr *hdr = BaseClass::OnNewModel();
	if ( hdr )
	{
#ifdef CLIENT_DLL
		InitializePoseParams();
#endif // CLIENT_DLL

		// Reset the players animation states, gestures
		if ( m_PlayerAnimState )
			m_PlayerAnimState->OnNewModel();
	}

	return hdr;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : collisionGroup - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHL2MP_Player::ShouldCollide( int collisionGroup, int contentsMask ) const
{
	if ( HL2MPRules()->IsTeamplay() )
	{
		if ( collisionGroup == COLLISION_GROUP_PLAYER_MOVEMENT || collisionGroup == COLLISION_GROUP_PROJECTILE )
		{
			switch( GetTeamNumber() )
			{
			case TEAM_REBELS:
				if ( !( contentsMask & CONTENTS_TEAM2 ) )
					return false;
				break;

			case TEAM_COMBINE:
				if ( !( contentsMask & CONTENTS_TEAM1 ) )
					return false;
				break;
			}
		}
	}

	return BaseClass::ShouldCollide( collisionGroup, contentsMask );
}

