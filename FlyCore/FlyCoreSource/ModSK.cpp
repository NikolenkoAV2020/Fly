//---------------------------------------------------------------------------
// ModSK.cpp
// А. Николенко 03.08.2018
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ZNUT.h>
#include <FlyCoreSource\\ZSK.h>
#include <FlyCoreSource\\ZHBL.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModAstro.h> 

//---------------------------------------------------------------------------
// Макроопределение новой версии переводов АСК-ГСК
#define _NEW_VERSIONGSK

//---------------------------------------------------------------------------
// Функция формирования матрицы прецессии P
//---------------------------------------------------------------------------
/*
void Calc_Matr_Prec( double *Prec, double *Matr )
{
	double S_Pr0, C_Pr0, S_Pr1, C_Pr1, S_Pr2, C_Pr2;
	double C_Pr1_C_Pr2, S_Pr1_C_Pr2;

	S_Pr0 = sin(Prec[0]);  C_Pr0 = cos(Prec[0]);
	S_Pr1 = sin(Prec[1]);  C_Pr1 = cos(Prec[1]);
	S_Pr2 = sin(Prec[2]);  C_Pr2 = cos(Prec[2]);
	C_Pr1_C_Pr2 = C_Pr1 * C_Pr2;
	S_Pr1_C_Pr2 = S_Pr1 * C_Pr2;

	Matr[0] = + C_Pr1_C_Pr2*C_Pr0 - S_Pr1 * S_Pr0;
	Matr[1] = - C_Pr1_C_Pr2*S_Pr0 - S_Pr1 * C_Pr0;
	Matr[2] = - C_Pr1*S_Pr2;
	Matr[3] = + S_Pr1_C_Pr2*C_Pr0 + C_Pr1 * S_Pr0;
	Matr[4] = - S_Pr1_C_Pr2*S_Pr0 + C_Pr1 * C_Pr0;
	Matr[5] = - S_Pr1*S_Pr2;
	Matr[6] = + S_Pr2*C_Pr0;
	Matr[7] = - S_Pr2*S_Pr0;
	Matr[8] = + C_Pr2;
}
*/
//---------------------------------------------------------------------------
/*
HRESULT Calc_Matr_Prec(double dIAT_E1, double dIAT_E2, double *Prec, double *AMatr, double *DMatr)
{
	double DPrec[3];
	short i;
	if(!Prec) Prec = DPrec;
	// Расчет прецессии на время IAT
	Calc_Prec_Par(dIAT_E1, dIAT_E2, Prec);
	// Расчет матрицы прецессии
	Calc_Matr_Prec( Prec, AMatr ) ;
	// Расчет производной матрицы прецессии
	if( DMatr ) {
		// Расчет прецессии на время dIAT + 1 сутки
		Calc_Prec_Par(dIAT_E1, dIAT_E2 + 1., DPrec);
		// Расчет матрицы прецессии на время dIAT + 1 сутки
		Calc_Matr_Prec(DPrec, DMatr);
		// Расчет производной от матрицы прецессии
		for( i = 9 ; --i >= 0 ; DMatr[ i ] -= AMatr[ i ] ) ;
	}
	return 0 ;
}
*/
//---------------------------------------------------------------------------
// Расчет матрицы прецессии
//---------------------------------------------------------------------------
/*
HRESULT Calc_Matr_Precess(double dTime, double* pre, double* M_Pre )
{
	HRESULT hCodRet;
	double ck, sk, cz, sz, co, so;

	if( !pre ) {
		if((hCodRet=AEJG(dTime,10))!=0) return hCodRet;
		pre = Wae->qpr;
	}
	ck=cos(pre[0]);  sk=sin(pre[0]);
	cz=cos(pre[1]);  sz=sin(pre[1]);
	co=cos(pre[2]);  so=sin(pre[2]);
	// Матрица прецессии
	M_Pre[0]=+ck*cz*co-sk*sz;  M_Pre[1]=-sk*cz*co-ck*sz;   M_Pre[2]=-cz*so;
	M_Pre[3]=+ck*sz*co+sk*cz;  M_Pre[4]=-sk*sz*co+ck*cz;   M_Pre[5]=-sz*so;
	M_Pre[6]=+ck*so;           M_Pre[7]=-sk*so;            M_Pre[8]=+co;
	return 0 ;
}
*/
//---------------------------------------------------------------------------
// Функция перевода из АСК одной фундаментальной эпохи в АСК другой ФЭ.
//---------------------------------------------------------------------------
// AskT1  -  координаты (и скорости) или ускорения в АСК эпохи Т1;
// AskT2  -  координаты (и скорости) или ускорения в АСК эпохи Т2 = Т1 + t;
// Preces -  параметры прецессии, рассчитанные функцией Calc_Prec_Par;
// Direct - направление перевода:
//           false:  AskT1 -> AskT2;
//           true:   AskT2 -> AskT1;
// Speed  - флаг перевода скоростей:
//           false:  только координаты (или ускорения);
//           true:   координаты и скорости.
//---------------------------------------------------------------------------
/*
void Convert_ASC_E1_ASC_E2(double *AskT1, double *AskT2, double *Preces, bool Direct, bool Speed )
{
	double Matr_Prec[9];
	// Формирование матрицы прецессии
	Calc_Matr_Prec(Preces, Matr_Prec);
	// Перевод из средней АСК одной эпохи в среднюю АСК другой эпохи

	if( !Direct ) {
		Um_Mat(Matr_Prec, AskT1, AskT2, 3, 3, 1);
		if( Speed ) Um_Mat(Matr_Prec, AskT1+3, AskT2+3, 3, 3, 1);
	} else { 
		Um_Mat(AskT2, Matr_Prec, AskT1, 1, 3, 3);
		if( Speed ) Um_Mat(AskT2+3, Matr_Prec, AskT1+3, 1, 3, 3);
	}
}
*/
//----------------------------------------------------------------------------
// Функция перевода из АСК одной эпохи (Е1) в АСК другой эпохи (Е2)
//----------------------------------------------------------------------------
/*
HRESULT Convert_ASC_E1_ASC_E2(double dTimeE1, double *dASC_E1, double dTimeE2, double *dASC_E2, bool Speed )
{
	double dMatr[9] ;
	//double Preces[3], dDifMatr[9];
	// Формирование матрицы прецессии
assert(0);

//	Calc_Matr_Prec(dTimeE1, dTimeE2, Preces, dMatr, dDifMatr);
	// Перевод в АСК эпохи Е2
	UMN(dMatr, dASC_E1, dASC_E2, 3, 3, 1);
	if(Speed) UMN(dMatr, dASC_E1+3, dASC_E2+3, 3, 3, 1);
	return 0 ;
}
*/
//---------------------------------------------------------------------------
// Функция перевода из экваториальной АСК одной эпохи в АСК другой эпохи.
//---------------------------------------------------------------------------
// ask1  -  координаты ( и скорость ) в АСК эпохи Т;
// ask2  -  координаты ( и скорость ) в АСК эпохи Т+t;
// Direct  =  0   ask1 -> ask2;
//         =  1   ask2 -> ask1;
//        +=  2   перевод не только координат, но и скорости;
// Prec  -  параметры прецессии, рассчитанные по Т и Т+t функцией preces.
//---------------------------------------------------------------------------
/*
void ask1_ask2( double* ask1, double* ask2, short Direct, double *Prec )
{
assert(0) ;
	double Cin_Tmp[3],Matr_Prec[9];
	short i;
	// Формирование матрицы прецессии
	Calc_Matr_Prec(Prec, Matr_Prec);
	// Перевод из средней АСК одной эпохи в среднюю АСК другой эпохи.
	i=Direct/2;  Direct-=2*i;
	if( !Direct ) {
		Um_Mat(Matr_Prec,ask1,Cin_Tmp,3,3,1);
		memcpy(ask2,Cin_Tmp,3*sizeof(double));
		if( i ) {
			Um_Mat(Matr_Prec,ask1+3,Cin_Tmp,3,3,1);
			memcpy(ask2+3,Cin_Tmp,3*sizeof(double));
		}
		return ;
	} 
	Um_Mat(ask2,Matr_Prec,Cin_Tmp,1,3,3);
	memcpy(ask1,Cin_Tmp,3*sizeof(double));
	if( i ) {
		Um_Mat(ask2+3,Matr_Prec,Cin_Tmp,1,3,3);
		memcpy(ask1+3,Cin_Tmp,3*sizeof(double));
	}
}
*/
//---------------------------------------------------------------------------
/// Функция формирования матрицы нутации N
//---------------------------------------------------------------------------
/*
void Calc_Matr_Nuta(double *Nuta, double *Matr)
{
	double S_Nu0, C_Nu0, S_Nu1, C_Nu1, S_Nu2, C_Nu2;
	double C_Nu0_C_Nu1, S_Nu0_C_Nu1;

	S_Nu0 = sin(Nuta[0]+Nuta[2]);
	C_Nu0 = cos(Nuta[0]+Nuta[2]);
	S_Nu1 = sin(Nuta[1]);  C_Nu1 = cos(Nuta[1]);
	S_Nu2 = sin(Nuta[0]);  C_Nu2 = cos(Nuta[0]);
	C_Nu0_C_Nu1 = C_Nu0 * C_Nu1;
	S_Nu0_C_Nu1 = S_Nu0 * C_Nu1;

	Matr[0] = + C_Nu1;
	Matr[1] = - S_Nu1*C_Nu2;
	Matr[2] = - S_Nu1*S_Nu2;
	Matr[3] = + C_Nu0*S_Nu1;
	Matr[4] = + C_Nu0_C_Nu1*C_Nu2 + S_Nu0 * S_Nu2;
	Matr[5] = + C_Nu0_C_Nu1*S_Nu2 - S_Nu0 * C_Nu2;
	Matr[6] = + S_Nu0*S_Nu1;
	Matr[7] = + S_Nu0_C_Nu1*C_Nu2 - C_Nu0 * S_Nu2;
	Matr[8] = + S_Nu0_C_Nu1*S_Nu2 + C_Nu0 * C_Nu2;
}
*/
//---------------------------------------------------------------------------
// Функция перевода из АСКС в АСКТ и обратно с учетом нутации Земли.
//---------------------------------------------------------------------------
//  Direct - направление перевода:
//            0 - из средней АСК (АСКС) в текущую АСК (АСКТ);
//            1 - из АСКТ в АСКС;
//  ParNut - средний наклон и параметры нутации Земли - рассчитываются
//            функцией AEJG(t,12), результат помещается в k_qnu.
//---------------------------------------------------------------------------
/*
void asks_t(double *asks, double *askt, short Direct, double *ParNut)
{
	double Matr_Nuta[9];
	// Формирование матрицы нутации
	Calc_Matr_Nuta(ParNut, Matr_Nuta);
	// Перевод кинематики
	if ( !Direct ) {
		Um_Mat(Matr_Nuta,asks,askt,3,3,1);
		Um_Mat(Matr_Nuta,asks+3,askt+3,3,3,1);
		return ;
	} 
	Um_Mat(askt,Matr_Nuta,asks,1,3,3);
	Um_Mat(askt+3,Matr_Nuta,asks+3,1,3,3);
}
*/
//---------------------------------------------------------------------------
//	pr=1 из НССК в АСК
//	pr=0 из АСК в НССК
//---------------------------------------------------------------------------
/*
short ASK_NSSK( short pr, double* ack, double* nssk, double t_st, double *X )
{ 
	double mct_grz[3][3],mct_ask[9];
	static double Mfi[9];
	double cosB,sinB,cosL,sinL,cosA,sinA;
	short jn;

	GR_HBL(0,0,0,X,Mfi,nullptr,nullptr);
	cosB=cos(Mfi[1]);  sinB=sin(Mfi[1]);
	cosL=cos(Mfi[2]);  sinL=sin(Mfi[2]);
	cosA=cos(X[3]);    sinA=sin(X[3]);

	mct_grz[0][0]=-cosL*sinB*cosA-sinL*sinA;
	mct_grz[1][0]=-sinL*sinB*cosA+cosL*sinA;
	mct_grz[2][0]= cosB*cosA;
	mct_grz[0][1]= cosL*cosB;
	mct_grz[1][1]= sinL*cosB;
	mct_grz[2][1]= sinB;
	mct_grz[0][2]= cosL*sinB*sinA-sinL*cosA;
	mct_grz[1][2]= sinL*sinB*sinA+cosL*cosA;
	mct_grz[2][2]=-cosB*sinA;

	jn=AB_GRZ(1,Mfi,Mfi,t_st); if(jn) return jn;
	UMN(mgrzab,&mct_grz[0][0],mct_ask,3,3,3);
	if( pr ) {
		UMN(mct_ask,&nssk[1],&ack[1],3,3,1);
		UMN(mct_ask,&nssk[4],&ack[4],3,3,1);
		ack[0]=nssk[0];
	} else {
		TRAM(mct_ask,mct_ask,3,3);
		UMN(mct_ask,&ack[1],&nssk[1],3,3,1);
		UMN(mct_ask,&ack[4],&nssk[4],3,3,1);
		nssk[0]=ack[0];
	}    
	return 0 ;
}
*/

//---------------------------------------------------------------------------
// Функции перехода от ГСК к АСК и обратно.
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Время и матрица перевода из АСК75 в ГСК ( последнее обращение )
//----------------------------------------------------------------------------
//static double  Matr_ASK75_GSK[ 9 ] ; 
//static double  Diff_ASK75_GSK[ 9 ] ;
//double  GLTIME_ASK75_GSK;
//double* GL_Matr_ASK75_GSK = Matr_ASK75_GSK;
//double* GL_Diff_ASK75_GSK = Diff_ASK75_GSK;
//----------------------------------------------------------------------------
// Функция получения матрицы перевода из АСК эпохи 1975 года в ГСК
//----------------------------------------------------------------------------
//double* Get_Matr_ASC1975_GSC( ) { return GL_Matr_ASK75_GSK; }
//---------------------------------------------------------------------------
//
// Переход между ГСК и АСК (при переводе скоростей учитывается производные от нутации и прецессии по времени)
// short AB_GR(short pr,double *a,double *g)
//
// Переход между АСК и ГСК замороженной на момент t от эпохи задачи         
// short AB_GRZ (short pr, double *a, double *g, double t)
//
// Расчет матрицы [6x6] перехода между АСК и ГСК
// short matr_AB_GR(short pr,double tm,double *fpre,double *fnut,double *s)
//
// Параметр pr:
//               0 - АСК -> ГСК (координаты и скорости);
//               1 - ГСК -> АСК (координаты и скорости);
//               2 - АСК -> ГСК (только координаты);
//               3 - ГСК -> АСК (только координаты).
//---------------------------------------------------------------------------
//HRESULT Precess_J2000( double dTime_FE, double *Prec ) ;
//short m_PN   ( double t, double* M, double* pre, double* nut ) ;
//short m_OmPVZ( double t, double *M, double *Ms ) ;
//----------------------------------------------------------------------------
// Функция формирования матрицы перевода из ГСК в АСК эпохи 1975 года
//----------------------------------------------------------------------------
/*
HRESULT Create_Matr_GSC_ASC1975( double dTime )
{
	HRESULT hCodRet;
	double Matr_NP[9], Matr_PVZ[9], Diff_PVZ[9];

	if(fabs(GLTIME_ASK75_GSK-dTime)>1.e-10) {
		GLTIME_ASK75_GSK = dTime;
		// Формирование матрицы перевода
		if((hCodRet = m_OmPVZ (dTime, Matr_PVZ, Diff_PVZ))!=0) return hCodRet;
		if((hCodRet = m_PN (dTime, Matr_NP, nullptr, nullptr))!=0) return hCodRet;
		UMN(Matr_PVZ, Matr_NP, GL_Matr_ASK75_GSK, 3, 3, 3);
		// Формирование производной от матрицы перевода
		UMN(Diff_PVZ, Matr_NP, GL_Diff_ASK75_GSK, 3, 3, 3);
	}
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// Функция перевода из гринвичской СК в абсолютную СК эпохи 1975 года
//----------------------------------------------------------------------------
/*
HRESULT Convert_GSC_ASC1975( double* dGSC, double* dASC_1975, bool Flag_R )
{
	HRESULT hCodRet;
	// Формирование матрицы перевода из ГСК в АСК эпохи 1975 года
	if(( hCodRet = Create_Matr_GSC_ASC1975(dGSC[0]) ) ) return hCodRet;
	// Перенос времени
	dASC_1975[0] = dGSC[0];
	// Перевод координат положения
	UMN_MATR_TRN_VECT (GL_Matr_ASK75_GSK, dGSC+1, dASC_1975+1);
	// Перевод проекций скорости
	if( !Flag_R ) {
		UMN_MATR_TRN_VECT (GL_Matr_ASK75_GSK, dGSC+4, dASC_1975+4);
		UMN_MATR_TRN_VECT_ADD (GL_Diff_ASK75_GSK, dGSC+1, dASC_1975+4);
	}
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// Функция перевода из абсолютной СК эпохи 1975 года в гринвичскую СК
//----------------------------------------------------------------------------
/*
HRESULT Convert_ASC1975_GSC( double* dASC_1975, double* dGSC, bool Flag_R )
{
	HRESULT hCodRet;
	// Формирование матрицы перевода из ГСК в АСК эпохи 1975 года
	if((hCodRet = Create_Matr_GSC_ASC1975(dASC_1975[0]))!=0) return hCodRet ;
	// Перенос времени
	dGSC[0] = dASC_1975[0];
	// Перевод координат положения
	UMN_MATR_PRM_VECT (GL_Matr_ASK75_GSK, dASC_1975+1, dGSC+1);
	// Перевод проекций скорости
	if( !Flag_R ) {
		UMN_MATR_PRM_VECT (GL_Matr_ASK75_GSK, dASC_1975+4, dGSC+4);
		UMN_MATR_PRM_VECT_ADD (GL_Diff_ASK75_GSK, dASC_1975+1, dGSC+4);
	}
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// Функция перевода из гринвичской СК в абсолютную СК эпохи 1975 года
//----------------------------------------------------------------------------
/*
HRESULT ASC1975_GSC(bool Flag_GSC, double *dASC_1975, double *dGSC, bool Flag_R, double* Out_mtAsk_Gsk)
{
	HRESULT hCodRet;
	short  i;
	double NP[9], Mo[9], Tmp_mtAsk_Gsk[9], Mos[9], *Icx, *Rez, *mtAsk_Gsk;
	double v[3],T[9],NPs[9];
	// Адрес формирования матрицы перевода
	if(Out_mtAsk_Gsk) mtAsk_Gsk = Out_mtAsk_Gsk; else mtAsk_Gsk = Tmp_mtAsk_Gsk;

	if(Flag_GSC) { Icx = dGSC; Rez = dASC_1975; }
	else         { Icx = dASC_1975; Rez = dGSC; }
	Rez[0] = Icx[0];

	if((hCodRet = m_OmPVZ (Icx[0],Mo,(Flag_R)?nullptr:Mos))!=0) return hCodRet;
	if((hCodRet = m_PN (Icx[0],NP,nullptr,nullptr))!=0) return hCodRet;
	UMN(Mo,NP,mtAsk_Gsk,3,3,3);
	if(Flag_GSC) TRAM (mtAsk_Gsk,mtAsk_Gsk,3,3);
	UMN(mtAsk_Gsk,Icx+1,Rez+1,3,3,1);
	if( !Flag_R ) {
		// Переводим скорости
		UMN(mtAsk_Gsk,Icx+4,Rez+4,3,3,1);
		if((hCodRet = m_PN (Icx[0]+1,NPs,nullptr,nullptr))!=0) return hCodRet;
		for(i=0; i<9; i++) NPs[i] -= NP[i];
		UMN(Mo,NPs,T,3,3,3);
		if(Flag_GSC) TRAM (T,T,3,3);
		UMN(T,Icx+1,v,3,3,1);
		for(i=0; i<3; i++) Rez[4+i] += v[i];
		UMN(Mos,NP,T,3,3,3);
		if(Flag_GSC) TRAM (T,T,3,3);
		UMN(T,Icx+1,v,3,3,1);
		for(i=0; i<3; i++) Rez[4+i] += v[i];
	}
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// Функция перевода из абсолютной СК 2000 года в гринвичскую СК
//----------------------------------------------------------------------------
/*
HRESULT ASC2000_GSC(double *dASC_2000, double *dGSC, bool Flag_R, double* Matr_Ask2000_Gsk)
{
	HRESULT hCodRet;
	short  i;
	double NP[9], Mo[9], Tmp_mtAsk_Gsk[9], Mos[9], *Icx, *Rez, *mtAsk_Gsk;
	double v[3],T[9],NPs[9];
	// Адрес формирования матрицы перевода
	if(Matr_Ask2000_Gsk) mtAsk_Gsk = Matr_Ask2000_Gsk; else mtAsk_Gsk = Tmp_mtAsk_Gsk;

	Icx = dASC_2000; Rez = dGSC;
	Rez[0] = Icx[0];

	if((hCodRet = m_OmPVZ(Icx[0], Mo, (Flag_R)?nullptr:Mos))!=0) return hCodRet;
	if((hCodRet = m_PN (Icx[0], NP, nullptr, nullptr))!=0) return hCodRet;
	UMN(Mo, NP, mtAsk_Gsk,3,3,3);
	UMN(mtAsk_Gsk, Icx+1, Rez+1, 3, 3, 1);
	if( !Flag_R ) {
		// Переводим скорости
		UMN(mtAsk_Gsk, Icx+4, Rez+4, 3, 3, 1);
		if((hCodRet = m_PN (Icx[0]+1, NPs, nullptr, nullptr))!=0) return hCodRet;
		for(i=0; i<9; i++) NPs[i] -= NP[i];
		UMN(Mo,NPs,T,3,3,3);
		UMN(T,Icx+1,v,3,3,1);
		for(i=0; i<3; i++) Rez[4+i] += v[i];
		UMN(Mos,NP,T,3,3,3);
		UMN(T,Icx+1,v,3,3,1);
		for(i=0; i<3; i++) Rez[4+i] += v[i];
	}
	return 0 ;
}
*/
/*
//----------------------------------------------------------------------------
// Функция перевода из гринвичской СК в абсолютную СК эпохи 2000 года
//----------------------------------------------------------------------------
HRESULT GSC_ASC2000(double *dGSC, double *dASC_2000, bool Flag_R, double* Out_mtAsk2000_Gsk)
{
	return 0 ;
}
//----------------------------------------------------------------------------
// Функция перевода из абсолютной СК текущей эпохи в гринвичскую СК
//----------------------------------------------------------------------------
HRESULT ASCT_GSC(double *dASCT, double *dGSC, bool Flag_R, double* Out_mtAskT_Gsk)
{
	return 0 ;
}
//----------------------------------------------------------------------------
// Функция перевода из гринвичской СК в абсолютную СК текущей эпохи
//----------------------------------------------------------------------------
HRESULT GSC_ASCT(double *dGSC, double *dASCT, bool Flag_R, double* Out_mtAskT_Gsk)
{
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// Функция перевода из гринвичской СК в абсолютную СК эпохи 1975 года
//----------------------------------------------------------------------------
/*
HRESULT AB_GR(short pr, double *a, double *g, double* Out_mtAsk_Gsk)
{
#ifdef _NEW_VERSIONGSK
	if( pr&1 ) return Convert_GSC_ASC1975( g, a, ( pr&2 ? true : false ) ) ;
	else       return Convert_ASC1975_GSC( a, g, ( pr&2 ? true : false ) ) ;
#else
	return ASC1975_GSC( pr&1, a, g, pr&2, nullptr ) ;
#endif
}
*/
//----------------------------------------------------------------------------
//  Перевод из гринвича в абсолют
//----------------------------------------------------------------------------
/*
HRESULT AB_GR2(short pr, double *a, double *g, double* Out_mtAsk_Gsk)
{
	short hCodRet ;
	double NP[9],Mo[9],Tmp_mtAsk_Gsk[9],Mos[9],*Icx,*Rez,*mtAsk_Gsk;

	if(Out_mtAsk_Gsk) mtAsk_Gsk = Out_mtAsk_Gsk; else mtAsk_Gsk = Tmp_mtAsk_Gsk;

	if(pr%2) { Icx = g; Rez = a; }
	else     { Icx = a; Rez = g; }
	Rez[0] = Icx[0];

	hCodRet = m_OmPVZ (Icx[0],Mo,(pr>1)?nullptr:Mos);
	if (!hCodRet) hCodRet = m_PN (Icx[0],NP,nullptr,nullptr);
	if (hCodRet) return hCodRet;
	UMN (Mo,NP,mtAsk_Gsk,3,3,3);
	if (pr%2) TRAM (mtAsk_Gsk,mtAsk_Gsk,3,3);
	UMN(mtAsk_Gsk,Icx+1,Rez+1,3,3,1);
	if (pr>1) return 0;
	// переводим скорости
	UMN(mtAsk_Gsk,Icx+4,Rez+4,3,3,1);  // абсолютная скорость
	{
	short  i;
	double v[3],T[9];
	double NPs[9];

	hCodRet = m_PN (Icx[0]+1,NPs,nullptr,nullptr);
	if (hCodRet) return hCodRet;
	for (i=0; i<9; i++) NPs[i] -= NP[i];
	UMN (Mo,NPs,T,3,3,3);
	if (pr%2) TRAM (T,T,3,3);
    UMN(T,Icx+1,v,3,3,1);
    for (i=0; i<3; i++) Rez[4+i] += v[i];
    UMN (Mos,NP,T,3,3,3);
    if (pr%2) TRAM (T,T,3,3);
    UMN(T,Icx+1,v,3,3,1);
    for (i=0; i<3; i++) Rez[4+i] += v[i];
   }
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// перевод из АСК в ГСК замороженную на момент t от эпохи задачи и обратно
//		pr:	0 - АСК -> ГРЗ;
//			1 - ГРЗ -> АСК.
//----------------------------------------------------------------------------
/*
short AB_GRZ (short pr, double *a, double *g, double t)
{
	short hCodRet;
	double NP[9],M[9];

	hCodRet = m_OmPVZ (t,M,nullptr); if(hCodRet) return hCodRet;
	hCodRet = m_PN (t,NP,nullptr,nullptr); if(hCodRet) return hCodRet;
	UMN(M,NP,mgrzab,3,3,3);

	if( !a || !g ) return 0 ;

	if ( pr ) { 
		TRAM(mgrzab,mgrzab,3,3);
		UMN(mgrzab,g,a,3,3,1);
		UMN(mgrzab,g+3,a+3,3,3,1);
	} else {
		UMN(mgrzab,a,g,3,3,1);
		UMN(mgrzab,a+3,g+3,3,3,1);
	}
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// Функция формирования матрицы перевода из ГСК в АСК эпохи 1975 года
//----------------------------------------------------------------------------
/*
short matr_AB_GR( short pr, double tm, double* fpre, double* fnut, double* s )
{
	short i,j,hCodRet;
	double NP[9],Mo[9],F[9],Mos[9],Fs[9];
	memset (s, 0, 36*sizeof(double));
	hCodRet = m_OmPVZ (tm,Mo,(pr>1)?nullptr:Mos); if(hCodRet) return hCodRet;
	hCodRet = m_PN (tm,NP,fpre,fnut); if(hCodRet) return hCodRet;
	UMN (Mo,NP,F,3,3,3);
	UMN (Mos,NP,Fs,3,3,3);
	if(pr%2) {
		TRAM (F,F,3,3);
		TRAM (Fs,Fs,3,3);
	}
	for (i=0; i<3; i++) {
		for (j=0; j<3; j++) {
			s[i*6+j] = s[i*6+21+j] = F[i*3+j];
			s[i*6+18+j] = Fs[i*3+j];
		}
	}
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// Расчет матрицы ПВЗ и ее производной
//----------------------------------------------------------------------------
/*
short m_OmPVZ (double t, double* M, double* Ms )
{
	short ko;
	double cS,sS,tt,xp,yp;

	if((ko=FZW(t,&tt))!=0) return ko;
	cS=cos(tt);
	sS=sin(tt);
	if( k_Flag_Pvz) {
		if ( ( ko = Model_PVZ( t, &xp, &yp, nullptr ) ) ) return ko ; 
	} else { xp = yp = 0.; }
	M[0]=+cS;           M[1]=+sS;           M[2]=+xp;
	M[3]=-sS;           M[4]=+cS;           M[5]=-yp;
	M[6]=-xp*cS-yp*sS;  M[7]=-xp*sS+yp*cS;  M[8]=+1.;

	if( Ms ) { 
		cS *= k_omega;
		sS *= k_omega;
		Ms[0]=-sS;           Ms[1]=+cS;           Ms[2]=0.;
		Ms[3]=-cS;           Ms[4]=-sS;           Ms[5]=0.;
		Ms[6]=+xp*sS-yp*cS;  Ms[7]=-xp*cS-yp*sS;  Ms[8]=0.;
	}
	return 0 ;
}
*/
//----------------------------------------------------------------------------
// Расчет матрицы прецессии-нутации для перехода от АСК75->ГСК
//----------------------------------------------------------------------------
/*
short m_PN (double t, double* M, double* pre, double* nut )
{
	short ko;
	double P[9], N[9];
	double cNf,sNf,ce0,se0,e,ce,se,cece0,sese0,cese0,sece0;
	double ck,sk,cz,sz,co,so;

	if( !pre ) {
		if((ko=AEJG(t,10))!=0) return ko;
		pre = Wae->qpr;
	}
	if( !nut ) {
		if((ko=AEJG(t,12))!=0) return ko;
		nut = Wae->qnu;
	}
	// Матрица нутации
	ce0=cos(nut[0]);  se0=sin(nut[0]);
	cNf=cos(nut[1]);  sNf=sin(nut[1]);
	e=nut[0]+nut[2]; ce=cos(e);  se=sin(e);
	cece0=ce*ce0; sese0=se*se0;
	cese0=ce*se0; sece0=se*ce0;

	N[0]=cNf;      N[1]=-sNf*ce0;           N[2]=-sNf*se0;
	N[3]=sNf*ce;   N[4]=cNf*cece0+sese0;    N[5]=cNf*cese0-sece0;
	N[6]=sNf*se;   N[7]=cNf*sece0-cese0;    N[8]=cNf*sese0+cece0;
	// Матрица прецессии
	ck=cos(pre[0]);  sk=sin(pre[0]);
	cz=cos(pre[1]);  sz=sin(pre[1]);
	co=cos(pre[2]);  so=sin(pre[2]);

	P[0]=ck*cz*co-sk*sz;  P[1]=-sk*cz*co-ck*sz;   P[2]=-cz*so;
	P[3]=ck*sz*co+sk*cz;  P[4]=-sk*sz*co+ck*cz;   P[5]=-sz*so;
	P[6]=ck*so;           P[7]=-sk*so;            P[8]=co;
	// Произведение матриц
	UMN(N,P,M,3,3,3);
	return 0 ;
}
*/
//---------------------------------------------------------------------------
// Расчет матрицы нутации
//---------------------------------------------------------------------------
/*
HRESULT Calc_Matr_Nutacia( double t, double* nut, double* M_Nut )
{
	HRESULT hCodRet;
	double cNf,sNf,ce0,se0,e,ce,se,cece0,sese0,cese0,sece0;

	if( !nut ) {
		if((hCodRet=AEJG(t,12))!=0) return hCodRet;
		nut = Wae->qnu;
	}

	ce0=cos(nut[0]);  se0=sin(nut[0]);
	cNf=cos(nut[1]);  sNf=sin(nut[1]);
	e=nut[0]+nut[2]; ce=cos(e);  se=sin(e);
	cece0=ce*ce0; sese0=se*se0;
	cese0=ce*se0; sece0=se*ce0;
	// Матрица нутации
	M_Nut[0]=+cNf;     M_Nut[1]=-sNf*ce0;          M_Nut[2]=-sNf*se0;
	M_Nut[3]=+sNf*ce;  M_Nut[4]=+cNf*cece0+sese0;  M_Nut[5]=+cNf*cese0-sece0;
	M_Nut[6]=+sNf*se;  M_Nut[7]=+cNf*sece0-cese0;  M_Nut[8]=+cNf*sese0+cece0;
	return 0 ;
}
*/
//---------------------------------------------------------------------------
// Расчет матрицы перехода из АСК-1975 в ГСК
//---------------------------------------------------------------------------
/*
HRESULT Create_Matr_Ask1975_Gsk(double dTime, double* Matr_AskGsk )
{
	HRESULT hCodRet;
	double NP[9],Mo[9];
	// Расчет матрицы от ПВЗ
	hCodRet = m_OmPVZ (dTime,Mo,nullptr); if(hCodRet) return hCodRet;
	// Расчет матрицы прецессии и нутации
	hCodRet = m_PN(dTime,NP, Wae->qpr, Wae->qnu ) ; if(hCodRet) return hCodRet;
	// Расчет суммарной матрицы
	UMN (Mo,NP,Matr_AskGsk,3,3,3);
	return 0 ;
}
*/
//---------------------------------------------------------------------------
// Расчет матрицы перехода из АСК-2000 в ГСК
//---------------------------------------------------------------------------
/*
HRESULT Create_Matr_Ask2000_Gsk(double dTime, double* Matr_AskGsk )
{
	HRESULT hCodRet;
	double N[9],P[9],NP[9],R[9],S[9],RS[9];
	// Расчет матрицы от ПВЗ
	hCodRet = m_OmPVZ (dTime,R,nullptr); if(hCodRet) return hCodRet;
	// Расчет матрицы прецессии и нутации
	hCodRet = m_PN(dTime,NP, Wae->qpr, Wae->qnu); if(hCodRet) return hCodRet;
	// Расчет суммарной матрицы прецессии-нутации АСК
	UMN (N,P,NP,3,3,3);
	// Расчет суммарной матрицы разворота ГСК
	UMN (R,S,RS,3,3,3);
	// Расчет суммарной матрицы АСК-ГСК
	UMN (RS,NP,Matr_AskGsk,3,3,3);
	return 0 ;
}
*/

//---------------------------------------------------------------------------
//	Преобразования между плоскими прямоугольными КРД и геодезическими c погрешностью не более 1 мм
//---------------------------------------------------------------------------
//Reg:  0 - B,L -> X,Y
//      1 - X,Y -> B,L
//	Y подставляется условной (отклонение от основного меридиана ближайшей 6 градусной зоны)
//	X,Y - в метрах
//	B,L - в радианах
//	Для перехода используется эллипсоид Красовского
//---------------------------------------------------------------------------
void GAUS_KR( char Reg, double* B, double* L, double* pX, double* pY )
{
	if( Reg ) {
	    double X = *pX, Y = *pY;
		double n = (long)( Y * 1.e-6);
	    double bet = X / 6367558.4968;
		double sin_bet = sin( bet );
	    long double sin_bet_2 = sin_bet * sin_bet;
		long double sin_bet_4 = sin_bet_2 * sin_bet_2;
	    long double B0 = bet + sin(2 * bet) * ( 0.00252588685 - 0.0000149186 * sin_bet_2 + 0.00000011904 * sin_bet_4 );
		long double cos_B0 = cos(B0);
	    long double sin_B0 = sin(B0);
		long double sin_B0_2 = sin_B0 * sin_B0;
	    long double sin_B0_4 = sin_B0_2 * sin_B0_2;
		long double sin_B0_6 = sin_B0_4 * sin_B0_2;
	    long double z0 = ( Y - ( 10. * n + 5 ) * 1.e5 ) / ( 6378245. * cos_B0 );
		long double z0_2 = z0 * z0;
	    long double dB = -z0_2 * sin(2 * B0) * ( 0.251684631 - 0.003369263 * sin_B0_2 + 0.000011276 * sin_B0_4 -
                    z0_2 * ( 0.10500614 - 0.04559916 * sin_B0_2 + 0.00228901 * sin_B0_4 -
                    0.00002987 * sin_B0_6 - z0_2 * ( 0.042858 - 0.025318 * sin_B0_2 + 0.014346 * sin_B0_4 -
                    0.001264 * sin_B0_6 - z0_2 * ( 0.01672 - 0.0063 * sin_B0_2 + 0.01188 * sin_B0_4 -
                    0.00328 * sin_B0_6 ))));
	    long double l = z0 * ( 1. - 0.0033467108 * sin_B0_2 - 0.0000056002 * sin_B0_4 -
                    0.0000000187 * sin_B0_6 - z0_2 * ( 0.16778975 + 0.16273586 * sin_B0_2 -
                    0.0005249 * sin_B0_4 - 0.00000846 * sin_B0_6 - z0_2 * ( 0.0420025 +
                    0.1487407 * sin_B0_2 + 0.005942 * sin_B0_4 - 0.000015 * sin_B0_6 -
                    z0_2 * (0.01225 + 0.09477 * sin_B0_2 + 0.03282 * sin_B0_4 - 0.00034 * sin_B0_6 -
                    z0_2 * ( 0.0038 + 0.0524 * sin_B0_2 +  0.0482 * sin_B0_4 + 0.0032 * sin_B0_6)))));
		*B = (double) ( B0 + dB );
	    *L = (double) ( 6. * ( n - 0.5 ) / 57.29577951 + l );
		return ;
	} 

	double L_Gr = *L * M_GRARAD;
    int n = (int)( ( 6. + L_Gr ) / 6. );
    long double l = ( L_Gr - ( 3. + 6. * ( n - 1 ) ) ) / 57.29577951;
    long double l_2 = l * l;
    long double Bb = *B;
    long double sin_B = sin(Bb);
    long double sin_2_B = sin_B * sin_B;
    long double sin_4_B = sin_2_B * sin_2_B;
    long double sin_6_B = sin_4_B * sin_2_B;
    long double X_Rez = 0, Y_Rez = 0;

    X_Rez = 6367558.4968 * Bb - sin( 2 * Bb) * (16002.89 + 66.9607 * sin_2_B +
            0.3515  * sin_4_B - l_2 * ( 1594561.25 + 5336.535 * sin_2_B +
            26.79   * sin_4_B + 0.149   * sin_6_B + l_2 * ( 672483.4 - 811219.9 * sin_2_B +
            5420.   * sin_4_B - 10.6    * sin_6_B + l_2 * ( 278194.  - 830174.  * sin_2_B +
            572434. * sin_4_B - 16010.  * sin_6_B + l_2 * ( 109500.  - 574700.  * sin_2_B +
            863700. * sin_4_B - 398600. * sin_6_B )))));
     *pX = (double)X_Rez;

     Y_Rez = ( 5. + 10. * n ) * 1.e5 + l * cos(Bb) * ( 6378245. + 21346.1415 * sin_2_B +
              107.159    * sin_4_B + 0.5977 * sin_6_B + l_2 * ( 1070204.16 -
              2136826.66 * sin_2_B + 17.98  * sin_4_B - 11.99 * sin_6_B +
              l_2 * ( 270806. - 1523417. * sin_2_B + 1327645. * sin_4_B -
              21701.  * sin_6_B + l_2 * ( 79690. - 866190. * sin_2_B + 1730360. * sin_4_B -
              945460. * sin_6_B ))));
     *pY = (double)Y_Rez;
}

//---------------------------------------------------------------------------
//ФУНКЦИИ РАБОТЫ И ПРЕОБРАЗОВАНИЯ ДАННЫХ (КООРДИНАТ)
//---------------------------------------------------------------------------
//Перевод координат в СК
//---------------------------------------------------------------------------
HRESULT Convert_SysTGP(short Num_SK, short New_SK, double *V_IS)
{
    double BufKrd[3] = {0};
    short  ko = 0;
    memcpy( BufKrd, V_IS, sizeof( BufKrd ) ) ;

    // Проверка на необходимость перевода
    if( Num_SK == New_SK ) return( ko );
    // Перевод из исходной СК в ГСК
    switch( Num_SK ) {
    case  _SKT_OZE: 
		ZGRtoHBL(V_IS,BufKrd,1,0,nullptr,nullptr);
        // BufKrd-в hbl,на выходе V_IS-в ГСК
        break;
	case  _SKT_EKR: 
		ZGSCtoHBLKR(V_IS, BufKrd, 1);
        // BufKrd-в hbl,на выходе V_IS-в ГСК
        break;
	case  _SKT_G_K: 
		GAUS_KR(1,BufKrd+1,BufKrd+2,V_IS+1,V_IS+2);
		ZGSCtoHBLKR(V_IS, BufKrd, 1);
        break;
/*
	case _SKT_ITRF: 
		if (New_SK!=_SK_WGS84) {
			ko=TransHalmert(BufKrd,V_IS,nullptr,_SK_ITRF,_SK_GSK);     //ITRF -> GSK
            if (ko==1) return ko;
		} else {
			ko=TransHalmert(BufKrd,V_IS,nullptr,_SK_ITRF,_SK_WGS84);  //ITRF -> WGS
            return ko;
		}
		break;
	case  _SKT_WGS84: 
		if(New_SK!=_SK_ITRF) {
			ko=TransHalmert(V_IS,BufKrd,nullptr,_SK_WGS84,_SK_ITRF);  //WGS -> ITRF
			if (ko==1) return ko;
            ko=TransHalmert(BufKrd,V_IS,nullptr,_SK_ITRF,_SK_GSK);    //ITRF -> GSK
            if (ko==1) return ko;
		} else {
			ko=TransHalmert(BufKrd,V_IS,nullptr,_SK_WGS84,_SK_ITRF);  //WGS -> ITRF
            return ko	;
		}	
		break ;
	case  _SKT_GSK_2: 
		if(New_SK!=_SK_ITRF) {
			ko=TransHalmert(V_IS,BufKrd,nullptr,_SK_GSK_2,_SK_ITRF);  //ПЗ90.2-> ITRF
            if (ko==1) return ko;
            ko=TransHalmert(BufKrd,V_IS,nullptr,_SK_ITRF,_SK_GSK);    //ITRF -> GSK
            if (ko==1) return ko;
		} else {
			ko=TransHalmert(BufKrd,V_IS,nullptr,_SK_GSK_2,_SK_ITRF);  //ПЗ90.2 -> ITRF
            return ko;
		}
		break;
*/
	}
    // Перевод из ГСК в заданную СК
    switch (New_SK) {
	case  _SKT_OZE: 
		ZGRtoHBL(V_IS,BufKrd,0,0,nullptr,nullptr);
        memcpy(V_IS, BufKrd, sizeof(BufKrd));
        break;
	case  _SKT_EKR: 
		ZGSCtoHBLKR(V_IS,BufKrd, 0);
        memcpy(V_IS, BufKrd, sizeof(BufKrd));
        break;
	case  _SKT_G_K: 
		ZGSCtoHBLKR(V_IS, BufKrd, 0);
        memcpy(V_IS, BufKrd, sizeof(BufKrd));
        GAUS_KR(0, BufKrd+1, BufKrd+2, V_IS+1,V_IS+2);
        break;
/*
	case  _SKT_ITRF:
		ko=TransHalmert(V_IS,BufKrd,nullptr,_SK_GSK,_SK_ITRF);
        if (ko==1) return ko;
        memcpy(V_IS, BufKrd, sizeof(BufKrd));
        break;
	case  _SKT_WGS84:
		ko=TransHalmert(V_IS,BufKrd,nullptr,_SK_GSK,_SK_ITRF);       //GSK -> ITRF
        if (ko==1) return ko;
        ko=TransHalmert(BufKrd,V_IS, nullptr,_SK_ITRF,_SK_WGS84);    //ITRF -> WGS
        if (ko==1) return ko;
        break;
	case  _SKT_GSK_2:
		ko=TransHalmert(V_IS,BufKrd,nullptr,_SK_GSK,_SK_ITRF);
        if (ko==1) return ko;
        ko=TransHalmert(BufKrd,V_IS, nullptr,_SK_ITRF,_SK_GSK_2);    //ITRF -> ПЗ90.2
        if (ko==1) return ko;
        break;
*/
	default: break ;
    }
	return ko ;
}
//---------------------------------------------------------------------------
// Переход из ПЗ-90 в ITRF
//---------------------------------------------------------------------------
int Convert_PZ90_ITRF( double* Cin_PZ90, double* Cin_ITRF, bool Flag_ITRF)
{
	if( Flag_ITRF ) {
		// ПЗ90 -> ITRF
		memcpy(Cin_ITRF,Cin_PZ90,sizeof(double)*3);
		Convert_SysTGP( _SKT_GSK, _SKT_ITRF, Cin_ITRF );
	} else {
		// ITRF -> ПЗ90
		memcpy(Cin_PZ90,Cin_ITRF,sizeof(double)*3);
		Convert_SysTGP(_SKT_ITRF,_SKT_GSK, Cin_PZ90);   
	}
	return 0 ;
}

//---------------------------------------------------------------------------


