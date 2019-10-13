//---------------------------------------------------------------------------
// ZSKNU.cpp
// А. Николенко 03.08.2018
//---------------------------------------------------------------------------
#include <stdafx.h>
#pragma   hdrstop
//---------------------------------------------------------------------------
#include <FlyMMath.h>

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZSK.h>
#include <FlyCoreSource\\ZSKNU.h>

//---------------------------------------------------------------------------
// Корректировка витка при переходе от одной системы координат к другой
int KorVit(double* Xn, SK_TYPE skn, double* Xk, SK_TYPE skk, long& vit)
{
	int    dV = 0 ;
	bool   f = 0 ; 
	double zn, zk, Vzn=1., Vzk=1.;

	switch (skn) {
		case _SK_ASK1975 :	
		case _SK_GSK :		
		case _SK_ASKTE :	
		case _SK_ASKTES :
		case _SK_ASK2000 : zn = Xn[2] ; Vzn = Xn[5] ; break ;
		case _SK_OSK1975 :
		case _SK_OSKTE :
		case _SK_OSKTES :
		case _SK_OSK2000 : 
			zn = Xn[0]*sin(Xn[5])*sin(Xn[2]) ; 
			Vzn= (Xn[5]>3*k_polpi || Xn[5]<k_polpi)?1.0:-1.0 ;
			// Угловые величины в НУ выражены в радианах
			//zn = Xn[5]>180.0?(360-Xn[5]):Xn[5] ;
			//zn = Xn[5]>k_pi?(k_dwapi-Xn[5]):Xn[5] ;
			//if (zn>90 || zn<-90) return 0 ;
			//if (zn>k_polpi || zn<-k_polpi) return 0 ;
			//zn = zn>0?1.0:-1.0 ;
			break ;
	default: 
			assert(0);
	}
	switch ( skk ) {
		case _SK_ASK1975 :	
		case _SK_GSK :		
		case _SK_ASKTE :	
		case _SK_ASKTES :
		case _SK_ASK2000 : zk = Xk[2] ; Vzk = Xk[5] ; break ;
		case _SK_OSK1975 :
		case _SK_OSKTE :
		case _SK_OSKTES :
		case _SK_OSK2000 : 
			zk = Xk[0]*sin(Xk[5])*sin(Xk[2]) ; 
			Vzk= (Xk[5]>3*k_polpi || Xk[5]<k_polpi)?1.0:-1.0 ;
			// Угловые величины в НУ выражены в радианах
			//zk = Xk[5]>180.0?(360-Xk[5]):Xk[5] ; 
			//zk = Xk[5]>k_pi?(k_dwapi-Xk[5]):Xk[5] ; 
			//if (zk>90 || zk<-90) return 0 ;
			//if (zk>k_polpi || zk<-k_polpi) return 0 ;
			//zk = zk>0?1.0:-1.0 ;		
			break ;
	default: 
			assert(0);
	}

	if (zn==0 && zk>=0) return 0 ;
	if (Vzn>0 && Vzk>0) {
		if (zn<0  && zk>=0) dV= 1 ; 
		if (zn>=0 && zk<0 ) dV=-1 ;
	}
	//if (Vzn>0 && Vzk>0) {
	//	if (zn<0 && zk>0) dV= 1 ; 
	//	if (zn>0 && zk<0) dV=-1 ;
	//}
	vit+= dV ;
	return dV ;
}

//---------------------------------------------------------------------------
int ConvertSkNu(ZNU& nu, SK_TYPE sk)
{
	SK_TYPE Skn = nu.sk ;
	if (Skn==sk) return 0 ;

	double Xn[7] ; memcpy(&Xn[1], nu.X, 6*sizeof(double)) ; 
	double X0[7] ; memcpy(&X0[1], nu.X, 6*sizeof(double)) ; 
	double Xk[7], X[7] ;
	X[0] = X0[0] = Xn[0] = Xk[0] = nu.t ;

	//-----------------------------------------------------------------------
	switch (Skn) {
		case _SK_OSK2000 :
			ZOSCtoASC(&Xn[1], &X[1]) ; break ;
		case _SK_ASK2000 :
			memcpy(X, Xn, 7*sizeof(double)) ; break ;
		case _SK_OSK1975 :
			ZOSCtoASC(&Xn[1], &X[1]) ; 
			memcpy( Xn, X, 7*sizeof( double ) ) ;
		case _SK_ASK1975 :
			ZASC_1975to2000(&Xn[1], &X[1], 1) ;	break ;
		case _SK_OSKTE :
			ZOSCtoASC(&Xn[1], &X[1]) ; 
			memcpy( Xn, X, 7*sizeof( double ) ) ;
		case _SK_ASKTE :
			ZASC_TEto2000(Xn[0], &Xn[1], &X[1], 1) ; break ;
		case _SK_OSKTES :
			ZOSCtoASC(&Xn[1], &X[1]) ; 
			memcpy( Xn, X, 7*sizeof( double ) ) ;
		case _SK_ASKTES :
			ZASC_TESto2000(Xn[0], &Xn[1], &X[1], 1) ; break ;
		case _SK_GSK :
			ZGSCtoASC2000(Xn[0], &Xn[1], &X[1], 0) ; break ;
	}

	//-----------------------------------------------------------------------
	switch (sk) {
		case _SK_OSK1975 :
		case _SK_ASK1975 :
			ZASC_2000to1975(&X[1], &Xk[1], 1) ;
			if (sk==_SK_OSK1975) {
				memcpy( X, Xk, 7*sizeof( double ) ) ;
				ZASCtoOSC(&X[1], &Xk[1]) ;
			}
			break ;
		case _SK_OSK2000 :
			ZASCtoOSC(&X[1], &Xk[1]) ; break ;
		case _SK_ASK2000 :
			memcpy( Xk, X, 7*sizeof( double ) ) ; break ;
		case _SK_OSKTE :
		case _SK_ASKTE :
			ZASC_2000toTE(X[0], &X[1], &Xk[1], 1) ;
			if (sk==_SK_OSKTE) {
				memcpy( X, Xk, 7*sizeof( double ) ) ;
				ZASCtoOSC(&X[1], &Xk[1]) ;
			}
			break ;
		case _SK_OSKTES :
		case _SK_ASKTES :
			ZASC_2000toTES(X[0], &X[1], &Xk[1], 1) ;
			if (sk==_SK_OSKTES) {
				memcpy( X, Xk, 7*sizeof( double ) ) ;
				ZASCtoOSC(&X[1], &Xk[1]) ;
			}
			break ;
		case _SK_GSK :
			ZASC2000toGSC(X[0], &X[1], &Xk[1], 0) ; break ;
	}

	//-----------------------------------------------------------------------
	// Устраняем погрешность в ккординате Z пр пересчёте из АСК ТЭ в ГСК или 
	// в ОЭ ТЭ, или обратно. Погрешность возникает вследствии пересчёта через
	// промежуточную СК (АСК 2000).
	// Из-за арифметической погрешности при 
	// таком преобразовании может ошибочно увеличиваться или уменьшаться на 1 
	// номер витка
	int vitn = nu.vit ;
	if ((Skn==_SK_ASKTE || Skn==_SK_GSK || Skn==_SK_OSKTE) &&
		(sk ==_SK_ASKTE || sk ==_SK_GSK || sk ==_SK_OSKTE)) {
		// Проверяем координату Z и скорость Vz в старой и новой системе координат 
		double Vz1, z1, Vz2, z2 ;
		if (Skn==_SK_ASKTE || Skn==_SK_GSK) {
			Vz1= nu.X[5] ; z1 = nu.X[2] ;
		} else {
			Vz1=(nu.X[5]>k_polpi && nu.X[5]>k_t2pi) ? -1.0:1.0 ;
			z1 = ValiDA(nu.X[5], 0) ;
		}
		if (sk==_SK_ASKTE || sk==_SK_GSK) {
			Vz2= Xk[6] ; z2 = Xk[3] ;
		} else {
			Vz2=(Xk[6]>k_polpi && Xk[6]>k_t2pi) ? -1.0:1.0 ;
			z2 = ValiDA(Xk[6], 0) ;
		}
		if (Vz1>0 && Vz2>0 && (z1*z2<0 || (z1==0 && z2<0)|| (z1<0 && z2==0))) {
			// В этом случае виток изменится на единицу хотя это неверно
			if ((Skn==_SK_ASKTE && sk==_SK_GSK) || (sk==_SK_ASKTE && Skn==_SK_GSK)) {
				Xk[3] = nu.X[2] ;
			} else {
				double R ; 
				if (Skn==_SK_OSKTE) {
					R = nu.X[0]*(1-nu.X[1]*nu.X[1])/(1+nu.X[1]*cos(nu.X[5]-nu.X[4])) ;
					Xk[3] = R*sin(nu.X[5])*sin(nu.X[2]) ;
				} 
				if (sk==_SK_OSKTE) {
					R = ModVector(nu.X, 6) ;
					double sf = nu.X[2]/R ;
					Xk[6] = asin(sf/sin(Xk[3])) ;
		}	}	}
	}

	//-----------------------------------------------------------------------
	memcpy(nu.X, &Xk[ 1 ], 6*sizeof(double)) ;
	nu.sk = sk ; 
	KorVit(&X0[1], Skn, &Xk[1], sk, nu.vit) ;
	return 0 ;
}

//---------------------------------------------------------------------------
