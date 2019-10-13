//-------------------------------------------------------------------------------
// ZCPrintFormat.cpp
//
// А. Николенко 03.08.2018
// Форматирование результатов прогноза
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCPrintManager.h>
#include <FlyCoreSource\\ZCPrintFormat.h>

//-------------------------------------------------------------------------------
ZCPrintFormat::ZCPrintFormat() 
{
	Formated() ;
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::Formated()
{
	vardimarg = 0 ;
	vardimBLF = 0 ;
	vardimTdr = 0 ;
	vardimR   = 0 ;
	vardimV   = 1 ;

	ApeakWidth = 18 ;

	decM      = 3 ;
	decR      = 6 ;
	decV      = 8 ;
	decTdrSec = 4 ;
	decTdrMin = 4 ;
	decSec    = 4 ;
	decMin    = 6 ;
	decSSS    = 9 ;
	decGrad   = 6 ;
	decRad    = 8 ;
	decASec   = 2 ;
	decAkm    = 6 ;
	decE      = 9 ;
	decHkm    = 3 ;
	decRkm    = 6 ;
	decVkm    = 8 ;
	decRm     = 4 ;
	decVm     = 6 ;

	lenM       = 8 ;
	lenVit     = 6 ;
	lenKR      = 8 ;
	lenKA      = 5 ;
	lenDate    = 13 ;
	lenTimeHMS = 11 ;
	lenTdrSec  = 8 ;
	lenTdrMin  = 6 ;
	lenAkm     = 8 ;
	lenE       = 4 ;
	lenGrad    = 7 ;
	lenRad     = 4 ;
	lenGMS     = 12 ;
	lenAkm	   = 8 ;
	lenE       = 4 ; 
	lenHkm     = 7 ;
	lenRkm     = 9 ;
	lenVkm     = 5 ;
	lenRm      = 12 ;
	lenVm      = 8 ;

	FDataTime.FormatedDateTime(0) ;
	FDataTime.SetFixedLastField(true) ; 
	FDataTime.SetDec(decSec) ;

	Ftmss.FormatedTime(0) ;
	Ftmss.SetFormatIO(EDF_SSS) ;
	Ftmss.SetFixedLastField(true) ; 
	Ftmss.SetDec(decSec) ;

	FArc.FormatedGMS(0) ;
	FArc.SetFixedLastField(true) ; 
	FArc.SetFixedFirstField(false) ;
	FArc.SetFormatIO(EDF_RAD) ;
	FArc.SetSign(true) ;
	FArc.SetDec(decASec) ;

	FArcBL.FormatedGMS(0) ;
	FArcBL.SetFixedLastField(true) ; 
	FArcBL.SetFixedFirstField(false) ;
	FArcBL.SetFormatIO(EDF_GRAD) ;
	FArcBL.SetSign(true) ;
	FArcBL.SetDec(decASec) ;

	FTdr.FormatedTimeHMS() ;
	FTdr.SetFixedLastField(true) ; 
	FTdr.SetFixedFirstField(false) ;
	FTdr.SetFormatIO(EDF_SSS) ;
	FTdr.SetInterval(true) ;
	FTdr.SetDec(decSec) ;
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintDateTime(ZCPrintManager* PrintMng, FILE* fp, char* S, double ta, char Key)
{ 
	char s[40] ; 
	FDataTime << ta ;
	switch(Key) {
		case 0: 
			if (S)
				sprintf(S, "%*s", PrintMng->MPK.IsAflat() ? (lenDate + lenTimeHMS + decSec) : ApeakWidth, FDataTime.GetText());
			else
				fprintf(fp, "%*s", PrintMng->MPK.IsAflat() ? (lenDate+lenTimeHMS+decSec):ApeakWidth, FDataTime.GetText()) ; 
			break ;
		case 1:
			FDataTime.GetDataText(s) ;
			if (S)
				sprintf(S, "%*s", PrintMng->MPK.IsAflat() ? lenDate : ApeakWidth, s);
			else 
				fprintf(fp, "%*s", PrintMng->MPK.IsAflat() ? lenDate:ApeakWidth, s) ;
			break ;
		case 2:
			FDataTime.GetTimeText(s) ;
			if (S)
				sprintf(S, "%*s", PrintMng->MPK.IsAflat() ? (lenTimeHMS + decSec) : ApeakWidth, s);
			else
				fprintf(fp, "%*s", PrintMng->MPK.IsAflat() ? (lenTimeHMS+decSec):ApeakWidth, s) ;
			break ;
	}
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintKA(ZCPrintManager* PrintMng, FILE* fp, char* S, int KA)
{ 
	if (S)
		sprintf(S, "%*ld", PrintMng->MPK.IsAflat() ? lenKA : ApeakWidth, KA);
	else 
		fprintf(fp, "%*ld", PrintMng->MPK.IsAflat() ? lenKA:ApeakWidth, KA ) ;
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintVit(ZCPrintManager* PrintMng, FILE* fp, char* S, int Vit)
{ 
	if (S)
		sprintf(S, "%*ld", PrintMng->MPK.IsAflat() ? lenVit : ApeakWidth, Vit);
	else 
		fprintf(fp, "%*ld", PrintMng->MPK.IsAflat() ? lenVit:ApeakWidth, Vit) ; 
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintM(ZCPrintManager* PrintMng, FILE* fp, char* S, double m)
{ 
	if (S)
		sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenM + decM) : ApeakWidth, decM, m);
	else
		fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenM+decM):ApeakWidth, decM, m) ; 
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintKR(ZCPrintManager* PrintMng, FILE* fp, char* S, int KR)
{ 
	if (S)
		sprintf(S, "%*ld", PrintMng->MPK.IsAflat() ? lenKR : ApeakWidth, KR);
	else
		fprintf(fp, "%*ld", PrintMng->MPK.IsAflat() ? lenKR:ApeakWidth, KR) ; 
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintMSSV(ZCPrintManager* PrintMng, FILE* fp, char* S, double t)
{ 
	Ftmss << t ;
	if (S)
		sprintf(S, "%*s", PrintMng->MPK.IsAflat() ? (lenTimeHMS + decSec) : ApeakWidth, Ftmss.GetText());
	else
		fprintf(fp, "%*s", PrintMng->MPK.IsAflat() ? (lenTimeHMS+decSec):ApeakWidth, Ftmss.GetText()) ;
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintHkm(ZCPrintManager* PrintMng, FILE* fp, char* S, double h)
{ 
	if (S)
		sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenHkm + decHkm) : ApeakWidth, decHkm, h);
	else
		fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenHkm+decHkm):ApeakWidth, decHkm, h) ; 
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintRkm(ZCPrintManager* PrintMng, FILE* fp, char* S, double R)
{ 
	if (S)
		sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenRkm + decRkm) : ApeakWidth, decRkm, R);
	else 
		fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenRkm+decRkm):ApeakWidth, decRkm, R) ; 
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintAkm(ZCPrintManager* PrintMng, FILE* fp, char* S, double a)
{ 
	if (S)
		sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenAkm + decAkm) : ApeakWidth, decAkm, a);
	else
		fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenAkm+decAkm):ApeakWidth, decAkm, a) ; 
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintE(ZCPrintManager* PrintMng, FILE* fp, char* S, double e)
{ 
	if (S)
		sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenE + decE) : ApeakWidth, decE, e);
	else 
		fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenE+decE):ApeakWidth, decE, e) ; 
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintR(ZCPrintManager* PrintMng, FILE* fp, char* S, double R)
{ 
	switch(vardimR) {
		case 0 : 
			if (S)
				sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenRkm + decRkm) : ApeakWidth, decRkm, R); 
			else
				fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenRkm+decRkm):ApeakWidth, decRkm, R) ; 
			break;
		default: 
			if (S)
				sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenRm + decRm) : ApeakWidth, decRm, 1e3*R);
			else
				fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenRm+decRm  ):ApeakWidth, decRm, 1e3*R) ; 
	}
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintV(ZCPrintManager* PrintMng, FILE* fp, char* S, double V)
{
	switch(vardimV) {
		case 0 : 
			if (S)
				sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenVkm + decVkm) : ApeakWidth, decVkm, V);
			else
				fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenVkm+decVkm):ApeakWidth, decVkm, V) ; 
			break ;
		default: 
			if (S)
				sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenVm + decVm) : ApeakWidth, decVm, 1e3*V);
			else
				fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenVm+decVm  ):ApeakWidth, decVm, 1e3*V) ; 
	}
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintTdr(ZCPrintManager* PrintMng, FILE* fp, char* S, double T)
{ 
 
	switch (vardimTdr) {
	case 0: 
		if (S)
			sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenTdrSec + decTdrSec) : ApeakWidth, decTdrSec, T);
		else
			fprintf(fp, "%*.*lf",  PrintMng->MPK.IsAflat() ? (lenTdrSec+decTdrSec):   ApeakWidth, decTdrSec, T) ; 
		break;  
	case 1: 
		if (S)
			sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenTdrMin + decTdrMin) : ApeakWidth, decTdrMin, T / 60.0);
		else
			fprintf(fp, "%*.*lf",  PrintMng->MPK.IsAflat() ? (lenTdrMin+decTdrMin):   ApeakWidth, decTdrMin, T/60.0) ; 
		break;  
	case 2:	
		FTdr << (T / k_cbc);
		if (S)
			sprintf(S, "%*s", PrintMng->MPK.IsAflat() ? (1 + lenTimeHMS + decTdrSec) : ApeakWidth, FTdr.GetText());
		else
			fprintf(fp, "%*s", PrintMng->MPK.IsAflat() ? (1+lenTimeHMS+decTdrSec):ApeakWidth, FTdr.GetText()) ; 
		break;  
	}
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintGrad(ZCPrintManager* PrintMng, FILE* fp, char* S, double a)
{ 
	switch (vardimarg) {
	case 0: 
		if (S)
			sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenGrad + decGrad) : ApeakWidth, decGrad, TO_GRAD(a));
		else
			fprintf(fp, "%*.*lf",          PrintMng->MPK.IsAflat() ? (lenGrad+decGrad):ApeakWidth, decGrad, TO_GRAD(a)) ; 
		break;  
	case 1: 
		if (S)
			sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenRad + decRad) : ApeakWidth, decRad, a);
		else
			fprintf(fp, "%*.*lf",          PrintMng->MPK.IsAflat() ? (lenRad+decRad  ):ApeakWidth, decRad, a) ; 
		break;  
	case 2:	
		FArc << a ;	
		if (S)
			sprintf(S, "%*s", PrintMng->MPK.IsAflat() ? (lenGMS + decASec) : ApeakWidth, FArc.GetText());
		else
			fprintf(fp, "%*s", PrintMng->MPK.IsAflat() ? (lenGMS+decASec ):ApeakWidth, FArc.GetText()) ; 
		break;  
	}
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintBLF(ZCPrintManager* PrintMng, FILE* fp, char* S, double a)
{ 
	switch (vardimBLF) {
	case 0: 
		if (S)
			sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenGrad + decGrad) : ApeakWidth, decGrad, a);
		else
			fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenGrad+decGrad):ApeakWidth, decGrad, a) ; 
		break;  
	case 1: 
		if (S)
			sprintf(S, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenRad + decRad) : ApeakWidth, decRad, TO_RAD(a));
		else
			fprintf(fp, "%*.*lf", PrintMng->MPK.IsAflat() ? (lenRad+decRad  ):ApeakWidth, decRad,TO_RAD(a)) ; 
		break;  
	case 2: 
		FArcBL << a ; 
		if (S)
			sprintf(S, "%*s", PrintMng->MPK.IsAflat() ? (lenGMS + decASec) : ApeakWidth, FArcBL.GetText());
		else
			fprintf(fp, "%*s", PrintMng->MPK.IsAflat() ? (lenGMS+decASec ):ApeakWidth, FArcBL.GetText()) ; 
		break;  
	}
}

//-------------------------------------------------------------------------------
void ZCPrintFormat::PrintLineAflet(FILE* fp, int Len)
{ 
	char S[] = "____________________" ;
	while(Len > 20) { 
		if (fp) fprintf(fp, "%s", S) ; 
		else printf("%s", S) ;  
		Len-= 20 ; 
	}
	while(Len > 0 ) { 
		if (fp) fprintf(fp, "_") ;
		else printf("_") ; 
		Len-= 1 ; 
	}
	if (fp) fprintf(fp, "\n") ;
	else printf("\n") ;
}

//-------------------------------------------------------------------------------
int ZCPrintFormat::CalcLineLenght(ZCPrintManager* PrintMng)
{
	int Len = 0 ;
	int N = PrintMng->MPK.SizeF() ;
	for(int i=0 ; i<N ; i++) Len+= CalcItemLenght(PrintMng, i) ; 
	return Len ;
}

//-------------------------------------------------------------------------------
int ZCPrintFormat::GetIDName(ZCPrintManager* PrintMng, string& name, 
							 int ID, bool fromHtml)
{
	int  Len = 0;

	switch (ID) {
	case MODPR_M:		name = "Масса КА"; break;
	case MODPRDATETIME:	name = "Дата-Время"; break;
	case MODPRDATE:		name = "Дата"; break;
	case MODPRTIME:		name = "Время"; break;
	case MODPR_V_VUdatetime:name = "Дата-Время ВУ"; break;
	case MODPR_V_VUdate:	name = "Дата ВУ"; break;
	case MODPR_V_VUtime:	name = "Время ВУ"; break;
	case MODPR_V_NUdatetime:name = "Дата-Время НУ"; break;
	case MODPR_V_NUdate:	name = "Дата НУ"; break;
	case MODPR_V_NUtime:	name = "Время НУ"; break;
	case MODPR_KA:			name = "КА"; break;
	case MODPR_VIT:			name = "Виток"; break;
	case MODPR_MSSV:		name = "МССВ"; break;
	case MODPR_Tosk:		name = "Tоск"; break;
	case MODPR_V_Tdr:		name = "Tдр"; break;
	case MODPR_R:			name = "Радиус"; break;
	case MODPR_H:			name = "Высота"; break;
	case MODPR_B:			name = "Широта"; break;
	case MODPR_L:			name = "Долгота"; break;
	case MODPR_V_DL:		name = fromHtml ? "МВ L<sub>ву</sub>" : "МВ Lву"; break;
	case MODPR_V_DLS:		name = fromHtml ? "Сут.см L<sub>ву</sub>" : "Сут.см Lву"; break;
	case MODPR_V_VUL:		name = fromHtml ? "L<sub>ву</sub>" : "Lву"; break;
	case MODPR_V_NUL:		name = fromHtml ? "L<sub>ну</sub>" : "Lну"; break;
	case MODPR_V_HminB:		name = fromHtml ? "B h<sub>min</sub>" : "B hmin"; break;
	case MODPR_V_HminL:		name = fromHtml ? "L h<sub>min</sub>" : "L hmin"; break;
	case MODPR_V_HmaxB:		name = fromHtml ? "B h<sub>max</sub>" : "B hmax"; break;
	case MODPR_V_HmaxL:		name = fromHtml ? "L h<sub>max</sub>" : "L hmax"; break;
	case MODPR_V_RminB:		name = fromHtml ? "B R<sub>min</sub>" : "B Rmin"; break;
	case MODPR_V_RminL:		name = fromHtml ? "L R<sub>min</sub>" : "L Rmin"; break;
	case MODPR_V_HminU:		name = fromHtml ? "u H<sub>min</sub>" : "U hmin"; break;
	case MODPR_V_HmaxU:		name = fromHtml ? "u H<sub>max</sub>" : "U hmax"; break;
	case MODPR_V_RminU:		name = fromHtml ? "u R<sub>min</sub>" : "U Rmin"; break;
	case MODPR_V_RmaxU:		name = fromHtml ? "u R<sub>max</sub>" : "U Rmax"; break;
	case MODPR_V_RmaxB:		name = fromHtml ? "B R<sub>max</sub>" : "B Rmax"; break;
	case MODPR_V_RmaxL:		name = fromHtml ? "L R<sub>max</sub>" : "L Rmax"; break;
	case MODPR_V_Hsr:		name = fromHtml ? "H<sub>ср</sub>" : "H ср."; break;
	case MODPR_V_VUH:		name = fromHtml ? "H<sub>ву</sub>" : "Hву"; break;
	case MODPR_V_NUH:		name = fromHtml ? "H<sub>ну</sub>" : "Hну"; break;
	case MODPR_V_Hmin:		name = fromHtml ? "H<sub>min</sub>" : "Hmin"; break;
	case MODPR_V_Hmax:		name = fromHtml ? "H<sub>max</sub>" : "Hmax"; break;
	case MODPR_Aosk:		name = "a"; break;
	case MODPR_eosk:		name = "e"; break;
	case MODPR_exosk:		name = fromHtml ? "e<sub>x</sub>" : "ex"; break;
	case MODPR_eyosk:		name = fromHtml ? "e<sub>y</sub>" : "ey"; break;
	case MODPR_V_KR:		name = "Крат."; break;
	case MODPR_V_VUR:		name = fromHtml ? "R<sub>ву</sub>" : "Rву"; break;
	case MODPR_V_NUR:		name = fromHtml ? "R<sub>ну</sub>" : "Rну"; break;
	case MODPR_V_Rmin:		name = fromHtml ? "R<sub>min</sub>" : "Rmin"; break;
	case MODPR_V_Rmax:		name = fromHtml ? "R<sub>max</sub>" : "Rmax"; break;
	case MODPR_T_R75:
	case MODPR_T_R00:
	case MODPR_T_RTE:
	case MODPR_T_RTES:
	case MODPR_T_RGSC: {
		Len = CalcIDLenght(PrintMng, ID) / 3;
		char SK[15];
		switch (ID){
		case MODPR_T_R75:	sprintf(SK, "%s", "75"); break;
		case MODPR_T_R00:	sprintf(SK, "%s", "00"); break;
		case MODPR_T_RTE:	sprintf(SK, "%s", "ТЭ"); break;
		case MODPR_T_RTES:	sprintf(SK, "%s", "ТЭС"); break;
		case MODPR_T_RGSC:	sprintf(SK, "%s", "ГСК"); break;
		}
		char S[64];
		char N[256];
		for (int i = 0; i<3; i++) {
			if (fromHtml) {
				if (i == 0) sprintf(S, "%s<sub>%s</sub>", "X", SK);
				if (i == 1) sprintf(S, "%s<sub>%s</sub>", "Y", SK);
				if (i == 2) sprintf(S, "%s<sub>%s</sub>", "Z", SK);
				sprintf(&N[strlen(N)], "%*s\0", Len + 11, S);
			} else {
				if (i == 0) sprintf(S, "%s-%s", "X", SK);
				if (i == 1) sprintf(S, "%s-%s", "Y", SK);
				if (i == 2) sprintf(S, "%s-%s", "Z", SK);
				sprintf(&N[strlen(N)], "%*s\0", Len, S);
			}
		}
		name = N;
	} break;
	case MODPR_T_V75:
	case MODPR_T_V00:
	case MODPR_T_VTE:
	case MODPR_T_VTES:
	case MODPR_T_VGSC: {
		Len = CalcIDLenght(PrintMng, ID) / 3;
		char SK[15];
		switch (ID){
		case MODPR_T_V75:	sprintf(SK, "%s", "75"); break;
		case MODPR_T_V00:	sprintf(SK, "%s", "00"); break;
		case MODPR_T_VTE:	sprintf(SK, "%s", "ТЭ"); break;
		case MODPR_T_VTES:	sprintf(SK, "%s", "ТЭС"); break;
		case MODPR_T_VGSC:	sprintf(SK, "%s", "ГСК"); break;
		}
		char S[64];
		char N[256];
		for (int i = 0; i<3; i++) {
			if (fromHtml) {
				if (i == 0) sprintf(S, "%s<sub>x%s</sub>", "V", SK);
				if (i == 1) sprintf(S, "%s<sub>y%s</sub>", "V", SK);
				if (i == 2) sprintf(S, "%s<sub>z%s</sub>", "V", SK);
				sprintf(&N[strlen(N)], "%*s\0", Len + 11, S);
			}
			else {
				if (i == 0) sprintf(S, "%s-%s", "Vx", SK);
				if (i == 1) sprintf(S, "%s-%s", "Vy", SK);
				if (i == 2) sprintf(S, "%s-%s", "Vz", SK);
				sprintf(&N[strlen(N)], "%*s\0", Len, S);
			}
		}
		name = N;
	} break;
	case MODPR_T_X75:		name = fromHtml ? "X<sub>75</sub>" : "X-75"; break;
	case MODPR_T_Y75:		name = fromHtml ? "Y<sub>75</sub>" : "Y-75"; break;
	case MODPR_T_Z75:		name = fromHtml ? "Z<sub>75</sub>" : "Z-75"; break;
	case MODPR_T_VX75:		name = fromHtml ? "V<sub>x75</sub>" : "Vx-75"; break;
	case MODPR_T_VY75:		name = fromHtml ? "V<sub>y75</sub>" : "Vy-75"; break;
	case MODPR_T_VZ75:		name = fromHtml ? "V<sub>z75</sub>" : "Vz-75"; break;
	case MODPR_T_X00:		name = fromHtml ? "X<sub>00</sub>" : "X-00"; break;
	case MODPR_T_Y00:		name = fromHtml ? "Y<sub>00</sub>" : "Y-00"; break;
	case MODPR_T_Z00:		name = fromHtml ? "Z<sub>00</sub>" : "Z-00"; break;
	case MODPR_T_VX00:		name = fromHtml ? "V<sub>x00</sub>" : "Vx-00"; break;
	case MODPR_T_VY00:		name = fromHtml ? "V<sub>y00</sub>" : "Vy-00"; break;
	case MODPR_T_VZ00:		name = fromHtml ? "V<sub>z00</sub>" : "Vz-00"; break;
	case MODPR_T_XTE:		name = fromHtml ? "X<sub>ТЕ</sub>" : "X-ТЕ"; break;
	case MODPR_T_YTE:		name = fromHtml ? "Y<sub>ТЕ</sub>" : "Y-ТЕ"; break;
	case MODPR_T_ZTE:		name = fromHtml ? "Z<sub>ТЕ</sub>" : "Z-ТЕ"; break;
	case MODPR_T_VXTE:		name = fromHtml ? "V<sub>xТЕ</sub>" : "Vx-ТЕ"; break;
	case MODPR_T_VYTE:		name = fromHtml ? "V<sub>yТЕ</sub>" : "Vy-ТЕ"; break;
	case MODPR_T_VZTE:		name = fromHtml ? "V<sub>zТЕ</sub>" : "Vz-ТЕ"; break;
	case MODPR_T_XTES:		name = fromHtml ? "X<sub>ТЕС</sub>" : "X-ТЕС"; break;
	case MODPR_T_YTES:		name = fromHtml ? "Y<sub>ТЕС</sub>" : "Y-ТЕС"; break;
	case MODPR_T_ZTES:		name = fromHtml ? "Z<sub>ТЕС</sub>" : "Z-ТЕС"; break;
	case MODPR_T_VXTES:		name = fromHtml ? "V<sub>xТЕС</sub>" : "Vx-ТЕС"; break;
	case MODPR_T_VYTES:		name = fromHtml ? "V<sub>yТЕС</sub>" : "Vy-ТЕС"; break;
	case MODPR_T_VZTES:		name = fromHtml ? "V<sub>zТЕС</sub>" : "Vz-ТЕС"; break;
	case MODPR_T_XGSC:		name = fromHtml ? "X<sub>ГСК</sub>" : "X-ГСК"; break;
	case MODPR_T_YGSC:		name = fromHtml ? "Y<sub>ГСК</sub>" : "Y-ГСК"; break;
	case MODPR_T_ZGSC:		name = fromHtml ? "Z<sub>ГСК</sub>" : "Z-ГСК"; break;
	case MODPR_T_VXGSC:		name = fromHtml ? "V<sub>xГСК</sub>" : "Vx-ГСК"; break;
	case MODPR_T_VYGSC:		name = fromHtml ? "V<sub>yГСК</sub>" : "Vy-ГСК"; break;
	case MODPR_T_VZGSC:		name = fromHtml ? "V<sub>zГСК</sub>" : "Vz-ГСК"; break;
	case MODPR_T_i75:		name = fromHtml ? "i<sub>75</sub>" : "i-75"; break;
	case MODPR_T_W75:		name = fromHtml ? "L<sub>75</sub>" : "Lабс-75"; break;
	case MODPR_T_w75:		name = fromHtml ? "<font face=\"symbol\">w</font><sub>75</sub>" : "w-75"; break;
	case MODPR_T_u75:		name = fromHtml ? "u<sub>75</sub>" : "u-75"; break;
	case MODPR_T_i00:		name = fromHtml ? "i<sub>00</sub>" : "i-00"; break;
	case MODPR_T_W00:		name = fromHtml ? "L<sub>00</sub>" : "Lабс-00"; break;
	case MODPR_T_w00:		name = fromHtml ? "<font face=\"symbol\">w</font><sub>00</sub>" : "w-00"; break;
	case MODPR_T_u00:		name = fromHtml ? "u<sub>00</sub>" : "u-00"; break;
	case MODPR_T_iTE:		name = fromHtml ? "i<sub>ТЕ</sub>" : "i-ТЕ"; break;
	case MODPR_T_WTE:		name = fromHtml ? "L<sub>ТЕ</sub>" : "Lабс-ТЕ"; break;
	case MODPR_T_wTE:		name = fromHtml ? "<font face=\"symbol\">w</font><sub>ТЕ</sub>" : "w-ТЕ"; break;
	case MODPR_T_uTE:		name = fromHtml ? "u<sub>ТЕ</sub>" : "u-ТЕ"; break;
	case MODPR_T_iTES:		name = fromHtml ? "i<sub>ТЭС</sub>" : "i-ТЕС"; break;
	case MODPR_T_WTES:		name = fromHtml ? "L<sub>ТЭС</sub>" : "Lабс-ТЕС"; break;
	case MODPR_T_wTES:		name = fromHtml ? "<font face=\"symbol\">w</font><sub>ТЭС</sub>" : "w-ТЕС"; break;
	case MODPR_T_uTES:		name = fromHtml ? "u<sub>ТЭС</sub>" : "u-ТЕС"; break;
	case MODPR_V_Hmint:		name = fromHtml ? "Время H<sub>min</sub>" : "Время Hmin"; break;
	case MODPR_V_Hmaxt:		name = fromHtml ? "Время H<sub>max</sub>" : "Время Hmax"; break;
	case MODPR_V_Rmint:		name = fromHtml ? "Время R<sub>min</sub>" : "Время Rmin"; break;
	case MODPR_V_Rmaxt:		name = fromHtml ? "Время R<sub>max</sub>" : "Время Rmax"; break;
	case MODPR_V_VUmssv:	name = "МССВ ВУ"; break;
	case MODPR_V_NUmssv:	name = "МССВ НУ"; break;
	}
	return 0;
}

//-------------------------------------------------------------------------------
int ZCPrintFormat::GetItemName(ZCPrintManager* PrintMng, string& name, 
							   int ParamIndex, bool fromHtml)
{
	int  Len = 0 ;
	int  N   = PrintMng->MPK.Size() ;
	int  NF  = PrintMng->MPK.SizeF() ;
	assert(ParamIndex<NF) ;
	if (ParamIndex>=N) {
		ParamIndex-=N ;
		ZSUserPrintParam SS = PrintMng->MPK.UPP(ParamIndex);
		name = fromHtml ? 
			   PrintMng->MPK.UPP(ParamIndex).namehtml :
			   PrintMng->MPK.UPP(ParamIndex).name;
		return 0;
	}
	int  ID  = PrintMng->MPK[ParamIndex] ;
	return(GetIDName(PrintMng, name, ID, fromHtml));
}

//-------------------------------------------------------------------------------
int ZCPrintFormat::GetItemID(ZCPrintManager* PrintMng, int ParamIndex)
{
	int  N   = PrintMng->MPK.Size() ;
	int  NF  = PrintMng->MPK.SizeF() ;
	assert(ParamIndex<NF) ;
	if (ParamIndex>=N) return-1 ;
	return(PrintMng->MPK[ParamIndex]) ;
}

//-------------------------------------------------------------------------------
int ZCPrintFormat::GetIDDim(ZCPrintManager* PrintMng, string& name, 
							int ID, bool fromHtml)
{
	name = "";
	// Выбор размерности по идентификатору
	switch (ID) {
	case MODPRDATETIME:
	case MODPR_V_VUdatetime:
	case MODPR_V_NUdatetime:
						name = "ч.м.с."; break;
	case MODPRDATE:
	case MODPR_V_VUdate:
	case MODPR_V_NUdate:name = ""; break;
	case MODPRTIME:
	case MODPR_V_VUtime:
	case MODPR_V_NUtime:name = "ч.м.с."; break;
	case MODPR_KA:		name = ""; break;
	case MODPR_VIT:		name = ""; break;
	case MODPR_M:		name = "кг."; break;
	case MODPR_MSSV:	name = "ч.м.с."; break;
	case MODPR_Tosk:
	case MODPR_V_Tdr:
		switch (vardimTdr) {
		case 0:			name = "сек."; break;
		case 1:			name = "мин."; break;
		case 2:			name = "ч.м.с."; break;
		}
		break;
	case MODPR_R:
	case MODPR_H:		name = "км."; break;
	case MODPR_B:
	case MODPR_L:
	case MODPR_V_DL:
	case MODPR_V_DLS:
	case MODPR_V_VUL:
	case MODPR_V_NUL:
	case MODPR_V_HminB:
	case MODPR_V_HminL:
	case MODPR_V_HmaxB:
	case MODPR_V_HmaxL:
	case MODPR_V_RminB:
	case MODPR_V_RminL:
	case MODPR_V_HminU:
	case MODPR_V_HmaxU:
	case MODPR_V_RminU:
	case MODPR_V_RmaxU:
	case MODPR_V_RmaxB:
	case MODPR_V_RmaxL:
		switch (vardimBLF) {
		case 0:			name = "град."; break;
		case 1:			name = "рад."; break;
		case 2:			name = "г.м.с."; break;
		}
		break;
	case MODPR_V_Hsr:
	case MODPR_V_VUH:
	case MODPR_V_NUH:
	case MODPR_V_Hmin:
	case MODPR_V_Hmax:	name = "км."; break;
	case MODPR_Aosk:	name = "км."; break;
	case MODPR_eosk:
	case MODPR_exosk:
	case MODPR_eyosk:
	case MODPR_V_KR:	name = ""; break;

	case MODPR_V_VUR:
	case MODPR_V_NUR:
	case MODPR_V_Rmin:
	case MODPR_V_Rmax:
		switch (vardimR) {
		case 0:		name = "км."; break;
		default:	name = "м."; break;
		}
		break;
	case MODPR_T_R75:
	case MODPR_T_R00:
	case MODPR_T_RTE:
	case MODPR_T_RTES:
	case MODPR_T_RGSC:
	case MODPR_T_X75:
	case MODPR_T_Y75:
	case MODPR_T_Z75:
	case MODPR_T_X00:
	case MODPR_T_Y00:
	case MODPR_T_Z00:
	case MODPR_T_XTE:
	case MODPR_T_YTE:
	case MODPR_T_ZTE:
	case MODPR_T_XTES:
	case MODPR_T_YTES:
	case MODPR_T_ZTES:
	case MODPR_T_XGSC:
	case MODPR_T_YGSC:
	case MODPR_T_ZGSC:
		switch (vardimR) {
		case 0:	 name = "км."; break;
		default: name = "м."; break;
		}
		break;
	case MODPR_T_V75:
	case MODPR_T_V00:
	case MODPR_T_VTE:
	case MODPR_T_VTES:
	case MODPR_T_VGSC:
	case MODPR_T_VX75:
	case MODPR_T_VY75:
	case MODPR_T_VZ75:
	case MODPR_T_VX00:
	case MODPR_T_VY00:
	case MODPR_T_VZ00:
	case MODPR_T_VXTE:
	case MODPR_T_VYTE:
	case MODPR_T_VZTE:
	case MODPR_T_VXTES:
	case MODPR_T_VYTES:
	case MODPR_T_VZTES:
	case MODPR_T_VXGSC:
	case MODPR_T_VYGSC:
	case MODPR_T_VZGSC:
		switch (vardimV) {
		case 0:		name = "км/сек"; break;
		default:	name = "м/сек"; break;
		}
		break;
	case MODPR_T_i75:
	case MODPR_T_W75:
	case MODPR_T_w75:
	case MODPR_T_u75:
	case MODPR_T_i00:
	case MODPR_T_W00:
	case MODPR_T_w00:
	case MODPR_T_u00:
	case MODPR_T_iTE:
	case MODPR_T_WTE:
	case MODPR_T_wTE:
	case MODPR_T_uTE:
	case MODPR_T_iTES:
	case MODPR_T_WTES:
	case MODPR_T_wTES:
	case MODPR_T_uTES:
		switch (vardimarg) {
		case 0:			name = "град."; break;
		case 1:			name = "рад."; break;
		case 2:			name = "г.м.с."; break;
		}
		break;
	case MODPR_V_Hmint:
	case MODPR_V_Hmaxt:
	case MODPR_V_Rmint:
	case MODPR_V_Rmaxt:
	case MODPR_V_VUmssv:
	case MODPR_V_NUmssv:name = "ч.м.с."; break;
	}
	return 0;
}

//-------------------------------------------------------------------------------
int ZCPrintFormat::GetItemDim(ZCPrintManager* PrintMng, string& name, 
							  int ParamIndex, bool fromHtml)
{
	if (!PrintMng->MPK.IsAflat()) {
		name = "";  return 0;
	}

	int N   = PrintMng->MPK.Size() ;	// Число заданных параметров документирования	
	int NF  = PrintMng->MPK.SizeF() ;	// Число параметров документирования
	if (ParamIndex >= NF) {				// вместе с пользовательскими параметрами 
		name = ""; return 0;		
	}

	// Если параметр пользовательский
	if (ParamIndex>=N) {
		// Индекс в массиве пользоватеьлских параметров
		ParamIndex-=N ;
		// Возврат размерности из массива пользовательских параметров
		name = fromHtml ?
			   PrintMng->MPK.UPP(ParamIndex).namedimhtml :
			   PrintMng->MPK.UPP(ParamIndex).namedim;
		return 0 ;
	}

	// Идентификатор параметра
	int ID = PrintMng->MPK[ParamIndex] ;
	// Выбор размерности по идентификатору
	return (GetIDDim(PrintMng, name, ID, fromHtml));
}

//-------------------------------------------------------------------------------
int ZCPrintFormat::CalcIDLenght(ZCPrintManager* PrintMng, int ID)
{
	if (!PrintMng->MPK.IsAflat()) return ApeakWidth;
	int Len = 0;

	switch (ID) {
	case MODPRDATETIME:
	case MODPR_V_VUdatetime:
	case MODPR_V_NUdatetime:
						Len = lenDate + lenTimeHMS + decSec; break;
	case MODPRDATE:
	case MODPR_V_VUdate:
	case MODPR_V_NUdate:Len = lenDate; break;
	case MODPRTIME:
	case MODPR_V_VUtime:
	case MODPR_V_NUtime:Len = lenTimeHMS + decSec; break;
	case MODPR_KA:		Len = lenKA; break;
	case MODPR_VIT:		Len = lenVit; break;
	case MODPR_M:		Len = lenM + decM; break;
	case MODPR_MSSV:	Len = lenTimeHMS + decSec; break;
	case MODPR_Tosk:
	case MODPR_V_Tdr:
		switch (vardimTdr) {
		case 0:			Len = lenTdrSec + decTdrSec; break;
		case 1:			Len = lenTdrMin + decTdrMin; break;
		case 2:			Len = 1 + lenTimeHMS + decTdrSec; break;
		}
		break;
	case MODPR_R:		Len = lenRkm + decRkm; break;
	case MODPR_H:		Len = lenHkm + decHkm; break;
	case MODPR_B:
	case MODPR_L:
	case MODPR_V_DL:
	case MODPR_V_DLS:
	case MODPR_V_VUL:
	case MODPR_V_NUL:
	case MODPR_V_HminB:
	case MODPR_V_HminL:
	case MODPR_V_HmaxB:
	case MODPR_V_HmaxL:
	case MODPR_V_RminB:
	case MODPR_V_RminL:
	case MODPR_V_HminU:
	case MODPR_V_HmaxU:
	case MODPR_V_RminU:
	case MODPR_V_RmaxU:
	case MODPR_V_RmaxB:
	case MODPR_V_RmaxL:
		switch (vardimBLF) {
		case 0:	Len = lenGrad + decGrad; break;
		case 1:	Len = lenRad + decRad; break;
		case 2:	Len = lenGMS + decASec; break;
		}
		break;
	case MODPR_V_Hsr:
	case MODPR_V_VUH:
	case MODPR_V_NUH:
	case MODPR_V_Hmin:
	case MODPR_V_Hmax:	Len = lenHkm + decHkm; break;
	case MODPR_Aosk:	Len = lenAkm + decAkm; break;
	case MODPR_eosk:	Len = lenE + decE;; break;
	case MODPR_exosk:	Len = lenE + decE;; break;
	case MODPR_eyosk:	Len = lenE + decE;; break;

	case MODPR_V_KR:	Len = lenKR; break;
	case MODPR_V_VUR:
	case MODPR_V_NUR:
	case MODPR_V_Rmin:
	case MODPR_V_Rmax:
		switch (vardimR) {
		case 0:	Len = lenRkm + decRkm; break;
		default:Len = lenRm + decRm;
		}
		break;
	case MODPR_T_R75:
	case MODPR_T_R00:
	case MODPR_T_RTE:
	case MODPR_T_RTES:
	case MODPR_T_RGSC:
	case MODPR_T_X75:
	case MODPR_T_Y75:
	case MODPR_T_Z75:
	case MODPR_T_X00:
	case MODPR_T_Y00:
	case MODPR_T_Z00:
	case MODPR_T_XTE:
	case MODPR_T_YTE:
	case MODPR_T_ZTE:
	case MODPR_T_XTES:
	case MODPR_T_YTES:
	case MODPR_T_ZTES:
	case MODPR_T_XGSC:
	case MODPR_T_YGSC:
	case MODPR_T_ZGSC:
		switch (vardimR) {
		case 0:	Len = 3 * (lenRkm + decRkm); break;
		default:Len = 3 * (lenRm + decRm);
		}
		break;
	case MODPR_T_V75:
	case MODPR_T_V00:
	case MODPR_T_VTE:
	case MODPR_T_VTES:
	case MODPR_T_VGSC:
	case MODPR_T_VX75:
	case MODPR_T_VY75:
	case MODPR_T_VZ75:
	case MODPR_T_VX00:
	case MODPR_T_VY00:
	case MODPR_T_VZ00:
	case MODPR_T_VXTE:
	case MODPR_T_VYTE:
	case MODPR_T_VZTE:
	case MODPR_T_VXTES:
	case MODPR_T_VYTES:
	case MODPR_T_VZTES:
	case MODPR_T_VXGSC:
	case MODPR_T_VYGSC:
	case MODPR_T_VZGSC:
		switch (vardimV) {
		case 0:	Len = 3 * (lenVkm + decVkm); break;
		default:Len = 3 * (lenVm + decVm);
		}
		break;
	case MODPR_T_i75:
	case MODPR_T_W75:
	case MODPR_T_w75:
	case MODPR_T_u75:
	case MODPR_T_i00:
	case MODPR_T_W00:
	case MODPR_T_w00:
	case MODPR_T_u00:
	case MODPR_T_iTE:
	case MODPR_T_WTE:
	case MODPR_T_wTE:
	case MODPR_T_uTE:
	case MODPR_T_iTES:
	case MODPR_T_WTES:
	case MODPR_T_wTES:
	case MODPR_T_uTES:
		switch (vardimarg) {
		case 0:	Len = lenGrad + decGrad; break;
		case 1:	Len = lenRad + decRad; break;
		case 2:	Len = lenGMS + decASec; break;
		}
		break;
	case MODPR_V_Hmint:
	case MODPR_V_Hmaxt:
	case MODPR_V_Rmint:
	case MODPR_V_Rmaxt:
	case MODPR_V_VUmssv:
	case MODPR_V_NUmssv:Len = lenTimeHMS + decSec; break;
	}
	return Len;
}

//-------------------------------------------------------------------------------
int ZCPrintFormat::CalcItemLenght(ZCPrintManager* PrintMng, int ParamIndex)
{
	if (!PrintMng->MPK.IsAflat()) return ApeakWidth ;

	int Len = 0 ;
	int N   = PrintMng->MPK.Size() ;
	int NF  = PrintMng->MPK.SizeF() ;
	assert(ParamIndex<NF) ;

	if (ParamIndex>=N) {
		// Если параметр пользовательский
		ParamIndex-=N ;
		Len = PrintMng->MPK.UPP(ParamIndex).len ;
		return Len ;
	}

	int ID  = PrintMng->MPK[ParamIndex] ;
	return(CalcIDLenght(PrintMng, ID));
}

//-------------------------------------------------------------------------------