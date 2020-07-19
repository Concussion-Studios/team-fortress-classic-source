//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "weapon_tfc_knife.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
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

// ----------------------------------------------------------------------------- //
// CTFCKnife implementation.
// ----------------------------------------------------------------------------- //
CTFCKnife::CTFCKnife()
{
}

// ----------------------------------------------------------------------------- //
// Purpose:
// ----------------------------------------------------------------------------- //
void CTFCKnife::ImpactEffect( trace_t &traceHit )
{
	// See if we hit water (we don't do the other impact effects in this case)
	if ( ImpactWater( traceHit.startpos, traceHit.endpos ) )
		return;

	CEffectData data;
	data.m_vOrigin = traceHit.endpos;
	data.m_vStart = traceHit.startpos;
	data.m_nSurfaceProp = traceHit.surface.surfaceProps;
	data.m_nDamageType = DMG_SLASH;
	data.m_nHitBox = traceHit.hitbox;
#ifdef CLIENT_DLL
	data.m_hEntity = traceHit.m_pEnt->GetRefEHandle();
#else
	data.m_nEntIndex = traceHit.m_pEnt->entindex();
#endif

	CPASFilter filter( data.m_vOrigin );
	
#ifndef CLIENT_DLL
	filter.RemoveRecipient( GetPlayerOwner() );
#endif

	data.m_vAngles = GetPlayerOwner()->GetAbsAngles();
	data.m_fFlags = 0x1;	//IMPACT_NODECAL;
	te->DispatchEffect( filter, 0.0, data.m_vOrigin, "KnifeSlash", data );
}