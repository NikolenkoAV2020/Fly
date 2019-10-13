//-------------------------------------------------------------------------------
// TaskFlightKanopus.cpp
// А. Николенко 31.01.2019
//-------------------------------------------------------------------------------
#include "stdafx.h"

//-------------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>		// Собственоно сама модель движения
#include <FlyMMath.h>		// Математические функции
#include <FlyOrbitDEsign.h>	// 

//-------------------------------------------------------------------------------
#include "FlightThisAndThat.h"

//-------------------------------------------------------------------------------
typedef struct {
	int  id ;
	char FileNU[512] ;
	char FilePrintSticker[64] ;
	void SetFileNU    (char* FN) {
		sprintf(FileNU, "%s\0", FN) ;
	} ;
	void SetStickerPrn(char* Sticker) {
		sprintf(FilePrintSticker, "%s\0", Sticker) ;
	} ;
} ZCalcOneID ;

//-------------------------------------------------------------------------------
DWORD __stdcall CalcOne(void* IDP) 
{
	ZCalcOneID* CID = (ZCalcOneID*)IDP ;
	ZLSF LSF ;
	LSF.num    = 1 ;
	LSF.vgpz   = GPZ90 ;
	LSF.ngpz   = 16 ;
	LSF.mgpz   = 16 ;
	LSF.vatm   = ATMSMA81 ;  // ATMSMA81 ; ATMNULL
	LSF.vsvd   = 0 ;       
	LSF.varsp  = 0 ;
	LSF.vTide  = TIDESTEP2 ; // TIDENULL, TIDESTEP2, TIDEPOSEIDON
	LSF.isDU   = 0 ;
	LSF.isSun  = 1 ;
	LSF.isMoon = 1 ;
	memset(LSF.isPlanet, 0, sizeof(LSF.isPlanet )) ;

	int rc ;
	ZNU NU ;
	rc = LoadNU01( CID->FileNU, NULL, NULL, NU) ; RRC ;
	//sprintf(FilePrintSticker, (char*)CID->FilePrintSticker.c_str()) ;
	rc = FlightCalcPrediction       (NU, LSF, 7, CID->FilePrintSticker) ; RRC ;
	//rc = FlightCalcHightSimplified(NU, LSF, CID->FilePrintSticker) ; RRC ;
	//rc = FlightCalcHightContour   (NU, LSF, CID->FilePrintSticker) ; RRC ;

	return 0 ;
}

//-------------------------------------------------------------------------------
int TaskFlightKanopus()
{
	ZCalcOneID CID1 ;
	CID1.id = 1 ;
	CID1.SetFileNU("Форма НУ01 312.txt") ;
	CID1.SetStickerPrn("NU_1");
	DWORD  ThreadId = 0 ;
 	HANDLE hthr1 = CreateThread(NULL, 0, CalcOne, (void*)(&CID1), 0, &ThreadId) ;

//	ZCalcOneID CID2 ;
//	CID2.id = 2 ;
//	CID2.SetFileNU(".\\NU01\\Форма НУ01 313.txt") ;
//	CID2.SetStickerPrn("NU_2");
//	HANDLE hthr2 = CreateThread(NULL, 0, CalcOne, (void*)(&CID2), 0, NULL) ;

//	ZCalcOneID CID3 ;
//	CID3.id = 2 ;
//	CID3.SetFileNU("Форма НУ01 314.txt") ;
//	CID3.SetStickerPrn("NU_3");
//	HANDLE hthr3 = CreateThread(NULL, 0, CalcOne, (void*)(&CID3), 0, NULL) ;
	
	printf("The End ...\n") ;
	getch() ;
	return 0 ; 
}

//-------------------------------------------------------------------------------
