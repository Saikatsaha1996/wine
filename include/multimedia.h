/* -*- tab-width: 8; c-basic-offset: 4 -*- */

/*****************************************************************************
 * Copyright 1998, Luiz Otavio L. Zorzella
 *
 * File:      multimedia.h
 * Purpose:   multimedia declarations
 *
 *****************************************************************************
 */
#ifndef __WINE_MULTIMEDIA_H 
#define __WINE_MULTIMEDIA_H

#include "mmsystem.h"

#define MAX_MIDIINDRV 	(16)
/* For now I'm making 16 the maximum number of midi devices one can
 * have. This should be more than enough for everybody. But as a purist,
 * I intend to make it unbounded in the future, as soon as I figure
 * a good way to do so.
 */
#define MAX_MIDIOUTDRV 	(16)

#if defined(HAVE_SYS_SOUNDCARD_H)
# include <sys/soundcard.h>
#elif defined(HAVE_MACHINE_SOUNDCARD_H)
# include <machine/soundcard.h>
#elif defined(HAVE_SOUNDCARD_H)
# include <soundcard.h>
#endif

#include <sys/errno.h>

#ifdef HAVE_OSS
#define MIDI_SEQ "/dev/sequencer"
#else
#define MIDI_DEV "/dev/midi"
#endif

#ifdef SOUND_VERSION
#define IOCTL(a,b,c)		ioctl(a,b,&c)
#else
#define IOCTL(a,b,c)		(c = ioctl(a,b,c))
#endif

struct WINE_MCIDRIVER {
	HDRVR16			hDrv;
	DRIVERPROC16		driverProc;
	MCI_OPEN_DRIVER_PARMS16	modp;
	MCI_OPEN_PARMS16	mop;
	DWORD			dwPrivate;
        YIELDPROC		lpfnYieldProc;
        DWORD	                dwYieldData;
        BOOL			bIs32;
        HTASK16			hCreatorTask;
};

extern struct WINE_MCIDRIVER mciDrv[MAXMCIDRIVERS];

#define MCI_GetDrv(wDevID) 	(&mciDrv[MCI_DevIDToIndex(wDevID)])
#define MCI_GetOpenDrv(wDevID)	(&(MCI_GetDrv(wDevID)->mop))

/* function prototypes */
extern BOOL MULTIMEDIA_Init(void);

extern int    			MCI_DevIDToIndex(UINT16 wDevID);
extern UINT16 			MCI_FirstDevID(void);
extern UINT16 			MCI_NextDevID(UINT16 wDevID);
extern BOOL 			MCI_DevIDValid(UINT16 wDevID);

extern int			MCI_MapMsg16To32A(WORD uDevType, WORD wMsg, DWORD* lParam);
extern int			MCI_UnMapMsg16To32A(WORD uDevTyp, WORD wMsg, DWORD lParam);

extern DWORD 			MCI_Open(DWORD dwParam, LPMCI_OPEN_PARMSA lpParms);
extern DWORD 			MCI_Close(UINT16 wDevID, DWORD dwParam, LPMCI_GENERIC_PARMS lpParms);
extern DWORD 			MCI_SysInfo(UINT uDevID, DWORD dwFlags, LPMCI_SYSINFO_PARMSA lpParms);

typedef LONG			(*MCIPROC16)(DWORD, HDRVR16,  WORD, DWORD, DWORD);
typedef LONG			(*MCIPROC)(DWORD, HDRVR16, DWORD, DWORD, DWORD);

extern WORD		   	MCI_GetDevType(LPCSTR str);
extern DWORD			MCI_WriteString(LPSTR lpDstStr, DWORD dstSize, LPCSTR lpSrcStr);
extern const char* 		MCI_CommandToString(UINT16 wMsg);

extern int			mciInstalledCount;
extern int			mciInstalledListLen;
extern LPSTR			lpmciInstallNames;

typedef struct {
    WORD	uDevType;
    char*	lpstrName;
    MCIPROC	lpfnProc;
} MCI_WineDesc;

extern	MCI_WineDesc		MCI_InternalDescriptors[];

extern LRESULT			MCI_CleanUp(LRESULT dwRet, UINT wMsg, DWORD dwParam2, BOOL bIs32);

extern DWORD 			MCI_SendCommand(UINT wDevID, UINT16 wMsg, DWORD dwParam1, DWORD dwParam2);
extern DWORD 			MCI_SendCommandAsync(UINT wDevID, UINT wMsg, DWORD dwParam1, DWORD dwParam2, UINT size);

LONG 				MCIWAVE_DriverProc(DWORD dwDevID, HDRVR16 hDriv, DWORD wMsg, 
						   DWORD dwParam1, DWORD dwParam2);
LONG 				MCIMIDI_DriverProc(DWORD dwDevID, HDRVR16 hDriv, DWORD wMsg, 
						   DWORD dwParam1, DWORD dwParam2);
LONG				MCICDAUDIO_DriverProc(DWORD dwDevID, HDRVR16 hDriv, DWORD wMsg, 
						      DWORD dwParam1, DWORD dwParam2);
LONG				MCIANIM_DriverProc(DWORD dwDevID, HDRVR16 hDriv, DWORD wMsg, 
						   DWORD dwParam1, DWORD dwParam2);
LONG				MCIAVI_DriverProc(DWORD dwDevID, HDRVR16 hDriv, DWORD wMsg, 
						  DWORD dwParam1, DWORD dwParam2);

#endif /* __WINE_MULTIMEDIA_H */
