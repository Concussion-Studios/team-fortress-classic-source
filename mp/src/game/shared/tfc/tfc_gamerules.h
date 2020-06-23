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
	#include "tfc_player.h"
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

	virtual void Precache( void ) OVERRIDE;
	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args ) OVERRIDE;
	virtual const char *GetGameDescription(void) OVERRIDE{ return TFC_GAME_DESCRIPTION; }
};

inline CTFCGameRules* TFCGameRules()
{
	return static_cast<CTFCGameRules*>(g_pGameRules);
}

#endif //TFC_GAMERULES_H
