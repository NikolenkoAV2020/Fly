//---------------------------------------------------------------------------
// ZMForceAtmMsis90.cpp
//		Расчет плотности атмосферы по модели MSIS-90
//		Версия 	: 01.00 / 14.02.2001 /
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h>
#include <FlyCoreSource\\ZCalc.h>
#include <FlyCoreSource\\ZSiderealTime.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZCISAManager.h>
#include <FlyCoreSource\\ZMForceAtmMsis.h>
#include <FlyCoreSource\\ZMForceAtmMsis90Data.h>
#include <FlyCoreSource\\ZMForceAtmMsis2000Struct.h>
#include <FlyCoreSource\\ZMForceAtmMsis2000.h>	

//---------------------------------------------------------------------------
//  I am forwarding to you the latest version of the MSIS model for your
//  analysis work. I would appreciate your bringing any problems you might
//  discover to my attention.  I would also appreciate hearing about your
//  results using this model.
//  Alan E. Hedin, 10/9/90
//---------------------------------------------------------------------------
void GTD6(short IYD, double SEC, double ALT, double GLAT, double GLONG, double STL, double F107A, double F107, double AP[], short MASS, double D[], double T[])
{
	//  Neutral Atmosphere Empirical Model from the surface to lower
	//  exosphere  MSISE90 (JGR, 96, 1159-1172, 1991)
	//  A.E.Hedin 4/24/90
	//     See subroutine GHP6 to specify a pressure rather than
	//     altitude.
	//
	//  INPUT:
	//  IYD   - YEAR AND DAY AS YYDDD or DDD (day of year from 1 to 365)
	//  SEC   - UT(SEC)
	//  ALT   - ALTITUDE(KM)
	//  GLAT  - GEODETIC LATITUDE(DEG)
	//  GLONG - GEODETIC LONGITUDE(DEG)
	//  STL   - LOCAL APPARENT SOLAR TIME(HRS) Местное истинное солнечное время выраженное в часах
	//  F107A - 3 MONTH AVERAGE OF F10.7 FLUX
	//  F107  - DAILY F10.7 FLUX FOR PREVIOUS DAY
	//  AP    - MAGNETIC INDEX(DAILY) OR WHEN SW(9)=-1. :
	//        - ARRAY CONTAINING:
	//          (1) DAILY AP
	//          (2) 3 HR AP INDEX FOR CURRENT TIME
	//          (3) 3 HR AP INDEX FOR 3 HRS BEFORE CURRENT TIME
	//          (4) 3 HR AP INDEX FOR 6 HRS BEFORE CURRENT TIME
	//          (5) 3 HR AP INDEX FOR 9 HRS BEFORE CURRENT TIME
	//          (6) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 12 TO 33 HRS PRIOR
	//              TO CURRENT TIME
	//          (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 59 HRS PRIOR
	//              TO CURRENT TIME
	//  MASS  - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS
	//          CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL.
	//  Note:  Ut, Local Time, and Longitude are used independently in the
	//  model and are not of equal importance for every situation.
	//  For the most physically realistic calculation these three
	//  variables should be consistent (STL=SEC/3600+GLONG/15).
	//  F107, F107A, and AP effects are not large below 80 km
	//  and these can be set to 150., 150., and 4. respectively.
	//
	//  OUTPUT:
	//  D(1) - HE NUMBER DENSITY(CM-3)
	//  D(2) - O NUMBER DENSITY(CM-3)
	//  D(3) - N2 NUMBER DENSITY(CM-3)
	//  D(4) - O2 NUMBER DENSITY(CM-3)
	//  D(5) - AR NUMBER DENSITY(CM-3)
	//  D(6) - TOTAL MASS DENSITY(GM/CM3)
	//  D(7) - H NUMBER DENSITY(CM-3)
	//  D(8) - N NUMBER DENSITY(CM-3)
	//  T(1) - EXOSPHERIC TEMPERATURE
	//  T(2) - TEMPERATURE AT ALT
	//
	//  TO GET OUTPUT IN M-3 and KG/M3:   CALL METERS(.TRUE.)
	//
	//  O, H, and N set to zero below 72.5 km
	//  Exospheric temperature set to average for altitudes below 120 km.
	//
	//  The following is for test and special purposes:
	//  TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW)
	//  WHERE SW IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1.
	//  FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON
	//  FOR THE FOLLOWING VARIATIONS
	//   1 - F10.7 EFFECT ON MEAN  
	//   2 - TIME INDEPENDENT
	//   3 - SYMMETRICAL ANNUAL    
	//   4 - SYMMETRICAL SEMIANNUAL
	//   5 - ASYMMETRICAL ANNUAL   
	//   6 - ASYMMETRICAL SEMIANNUAL
	//   7 - DIURNAL   
	//   8 - SEMIDIURNAL
	//   9 - DAILY AP 
	//   10- ALL UT/LONG EFFECTS
	//   11- LONGITUDINAL   
	//   12- UT AND MIXED UT/LONG
	//   13- MIXED AP/UT/LONG     
	//   14- TERDIURNAL
	//   15- DEPARTURES FROM DIFFUSIVE EQUILIBRIUM
	//   16- ALL TINF VAR   
	//   17- ALL TLB VAR
	//   18- ALL TN1 VAR     
	//   19- ALL S VAR
	//   20- ALL TN2 VAR     
	//   21- ALL NLB VAR
	//   22- ALL TN3 VAR     
	//   23- TURBO SCALE HEIGHT VAR
	//  To get current values of SW: CALL TRETRV(SW)
	//
	static	int i = 0;
	static	int	V1, MSS;
	static	int MN2 = 4;
	static	int MN3 = 5;
	static	double W1, W2;
	static	double XLAT, XMM, ALTT, D6[8], T6[2], DM28M, DZ28, DMC, DMR, TZ;
	//static	double* pD6 = &D6[0];
	//static	double* pT6 = &T6[0];
	static	double ZN2[] = {72.5,55.,45.,32.5};
	static	double ZN3[] = {32.5,20.,15.,10.,0.};
	static	double* pZN2 = &ZN2[0];
	static	double* pZN3 = &ZN3[0];
	static	double ZMIX	 = 62.5;
	static	double ALAST = 99999.;
	static	int MSSL	 = -999;

	//  Test for changed input
	//		IC = 0 OR 1 !
	V1 = VTST(IYD, SEC, GLAT, GLONG, STL, F107A, F107, AP, 0);

	// Latitude variation of gravity (none for SW(2)=0)
	XLAT = GLAT;
	if (SW[1]==0) XLAT = 45.0 ;
	GLATF(XLAT, &GSURF, &RE);

	XMM = PDM[2][4] ;

	// THERMOSPHERE/UPPER MESOSPHERE [above ZN2(1)]
	//Returns the larger of two values.
	//type __max( type a, type b );

	ALTT = __max(ALT, ZN2[0]) ;
	MSS  = MASS ;

	// Only calculate N2 in thermosphere if alt in mixed region
	if(ALT<ZMIX && MASS>0) MSS = 28 ;

	// Only calculate thermosphere if input parameters changed
	//   or altitude above ZN2(1) in mesosphere
	if (V1 == 1.0 || ALT>ZN2[0] || ALAST>ZN2[0] || MSS!= MSSL) {
		GTS6(IYD, SEC, ALTT, GLAT, GLONG, STL, F107A, F107, AP, MSS, &D6[0], &T6[0]);
		DM28M = DM28;
		// metric adjustment
		if (IMR==1)	DM28M = DM28*1.E6 ;
		MSSL = MSS;
	}

	T[0] = T6[0] ;
	T[1] = T6[1] ;
	if (ALT>=ZN2[0]) {
		for(i=0; i<8; i++) D[i] = D6[i] ;
		goto M10 ;
	}

	// LOWER MESOSPHERE/UPPER STRATOSPHERE [between ZN3(1) and ZN2(1)]
	//   Temperature at nodes and gradients at end nodes
	//   Inverse temperature a linear function of spherical harmonics
	//  Only calculate nodes if input changed
	if(V1 == 1. || ALAST >= ZN2[0]) {
		TGN2[0]= TGN1[1];
		TN2[0] = TN1[4];
		pPMA   = &PMA[0][0];
		TN2[1] = PMA[0][0]*PAVGM[0]/(1.0-SW[19]*GLOB6S(pPMA));
		pPMA   = &PMA[1][0];
		TN2[2] = PMA[1][0]*PAVGM[1]/(1.0-SW[19]*GLOB6S(pPMA));
		pPMA   = &PMA[2][0];
		TN2[3] = PMA[2][0]*PAVGM[2]/(1.0-SW[19]*SW[21]*GLOB6S(pPMA));
		pPMA   = &PMA[9][0];
		TGN2[1]= PAVGM[8]*PMA[9][0]*(1.0+SW[19]*SW[21]*GLOB6S(pPMA))
			     *TN2[3]*TN2[3]/((PMA[2][0]*PAVGM[2])*(PMA[2][0]*PAVGM[2]));
		TN3[0] = TN2[3];
	}

	if (ALT>=ZN3[0]) goto M6 ;

	// LOWER STRATOSPHERE AND TROPOSPHERE [below ZN3(1)]
	//   Temperature at nodes and gradients at end nodes
	//   Inverse temperature a linear function of spherical harmonics
	//  Only calculate nodes if input changed
	if (V1 == 1.0 || ALAST >= ZN3[0]) {
		TGN3[0]= TGN2[1];
		pPMA   = &PMA[3][0];
		TN3[1] = PMA[3][0]*PAVGM[3]/(1.0-SW[21]*GLOB6S(pPMA));
		pPMA   = &PMA[4][0];
		TN3[2] = PMA[4][0]*PAVGM[4]/(1.0-SW[21]*GLOB6S(pPMA));
		pPMA   = &PMA[5][0];
		TN3[3] = PMA[5][0]*PAVGM[5]/(1.0-SW[21]*GLOB6S(pPMA));
		pPMA   = &PMA[6][0];
		TN3[4] = PMA[6][0]*PAVGM[6]/(1.-SW[21]*GLOB6S(pPMA));
		pPMA   = &PMA[7][0];
		TGN3[1]= PMA[7][0]*PAVGM[7]*(1.0+SW[21]*GLOB6S(pPMA))
			     *TN3[4]*TN3[4]/(PMA[6][0]*PAVGM[6]*PMA[6][0]*PAVGM[6]);
	}

M6:	if(MASS == 0) goto M50 ;

	// Linear transition to full mixing at ZMIX from almost
	//full mixing at ZN2(1) to improve efficiency
	DMC = 0;
	if (ALT>ZMIX) DMC = 1.0-(ZN2[0]-ALT)/(ZN2[0]-ZMIX);
	DZ28 = D6[2];

	// N2 DENSITY
	DMR = D6[2]/DM28M-1.0;

	D[2] = DENSM(ALT, DM28M, XMM, &TZ, MN3, pZN3, pTN3, pTGN3, MN2, pZN2, pTN2, pTGN2) ;
	D[2] = D[2]*(1.0+DMR*DMC) ;

	// HE DENSITY
	D[0] = 0;
	if (MASS!=4 && MASS!= 48) goto M204 ;

	DMR = D6[0]/(DZ28*PDM[0][1])-1.0 ;
	D[0]= D[2]*PDM[0][1]*(1.0+DMR*DMC) ;

M204:
	// O DENSITY 
	D[1] = 0 ;
	// O2 DENSITY
	D[3] = 0 ;
	if (MASS != 32 && MASS != 48) goto M232 ;

	DMR  = D6[3]/(DZ28*PDM[3][1])-1.0 ;
	D[3] = D[2]*PDM[3][1]*(1.0+DMR*DMC) ;

M232:
	// AR DENSITY
	D[4] = 0 ;
	if (MASS!=40 && MASS!=48) goto M240 ;

	DMR = D6[4]/(DZ28*PDM[4][1])-1.0 ;
	D[4]= D[2]*PDM[4][1]*(1.0+DMR*DMC) ;

M240:
	// HYDROGEN DENSITY
	D[6] = 0 ;
	// ATOMIC NITROGEN DENSITY
	D[7] = 0 ;
	// TOTAL MASS DENSITY
	if(MASS == 48) {
		D[5] = 1.66E-24*(4.*D[0]+16.0*D[1]+28.0*D[2]+32.0*D[3]+40.0*D[4]+D[6]+14.0*D[7]) ;
	}
	T[1] = TZ ;	
M10: goto M90;
M50: 
	W1 = 1.0 ;
	W2 = 0.0 ;
	DD = DENSM(ALT, W1, W2, &TZ, MN3, pZN3, pTN3, pTGN3, MN2, pZN2, pTN2, pTGN2) ;
	T[1] = TZ ;
M90:
	ALAST = ALT ;
}

//---------------------------------------------------------------------------
void GHP6(short IYD, double SEC, double ALT, double GLAT, double GLONG, double STL, double F107A, double F107, double AP[], double D[], double T[], double PRESS)
{
	// FIND ALTITUDE OF PRESSURE SURFACE (PRESS) FROM GTD6
	//     INPUT:
	//  IYD - YEAR AND DAY AS YYDDD
	//  SEC - UT(SEC)
	//  GLAT - GEODETIC LATITUDE(DEG)
	//  GLONG - GEODETIC LONGITUDE(DEG)
	//  STL - LOCAL APPARENT SOLAR TIME(HRS)
	//  F107A - 3 MONTH AVERAGE OF F10.7 FLUX
	//  F107 - DAILY F10.7 FLUX FOR PREVIOUS DAY
	//  AP - MAGNETIC INDEX(DAILY) OR WHEN SW(9)=-1. :
	//     - ARRAY CONTAINING:
	// (1) DAILY AP
	// (2) 3 HR AP INDEX FOR CURRENT TIME
	// (3) 3 HR AP INDEX FOR 3 HRS BEFORE CURRENT TIME
	// (4) 3 HR AP INDEX FOR 6 HRS BEFORE CURRENT TIME
	// (5) 3 HR AP INDEX FOR 9 HRS BEFORE CURRENT TIME
	// (6) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 12 TO 33 HRS PRIOR
	//  TO CURRENT TIME
	// (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 59 HRS PRIOR
	//  TO CURRENT TIME
	//  PRESS - PRESSURE LEVEL(MB)
	//     OUTPUT:
	//  ALT - ALTITUDE(KM)
	//  D(1) - HE NUMBER DENSITY(CM-3)
	//  D(2) - O NUMBER DENSITY(CM-3)
	//  D(3) - N2 NUMBER DENSITY(CM-3)
	//  D(4) - O2 NUMBER DENSITY(CM-3)
	//  D(5) - AR NUMBER DENSITY(CM-3)
	//  D(6) - TOTAL MASS DENSITY(GM/CM3)
	//  D(7) - H NUMBER DENSITY(CM-3)
	//  D(8) - N NUMBER DENSITY(CM-3)
	//  T(1) - EXOSPHERIC TEMPERATURE
	//  T(2) - TEMPERATURE AT ALT
	//
	static	double BM = 1.3806E-19, RGAS = 831.4, TEST = .00043;
	static	int I48, L;
	static	double PL, ZI, CL, CL2, CD, CA, Z, IDAY, XN, XM, P, DIFF, G, SH;

	PL = log10(PRESS);		// ALOG10
	//	Initial altitude estimate
	if(PL >= -5.) {
		if (PL>2.5) 
			ZI =18.06*(3.00-PL);
		if (PL>75. && PL<=2.5) 
			ZI = 14.98*(3.08-PL);
		if(PL>-1. && PL<=.75)
			ZI = 17.8*(2.72-PL);
		if(PL>-2. && PL<=-1.)
			ZI = 14.28*(3.64-PL);
		if(PL>-4. && PL<=-2.)
			ZI = 12.72*(4.32-PL);
		if(PL<=-4.)
			ZI = 25.3*(.11-PL);
		IDAY = fmod((double)IYD, 1000.0);			
		CL   = GLAT/90.;
		CL2  = CL*CL;

		if (IDAY< 182) CD = 1.-IDAY/91.25;
		if (IDAY>=182) CD = IDAY/91.25-3.;

		CA = 0.;
		if (PL>-1.11 && PL<=-.23)
			CA = 1.0;
		if (PL>-.23)
			CA = (2.79-PL)/(2.79+.23);
		if (PL<=-1.11 && PL>-3.)
			CA = (-2.93-PL)/(-2.93+1.11);
		Z = ZI-4.87*CL*CD*CA-1.64*CL2*CA+.31*CA*CL;
	}

	if(PL<-5.)
		Z = 22.*(PL+4.)*(PL+4.)+110.;
	L = 0;
	//ITERATION LOOP
M10:
	L ++;
	I48 = 48;
	GTD6(IYD, SEC, Z, GLAT, GLONG, STL, F107A, F107, AP, I48, D, T);
	XN = D[0]+D[1]+D[2]+D[3]+D[4]+D[6]+D[7];
	P = BM*XN*T[1];
	if (IMR==1) P = P*1.E-6 ;

	DIFF = PL-log10(P);
	if(fabs(DIFF)<TEST || L == 6) goto M20;
	XM = D[5]/XN/1.66E-24;
	G  = GSURF/((1.+Z/RE)*(1.+Z/RE));
	SH = RGAS*T[1]/(XM*G);
	// New altitude estimate using scale height
	Z  = Z-SH*DIFF*2.302;
	goto M10;
M20:
	ALT = Z ;
}

//---------------------------------------------------------------------------
void GLATF(double LAT, double* GV, double* REFF)
{
	//CALCULATE LATITUDE VARIABLE GRAVITY (GV) AND EFFECTIVE
	//RADIUS (REFF)
	static double C2;
	static double DGTR = 1.74533E-2;
	static double LATL = -999.;
	if(LAT != LATL)	C2 = cos(2. * DGTR * LAT);
	LATL  = LAT;
	*GV   = 980.616 * (1. - .0026373 * C2);
	*REFF = 2. * (*GV) / (3.085462E-6 + 2.27E-9 * C2) * 1.E-5;
}

//---------------------------------------------------------------------------
int  VTST(short IYD, double SEC, double GLAT, double GLONG, double STL, double F107A, double F107, double AP[], short IC)
{
	//		IC = 0 OR 1 !
	// Test if geophysical variables or switches changed and save
	// Return 0 if unchanged and 1 if changed
	static	int		IYDL[2]	 = { -999,  -999  };
	static	double	SECL[2]	 = { -999., -999. };
	static	double	GLATL[2] = { -999., -999. };
	static	double	GLL[2]	 = { -999., -999. };
	static	double	STLL[2]	 = { -999., -999. };
	static	double	FAL[2]	 = { -999., -999. };
	static	double	FL[2]	 = { -999., -999. };
	static	double	APL[2][7]	 = { 
					{-999., -999., -999., -999., -999., -999., -999.},
					{-999., -999., -999., -999., -999., -999., -999. } };
	static	double	SWL[2][25];
	static	double	SWCL[2][25];
	static	int	i, VTST, I;

	for (i=0; i<25; i++) {
		SWL[0][i] =  -999.;
		SWL[1][i] =  -999.;
		SWCL[0][i] =  -999.;
		SWCL[1][i] =  -999.;
	}
	VTST = 0;
  /*
	goto M10; // Временно
	if(IYD != IYDL[IC])	goto M10;
	if(SEC != SECL[IC])	goto M10;
	if(GLAT != GLATL[IC]) goto M10;
	if(GLONG != GLL[IC]) goto M10;
	if(STL != STLL[IC])	goto M10;
	if(F107A != FAL[IC]) goto M10;
	if(F107 != FL[IC]) goto M10;
	for( i = 0; i < 7; i++ ) {
		if(AP[i] != APL[IC][i])	goto M10;
	}
	for( i = 0; i < 25; i++ ) {
		if(SW[i] != SWL[IC][i])	goto M10;
		if(SWC[i] != SWCL[IC][i]) goto M10;
	}
	goto M20;
M10:
  */
	VTST      = 1 ;
	IYDL[IC]  = IYD ;
	SECL[IC]  = SEC ;
	GLATL[IC] = GLAT ;
	GLL[IC]   = GLONG ;
	STLL[IC]  = STL ;
	FAL[IC]   = F107A ;
	FL[IC]    = F107 ;

	for (i=0; i<7; i++) APL[IC][i] = AP[i];		
	for (I=0; I<25; I++) {
		SWL[IC][I]  = SW[I];
		SWCL[IC][I] = SWC[I];
	}
//M20:
	return VTST ;
}	  
//---------------------------------------------------------------------------
void GTS6(short IYD, double SEC, double ALT, double GLAT, double GLONG, double STL, double F107A, double F107, double AP[], short MASS, double D[], double T[])
{
	//  Neutral Thermosphere Model above 72.5 km for MSISE-90
	//   A.E.Hedin 3/9/90
	//   Coefficients not changed for 120km and above, but results may differ
	//  by a few percent from MSIS-86 (GTS5) with introduction of a
	//  latitude dependent accel. of gravity.
	//   Lower thermosphere reformulated for better continuation into
	//  lower atmosphere.
	//  For efficiency:
	//   Exospheric temperature left at average value for alt below 120km;
	//   120 km gradient left at average value for alt below 72 km;
	//     INPUT:
	//  IYD - YEAR AND DAY AS YYDDD
	//  SEC - UT(SEC)
	//  ALT - ALTITUDE(KM) (GREATER THAN 72.5 KM)
	//  GLAT - GEODETIC LATITUDE(DEG)
	//  GLONG - GEODETIC LONGITUDE(DEG)
	//  STL - LOCAL APPARENT SOLAR TIME(HRS)
	//  F107A - 3 MONTH AVERAGE OF F10.7 FLUX
	//  F107 - DAILY F10.7 FLUX FOR PREVIOUS DAY
	//  AP - MAGNETIC INDEX(DAILY) OR WHEN SW(9)=-1. :
	//     - ARRAY CONTAINING:
	// (1) DAILY AP
	// (2) 3 HR AP INDEX FOR CURRENT TIME
	// (3) 3 HR AP INDEX FOR 3 HRS BEFORE CURRENT TIME
	// (4) 3 HR AP INDEX FOR 6 HRS BEFORE CURRENT TIME
	// (5) 3 HR AP INDEX FOR 9 HRS BEFORE CURRENT TIME
	// (6) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 12 TO 33 HRS PRIOR
	//  TO CURRENT TIME
	// (7) AVERAGE OF EIGHT 3 HR AP INDICIES FROM 36 TO 59 HRS PRIOR
	//  TO CURRENT TIME
	//  MASS - MASS NUMBER (ONLY DENSITY FOR SELECTED GAS IS
	//     CALCULATED.  MASS 0 IS TEMPERATURE.  MASS 48 FOR ALL.
	//     Note:  Ut, Local Time, and Longitude are used independently in the
	//model and are not of equal importance for every situation.
	//For the most physically realistic calculation these three
	//variables should be consistent (STL=SEC/3600+GLONG/15).
	//     OUTPUT:
	//  D(1) - HE NUMBER DENSITY(CM-3)
	//  D(2) - O NUMBER DENSITY(CM-3)
	//  D(3) - N2 NUMBER DENSITY(CM-3)
	//  D(4) - O2 NUMBER DENSITY(CM-3)
	//  D(5) - AR NUMBER DENSITY(CM-3)
	//  D(6) - TOTAL MASS DENSITY(GM/CM3)
	//  D(7) - H NUMBER DENSITY(CM-3)
	//  D(8) - N NUMBER DENSITY(CM-3)
	//  T(1) - EXOSPHERIC TEMPERATURE
	//  T(2) - TEMPERATURE AT ALT
	//
	//     The following is for test and special purposes:
	//     (1) LOWER BOUND QUANTITIES IN COMMON/GTS3C/
	//     (2) TO TURN ON AND OFF PARTICULAR VARIATIONS CALL TSELEC(SW)
	//   WHERE SW IS A 25 ELEMENT ARRAY CONTAINING 0. FOR OFF, 1.
	//   FOR ON, OR 2. FOR MAIN EFFECTS OFF BUT CROSS TERMS ON
	//   FOR THE FOLLOWING VARIATIONS
	//   1 - F10.7 EFFECT ON MEAN  2 - TIME INDEPENDENT
	//   3 - SYMMETRICAL ANNUAL    4 - SYMMETRICAL SEMIANNUAL
	//   5 - ASYMMETRICAL ANNUAL   6 - ASYMMETRICAL SEMIANNUAL
	//   7 - DIURNAL   8 - SEMIDIURNAL
	//   9 - DAILY AP 10 - ALL UT/LONG EFFECTS
	//  11 - LONGITUDINAL   12 - UT AND MIXED UT/LONG
	//  13 - MIXED AP/UT/LONG     14 - TERDIURNAL
	//  15 - DEPARTURES FROM DIFFUSIVE EQUILIBRIUM
	//  16 - ALL TINF VAR   17 - ALL TLB VAR
	//  18 - ALL TN1 VAR     19 - ALL S VAR
	//  20 - ALL TN2 VAR     21 - ALL NLB VAR
	//  22 - ALL TN3 VAR     23 - TURBO SCALE HEIGHT VAR
	//
	//  To get current values of SW: CALL TRETRV(SW)

	static int	   MT[] = {48, 0, 4, 16, 28, 32, 40, 1, 49, 14 };
	static int	   MN1 = 5, I1;
	static double  ALTL[] = {200., 400., 160., 200., 240., 450., 320., 450.};
	static double  ZN1[] = {120., 110., 100., 90., 72.5 };
	static double* pZN1 = &ZN1[0];
	static double  DGTR =  1.74533E-2;
	static double  dDR = 1.72142E-2;
	static double  ALAST = -999.;
	static double  W1, W2, W3;
	static int	   J, I;
	static double  V2, YRD, ZA, TINF, ZLB, G28, ZHF, XMM, ZH28, ZHM28, XMD, B28, TZ, G4, ZH04, ZHM04;
	static double  G16, B04, ZC04, HC04, ZH16, ZHM16, B16, HC16, ZC16, HCC16, ZCC16, RC16, G32, ZH32;
	static double  ZHM32, B32, HC32, ZC32, G40, ZH40, ZHM40, B40, HC40, ZC40, G1, ZH01, ZHM01, B01, DDUM; 
	static double  HC01, ZC01, HCC01, ZCC01, RC01, G14, ZH14, ZHM14, B14, HC14, ZC14, HCC14, ZCC14, RC14;

	//  Test for changed input
	I1 = 1;
	V2 = VTST(IYD, SEC, GLAT, GLONG, STL, F107A, F107, AP, I1);		// 0 || 1

	YRD = IYD;
	ZA  = PDL[1][15];				
	ZN1[0] = ZA;

	for (J=0; J<8; J++) D[J] = 0.;
    
	// TINF VARIATIONS NOT IMPORTANT BELOW ZA OR ZN1(1)
	if(ALT > ZN1[0]) {
		if(V2 == 1. || ALAST <= ZN1[0])
			TINF = PTM[0]*PT[0]*(1.+ SW[15]
			*GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPT));
	} else	TINF = PTM[0] * PT[0];

	T[0] = TINF;
	// GRADIENT VARIATIONS NOT IMPORTANT BELOW ZN1(5)
	if(ALT > ZN1[4]) {
		if(V2 == 1. || ALAST <= ZN1[4])
			G0 = PTM[3]*PS[0]*(1.+SW[18] 
			*GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPS));
	} else	G0 = PTM[3] * PS[0];
	//Calculate these temperatures only if input changed
	if(V2 == 1.) {
		pPD = &PD[3][0];
		TLB = PTM[1]*(1.+SW[16]
		*GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPD)) * PD[3][0];
	}
	S = G0/(TINF-TLB);
	// Lower thermosphere temp variations not significant for
	//  density above 300 km
	if(ALT < 300.) {
		if(V2==1. || ALAST>=300.) {
			pPTL = &PTL[0][0];
			TN1[1] = PTM[6] * PTL[0][0] / (1. - SW[17] * GLOB6S(pPTL));
			pPTL = &PTL[1][0];
			TN1[2] = PTM[2] * PTL[1][0] / (1. - SW[17] * GLOB6S(pPTL));
			pPTL = &PTL[2][0];
			TN1[3] = PTM[7] * PTL[2][0] / (1. - SW[17] * GLOB6S(pPTL));
			pPTL = &PTL[3][0];
			TN1[4] = PTM[4] * PTL[3][0] / (1. - SW[17] * SW[19] * GLOB6S(pPTL));
			pPMA = &PMA[8][0];
			TGN1[1] = PTM[8] * PMA[8][0] * (1. + SW[17] * SW[19] * GLOB6S(pPMA))
					* TN1[4] * TN1[4] / ((PTM[4] * PTL[3][0]) * (PTM[4] * PTL[3][0]));
		}
	} else {
		TN1[1] = PTM[6] * PTL[0][0];
		TN1[2] = PTM[2] * PTL[1][0];
		TN1[3] = PTM[7] * PTL[2][0];
		TN1[4] = PTM[4] * PTL[3][0];
		TGN1[1] = PTM[8] * PMA[8][0] * TN1[4] * TN1[4] / ((PTM[4] * PTL[3][0]) * (PTM[4] * PTL[3][0]));
	}

	Z0  = ZN1[3];
	T0  = TN1[3];
	ZLB = PTM[5];
	TR12= 1.;

	if(MASS==0.) goto M50;

	// N2 variation factor at Zlb
	pPD = &PD[2][0];
	G28 = SW[20] * GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPD);
	//  Variation of turbopause height
	DAY = fmod(YRD, 1000.);
	ZHF = PDL[1][24] * (1. + SW[4] * PDL[0][24] * sin(DGTR * GLAT) * cos(dDR * (DAY - PT[13])));
	//
	YRD = IYD;
	T[0] = TINF;
	XMM = PDM[2][4];

	for(J=0; J<10; J++)	{
		if(MASS == MT[J]) goto M15;
	}
	goto M90;
M15:
	if(ALT>ALTL[5] && MASS!=28 && MASS!=48)	goto M17;

	// **** N2 DENSITY ****
	//Diffusive density at Zlb
	DB28= PDM[2][0]*exp(G28)*PD[2][0];
	//Diffusive density at Alt
	W1  = 28.;
	W2  = 0.;
	D[2]= DENSU(ALT, DB28, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	DD  = D[2];
	//Turbopause
	ZH28 = PDM[2][2]*ZHF;
	ZHM28= PDM[2][3]*PDL[1][5];
	XMD  = 28.-XMM;
	//Mixed density at Zlb
	W2 = -1.;
	B28 = DENSU(ZH28, DB28, TINF, TLB, XMD, W2, &TZ, ZLB, S, MN1, pZN1, pTN1, pTGN1);
	if(ALT>ALTL[2] || SW[14]==0.) goto M17;
	//Mixed density at Alt
	W2 = 0.;
	DM28 = DENSU(ALT, B28, TINF, TLB, XMM, W2, &TZ, ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Net density at Alt
	W1  = 28.;
	D[2]= DNET(D[2], DM28, ZHM28, XMM, W1);
M17:
	switch (J+1){
		case 1: goto M20;
		case 2: goto M50;
		case 3: goto M20;
		case 4: goto M25;
		case 5: goto M90;
		case 6: goto M35;
		case 7: goto M40;
		case 8: goto M45;
		case 9: goto M25;
		case 10: goto M48;
	}
M20:
	// **** HE DENSITY ****

	// Density variation factor at Zlb
	pPD = &PD[0][0];
	G4  = SW[20] * GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPD);
	//Diffusive density at Zlb
	DB04 = PDM[0][0] * exp(G4) * PD[0][0];
	//Diffusive density at Alt
	W1 = 4.;
	W2 =-0.4;
	D[0] = DENSU(ALT, DB04, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	DD = D[0];
	if(ALT>ALTL[0] || SW[14]==0.) goto M24;

	//Turbopause
	ZH04 = PDM[0][2];
	ZHM04= ZHM28;
	//Mixed density at Zlb
	W1 = 4.-XMM;
	W2 =-1.4;
	B04 = DENSU(ZH04, DB04, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Mixed density at Alt
	W1 = 0.;
	DM04 = DENSU(ALT, B04, TINF, TLB, XMM, W1, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Net density at Alt
	W2 = 4.;
	D[0] = DNET(D[0], DM04, ZHM04, XMM, W2);
	//Correction to specified mixing ratio at ground
	RL = log(B28 * PDM[0][1] / B04);
	ZC04 = PDM[0][4] * PDL[1][0];
	HC04 = PDM[0][5] * PDL[1][1];
	//Net density corrected at Alt
	D[0] = D[0] * CCOR(ALT, RL, HC04, ZC04);
M24:
	if(MASS!= 48) goto M90;
M25:

	//**** O DENSITY ****
	// Density variation factor at Zlb
	pPD = &PD[1][0];
	G16 = SW[20] * GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPD);
	//Diffusive density at Zlb
	DB16 =  PDM[1][0] * exp(G16) * PD[1][0];
	// Diffusive density at Alt
	W1 = 16.;
	W2 = 0.;
	D[1] = DENSU(ALT, DB16, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	DD = D[1];
	if(ALT>ALTL[1] || SW[14]== 0.) goto M34;

	//  Corrected from PDM(3,1) to PDM(3,2)  12/2/85
	// Turbopause
	ZH16 = PDM[1][2];
	ZHM16 = ZHM28;
	//Mixed density at Zlb
	W1 = 16 - XMM;
	W2 = -1.;
	B16 = DENSU(ZH16, DB16, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Mixed density at Alt
	W1 = 0.;
	DM16 = DENSU(ALT, B16, TINF, TLB, XMM, W1, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Net density at Alt
	W2 = 16.;
	D[1] = DNET(D[1], DM16, ZHM16, XMM, W2);
	// Correction to specified mixing ratio at ground
	RL = log(B28 * PDM[1][1] * fabs(PDL[1][16]) / B16);
	HC16 = PDM[1][5] * PDL[1][3];
	ZC16 = PDM[1][4] * PDL[1][2];
	D[1] = D[1] * CCOR(ALT, RL, HC16, ZC16);
	// Chemistry correction
	HCC16 = PDM[1][7] * PDL[1][13];
	ZCC16 = PDM[1][6] * PDL[1][12];
	RC16 = PDM[1][3] * PDL[1][14];
	//Net density corrected at Alt
	D[1] = D[1] * CCOR(ALT, RC16, HCC16, ZCC16);
M34:
	if(MASS!=48 && MASS!=49) goto M90;
M35:
	// **** O2 DENSITY ****
	// Density variation factor at Zlb
	pPD = &PD[4][0];
	G32 = SW[20] * GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPD);
	//Diffusive density at Zlb
	DB32 = PDM[3][0] * exp(G32) * PD[4][0];
	// Diffusive density at Alt
	W1 = 32.;
	W2 = 0.;
	D[3] = DENSU(ALT, DB32, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	if(MASS==49) DD = DD + 2. * D[3];
	else         DD = D[3];

	if(ALT>ALTL[3] || SW[14]==0.) goto M39;
	// Turbopause
	ZH32 = PDM[3][2];
	ZHM32 = ZHM28;
	//Mixed density at Zlb
	W1 = 32. - XMM;
	W2 = -1;
	B32 = DENSU(ZH32, DB32, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Mixed density at Alt
	W2 = 0.;
	DM32 = DENSU(ALT, B32, TINF, TLB, XMM, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Net density at Alt
	W1 = 32.;
	D[3] = DNET(D[3], DM32, ZHM32, XMM, W1);
	// Correction to specified mixing ratio at ground
	RL = log(B28 * PDM[3][1] / B32);
	HC32 = PDM[3][5] * PDL[1][7];
	ZC32 = PDM[3][4] * PDL[1][6];
	//Net density corrected at Alt
	D[3] = D[3] * CCOR(ALT, RL, HC32, ZC32);
M39:
	if(MASS != 48) goto M90;
M40:
	// **** AR DENSITY ****
	// Density variation factor at Zlb
	pPD = &PD[5][0];
	G40 = SW[20] * GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPD);
	//Diffusive density at Zlb
	DB40 = PDM[4][0] * exp(G40) * PD[5][0];
	// Diffusive density at Alt
	W1 = 40.;
	W2 = 0.;
	D[4] = DENSU(ALT, DB40, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	DD = D[4];
	if(ALT>ALTL[4] || SW[14]==0.) goto M44;
	// Turbopause
	ZH40 = PDM[4][2];
	ZHM40 = ZHM28;
	//Mixed density at Zlb
	W1 = 40. - XMM;
	W2 = -1.;
	B40 = DENSU(ZH40, DB40, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Mixed density at Alt
	W2 = 0.;
	DM40 = DENSU(ALT, B40, TINF, TLB, XMM, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Net density at Alt
	W1 = 40.;
	D[4] = DNET(D[4], DM40, ZHM40, XMM, W1);
	// Correction to specified mixing ratio at ground
	RL = log(B28 * PDM[4][1] / B40);
	HC40 = PDM[4][5] * PDL[1][9];
	ZC40 = PDM[4][4] * PDL[1][8];
	//Net density corrected at Alt
	D[4] = D[4] * CCOR(ALT, RL, HC40, ZC40);
M44:
	if(MASS != 48) goto M90;
M45:
	//  **** HYDROGEN DENSITY ****
	// Density variation factor at Zlb
	pPD = &PD[6][0];
	G1 = SW[20] * GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPD);
	//Diffusive density at Zlb
	DB01 = PDM[5][0] * exp(G1) * PD[6][0];
	// Diffusive density at Alt
	W1 = 1.;
	W2 = -0.4;
	D[6] = DENSU(ALT, DB01, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	DD = D[6];
	if(ALT>ALTL[6] || SW[14]==0.) goto M47;
	// Turbopause
	ZH01 = PDM[5][2];
	ZHM01 = ZHM28;
	//Mixed density at Zlb
	W1 = 1. - XMM;
	W2 = -1.4;
	B01 = DENSU(ZH01, DB01, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Mixed density at Alt
	W2 = 0.;
	DM01 = DENSU(ALT, B01, TINF, TLB, XMM, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Net density at Alt
	W1 = 1.;
	D[6] = DNET(D[6], DM01, ZHM01, XMM, W1);
	// Correction to specified mixing ratio at ground
	RL = log(B28 * PDM[5][1] * fabs(PDL[1][17]) / B01);
	HC01 = PDM[5][5] * PDL[1][11];
	ZC01 = PDM[5][4] * PDL[1][10];
	D[6] = D[6] * CCOR(ALT, RL, HC01, ZC01);
	// Chemistry correction
	HCC01 = PDM[5][7] * PDL[1][19];
	ZCC01 = PDM[5][6] * PDL[1][18];
	RC01 = PDM[5][3] * PDL[1][20];
	//Net density corrected at Alt
	D[6] = D[6] * CCOR(ALT, RC01, HCC01, ZCC01);
M47:
M48:
	//  **** ATOMIC NITROGEN DENSITY ****
	// Density variation factor at Zlb
	pPD = &PD[7][0];
	G14 = SW[20] * GLOBE6(YRD, SEC, GLAT, GLONG, STL, F107A, F107, AP, pPD);
	//Diffusive density at Zlb
	DB14 = PDM[6][0] * exp(G14) * PD[7][0];
	// Diffusive density at Alt
	W1 = 14.;
	W2 = 0.;
	D[7] = DENSU(ALT, DB14, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	DD = D[7];
	if(ALT>ALTL[7] || SW[14]==0.) goto M49;
	// Turbopause
	ZH14 = PDM[6][2];
	ZHM14 = ZHM28;
	//Mixed density at Zlb
	W1 = 14. - XMM;
	W2 = -1.;
	B14 = DENSU(ZH14, DB14, TINF, TLB, W1, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	//Mixed density at Alt
	W2 = 0.;
	DM14 = DENSU(ALT, B14, TINF, TLB, XMM, W2, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1) ;
	//Net density at Alt
	W1 = 14.;
	D[7] = DNET(D[7], DM14, ZHM14, XMM, W1);
	// Correction to specified mixing ratio at ground
	RL = log(B28 * PDM[6][1] * fabs(PDL[0][2]) / B14);
	HC14 = PDM[6][5] * PDL[0][1];
	ZC14 = PDM[6][4] * PDL[0][0];
	D[7] = D[7] * CCOR(ALT, RL, HC14, ZC14);
	// Chemistry correction
	HCC14 = PDM[6][7] * PDL[0][4];
	ZCC14 = PDM[6][6] * PDL[0][3];
	RC14 = PDM[6][3] * PDL[0][5];
	//Net density corrected at Alt
	D[7] = D[7] * CCOR(ALT, RC14, HCC14, ZCC14);
M49:
	if(MASS != 48) goto M90;
	// TOTAL MASS DENSITY
	D[5] = 1.66E-24 * (4. * D[0] + 16. * D[1] + 28. * D[2] + 32. * D[3] + 40. * D[4]
		+ D[6] + 14. * D[7]);
	DB48 = 1.66E-24 * (4. * DB04 + 16. * DB16 + 28. * DB28 + 32. * DB32 + 40. * DB40
		+ DB01 + 14. * DB14);
	goto M90;
	// TEMPERATURE AT ALTITUDE
M50:
	W1 = 1.;
	W2 = 0.;
	W3 = 0.;
	DDUM = DENSU(ALT, W1, TINF, TLB, W2, W3, &T[1], ZLB, S, MN1, pZN1, pTN1, pTGN1);
	goto M90;
M90:
	// ADJUST DENSITIES FROM CGS TO KGM
	if(IMR == 1) {
		for(I=0; I<8; I++) D[I] = D[I]*1.E6;
		D[5] = D[5]/1000.;
	}
	ALAST = ALT ;
}

//---------------------------------------------------------------------------
double GLOBE6(double YRD, double SEC, double LAT, double LONG_B, double TLOC, double F107A, double F107, double AP[], double P[])
{
	// CALCULATE G(L) FUNCTION
	// Upper Thermosphere Parameters
	static double  DGTR = 1.74533E-2, dDR = 1.72142E-2, XL = 1000., TLL = 1000.,
	SW9 = 1., DAYL = -1., P14 = -1000., P18 = -1000., P32 = -1000.,
	HR = .2618, SR = 7.2722E-5, P39 = -1000., LONGL = -999.; 
	static double  SV[25] = { 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1. };
	static double* pSV = &SV[0];
	static int	   NSW = 14,
	// ISW,
	J, I;
	static double GLOBE6,
	// IYR, DAY,
	// XLONG,
	T[15], C, C2, C4, S, S2, CD14, CD18, CD32, CD39, F1, F2, T71, T72, T81, T82;
	static double  P44, P45, EXP1, EXP2, TINF;
	// ----------
	// 3hr Magnetica activity functions
#define G0(A) (double)((A)-4.+(P[25]-1.)*((A)-4.+(exp(-fabs( P[24])*((A)-4.))-1.)/fabs( P[24])))
#define SUMEX(EX) (double)(1.+(1.-pow((EX),19))/(1.-(EX))*pow((EX),.5))
#define SG0(EX)	(double)((G0(AP[1])+(G0(AP[2])*(EX)+G0(AP[3])*(EX)*(EX)+G0(AP[4])*pow((EX),3)+(G0(AP[5])*pow((EX),4)+\
	    G0(AP[6])*pow((EX),12))*(1.-pow((EX),8))/(1.-(EX))))/SUMEX((EX)))

	if (ISW!=64999) TSELEC(pSV, 0) ;
	for (J=0; J<14; J++) T[J] = 0.0 ;
	if (SW[8]>0.) SW9 = 1.0 ;
	if (SW[8]<0.) SW9 =-1.0 ;

	IYR = (short)(YRD / 1000.);
	DAY = YRD - IYR * 1000.;
	XLONG = LONG_B;
	if(XL == LAT) goto M15 ;
	// CALCULATE LEGENDRE POLYNOMIALS
	C = sin(LAT * DGTR);
	S = cos(LAT * DGTR);
	C2 = C * C;
	C4 = C2 * C2;
	S2 = S * S;
	PLG[0][1] = C;
	PLG[0][2] = 0.5 * (3. * C2 - 1.);
	PLG[0][3] = 0.5 * (5. * C * C2 - 3. * C);
	PLG[0][4] = (35. * C4 - 30. * C2 + 3.) / 8.;
	PLG[0][5] = (63. * C2 * C2 * C - 70. * C2 * C + 15. * C) / 8.;
	PLG[0][6] = (11. * C * PLG[0][5] - 5. * PLG[0][4]) / 6.;
	PLG[1][1] = S;
	PLG[1][2] = 3. * C * S;
	PLG[1][3] = 1.5 * (5. * C2 - 1.) * S;
	PLG[1][4] = 2.5 * (7. * C2 * C - 3. * C) * S;
	PLG[1][5] = 1.875 * (21. * C4 - 14. * C2 + 1.) * S;
	PLG[1][6] = (11. * C * PLG[1][5] - 6. * PLG[1][4]) / 5.;
	PLG[2][2] = 3. * S2;
	PLG[2][3] = 15. * S2 * C;
	PLG[2][4] = 7.5 * (7. * C2 - 1.) * S2;
	PLG[2][5] = 3. * C * PLG[2][4] - 2. * PLG[2][3];
	PLG[2][6] = (11. * C * PLG[2][5] - 7. * PLG[2][4]) / 4.;
	PLG[2][7] = (13. * C * PLG[2][6] - 8. * PLG[2][5]) / 5.;
	PLG[3][3] = 15. * S2 * S;
	PLG[3][4] = 105. * S2 * S * C;
	PLG[3][5] = (9. * C * PLG[3][4] - 7. * PLG[3][3]) / 2.;
	PLG[3][6] = (11. * C * PLG[3][5] - 8. * PLG[3][4]) / 3.;
	XL = LAT;
M15:
	if(TLL == TLOC) goto M16;

	if(SW[6] == 0. && SW[7] == 0. && SW[13] == 0.) goto M16;

	STLOC = sin(HR * TLOC);
	CTLOC = cos(HR * TLOC);
	S2TLOC = sin(2. * HR * TLOC);
	C2TLOC = cos(2. * HR * TLOC);
	S3TLOC = sin(3. * HR * TLOC);
	C3TLOC = cos(3. * HR * TLOC);
	TLL = TLOC;
M16:
	if (LONG_B!=LONGL) {
		CLONG = cos(DGTR * LONG_B);
		SLLONG = sin(DGTR * LONG_B);
	}

	LONGL = LONG_B;
	if(DAY!=DAYL || P[13]!=P14) CD14 = cos(dDR*(DAY-P[13]));
	if(DAY!=DAYL || P[17]!=P18)	CD18 = cos(2.*dDR*(DAY-P[17]));
	if(DAY!=DAYL || P[31]!=P32) CD32 = cos(dDR*(DAY-P[31]));
	if(DAY!=DAYL || P[38]!=P39)	CD39 = cos(2.*dDR*(DAY-P[38]));

	DAYL = DAY;
	P14 = P[13];
	P18 = P[17];
	P32 = P[31];
	P39 = P[38];
	//   F10.7 EFFECT
	DF = F107 - F107A;
	DFA = F107A - 150.;
	T[0] =  P[19] * DF + P[20] * DF * DF + P[21] * DFA + P[29] * DFA * DFA;
	F1 = 1. + (P[47] * DFA + P[19] * DF + P[20] * DF * DF) * SWC[0];
	F2 = 1. + (P[49] * DFA + P[19] * DF + P[20] * DF * DF) * SWC[0];
	//  TIME INDEPENDENT
	T[1] = (P[1] * PLG[0][2] + P[2] * PLG[0][4] + P[22] * PLG[0][6])
			+ (P[14] * PLG[0][2]) * DFA * SWC[0] + P[26] * PLG[0][1];
	//  SYMMETRICAL ANNUAL
	T[2] = P[18] * CD32;
	//  SYMMETRICAL SEMIANNUAL
	T[3] = (P[15] + P[16] * PLG[0][2]) * CD18;
	//  ASYMMETRICAL ANNUAL
	T[4] =  F1 * (P[9] * PLG[0][1] + P[10] * PLG[0][3]) * CD14;
	//   ASYMMETRICAL SEMIANNUAL
	T[5] = P[37] * PLG[0][1] * CD39;
	//  DIURNAL
	if(SW[6] == 0.) goto M200;

	T71 = (P[11] * PLG[1][2]) * CD14 * SWC[4];
	T72 = (P[12] * PLG[1][2]) * CD14 * SWC[4];
	T[6] = F2 * ((P[3] * PLG[1][1] + P[4] * PLG[1][3] + P[27] * PLG[1][5] + T71) * CTLOC
			+ (P[6] * PLG[1][1] + P[7] * PLG[1][3] + P[28] * PLG[1][5] + T72) * STLOC);
M200:
	//  SEMIDIURNAL
	if(SW[7] == 0.)	goto M210;

	T81 = (P[23] * PLG[2][3] + P[35] * PLG[2][5]) * CD14 * SWC[4];
	T82 = (P[33] * PLG[2][3] + P[36] * PLG[2][5]) * CD14 * SWC[4];
	T[7] = F2 * ((P[5] * PLG[2][2] + P[41] * PLG[2][4] + T81) * C2TLOC
			+ (P[8] * PLG[2][2] + P[42] * PLG[2][4] + T82) * S2TLOC);
M210:
	//  TERDIURNAL
	if(SW[13] == 0.) goto M220;

	T[13] = F2 * ((P[39] * PLG[3][3] + (P[93] * PLG[3][4] + P[46] * PLG[3][6]) * CD14 * SWC[4])
			* S3TLOC +(P[40] * PLG[3][3] + (P[94] * PLG[3][4] + P[48] * PLG[3][6]) * CD14 * SWC[4]) * C3TLOC);
M220:
	//    MAGNETIC ACTIVITY BASED ON DAILY AP
	if(SW9==-1.) goto M30;

	APD = (AP[0] - 4.);	
	P44 = P[43];
	P45 = P[44];
	if(P44 < 0.) P44 = 1.E-5;

	APDF = (APD + (P45 -1.) * (APD + (exp(-P44 * APD) -1.) / P44));
	if(SW[8] == 0.)	goto M40;

	T[8] = APDF * (P[32] + P[45] * PLG[0][2] + P[34] * PLG[0][4] 
			+ (P[100] * PLG[0][1] + P[101] * PLG[0][3] + P[102] * PLG[0][5]) * CD14 * SWC[4]
			+ (P[121] * PLG[1][1] + P[122] * PLG[1][3] + P[123] * PLG[1][5]) * SWC[6] * cos(HR * (TLOC - P[124])));
	goto M40;

M30:
	if(P[51]==0.) goto M40;

	EXP1 = exp(-10800.0*fabs(P[51])/(1.0+P[138]*(45.0-fabs(LAT))));
	if (EXP1>0.99999) EXP1=0.99999 ;

	EXP2 = exp(-10800.0*fabs(P[53])) ;
	if(EXP2 >0.99999) EXP2 =0.99999 ;

	if (P[24]<1.E-4) P[24] = 1.E-4 ;

	APT[0] = SG0(EXP1);
	APT[2] = SG0(EXP2);
	if(SW[8]==0.0) goto M40 ;

	T[8] = APT[0]*(P[50]+P[96]*PLG[0][2]+P[54]*PLG[0][4]
		   +(P[125]*PLG[0][1]+P[126]*PLG[0][3]+P[127]*PLG[0][5])*CD14*SWC[4]
		   +(P[128]*PLG[1][1]+P[129]*PLG[1][3]+P[130]*PLG[1][5])*SWC[6]*cos(HR*(TLOC-P[131])));
M40:
	if(SW[9]==0.0 || LONG_B<=-1000.0) goto M49 ;
	//  LONGITUDINAL
	if(SW[10] == 0.) goto M230;

	T[10] = (1. + P[80] * DFA * SWC[0]) * ((P[64] * PLG[1][2]
			+ P[65] * PLG[1][4] + P[66] * PLG[1][6] + P[103] * PLG[1][1]
			+ P[104] * PLG[1][3] + P[105] * PLG[1][5] + SWC[4]
			* (P[109] * PLG[1][1] + P[110] * PLG[1][3] + P[111] * PLG[1][5])
			* CD14) * CLONG + (P[90] * PLG[1][2] + P[91] * PLG[1][4]
			+ P[92] * PLG[1][6] + P[106] * PLG[1][1] + P[107] * PLG[1][3]
			+ P[108] * PLG[1][5] + SWC[4] * (P[112] * PLG[1][1]
			+ P[113] * PLG[1][3] + P[114] * PLG[1][5]) * CD14) * SLLONG);
M230:
	//  UT AND MIXED UT,LONGITUDE
	if(SW[11] == 0.) goto M240;

	T[11] = (1. + P[95] * PLG[0][1]) * (1. + P[81] * DFA * SWC[0])
			* (1. + P[119] * PLG[0][1] * SWC[4] * CD14)
			* ((P[68] * PLG[0][1] + P[69] * PLG[0][3] + P[70] * PLG[0][5])
			* cos(SR * (SEC - P[71])));
	T[11] = T[11] + SWC[10] * (P[76] * PLG[2][3] + P[77] * PLG[2][5] + P[78] * PLG[2][7])
			* cos(SR * (SEC - P[79]) + 2. * DGTR * LONG_B) * (1. + P[137] * DFA * SWC[0]);
M240:
	//  UT,LONGITUDE MAGNETIC ACTIVITY
	if(SW[12] == 0.) goto M48;

	if(SW9 == -1.) goto M45;

	T[12] = APDF * SWC[10] * (1. + P[120] * PLG[0][1])
			* ((P[60] * PLG[1][2] + P[61] * PLG[1][4] + P[62] * PLG[1][6])
			* cos(DGTR * (LONG_B - P[63])))
			+ APDF * SWC[10] * SWC[4]
			* (P[115] * PLG[1][1] + P[116] * PLG[1][3] + P[117] * PLG[1][5])
			* CD14 * cos(DGTR * (LONG_B - P[118])) + APDF * SWC[11] 
			* (P[83] * PLG[0][1] + P[84] * PLG[0][3] + P[85] * PLG[0][5])
			* cos(SR * (SEC - P[75]));
	goto M48;

M45:
	if(P[51] == 0.)	goto M48;

	T[12] = APT[0] * SWC[10] * (1. + P[132] * PLG[0][1])
			* ((P[52] * PLG[1][2] + P[98] * PLG[1][4] + P[67] * PLG[1][6])
			* cos(DGTR * (LONG_B - P[97]))) + APT[0] * SWC[10] * SWC[4]
			* (P[133] * PLG[1][1] + P[134] * PLG[1][3] + P[135] * PLG[1][5])
			* CD14 * cos(DGTR * (LONG_B - P[136])) + APT[0] * SWC[11]
			* (P[55] * PLG[0][1] + P[56] * PLG[0][3] + P[57] * PLG[0][5])
			* cos(SR * (SEC - P[58]));
M48:
M49:
	TINF = P[30];
	for( I = 0; I < NSW; I++ ) TINF = TINF + fabs(SW[I]) * T[I];
	GLOBE6 = TINF;

	return GLOBE6 ;
}

//---------------------------------------------------------------------------
void TSELEC( double SV[], char Regim)
{
	//  SET SWITCHES
	//  SW FOR MAIN TERMS, SWC FOR CROSS TERMS
	static	int I ;
	static	double SAV[25], SVV[25] ;
	switch(Regim) {
		case 0:
			for (I=0; I<25; I++) {
				SVV[I] = 0.0 ;		// ???
				SAV[I] = SV[I] ;
				// AMOD
				SW[I] = fmod(SV[I], 2.);				
				// ABS
				if (fabs(SV[I])==1. || fabs(SV[I])==2.)	SWC[I] = 1.;
				else SWC[I] = 0.;
			}
			ISW = 64999 ;
			break ;
		case 1:
			// LIT_LINE entry TRETRV(SVV)
			for (I=0; I<25; I++) SVV[I] = SAV[I] ;
			break ;
		default : break ;
	}
}

//---------------------------------------------------------------------------
double GLOB6S(double P[])
{	
	//VERSION OF GLOBE FOR LOWER ATMOSPHERE 1/17/90
	static	double dDR = 1.72142E-2, DAYL = -1.; // DGTR = 1.74533E-2
	static	double P32 = -1000., P18 = -1000., P14 = -1000., P39 = -1000.;
	static	int J, I;
	static	double T[14], CD32, CD18, CD14, CD39, T71, T72, T81, T82, GLOB6S, TT;

	for (J=0; J<14; J++) T[J] = 0.;

	if(DAY!=DAYL || P32!=P[31]) CD32 = cos(dDR*(DAY-P[31]));
	if(DAY!=DAYL || P18!=P[17])	CD18 = cos(2.*dDR*(DAY-P[17]));
	if(DAY!=DAYL || P14!=P[13])	CD14 = cos(dDR*(DAY-P[13]));

	if(DAY!=DAYL || P39!=P[38])	CD39 = cos(2.*dDR*(DAY-P[38]));

	DAYL = DAY;
	P32 = P[31];
	P18 = P[17];
	P14 = P[13];
	P39 = P[38];

	// F10.7
	T[0] = P[21] * DFA;
	// TIME INDEPENDENT
	T[1] = P[1] * PLG[0][2] + P[2] * PLG[0][4] + P[22] * PLG[0][6] + P[26] * PLG[0][1] + P[27] * PLG[0][3] + P[28] * PLG[0][5];
	// SYMMETRICAL ANNUAL
	T[2] = (P[18] + P[47] * PLG[0][2] + P[29] * PLG[0][4]) * CD32;
	// SYMMETRICAL SEMIANNUAL
	T[3] = (P[15] + P[16] * PLG[0][2] + P[30] * PLG[0][4]) * CD18;
	// ASYMMETRICAL ANNUAL
	T[4] = (P[9] * PLG[0][1] + P[10] * PLG[0][3] + P[35] * PLG[0][5]) * CD14;
	// ASYMMETRICAL SEMIANNUAL
	T[5] = (P[37] * PLG[0][1]) * CD39;
	//  DIURNAL
	if(SW[6] == 0.)	goto M200;

	T71 = P[11] * PLG[1][2] * CD14 * SWC[4];
	T72 = P[12] * PLG[1][2] * CD14 * SWC[4];
	T[6] = (P[3] * PLG[1][1] + P[4] * PLG[1][3] + T71) * CTLOC + (P[6] * PLG[1][1] + P[7] * PLG[1][3] + T72) * STLOC;
M200:
	//  SEMIDIURNAL
	if(SW[7] == 0.)	goto M210;
	T81 = (P[23] * PLG[2][3] + P[46] * PLG[2][5]) * CD14 * SWC[4];
	T82 = (P[33] * PLG[2][3] + P[48] * PLG[2][5]) * CD14 * SWC[4];
	T[7]= (P[5] * PLG[2][2] + P[41] * PLG[2][4] + T81) * C2TLOC + (P[8] * PLG[2][2] + P[42] * PLG[2][4] + T82) * S2TLOC;
M210:
	//  TERDIURNAL
	if(SW[13] == 0.) goto M220;
	T[13] = P[39] * PLG[3][3] * S3TLOC + P[40] * PLG[3][3] * C3TLOC;
M220:
	// MAGNETIC ACTIVITY
	if(SW[8]== 0.) goto M40;
	if(SW[8]== 1.) T[8] = APDF*(P[32]+P[45]*PLG[0][2]*SWC[1]);
	if(SW[8]==-1.) T[8] = P[50]*APT[2]+P[96]*PLG[0][2]*APT[2]*SWC[1];
M40:
	if(SW[9]==0. || SW[10]==0. || XLONG<=-1000.) goto M49 ;
	//  LONGITUDINAL
	T[10] = (1. + PLG[0][1] * (P[80] * SWC[4] * cos(dDR * (DAY - P[81]))
			+ P[85] * SWC[5] * cos(2. * dDR * (DAY - P[86])))
			+ P[83] * SWC[2] * cos(dDR * (DAY - P[84]))
			+ P[87] * SWC[3] * cos(2. * dDR * (DAY - P[88])))
			* ((P[64] * PLG[1][2] + P[65] * PLG[1][4] + P[66] * PLG[1][6]
			+ P[74] * PLG[1][1] + P[75] * PLG[1][3] + P[76] * PLG[1][5]) * CLONG
			+ (P[90] * PLG[1][2] + P[91] * PLG[1][4] + P[92] * PLG[1][6]
			+ P[77] * PLG[1][1] + P[78] * PLG[1][3] + P[79] * PLG[1][5]) * SLLONG);
M49:
	TT = 0.;
	for (I=0; I<14; I++) TT=TT+fabs(SW[I])*T[I];
	GLOB6S = TT;
	return GLOB6S ;
}

//---------------------------------------------------------------------------
double DENSU(double ALT, double DLB, double TINF, double TLB, double XM, double ALPHA, double* TZ, double ZLB, double S2, short MN1, double ZN1[], double TN1[], double TGN1[])
{
	// Calculate Temperature and Density Profiles for MSIS models
	// New lower thermo polynomial 10/30/89
	static	int MN, K;
	static	double RGAS = 831.4, DENSU = 1.;
	static	double X, Y, Z, ZG, ZGDIF, XS[5], YS[5], Y2OUT[5], ZG2, TT, TA, DTA, Z1, Z2, T1, T2;
	static	double GAMM, YD1, YD2, GLB, GAMMA, EXPL, DENSA, YI;
	static	double*	pXS;
	static	double*	pYS;
	static	double*	pY2OUT;

	//  Joining altitude of Bates and spline
	ZA = ZN1[0];
	Z = __max(ALT, ZA);	// AMAX1

	//Geopotential altitude difference from ZLB
	ZG2 = ZETA(Z, ZLB);
	//Bates temperature
	TT = TINF - (TINF - TLB) * exp(-S2 * ZG2);
	TA = TT;
	*TZ = TT;
	DENSU = *TZ;
	if(ALT >= ZA) goto M10;

	// CALCULATE TEMPERATURE BELOW ZA
	//Temperature gradient at ZA from Bates profile
	DTA = (TINF - TA) * S2 * (((RE + ZLB) / (RE + ZA)) * ((RE + ZLB) / (RE + ZA)));
	TGN1[0] = DTA;
	TN1[0] = TA;
	Z = __max(ALT, ZN1[MN1-1]);		// AMAX1
	MN = MN1;
	Z1 = ZN1[0];
	Z2 = ZN1[MN-1];
	T1 = TN1[0];
	T2 = TN1[MN-1];
	//Geopotental difference from Z1
	ZG = ZETA(Z, Z1);
	ZGDIF = ZETA(Z2, Z1);
	// Set up spline nodes
	for( K = 0; K < MN; K++ ) {
		XS[K] = ZETA(ZN1[K], Z1) / ZGDIF;
		YS[K] = 1. / TN1[K];
	}

	//  End node derivatives
	YD1 = -TGN1[0] / (T1 * T1) * ZGDIF;
	YD2 = -TGN1[1] / (T2 * T2) * ZGDIF * ((RE+Z2)/(RE+Z1)) * ((RE+Z2)/(RE+Z1));
	// Calculate spline coefficients
	pXS = &XS[0];
	pYS = &YS[0];
	pY2OUT = &Y2OUT[0];
	SPLINE(pXS, pYS, MN, &YD1, &YD2, pY2OUT);
	X = ZG / ZGDIF;
	SPLINT(pXS, pYS, pY2OUT, MN, &X, &Y);
	// temperature at altitude
	*TZ = 1. / Y;
	DENSU = *TZ;	

M10:
	if(XM == 0.) goto M50;
	//CALCULATE DENSITY ABOVE ZA
	GLB = GSURF / ((1. + ZLB / RE) * (1. + ZLB / RE));
	GAMMA = XM * GLB / (S2 * RGAS * TINF);
	EXPL = exp(-S2 * GAMMA * ZG2);
	if(EXPL>50. || TT<=0.) EXPL = 50.;
	// Density at altitude
	DENSA = DLB*pow(TLB/TT, 1.+ALPHA+GAMMA)*EXPL;
	DENSU = DENSA;
	if(ALT>=ZA)	goto M50;

	//CALCULATE DENSITY BELOW ZA
	GLB = GSURF / ((1. + Z1 / RE) * (1. + Z1 / RE));
	GAMM = XM * GLB * ZGDIF / RGAS;
	// integrate spline temperatures
	SPLINI(pXS, pYS, pY2OUT, MN, &X, &YI);
	EXPL = GAMM * YI;
	if(EXPL>50. || *TZ<= 0.) EXPL = 50.;
	// Density at altitude
	DENSU = DENSU*pow(T1/(*TZ), 1.+ALPHA)*exp(-EXPL);
M50:
	return DENSU;
}

//---------------------------------------------------------------------------
double DENSM(double ALT, double D0, double XM, double* TZ, short MN3, double ZN3[], double TN3[], double TGN3[], short MN2, double ZN2[], double TN2[], double TGN2[])
{
	// Calculate Temperature and Density Profiles for lower atmos.
	static	double	RGAS = 831.4;
	static	int		MN, K;
	static	double	DENSM, X, Y, Z, Z1, Z2, T1, T2, ZG, ZGDIF, XS[10], YS[10], Y2OUT[10], YD1, YD2, GLB, GAMM, YI, EXPL;
	static	double* pXS;
	static	double* pYS;
	static	double* pY2OUT;

	DENSM = D0;
	if(ALT > ZN2[0]) goto M50;

	//STRATOSPHERE/MESOSPHERE TEMPERATURE
	Z = __max(ALT, ZN2[MN2]);					// AMAX1
	MN = MN2;
	Z1 = ZN2[0];
	Z2 = ZN2[MN-1];
	T1 = TN2[0];
	T2 = TN2[MN-1];
	ZG = ZETA(Z, Z1);
	ZGDIF = ZETA(Z2, Z1);
	// Set up spline nodes
	for (K=0; K<MN; K++) {
		XS[K] = ZETA(ZN2[K], Z1) / ZGDIF;
		YS[K] = 1. / TN2[K];
	}
	YD1 = -TGN2[0] / (T1 * T1) * ZGDIF;
	YD2 = -TGN2[1] / (T2 * T2) * ZGDIF * ((RE + Z2) / (RE + Z1)) * ((RE + Z2) / (RE + Z1));
	// Calculate spline coefficients
	pXS = &XS[0];
	pYS = &YS[0];
	pY2OUT = &Y2OUT[0]; 
	SPLINE(pXS, pYS, MN, &YD1, &YD2, pY2OUT);
	X = ZG / ZGDIF;
	SPLINT(pXS, pYS, pY2OUT, MN, &X, &Y);
	// Temperature at altitude
	*TZ = 1./Y;
	if(XM == 0.) goto M20;

	//CALCULATE STRATOSPHERE/MESOSPHERE DENSITY
	GLB = GSURF / ((1. + Z1 / RE) * (1. + Z1 / RE));
	GAMM = XM * GLB * ZGDIF / RGAS;

	// Integrate temperature profile
	SPLINI(pXS, pYS, pY2OUT, MN, &X, &YI);
	EXPL = GAMM * YI;
	if(EXPL > 50.) EXPL = 50.;

	// Density at altitude
	DENSM = DENSM * (T1 / *TZ) * exp(-EXPL); // EXP
M20:	
	if(ALT>ZN3[0]) goto M50;

	//TROPOSPHERE/STRATOSPHERE TEMPERATURE
	Z = ALT;
	MN = MN3;
	Z1 = ZN3[0];
	Z2 = ZN3[MN-1];
	T1 = TN3[0];
	T2 = TN3[MN-1];
	ZG = ZETA(Z, Z1);
	ZGDIF = ZETA(Z2, Z1);

	// Set up spline nodes
	for (K=0; K<MN; K++) {
		XS[K] = ZETA(ZN3[K], Z1)/ZGDIF;
		YS[K] = 1./TN3[K];
	}

	YD1 = -TGN3[0] / (T1 * T1) * ZGDIF;
	YD2 = -TGN3[1] / (T2 * T2) * ZGDIF * ((RE+Z2)/(RE+Z1)) * ((RE+Z2)/(RE+Z1));
	// Calculate spline coefficients
	SPLINE(pXS, pYS, MN, &YD1, &YD2, pY2OUT);
	X = ZG / ZGDIF;
	SPLINT(pXS, pYS, pY2OUT, MN, &X, &Y);
	// temperature at altitude
	*TZ = 1. / Y;
	if(XM == 0.) goto M30;

	//CALCULATE TROPOSPHERIC/STRATOSPHERE DENSITY
	GLB  = GSURF / ((1. + Z1 / RE) * (1. + Z1 / RE));
	GAMM = XM * GLB * ZGDIF / RGAS;
	//  Integrate temperature profile
	SPLINI(pXS, pYS, pY2OUT, MN, &X, &YI);
	EXPL = GAMM * YI;
	if(EXPL > 50.) EXPL = 50.;
	//  Density at altitude
	DENSM = DENSM * (T1 / *TZ) * exp(-EXPL)	;
M30:
M50:
	if(XM == 0.) DENSM = *TZ ;
	return DENSM;
}

//---------------------------------------------------------------------------
void SPLINE(double X[], double Y[], short N, double* YP1, double* YPN, double* Y2)
{
	//  CALCULATE 2ND DERIVATIVES OF CUBIC SPLINE INTERP FUNCTION
	//  ADAPTED FROM NUMERICAL RECIPES BY PRESS ET AL
	//  X,Y: ARRAYS OF TABULATED FUNCTION IN ASCENDING ORDER BY X
	//  N: SIZE OF ARRAYS X,Y
	//  YP1,YPN: SPECIFIED DERIVATIVES AT X(1) AND X(N); VALUES
	//     >= 1E30 SIGNAL SIGNAL SECOND DERIVATIVE ZERO
	//  Y2: OUTPUT ARRAY OF SECOND DERIVATIVES
	static	int I, K;
	static	double U[100], SIG, P, QN, UN;

	if(*YP1 > 99E30) {
		Y2[0] = 0.;
		U[0] = 0.;
	} else {
		Y2[0] = -.5;
		U[0] = (3. / (X[1] - X[0])) * ((Y[1] - Y[0]) / (X[1] - X[0]) - *YP1);
	}
	for (I=1; I<N-1; I++)	{
		SIG = (X[I] - X[I-1]) / (X[I+1] - X[I-1]);
		P = SIG * Y2[I-1] + 2.;
		Y2[I] = (SIG - 1.) / P;
		U[I] = (6. * ((Y[I+1] - Y[I]) / (X[I+1] - X[I]) - (Y[I] - Y[I-1])
			/ (X[I] - X[I-1])) / (X[I+1] - X[I-1]) - SIG * U[I-1]) / P;
	}
	if(*YPN > 99E30) {
		QN = 0.;
		UN = 0.;
	} else {
		QN = .5;
		UN = (3. / (X[N-1] - X[N-2])) * (*YPN - (Y[N-1] - Y[N-2]) / (X[N-1] - X[N-2]));
	}
	Y2[N-1] = (UN - QN * U[N-2]) / (QN * Y2[N-2] + 1.);
	for (K = N-1; K >= 1; K--) Y2[K-1] = Y2[K-1] * Y2[K] + U[K-1];
}

//---------------------------------------------------------------------------
void SPLINT(double XA[], double YA[], double Y2A[], short N, double* X, double* Y)
{
	//  CALCULATE CUBIC SPLINE INTERP VALUE
	//  ADAPTED FROM NUMBERICAL RECIPES BY PRESS ET AL.
	//  XA,YA: ARRAYS OF TABULATED FUNCTION IN ASCENDING ORDER BY X
	//  Y2A: ARRAY OF SECOND DERIVATIVES
	//  N: SIZE OF ARRAYS XA,YA,Y2A
	//  X: ABSCISSA FOR INTERPOLATION
	//  Y: OUTPUT VALUE
	static	short  KLO, KHI, K;
	static	double A, B, H;

	KLO = 1 ; // 1	нач. индекс масс.
	KHI = N ; // N	посл индекс масс. уже учт. -1
M1:
	if (KHI-KLO>1) {
		// Так ли????
		K = (KHI+KLO)/2 ;		
		if(XA[K-1]>*X) KHI = K;
		else KLO = K ;
		goto M1 ;
	}
	H = XA[KHI-1] - XA[KLO-1];
	//if(H == 0); // WRITE(6,*) 'BAD XA INPUT TO SPLINT'
	A = (XA[KHI-1] - *X) / H;
	B = (*X - XA[KLO-1]) / H;
	*Y = A * YA[KLO-1] + B * YA[KHI-1] + (( A * A * A - A) * Y2A[KLO-1] + (B * B * B - B) * Y2A[KHI-1]) * H * H / 6.;
}

//---------------------------------------------------------------------------
void SPLINI(double XA[], double YA[], double Y2A[], short N, double* X, double* YI)
{
	// INTEGRATE CUBIC SPLINE FUNCTION FROM XA(1) TO X
	//  XA,YA: ARRAYS OF TABULATED FUNCTION IN ASCENDING ORDER BY X
	//  Y2A: ARRAY OF SECOND DERIVATIVES
	//  N: SIZE OF ARRAYS XA,YA,Y2A
	//  X: ABSCISSA ENDPOINT FOR INTEGRATION
	//  YI: OUTPUT VALUE
	static	int KLO, KHI;
	static	double A,A2, B, B2, XX, H;

	*YI = 0.;
	KLO = 1;		// 1
	KHI = 2;		// 2
M1:
	if(*X > XA[KLO-1] && KHI <= N) {
		XX = *X;
		if(KHI < N) XX =__min(*X, XA[KHI-1]) ; // AMIN1

		H = XA[KHI-1] - XA[KLO-1];
		A = (XA[KHI-1] - XX) / H;
		B = (XX - XA[KLO-1]) / H;
		A2 = A * A;
		B2 = B * B;
		*YI = *YI + ((1. - A2) * YA[KLO-1] / 2. + B2 * YA[KHI-1] / 2.
			+ (( -(1. + A2 * A2) / 4. + A2 / 2.) * Y2A[KLO-1]
			+ (B2 * B2 / 4. -B2 / 2.) * Y2A[KHI-1]) * H * H / 6.) * H;
		KLO ++;
		KHI ++;
		goto M1;
	}
}

//---------------------------------------------------------------------------
double DNET(double DD, double DM, double ZHM, double XMM, double XM)
{
	// TURBOPAUSE CORRECTION FOR MSIS MODELS
	//   Root mean density
	// 8/20/80
	//    DD - diffusive density
	//    DM - full mixed density
	//    ZHM - transition scale length
	//    XMM - full mixed molecular weight
	//    XM  - species molecular weight
	//    DNET - combined density
	static	double A, DNET, YLOG, X, Y;

	A = ZHM / (XMM - XM);
	if (DM>0. && DD>0.) goto M5;
	if (DD==0. && DM==0.) DD = 1.;
	if (DM==0.) goto M10;
	if (DD==0.) goto M20;
M5:
	YLOG = A * log( DM / DD);				// ALOG
	if (YLOG<-10.) goto M10;
	if (YLOG>10.) goto M20;
	X = 1. + exp(YLOG);
	Y = 1 / A;
	DNET = DD * pow(X, Y);
	goto M50 ;
M10:
	DNET = DD;
	goto M50;
M20:
	DNET = DM;
	goto M50;
M50:	
	return DNET ;
}

//---------------------------------------------------------------------------
double CCOR(double ALT, double R, double H1, double ZH)
{
	// CHEMISTRY/DISSOCIATION CORRECTION FOR MSIS MODELS
	// ALT- altitude
	// R  - target ratio
	// H1 - transition scale length
	// ZH - altitude of 1/2 R
	static	double E, EX, CCOR;

	E = (ALT - ZH) / H1;
	if(E > 70.)	goto M20;
	if(E < -70.) goto M10;
	EX = exp(E);			// EXP
	CCOR = R / (1. + EX);
	goto M50;
M10:
	CCOR = R;
	goto M50;
M20:
	CCOR = 0.;
	goto M50;
M50:
	CCOR = exp(CCOR);
	return CCOR;
}

//---------------------------------------------------------------------------
void GGM(short ART, double XLG, double BG, double XLM, double BM)
{
	//  CALCULATES GEOMAGNETIC LONGITUDE (XLM) AND LATITUDE (BM) 
	//  FROM GEOGRAFIC LONGITUDE (XLG) AND LATITUDE (BG) FOR ART=0
	//  AND REVERSE FOR ART=1. ALL ANGLES IN DEGREE.
	//  LATITUDE:-90 TO 90. LONGITUDE:0 TO 360 EAST.
	static	double 	  FAKTOR, ZPI, CBG, CI, SI, CBM, SBM, CLM, SLM, SBG, SLG, CLG, YLG; 

	FAKTOR = .0174532925;   
	ZPI = FAKTOR * 360.;
	CBG = 11.4 * FAKTOR;
	CI = cos(CBG);     
	SI = sin(CBG);
	if(ART!= 0.) { 
		CBM = cos(BM * FAKTOR);
		SBM = sin(BM * FAKTOR);
		CLM = cos(XLM * FAKTOR);
		SLM = sin(XLM * FAKTOR);
		SBG = SBM * CI - CBM * CLM * SI;   
		BG = asin(SBG);   
		CBG = cos(BG);     
		SLG = (CBM * SLM) / CBG;  
		CLG = (SBM * SI + CBM * CLM * CI) / CBG;
		if(CLG > 1.) CLG = 1.;
		XLG = acos(CLG);
		if(SLG < 0.0) XLG = ZPI - acos(CLG);
		BG = BG / FAKTOR;    
		XLG = XLG / FAKTOR;
		XLG = XLG - 69.8;    
		if(XLG < 0.0) XLG = XLG + 360.0;
	} else {
		YLG = XLG + 69.8;
		CBG = cos(BG * FAKTOR);
		SBG = sin(BG * FAKTOR);
		CLG = cos(YLG * FAKTOR);
		SLG = sin(YLG * FAKTOR);
		SBM = SBG * CI + CBG * CLG * SI;
		BM  = asin(SBM);   
		CBM = cos(BM);
		SLM = (CBG * SLG) / CBM;
		CLM	= (-SBG * SI + CBG * CLG * CI) / CBM;
		XLM = acos(CLM);
		if(SLM < 0.0) XLM = ZPI - acos(CLM);
		BM = BM / FAKTOR;
		XLM = XLM / FAKTOR;
	}
}

//---------------------------------------------------------------------------
