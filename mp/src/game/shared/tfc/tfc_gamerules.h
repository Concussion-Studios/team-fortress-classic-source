//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose:
//
//=============================================================================//
#ifndef TFC_GAMERULES_H
#define TFC_GAMERULES_H
#ifdef _WIN32
#pragma once
#endif

#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "hl2mp_gamerules.h"
#include "gamevars_shared.h"

#ifndef CLIENT_DLL
	#include "hl2mp_player.h"
#endif

#ifdef CLIENT_DLL
	#define CTFCGameRules C_TFCGameRules
	#define CTFCGameRulesProxy C_TFCGameRulesProxy
#endif

class CTFCGameRulesProxy : public CHL2MPGameRulesProxy
{
public:
	DECLARE_CLASS( CTFCGameRulesProxy, CHL2MPGameRulesProxy );
	DECLARE_NETWORKCLASS();
};

class CTFCGameRules : public CHL2MPRules
{
public:
	DECLARE_CLASS( CTFCGameRules, CHL2MPRules );

#ifdef CLIENT_DLL
	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.
#else
	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.
#endif

			CTFCGameRules();
	virtual ~CTFCGameRules();

	virtual const char *GetGameDescription(void) OVERRIDE{ return TFC_GAME_DESCRIPTION; }

#ifdef CLIENT_DLL
#else
	virtual void CreateStandardEntities( void ) OVERRIDE;

	virtual void ClientSettingsChanged( CBasePlayer *pPlayer ) OVERRIDE;
	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args ) OVERRIDE;

	virtual bool  FlPlayerFallDeathDoesScreenFade( CBasePlayer *pl ) OVERRIDE { return false; }
	virtual bool UseSuicidePenalty() OVERRIDE { return false; }

	// Disable autoaim
	virtual bool ShouldAutoAim( CBasePlayer *pPlayer, edict_t *target ) OVERRIDE { return false; }
	virtual float GetAutoAimScale( CBasePlayer *pPlayer ) OVERRIDE { return 0.0f; }
	virtual int	GetAutoAimMode() OVERRIDE { return AUTOAIM_NONE; }
#endif
};

inline CTFCGameRules* TFCGameRules()
{
	return static_cast<CTFCGameRules*>(g_pGameRules);
}

#endif //TFC_GAMERULES_H
