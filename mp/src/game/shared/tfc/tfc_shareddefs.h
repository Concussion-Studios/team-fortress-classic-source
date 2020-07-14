//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose: Definitions that are shared by the server DLL and the client DLL.
//
//=============================================================================//
#ifndef TFC_SHAREDDEFS_H
#define TFC_SHAREDDEFS_H
#ifdef _WIN32
#pragma once
#endif

#include "shareddefs.h"
#include "mp_shareddefs.h"

// Using MAP_DEBUG mode?
#ifdef MAP_DEBUG
	#define MDEBUG(x) x
#else
	#define MDEBUG(x)
#endif

//-----------------------------------------------------------------------------
// Gamerules.
//-----------------------------------------------------------------------------
#define TFC_GAME_DESCRIPTION "Team Fortress Classic: Source"
#define TFC_MAX_PLAYERS 128

//-----------------------------------------------------------------------------
// Player settings.
//-----------------------------------------------------------------------------
#define TFC_DEFAULT_RUN_SPEED 220.0f

#define TFC_SELF_DAMAGE_MULTIPLIER 0.75
#define TFC_DEMOMAN_EXPLOSION_MULTIPLIER 0.85
#define TFC_PYRO_FIRE_RESIST_MULTIPLIER 0.5

#define TFC_MEDIKIT_HEAL 200
#define TFC_MEDIKIT_OVERHEAL 10
#define TFC_MEDIKIT_MAX_OVERHEAL 50
#define TFC_PLAYER_PHYSDAMAGE_SCALE 4.0f

#define TFC_PLAYER_VIEW_OFFSET	Vector( 0, 0, 53.5 )

//-----------------------------------------------------------------------------
// Teams.
//-----------------------------------------------------------------------------
enum TFCTeams_e
{
	TEAM_BLUE = FIRST_GAME_TEAM,
	TEAM_RED,

	TEAM_COUNT
};

#define TEAM_AUTOASSIGN ( TEAM_COUNT + 1 )

extern const char *g_aTeamNames[TEAM_COUNT];
extern color32 g_aTeamColors[TEAM_COUNT];

#define COLOR_SPECTATOR Color( 245, 229, 196, 255 )
#define COLOR_TFC_BLUE	Color( 64, 64, 255, 255 )
#define COLOR_TFC_RED	Color( 255, 64, 64, 255 )

#define CONTENTS_REDTEAM	CONTENTS_TEAM1
#define CONTENTS_BLUETEAM	CONTENTS_TEAM2

//-----------------------------------------------------------------------------
// TFC-specific viewport panels
//-----------------------------------------------------------------------------
#define PANEL_CLASS_BLUE		"class_blue"
#define PANEL_CLASS_RED			"class_red"

//-----------------------------------------------------------------------------
// Player Classes.
//-----------------------------------------------------------------------------
#define CLASS_COUNT			( CLASS_COUNT_ALL - 1 )

#define FIRST_NORMAL_CLASS	( CLASS_UNDEFINED + 1 )
#define LAST_NORMAL_CLASS	( CLASS_CIVILIAN - 1 )

#define	CLASS_MENU_BUTTONS	( CLASS_RANDOM + 1 )

enum TFCClassID
{
	CLASS_UNDEFINED = 0,

	CLASS_SCOUT,			// FIRST_NORMAL_CLASS
	CLASS_SNIPER,
	CLASS_SOLDIER,
	CLASS_DEMOMAN,
	CLASS_MEDIC,
	CLASS_HWGUY,
	CLASS_PYRO,
	CLASS_SPY,
	CLASS_ENGINEER,			// LAST_NORMAL_CLASS

	// Add any new classes after Engineer
	CLASS_CIVILIAN,
	CLASS_COUNT_ALL,		// Civilians are a special class. It is not a player class.

	CLASS_RANDOM
};

extern const char *g_aClassNames[];					// localized class names
extern const char *g_aClassNames_NonLocalized[];	// non-localized class names

//-----------------------------------------------------------------------------
// Ammo types.
//-----------------------------------------------------------------------------
enum TFCAmmoID
{
	AMMO_DUMMY = 0,
	AMMO_SHELLS,
	AMMO_NAILS,
	AMMO_ROCKETS,
	AMMO_CELLS,
	AMMO_GRENADE1,
	AMMO_GRENADE2,
	AMMO_DETPACK,
	AMMO_LAST
};

extern const char* g_aAmmoNames[];

//-----------------------------------------------------------------------------
// Projectile types.
//-----------------------------------------------------------------------------
enum TFCProjectileID
{
	PROJECTILE_NONE = 0,
	PROJECTILE_BULLET,
	PROJECTILE_ROCKET,
	PROJECTILE_NAIL,
	PROJECTILE_GRENADE,
	PROJECTILE_PIPEBOMB,

	PROJECTILE_LAST
};

extern const char* g_aProjectileNames[];

//-----------------------------------------------------------------------------
// Weapon names.
//-----------------------------------------------------------------------------
#define TFC_MAX_WEAPON_SLOTS 10
#define TFC_MAX_GRENADE_SLOTS 3

int AliasToWeaponID( const char *alias );
const char *WeaponIDToAlias( int iWeaponID );
extern bool IsAmmoType( int iAmmoType, const char *pAmmoName );
bool IsPrimaryWeapon( int id );
bool IsSecondaryWeapon( int id );
bool IsTertiaryWeapon( int id );
bool IsQuaternaryWeapon( int id );
bool IsQuinaryWeapon( int id );

enum TFCWeaponID
{
	TF_WEAPON_NONE = 0,

	// Slot 1 Weapons
	TF_WEAPON_CROWBAR,
	TF_WEAPON_MEDIKIT,
	TF_WEAPON_KNIFE,
	TF_WEAPON_SPANNER,
	TF_WEAPON_UMBRELLA,

	// Slot 2 Weapons
	TF_WEAPON_SHOTGUN,
	TF_WEAPON_RIFLE,
	TF_WEAPON_TRANQ,
	TF_WEAPON_RAILGUN,

	// Slot 3 Weapons
	TF_WEAPON_AUTORIFLE,
	TF_WEAPON_SUPER_SHOTGUN,

	// Slot 4 Weapons
	TF_WEAPON_NAILGUN,
	TF_WEAPON_GRENADELAUNCHER,
	TF_WEAPON_SUPER_NAILGUN,
	TF_WEAPON_FLAMETHROWER,

	// Slot 5 Weapons
	TF_WEAPON_RPG,
	TF_WEAPON_PIPEBOMB,
	TF_WEAPON_MINIGUN,
	TF_WEAPON_IC,

	TF_WEAPON_COUNT,
};

#endif // TFC_SHAREDDEFS_H
