//-------------------------------------------------------------------------------
// DemandInit.cpp : 
// А. Николенко 12.10.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>

//---------------------------------------------------------------------------
// Компоненты Fly
#include <FlyMMath.h>		// Математические функции
#include <FlyCore.h>		// Математические функции
#include <FlyMagicLNS.h>	//

//-------------------------------------------------------------------------------
// Формирование структуры начальных заданных параметров.
// Дословно инициализация требований к траектории на заданном витке.
//		PN	- Параметры орбиты. Начальные (исходные) на заданный виток.
//		PTN	- Требуемые параметры начальные.
void DemandInit(ZCLnsNpmJob& MZ, ZSLoopFact& PN, ZCLnsNpmClaim &PTN) 
{ 
    PTN.Clr( ) ;
    PTN.VarKor = MZ.TASK() ;	// Условный номер варианта: 11,12,13,...,20,21,...,32,33
    PTN.VarOrb = MZ.ORB() ;		// Орбита: эллипс (0), ОМИВ (1), ДУ (2)
    PTN.Vit    = MZ.VIT() ;		// Заданный вииток
    PTN.VarH = 0 ;				// высота: минимальная (0), максимальная (1)
    PTN.VarB = 0 ;				// ветвь: восходящая (0), нисходящая (1)
    if (MZ.ContL   ()) { PTN.L    = MZ.PARL() ; }
    if (MZ.ContT   ()) { PTN.T    = MZ.PART() ; }
    if (MZ.ContDL  ()) { PTN.dL   = MZ.PARdL() ; PTN.N = PN.Ns ; }
    if (MZ.ContHs  ()) { PTN.Hs   = MZ.PARhs() ; }
    if (MZ.ContHmin()) { PTN.Hmin = MZ.PARh() ; }
    if (MZ.ContHmax()) { PTN.Hmax = MZ.PARh() ; }
    //if (MZ.ContH   ()) { PTN.H  = MZ.PARh() ; ( MZ.ORB() == SPO_ORB_EL ? thmin : thmax ) = &PTN.H ; }
    if (MZ.ContBH  ()) { PTN.BH   = MZ.PARBh() ; PTN.VarB = MZ.PARBv( ) ; }
	if (MZ.Contu()) { 
		PTN.u = MZ.PARu(); 
		PTN.t = MZ.PARt();
	}
    if (MZ.ContA   ()) { PTN.a    = MZ.PARa() ; }
	if (MZ.ORB()==SPO_ORB_EL) {
		// Если орбита требуемая - просто эллипс и задаётся геометрия орбиты,
		// то есть эксцентриситет и положение перигея, то...
		bool ise = 0, isw = 0 ;
		if (MZ.ContE()) { 
			PTN.e=MZ.PARe() ; ise=1 ; 
		}
		if (MZ.ContW()) { 
			PTN.w=MZ.PARw() ; isw=1 ; 
		}
		if (ise && isw) {
			// ...для лучшей сходимости алгоритма в качестве контролируемых параметров
			// назначаем компоненты вектора эксцентриситета
			if (MZ.SecondDemandsKey&MKN_PARAM_ORB_e && MZ.SecondDemandsKey&MKN_PARAM_ORB_w) {
				MZ.SecondDemandsKey&=~(MKN_PARAM_ORB_e|MKN_PARAM_ORB_w) ;
				MZ.SecondDemandsKey|= MKN_PARAM_ORB_e1|MKN_PARAM_ORB_e2 ;
			} else {
				MZ.DemandsKey&=~(MKN_PARAM_ORB_e|MKN_PARAM_ORB_w) ;
				MZ.DemandsKey|= MKN_PARAM_ORB_e1|MKN_PARAM_ORB_e2 ;
			}
			PTN.e1 = PTN.e*Cos(PTN.w) ; PTN.e2 = PTN.e*Sin(PTN.w) ;
		}
	}
    // Наклонение в результате коррекций не изменяется и должно равнятся наклонению
    // исходнойорбиты на заданном витке
    PTN.i = PN.i ;
}

//-------------------------------------------------------------------------------