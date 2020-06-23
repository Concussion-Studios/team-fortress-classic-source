//============= Copyright Concussion Studios, All rights reserved. =============//
//
// Purpose: Definitions that are shared by the server DLL and the client DLL.
//
//=============================================================================//
#include "cbase.h"
#include "tfc_shareddefs.h"

//-----------------------------------------------------------------------------
// Teams.
//-----------------------------------------------------------------------------
const char *g_aTeamNames[TEAM_COUNT] =
{
	"Unassigned",
	"Spectator",
	"Red",
	"Blue"
};

color32 g_aTeamColors[TEAM_COUNT] = 
{
	{ 0, 0, 0, 0 },		// Unassigned
	{ 0, 0, 0, 0 },		// Spectator
	{ 255, 0, 0, 0 },	// Red
	{ 0, 0, 255, 0 },	// Blue
};

//-----------------------------------------------------------------------------
// Classes.
//-----------------------------------------------------------------------------
const char *g_aClassNames[13] =
{
	"#TFC_Class_Name_Undefined",
	"#TFC_Class_Name_Scout",
	"#TFC_Class_Name_Sniper",
	"#TFC_Class_Name_Soldier",
	"#TFC_Class_Name_Demoman",
	"#TFC_Class_Name_Medic",
	"#TFC_Class_Name_HWGuy",
	"#TFC_Class_Name_Pyro",
	"#TFC_Class_Name_Spy",
	"#TFC_Class_Name_Engineer",
	"#TFC_Class_Name_Civillian"
};

const char* g_aClassNames_NonLocalized[13] =
{
	"Undefined",
	"Sniper",
	"Scout",
	"Soldier",
	"Demoman",
	"Medic",
	"Heavy",
	"Pyro",
	"Spy",
	"Engineer",
	"Civillian"
};

//-----------------------------------------------------------------------------
// Ammo.
//-----------------------------------------------------------------------------
const char* g_aAmmoNames[] =
{
	"AMMO_NONE",
	"AMMO_SHELLS",
	"AMMO_NAILS",
	"AMMO_ROCKETS",
	"AMMO_NAILS",
	"AMMO_GRENADE1",
	"AMMO_GRENADE2",
	"AMMO_DETPACK",
	"AMMO_LAST"
};

//-----------------------------------------------------------------------------
// Projectiles.
//-----------------------------------------------------------------------------
const char* g_aProjectileNames[] =
{
	"PROJECTILE_NONE",
	"PROJECTILE_BULLET",
	"PROJECTILE_ROCKET",
	"PROJECTILE_NAIL",
	"PROJECTILE_GRENADE",
	"PROJECTILE_PIPEBOMB",
	"PROJECTILE_LAST"
};

//-----------------------------------------------------------------------------
// Weapons.
//-----------------------------------------------------------------------------
const char* g_aWeaponNames[] =
{
	"WEAPON_NONE",

	// Slot 1 Weapons
	"WEAPON_CROWBAR",
	"WEAPON_MEDIKIT",
	"WEAPON_KNIFE",
	"WEAPON_SPANNER",
	"WEAPON_UMBRELLA",

	// Slot 2 Weapons
	"WEAPON_SHOTGUN",
	"WEAPON_RIFLE",
	"WEAPON_TRANQ",
	"WEAPON_RAILGUN",

	// Slot 3 Weapons
	"WEAPON_AUTORIFLE",
	"WEAPON_SUPER_SHOTGUN",

	// Slot 4 Weapons
	"WEAPON_NAILGUN",
	"WEAPON_GRENADELAUNCHER",
	"WEAPON_SUPER_NAILGUN",
	"WEAPON_FLAMETROWER",

	// Slot 5 Weapons
	"WEAPON_RPG",
	"WEAPON_PIPEBOMB",
	"WEAPON_MINIGUN",
	"WEAPON_IC",

	"WEAPON_LAST"
};

//--------------------------------------------------------------------------------------------------------
// Given an alias, return the associated weapon ID
//--------------------------------------------------------------------------------------------------------
int AliasToWeaponID( const char *alias )
{
	if ( alias )
	{
		for ( int i = 0; i < WEAPON_LAST; ++i )
		{
			if ( !Q_stricmp( g_aWeaponNames[i], alias ) )
				return i;
		}
	}
	return WEAPON_NONE;
}

//--------------------------------------------------------------------------------------------------------
// Given a weapon ID, return its alias
//--------------------------------------------------------------------------------------------------------
const char *WeaponIDToAlias( int iWeaponID )
{
	if ( iWeaponID <= 0 || iWeaponID > WEAPON_LAST )
		return NULL;

	return g_aWeaponNames[iWeaponID];
}

//--------------------------------------------------------------------------------------------------------
// Return true if given weapon ID is a primary weapon
//--------------------------------------------------------------------------------------------------------
bool IsPrimaryWeapon( int id )
{
	switch( id )
	{
		case WEAPON_CROWBAR:
		case WEAPON_MEDIKIT:
		case WEAPON_KNIFE:
		case WEAPON_SPANNER:
		case WEAPON_UMBRELLA:
			return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------
// Return true if given weapon ID is a secondary weapon
//--------------------------------------------------------------------------------------------------------
bool IsSecondaryWeapon( int id )
{
	switch( id )
	{
		case WEAPON_SHOTGUN:
		case WEAPON_RIFLE:
		case WEAPON_TRANQ:
		case WEAPON_RAILGUN:
			return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------
// Return true if given weapon ID is a tertiary weapon
//--------------------------------------------------------------------------------------------------------
bool IsTertiaryWeapon( int id )
{
	switch( id )
	{
		case WEAPON_AUTORIFLE:
		case WEAPON_SUPER_SHOTGUN:
			return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------
// Return true if given weapon ID is a quaternary weapon
//--------------------------------------------------------------------------------------------------------
bool IsQuaternaryWeapon( int id )
{
	switch( id )
	{
		case WEAPON_NAILGUN:
		case WEAPON_GRENADELAUNCHER:
		case WEAPON_SUPER_NAILGUN:
		case WEAPON_FLAMETROWER:
			return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------
// Return true if given weapon ID is a quinary weapon
//--------------------------------------------------------------------------------------------------------
bool IsQuinaryWeapon( int id )
{
	switch( id )
	{
		case WEAPON_RPG:
		case WEAPON_PIPEBOMB:
		case WEAPON_MINIGUN:
		case WEAPON_IC:
			return true;
	}

	return false;
}