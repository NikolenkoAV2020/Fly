//-----------------------------------------------------------------------------
// TaskMagicFlyBegin.cpp
// А. Николенко 14.02.2019
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Компоненты Fly
#include <FlyCore.h>			// Собственоно сама модель движения
#include <FlyMMath.h>			// Математические функции
#include <FlyOrbitDesign.h>		// Проектирование орбит
#include <FlyMagicLNS.h>		// Метод узловых точек

//-----------------------------------------------------------------------------
// Инициализация НУ шкалы и модели движения, настройка документирования
// для всех задач расчёта параметров коррекции орбиты
//-----------------------------------------------------------------------------
int TaskMagicFlyBegin(ZSMagicLnsSolution* MS, int task, ZMSC& KA, ZNU& NU, 
					  ZLSF& LSF, bool ResetNU, ZCPrintManager* PrintMng, 
					  FILE** pfp)
{
	int rc = 0;

	DefaultParam(MS->MP);	// Параметризация задачи расчета параметров управлени движением ЦМ 
	LoadDU(1, 1, MS->DU);	// Загрузка ДУ с условным номером 1 для КА 1

	if (ResetNU) {
		//---------------------------------------------------------------------
		// НУ
		NU.Key.ka = 1; NU.Key.nom = 15; NU.Key.mod = 0;
		NU.Key.type = 8; NU.Key.bc = 1;
		NU.numlsf = 1;

		if (task >= 10 && task < 20) {
			NU.t << DATETIMEDMB(18, 7, 2018, 9, 30, 0, 0);
			NU.vit = 99;

			NU.sk = _SK_OSK1975;
			NU.X[0] = TO_DTKM(7000.0);
			NU.X[1] = 0.0005;
			NU.X[2] = TO_RAD(90.0);
			NU.X[3] = TO_RAD(120.0);
			NU.X[4] = TO_RAD(45.0);
			NU.X[5] = TO_RAD(15);
			NU.Sb = 0.0001;
			NU.Kp = 0;
			NU.M0 = 900;
			NU.MF = 50;
		}

		// НУ для сверки вариант 20, 21, 22, 23, 
		if (task >= 20 && task < 30 && task != 24) {
			NU.t << DATETIMEDMB(14, 12, 2016, 12, 0, 0, 0);
			NU.vit = 1;
			NU.sk = _SK_GSK;
			NU.X[0] = TO_DTKM(-6587.1866159);
			NU.X[1] = TO_DTKM(-436.2698529);
			NU.X[2] = TO_DTKM(0);
			NU.X[3] = TO_DTKMSSS(0.1687606245);
			NU.X[4] = TO_DTKMSSS(-2.5480965961);
			NU.X[5] = TO_DTKMSSS(7.1501923050);
			NU.Sb = 0.014 / 9.80665*2.0;
			NU.Kp = 0;
			NU.M0 = 5500;
			NU.MF = 500;
		}
		if (task == 24) {
			// НУ для сверки вариант 24 - ТЕСТ 1,
			NU.t << DATETIMEDMB(14, 12, 2015, 12, 0, 0, 0);
			NU.vit = 1;
			NU.sk = _SK_GSK;
			NU.X[0] = TO_DTKM(-6587.1866159);
			NU.X[1] = TO_DTKM(-436.2698529);
			NU.X[2] = TO_DTKM(0);
			NU.X[3] = TO_DTKMSSS(0.1687606245);
			NU.X[4] = TO_DTKMSSS(-2.5480965961);
			NU.X[5] = TO_DTKMSSS(7.1501923050);
			NU.Sb = 0.014 / 9.80665*2.0;
			NU.Kp = 0;
			NU.M0 = 5500;
			NU.MF = 500;
			/*
			// НУ для сверки вариант 24 - ТЕСТ 2
			NU.t << DATETIMEDMB(14, 12, 2015, 12, 0, 0, 0) ;
			NU.vit  = 2 ;
			NU.sk   = _SK_GSK ;
			NU.X[0] = TO_DTKM(-6253.8800322 ) ;
			NU.X[1] = TO_DTKM( 2113.7267198 ) ;
			NU.X[2] = TO_DTKM( 0 ) ;
			NU.X[3] = TO_DTKMSSS(-0.8176778949 ) ;
			NU.X[4] = TO_DTKMSSS(-2.419300063 ) ;
			NU.X[5] = TO_DTKMSSS( 7.1503057135 ) ;
			NU.Sb   = 0.014/9.80665*2.0 ;
			NU.Kp   = 0 ;
			NU.M0   = 5500 ;
			NU.MF   = 500 ;
			*/
		}

		if (task >= 30 && task < 40 || task == 311) {
			// НУ для сверки вариантов 30, 31, 32, 33
			NU.t << DATETIMEDMB(14, 12, 2015, 12, 0, 0, 0);
			NU.vit = 1;
			NU.sk = _SK_GSK;
			NU.X[0] = TO_DTKM(-6587.1866159);
			NU.X[1] = TO_DTKM(-436.2698529);
			NU.X[2] = TO_DTKM(0);
			NU.X[3] = TO_DTKMSSS(0.1687606245);
			NU.X[4] = TO_DTKMSSS(-2.5480965961);
			NU.X[5] = TO_DTKMSSS(7.1501923050);
			NU.Sb = 0.014 / 9.80665;
			NU.Kp = 0;
			NU.M0 = 5500;
			NU.MF = 500;
		}
	}

	//-------------------------------------------------------------------------
	// Шкала сил
	memset(&LSF, 0, sizeof(LSF));
	LSF.num = 1;
	LSF.vgpz = GPZ90;
	LSF.ngpz = 8;
	LSF.mgpz = 8;
	LSF.vatm = ATMSMA81;
	LSF.vsvd = 0;
	LSF.varsp = 0;
	LSF.vTide = TIDENULL;
	LSF.isDU = 1;
	LSF.isSun = 1;
	LSF.isMoon = 1;

	//-------------------------------------------------------------------------
	// Параметризация документирования результатов прогноза
	PrintMng->MPK.ApeakOn();
	PrintMng->MPK.FromVitOn();
	PrintMng->MPK << MODPRDATE;
	PrintMng->MPK << MODPRTIME;
	PrintMng->MPK << MODPR_VIT;
	PrintMng->MPK << MODPR_MSSV;
	PrintMng->MPK << MODPR_M;
	PrintMng->MPK << MODPR_V_Tdr;
	PrintMng->MPK << MODPR_L;
	PrintMng->MPK << MODPR_V_DL;
	PrintMng->MPK << MODPR_V_DLS;
	PrintMng->MPK << MODPR_Aosk;
	PrintMng->MPK << MODPR_eosk;
	PrintMng->MPK << MODPR_V_Hsr;
	PrintMng->MPK << MODPR_V_Hmaxt;
	PrintMng->MPK << MODPR_V_HmaxU;
	PrintMng->MPK << MODPR_V_Hmax;
	PrintMng->MPK << MODPR_V_Hmint;
	PrintMng->MPK << MODPR_V_HminU;
	PrintMng->MPK << MODPR_V_Hmin;
	//PrintMng->MPK << MODPR_V_Rmaxt ;
	//PrintMng->MPK << MODPR_V_RmaxU ;
	//PrintMng->MPK << MODPR_V_Rmax ;
	//PrintMng->MPK << MODPR_V_Rmint ;
	//PrintMng->MPK << MODPR_V_RminU ;
	//PrintMng->MPK << MODPR_V_Rmin ;

	//-------------------------------------------------------------------------
	MS->MZ.ks    = 0;					// Тип КА - тип рассчитываемых манёвров (0-как для "Янтаря")
	MS->MZ.KA()  = NU.Key.ka;			// Условный номер КА
	MS->MZ.DU()  = 1;					// Используемая ДУ (её номер)
	MS->MZ.VAR() = MS->MZ.FINDVARTASK(task);// Номер задачи в общем списке по порядку.	

	// Формирование ключей (основного и дополнительного) 
	// требуемых на правом конце траектории параметров 
	MS->MZ.GetParKorFromVar(MS->MZ.DemandsKey);
	MS->MZ.GetAdvParKorFromVar(MS->MZ.SecondDemandsKey);

	//-------------------------------------------------------------------------
	// Начало документирование .... Создание документа
	char NameOrb[64] = "_";
	switch (MS->MZ.ORB()) {
	case SPO_ORB_OMIV: sprintf(NameOrb, "OMIV\0"); break;
	case SPO_ORB_DU:   sprintf(NameOrb, "DU\0"); break;
	case SPO_ORB_EL:   sprintf(NameOrb, "EL\0"); break;
	case SPO_ORB_HBH:  sprintf(NameOrb, "HBH\0"); break;
	}
	char NameFile[256]; sprintf(NameFile, "_TaskMagicFly%d_%s\0", task, NameOrb);
	PrintMng->PrePrint(&KA, "Расчёт коррекции орблиты", NameFile, pfp);

	return 0;
}

//-----------------------------------------------------------------------------