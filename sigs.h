/**
 * vim: set ts=4 :
 * =============================================================================
 * BoomerVomitPatch
 * Copyright (C) 2012 Michael "ProdigySim" Busby
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, the author(s) give you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, the author(s) grant
 * this exception to all derivative works.  The author(s) define further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */
#ifndef _SIGS_H_
#define _SIGS_H_

/* Platform specific offset+sig data */

/* CVomit::UpdateAbility() function finding */
// Same symbol on both l4d1+2
#define LIN_CVomit_UpdateAbility_SYMBOL "_ZN6CVomit13UpdateAbilityEv"
// search for "stopvomit" string in CVomit::StopVomitEffect() + ~0x1A0, xref StopVomitEffect + ~0xE0 (farther than other similar)
#if defined (_L4D)
	// 81 EC ? ? ? ? 53 55 56 57 8B F9 8B 87
	#define WIN_CVomit_UpdateAbility_SIG "\x81\xEC\x2A\x2A\x2A\x2A\x53\x55\x56\x57\x8B\xF9\x8B\x87"
	#define WIN_CVomit_UpdateAbility_SIGLEN 14
#elif defined (_L4D2)
	// 55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 FC 53 56 57 8B F9 8B 87 ? ? ? ? 83
	#define WIN_CVomit_UpdateAbility_SIG "\x55\x8B\xEC\x81\xEC\x2A\x2A\x2A\x2A\xA1\x2A\x2A\x2A\x2A\x33\xC5\x89\x45\xFC\x53\x56\x57\x8B\xF9\x8B\x87\x2A\x2A\x2A\x2A\x83"
	#define WIN_CVomit_UpdateAbility_SIGLEN 31
#endif


/* gpGlobals read offsets into CVomit::UpdateAbility() */
const int g_FrameTimeReadOffsets[] =
#if defined (_LINUX)
#if defined (_L4D)
	{0x13D, 0x380}; // L4D1 LINUX
#elif defined (_L4D2)
	{0x12D, 0x5FD}; // L4D2 LINUX
#endif
#elif defined (_WIN32)
#if defined (_L4D)
	{0x173, 0x2CD, 0x476};
	/* 371, 717, 1142 */
	/* 
	0x173	=	371		= .text:102CEB33 A1 B0 9A 5E 10                          mov     eax, dword_105E9AB0
	0x2CD	=	717 	= .text:102CEC8D A1 B0 9A 5E 10                          mov     eax, dword_105E9AB0
	0x476	=	1142	= .text:102CEE36 A1 B0 9A 5E 10                          mov     eax, dword_105E9AB0
	*/
#elif defined (_L4D2)
	{0x1DC, 0x36E, 0x533};
	/* 476, 878, 1331 */
	/* 
	0x1DC	=	476		= .text:1035C3CC A1 E8 1B 7D 10                          mov     eax, dword_107D1BE8
	0x36E	=	878 	= .text:1035C55E A1 E8 1B 7D 10                          mov     eax, dword_107D1BE8
	0x533	=	1331	= .text:1035C723 A1 E8 1B 7D 10                          mov     eax, dword_107D1BE8
	*/
#endif
#endif
#define NUM_FRAMETIME_READS (sizeof(g_FrameTimeReadOffsets)/sizeof(g_FrameTimeReadOffsets[0]))


#if defined (_WIN32)
#if defined (_L4D)
	// F3 0F 10 44 24 04 F3 0F 10 0D ? ? ? ? 0F 2F C1 76 10
	#define SIG_CNETCHAN_SETDATARATE "\xF3\x0F\x10\x44\x24\x04\xF3\x0F\x10\x0D\x2A\x2A\x2A\x2A\x0F\x2F\xC1\x76\x10"
	#define SIG_CNETCHAN_SETDATARATE_LEN 19
	// Jump from just after first instruction (6 bytes, loads argument float to xmm0)
	// to the lower-bound comparison instructions
	//(func+35 bytes, jump offset 35-6-2 = 27 = 0x1B)
	#define CNETCHAN_PATCH_OFFSET 6
	// movss
	#define CNETCHAN_PATCH_CHECK_BYTE 0xF3
	#define CNETCHAN_PATCH_JUMP_OFFSET 0x1B
#elif defined (_L4D2)
	// 55 8B EC F3 0F 10 45 08 F3 0F 10 0D ? ? ? ? 0F 2F C1 76
	#define SIG_CNETCHAN_SETDATARATE "\x55\x8B\xEC\xF3\x0F\x10\x45\x08\xF3\x0F\x10\x0D\x2A\x2A\x2A\x2A\x0F\x2F\xC1\x76"
	#define SIG_CNETCHAN_SETDATARATE_LEN 20
	#define CNETCHAN_PATCH_OFFSET 8
	#define CNETCHAN_PATCH_CHECK_BYTE 0xF3
	#define CNETCHAN_PATCH_JUMP_OFFSET 0x1C
#endif
#elif defined (_LINUX)
	#define SIG_CNETCHAN_SETDATARATE "_ZN8CNetChan11SetDataRateEf"
#if defined (_L4D)
	// Change comparison jump at +0x1E to NOP2, removing upper bound check.
	#define CNETCHAN_PATCH_OFFSET 0x1E
	#define CNETCHAN_PATCH_CHECK_BYTE JA_8_OPCODE
#elif defined (_L4D2)
	#define CNETCHAN_PATCH_OFFSET 0x18
	#define CNETCHAN_PATCH_CHECK_BYTE JB_8_OPCODE
#endif
#endif

#if defined (_WIN32)
#if defined (_L4D)
	// A1 ? ? ? ? 8B 40 30 85 C0 7E 0C 8B
	#define SIG_CGAMECLIENT_SETDATARATE "\xA1\x2A\x2A\x2A\x2A\x8B\x40\x30\x85\xC0\x7E\x0C\x8B"
	#define SIG_CGAMECLIENT_SETDATARATE_LEN 13
	// Offset 0x2F should be cmp eax, 30000; jle +0x06
	// Change to JMP +0x0B
	/*
	+0x2F
	3D 30 75 00 00		cmp     eax, 7530h
	7E 06				jle     short loc_10180DFC
	*/
	#define CGAMECLIENT_PATCH_OFFSET 0x2F
	#define CGAMECLIENT_PATCH_CHECK_BYTE 0x3D
	#define CGAMECLIENT_PATCH_JUMP_OFFSET 0x0B
#elif defined (_L4D2)
	//55 8B EC A1 ? ? ? ? 8B 40 30 85 C0 7E 0B
	#define SIG_CGAMECLIENT_SETDATARATE "\x55\x8B\xEC\xA1\x2A\x2A\x2A\x2A\x8B\x40\x30\x85\xC0\x7E\x0B"
	#define SIG_CGAMECLIENT_SETDATARATE_LEN 15
	#define CGAMECLIENT_PATCH_OFFSET 0x30
	#define CGAMECLIENT_PATCH_CHECK_BYTE 0x3D
	#define CGAMECLIENT_PATCH_JUMP_OFFSET 0x0C
#endif
#elif defined (_LINUX)
	#define SIG_CGAMECLIENT_SETDATARATE "_ZN11CGameClient7SetRateEib"
	//only for l4d1
	/*
	+0x4E
	B8 30 75 00 00 		mov     eax, 7530h
	81 FA 30 75 00 00	cmp     edx, 7530h
	0F 4E C2			cmovle  eax, edx
	
	Filling this with NOPs
	*/
	#define CGAMECLIENT_PATCH_OFFSET 0x4D
	#define CGAMECLIENT_PATCH_CHECK_BYTE MOV_R32_IMM32_OPCODE
#endif

#if defined (_WIN32)
#if defined (_L4D2)
	//55 8B EC 8B 45 08 53 8B 5D 0C 84 DB 75
	#define SIG_CLAMPCLIENTRATE "\x55\x8B\xEC\x8B\x45\x08\x53\x8B\x5D\x0C\x84\xDB\x75"
	#define SIG_CLAMPCLIENTRATE_LEN 13
	// Jump over the upper bound check
	#define CLAMPCLIENTRATE_PATCH_CHECK_BYTE 0x3D
	#define CLAMPCLIENTRATE_PATCH_OFFSET 0x34
	#define CLAMPCLIENTRATE_PATCH_JUMP_OFFSET 0x16
#endif
#elif defined (_LINUX)
	#define SIG_CLAMPCLIENTRATE "_Z15ClampClientRatei"
	/*
	+CLAMPCLIENTRATE_PATCH_OFFSET:
	B8 30 75 00 00 		mov     eax, 7530h
	81 FA 30 75 00 00	cmp     edx, 7530h
	0F 4E C2			cmovle  eax, edx
	*/
	#define CLAMPCLIENTRATE_PATCH_CHECK_BYTE MOV_R32_IMM32_OPCODE
#if defined (_L4D)
	#define CLAMPCLIENTRATE_PATCH_OFFSET 0x40
#elif defined (_L4D2)
	#define CLAMPCLIENTRATE_PATCH_OFFSET 0x38
#endif
#endif

#endif // _SIGS_H_