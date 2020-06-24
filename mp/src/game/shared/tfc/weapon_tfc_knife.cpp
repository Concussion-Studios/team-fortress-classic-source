//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_knife.h"

#if defined( CLIENT_DLL )
	#include "c_tfc_player.h"
#else
	#include "tfc_player.h"
#endif

// ----------------------------------------------------------------------------- //
// CTFCKnife tables.
// ----------------------------------------------------------------------------- //
IMPLEMENT_NETWORKCLASS_ALIASED( TFCKnife, DT_WeaponKnife )

BEGIN_NETWORK_TABLE( CTFCKnife, DT_WeaponKnife )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFCKnife )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_knife, CTFCKnife );
PRECACHE_WEAPON_REGISTER( tf_weapon_knife );

#ifndef CLIENT_DLL
BEGIN_DATADESC( CTFCKnife )
END_DATADESC()
#endif

// ----------------------------------------------------------------------------- //
// CTFCKnife implementation.
// ----------------------------------------------------------------------------- //
CTFCKnife::CTFCKnife()
{
}

void CTFCKnife::Smack( void )
{
	if ( !GetPlayerOwner() )
		return;

	m_trHit.m_pEnt = m_pTraceHitEnt;

	if ( !m_trHit.m_pEnt || ( m_trHit.surface.flags & SURF_SKY ) )
		return;

	if ( m_trHit.fraction == 1.0 )
		return;

	CEffectData data;
	data.m_vOrigin = m_trHit.endpos;
	data.m_vStart = m_trHit.startpos;
	data.m_nSurfaceProp = m_trHit.surface.surfaceProps;
	data.m_nDamageType = DMG_SLASH;
	data.m_nHitBox = m_trHit.hitbox;
#ifdef CLIENT_DLL
	data.m_hEntity = m_trHit.m_pEnt->GetRefEHandle();
#else
	data.m_nEntIndex = m_trHit.m_pEnt->entindex();
#endif

	CPASFilter filter( data.m_vOrigin );
	
#ifndef CLIENT_DLL
	filter.RemoveRecipient( GetPlayerOwner() );
#endif

	data.m_vAngles = GetPlayerOwner()->GetAbsAngles();
	data.m_fFlags = 0x1;	//IMPACT_NODECAL;
	te->DispatchEffect( filter, 0.0, data.m_vOrigin, "KnifeSlash", data );
}