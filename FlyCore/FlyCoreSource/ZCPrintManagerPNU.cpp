//-------------------------------------------------------------------------------
// ZCPrintManagerPNU.cpp
// А. Николенко 12.10.2018
// Документирование НУ
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>
#include <FlyTime.h>			// Всё про время
#include <FlyHtmlWriter.h>		// Документирование в формате Html

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZSKNU.h>
#include <FlyCoreSource\\ZCPrintManager.h>

//-------------------------------------------------------------------------------
char ZCPrintManager::CommonFileSticker[64] ="\0" ;

//-------------------------------------------------------------------------------
int ZCPrintManager::PrintNU(ZNU& NU, char* Title, int VarPrint, FILE* fp)
{
	if ( !fp ) fp = TextFile() ;
	if ( !fp ) return-1 ;

	char	TxtStrNu[500] ;
	double	C00[9], K00[9], C75[9], CTE[9], KTE[9], CG[9], k1=1.e4, k2=1000./8.64, k3=180.0/k_pi ;
	char	NameSK[64] ;
	long	Vit ;
	char*Separator="___________________________________________________________________\n";
	bool Flag_Ext_NU;

	if (VarPrint<0 || VarPrint>3) VarPrint = 3 ;  

	switch(NU.sk) {
		case _SK_ASK1975: sprintf(NameSK, "АСК 1975г.\0" ) ; break ;
		case _SK_ASK2000: sprintf(NameSK, "АСК 2000г.\0" ) ; break ; 
		case _SK_ASKTE:   sprintf(NameSK, "АСК ТЭ\0" ) ; break ; 
		case _SK_ASKTES:  sprintf(NameSK, "АСК ТЭС\0" ) ; break ; 
		case _SK_OSK1975: sprintf(NameSK, "КЭ 1975г.\0" ) ; break ; 
		case _SK_OSK2000: sprintf(NameSK, "КЭ 2000г.\0" ) ; break ; 
		case _SK_OSKTE:   sprintf(NameSK, "КЭ ТЭ\0" ) ; break ; 
		case _SK_OSKTES:  sprintf(NameSK, "КЭ ТЭС\0" ) ; break ; 
		case _SK_GSK:     sprintf(NameSK, "ГСК\0" ) ; break ; 
	}

	ZNU	 TNU ;
	DATE dk;
	TIME vk ;
	TA_DMB(0, &dk, &vk, &NU.t) ;

	if (VarPrint==1) {
		// Кондор
		TNU = NU;
		sprintf(NameSK, "СК-2000г.\0" ) ;
		ConvertSkNu(TNU, _SK_ASK2000) ; 
		C00[0] = TNU.t ;
		memcpy(&C00[1],TNU.X,6*sizeof(double));

		fprintf(fp, "%s", Separator) ;
		sprintf(TxtStrNu,"КА-%d  НУ-%ld/%d/%d/%d %s  ЛШС-%d ",
		NU.Key.ka, NU.Key.nom, NU.Key.type, NU.Key.mod, NU.Key.bc, NameSK, NU.numlsf);
		fprintf(fp, "%s\n", TxtStrNu) ;
		fprintf(fp, "%s", Separator) ;

		C00[0]=NU.t ;
		memcpy(C00+1, &TNU.X, 6*sizeof(double)) ;	

		sprintf(TxtStrNu,"ДАТА    %02d.%02d.%04d       ВРЕМЯ    %02d:%02d:%07.4lf      Виток   %6d",
		dk.d, dk.m, dk.g, vk.h, vk.m, (double)(vk.s + vk.d), TNU.vit) ;
		fprintf(fp, "%s\n", TxtStrNu) ;
		fprintf(fp, "%s", Separator) ;

		sprintf(TxtStrNu,"X   %13.6lf  км  Y   %13.6lf  км  Z   %13.6lf  км ", C00[1]*10000.,C00[2]*10000.,C00[3]*10000.);
		fprintf(fp, "%s\n", TxtStrNu) ;
		sprintf(TxtStrNu,"Vx  %13.6lf м/с  Vy  %13.6lf м/с  Vz  %13.6lf м/с ", C00[4]*k2,C00[5]*k2,C00[6]*k2);
		fprintf(fp, "%s\n", TxtStrNu) ;
		fprintf(fp, "%s", Separator) ;

		Flag_Ext_NU = false;
		TxtStrNu[0] = 0;
		if(Flag_Ext_NU) {
			fprintf(fp, "%s\n", TxtStrNu) ;
			fprintf(fp, "%s", Separator) ;
		}
		fprintf(fp, "\n") ;

		return 0 ;
	} 
	
	TNU = NU ;
	ConvertSkNu(TNU, _SK_ASK1975) ; 
	C75[0] = TNU.t ;
	memcpy(&C75[1], TNU.X, 6*sizeof(double)) ;

	TNU = NU ;
	ConvertSkNu(TNU, _SK_GSK) ; 
	CG[0] = TNU.t ;
	memcpy(&CG[1], TNU.X, 6*sizeof(double)) ;

	TNU = NU ;
	ConvertSkNu(TNU, _SK_ASK2000) ; 
	C00[0] = NU.t ;
	memcpy(&C00[1], TNU.X, 6*sizeof(double)) ;

    TNU = NU;
	ConvertSkNu(TNU, _SK_ASKTE) ; 
	CTE[0] = TNU.t ;
	memcpy(&CTE[1], TNU.X, 6*sizeof(double)) ;
	Vit = TNU.vit ;

    TNU = NU;
	ConvertSkNu(TNU, _SK_OSK2000) ; 
	K00[0] = TNU.t ;
	memcpy(&K00[1], TNU.X, 6*sizeof(double)) ;

    TNU = NU;
	ConvertSkNu(TNU, _SK_OSKTE) ; 
	KTE[0] = TNU.t ;
	memcpy(&KTE[1], TNU.X, 6*sizeof(double)) ;

	if(VarPrint==2) {
		// ЕССС
		// Короткие НУ
		sprintf(TxtStrNu,"КА-%d НУ-%ld/%d/%d/%d СК-%s  ЛШС-%d  ",
		NU.Key.ka, NU.Key.nom, NU.Key.type, NU.Key.mod, NU.Key.bc, (VarPrint==2 ? "1975г.":"2000г."), NU.numlsf) ;
		fprintf(fp, "%s\n", TxtStrNu) ;

		sprintf(TxtStrNu,"ДАТА%4.2d.%2.2d.%4.4d  ВРЕМЯ  %02d:%02d:%07.4lf  Виток %d",
		dk.d, dk.m, dk.g, vk.h, vk.m, (double)(vk.s + vk.d), Vit);
		fprintf(fp, "%s\n", TxtStrNu) ;

		double* C = (VarPrint==2 ? C75:C00) ;
		sprintf(TxtStrNu,"X%16.6lf км   Y%16.6lf км   Z%16.6lf км   ", C[1]*k1, C[2]*k1, C[3]*k1) ;
		fprintf(fp, "%s\n", TxtStrNu) ;
		sprintf(TxtStrNu,"Vx%15.9lf км/с Vy%15.9lf км/с Vz%15.9lf км/с ", C[4]*k2/1000., C[5]*k2/1000., C[6]*k2/1000.) ;
		fprintf(fp, "%s\n", TxtStrNu) ;

		TxtStrNu[0]='\0';
		if (NU.Sb>0) sprintf(TxtStrNu,"Sбал %9.6lf (кг.м2)  ", NU.Sb) ;
		if (NU.Kp>0) sprintf(TxtStrNu+strlen(TxtStrNu),"Ксвд %9.4lf  ", NU.Kp) ;
		if (NU.M0>0) sprintf(TxtStrNu+strlen(TxtStrNu),"Масса %9.4lf (кг)  ", NU.M0+NU.MF) ;
		if (TxtStrNu[0]!=0) fprintf(fp, "%s\n\n", TxtStrNu) ; 

		return 0;
	} 

	//-------------------------------------------------------------------------
	// Развернутые НУ
	//-------------------------------------------------------------------------
	if (HtmlCreator) {
		// Индекс элемкента документа который будет являться таблицей
		int j = HtmlCreator->CountItems();
		// После НУ в документе следует пустая строка
		(*HtmlCreator)(j+1).n = "br";
		// Получаем ссылку на элемент документа из которого будем делать таблицу
		ZСHtmlItem& I = (*HtmlCreator)(j);

		// !!!! Очень важно !!!!
		// при получении ссылки на элемент html-документа нельзя допускать 
		// создания ноавх элементов в области программного кода где используется
		// упомянутая ссылка. 
		// Причина состоит в том, что при создании нового элемента ссылка 
		// становится не действительной, так как контейнер html-документа хранит
		// массив элементов как vector и при создании нового элемента документа
		// возможно перераспределение памяти vector-а ...
		// Так например код:
		//		
		//		int j = HtmlCreator->CountItems();
		//		// Создание элемента с индексом j
		//		ZСHtmlItem& I = (*HtmlCreator)(j);
		//		// Создание элемента с индексом j+1 и одновременно получение 
		//		// ссылки на него
		//		(*HtmlCreator)(j+1).n = "br";
		//		// !!!! А вот теперь следующий оператор во время исполнения 
		//		// скорее всего вызовет ошибку !!!!
		//		I.n = "table";			// I уже не действительная ссылка
		//
		// Поэтому правильно писать так как изложенно непосредсвенно перед
		// настоящим комментарием ... итак продолжаем
		//
		
		// По ссылке I создаём таблицу 
		I.n = "table";
		I[0] << "class\n NU";

		//---------------------------------------------------------------------
		// Создание первой строки такблицы
		j = 0;
		I(j).n = "tr";
		I(j)(0).n = "th";
		sprintf(TxtStrNu, "НУ КА-%d\0", NU.Key.ka);
		I(j)(0).txt = TxtStrNu;
		I(j)(0)[0] << "colspan \n 5";
		j++;

		I(j)(0).n = "td";
		I(j)(0)[0] << "colspan \n 5";
		I(j)(0)[1] << "style \n text-align : left";
		sprintf(TxtStrNu, "Номер-%ld Тип-%d Мод-%d БЦ-%d  Номер ЛШС-%d<br>Дата  %2.2d.%2.2d.%4.4d   Время  %02d:%02d:%07.4lf   Виток %-6d", 
		NU.Key.nom, NU.Key.type, NU.Key.mod, NU.Key.bc, NU.numlsf,
		dk.d, dk.m, dk.g, vk.h, vk.m, (double)(vk.s + vk.d), Vit);
		I(j)(0).txt = TxtStrNu;
		j++;

		//---------------------------------------------------------------------
		// Создание второй строки такблицы
		I(j).n = "tr";
		I(j)(0).n = "th";
		I(j)(0)[0] << "colspan \n 2";
		I(j)(0).txt = "Масса, кг";
		I(j)(1).n = "th";
		I(j)(1).txt = "S<sub>бал</sub>, кг*м2";
		I(j)(2).n = "th";
		I(j)(2).txt = "<font face=\"symbol\">K</font>";
		I(j)(3).n = "th";
		I(j)(3).txt = "";
		j++;

		//---------------------------------------------------------------------
		I(j).n = "tr";

		I(j)(0).n = "td";
		I(j)(0)[0] << "colspan \n 2";
		sprintf(TxtStrNu, "%9.4lf", NU.M0 + NU.MF);
		I(j)(0).txt = NU.M0 + NU.MF > 0 ? TxtStrNu : "";

		I(j)(1).n = "td";
		sprintf(TxtStrNu, "%9.6lf", NU.Sb);
		I(j)(1).txt = NU.Sb > 0 ? TxtStrNu : "";

		I(j)(2).n = "td";
		sprintf(TxtStrNu, "%9.4lf", NU.Kp);
		I(j)(2).txt = NU.Kp > 0 ? TxtStrNu : "";

		I(j)(3).n = "td";
		I(j)(3).txt = "";
		j++;

		//---------------------------------------------------------------------
		I(j).n = "tr";
		I(j)(0).n   = "td";
		I(j)(0)[0] << "colspan \n 2";
		I(j)(0).txt = "";

		I(j)(1).n   = "th";
		I(j)(1).txt = "АСК-2000";

		I(j)(2).n   = "th";
		I(j)(2).txt = "ГСК    ";

		I(j)(3).n   = "th";
		I(j)(3).txt = "АСК-ИстЭ";
		j++;

		//---------------------------------------------------------------------
		I(j).n = "tr";
		I(j)(0).n   = "th";
		I(j)(0).txt = "X,<br>Y,<br>Z,<br>V<sub>x</sub>,<br>V<sub>y</sub>,<br>V<sub>z</sub>,";

		I(j)(1).n   = "th";
		I(j)(1).txt = "км<br>км<br>км<br>м/сек<br>м/сек<br>м/сек";

		I(j)(2).n   = "td";
		sprintf(TxtStrNu, " %15.6lf<br>%15.6lf<br>%15.6lf<br>%15.6lf<br>%15.6lf<br>%15.6lf",
		C00[1] * k1, C00[2] * k1, C00[3] * k1, 
		C00[4] * k2, C00[5] * k2, C00[6] * k2);
		I(j)(2).txt = TxtStrNu;

		I(j)(3).n = "td";
		sprintf(TxtStrNu, " %18.6lf<br>%18.6lf<br>%18.6lf<br>%18.6lf<br>%18.6lf<br>%18.6lf",
		CG[1] * k1, CG[2] * k1, CG[3] * k1, 
		CG[4] * k2, CG[5] * k2, CG[6] * k2);
		I(j)(3).txt = TxtStrNu;

		I(j)(4).n = "td";
		sprintf(TxtStrNu, " %18.6lf<br>%18.6lf<br>%18.6lf<br>%18.6lf<br>%18.6lf<br>%18.6lf",
		CTE[1] * k1, CTE[2] * k1, CTE[3] * k1, 
		CTE[4] * k2, CTE[5] * k2, CTE[6] * k2);
		I(j)(4).txt = TxtStrNu;
		j++;

		//---------------------------------------------------------------------------
		I(j).n = "tr";
		I(j)(0).n   = "td";
		I(j)(0)[0] << "colspan \n 2";
		I(j)(0).txt = "";

		I(j)(1).n   = "th";
		I(j)(1).txt = "ОСК-2000";

		I(j)(2).n   = "th";
		I(j)(2).txt = "ОСК-ИстЭ";

		I(j)(3).n   = "th";
		I(j)(3).txt = "";
		j++;

		//---------------------------------------------------------------------------
		I(j).n = "tr";
		I(j)(0).n   = "th";
		I(j)(0).txt = "a,<br>e<br>i,<br><font face=\"symbol\">W</font>,<br><font face=\"symbol\">w</font>, <br>u,";

		I(j)(1).n   = "th";
		I(j)(1).txt = "км<br> <br>град<br>град<br>град<br>град";

		I(j)(2).n   = "td";
		sprintf(TxtStrNu, " %15.6lf<br>%18.9lf<br>%15.6lf<br>%15.6lf<br>%15.6lf<br>%15.6lf",
		K00[1] * k1, K00[2],      K00[3] * k3,
		K00[4] * k3, K00[5] * k3, K00[6] * k3);
		I(j)(2).txt = TxtStrNu;

		I(j)(3).n   = "td";
		sprintf(TxtStrNu, " %15.6lf<br>%18.9lf<br>%15.6lf<br>%15.6lf<br>%15.6lf<br>%15.6lf",
		KTE[1] * k1, KTE[2],      KTE[3] * k3,
		KTE[4] * k3, KTE[5] * k3, KTE[6] * k3);
		I(j)(3).txt = TxtStrNu;

	}

	//--------------------------------------------------------------------------
	sprintf(TxtStrNu, "__НУ КА-%d\0", NU.Key.ka);
	if (Title && Title[0]!='\0') sprintf( &TxtStrNu[strlen(TxtStrNu)], "%s\0", Title) ;
	int lenstring = strlen(TxtStrNu) ;
	while (lenstring<60) { TxtStrNu[lenstring] = '_' ; lenstring++ ; }
	TxtStrNu[lenstring] = '\0' ;
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu," Номер-%ld Тип-%d Мод-%d БЦ-%d  Номер ЛШС-%d", NU.Key.nom, NU.Key.type, NU.Key.mod, NU.Key.bc, NU.numlsf) ;
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Дата  %2.2d.%2.2d.%4.4d   Время  %02d:%02d:%07.4lf   Виток %-6d", dk.d, dk.m, dk.g, vk.h, vk.m, (double)(vk.s+vk.d), Vit);
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu, "____________________________________________________________");
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu,"             АСК-2000             ГСК            АСК-ИстЭ");
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," X,   км%15.6lf%18.6lf%18.6lf",C00[1]*k1,CG[1]*k1,CTE[1]*k1);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Y,   км%15.6lf%18.6lf%18.6lf",C00[2]*k1,CG[2]*k1,CTE[2]*k1);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Z,   км%15.6lf%18.6lf%18.6lf",C00[3]*k1,CG[3]*k1,CTE[3]*k1);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Vx, м/с%15.6lf%18.6lf%18.6lf",C00[4]*k2,CG[4]*k2,CTE[4]*k2);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Vy, м/с%15.6lf%18.6lf%18.6lf",C00[5]*k2,CG[5]*k2,CTE[5]*k2);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Vz, м/с%15.6lf%18.6lf%18.6lf",C00[6]*k2,CG[6]*k2,CTE[6]*k2);
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu, "____________________________________________________________");
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu,"             ОСК-2000          ОСК-ИстЭ");
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," a,   км%15.6lf   %15.6lf", K00[1]*k1, KTE[1]*k1);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," e,     %18.9lf   %15.9lf", K00[2], KTE[2]);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," i, град%15.6lf   %15.6lf", K00[3]*k3, KTE[3]*k3);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," W, град%15.6lf   %15.6lf", K00[4]*k3, KTE[4]*k3);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," w, град%15.6lf   %15.6lf", K00[5]*k3, KTE[5]*k3);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," u, град%15.6lf   %15.6lf", K00[6]*k3, KTE[6]*k3);
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu, "____________________________________________________________");
	fprintf(fp, "%s\n", TxtStrNu) ;

	TxtStrNu[0]='\0';
	if (NU.Sb>0) sprintf(TxtStrNu,"Sбал %9.6lf (кг.м2)  ", NU.Sb) ;
	if (NU.Kp>0) sprintf(TxtStrNu+strlen(TxtStrNu),"Ксвд %9.4lf  ", NU.Kp) ;
	if (NU.M0>0) sprintf(TxtStrNu+strlen(TxtStrNu),"Масса %9.4lf (кг)  ", NU.M0+NU.MF) ;
	if (TxtStrNu[0]!=0) fprintf(fp, "%s\n", TxtStrNu) ; 

	sprintf(TxtStrNu, "____________________________________________________________");
	fprintf(fp, "%s\n\n", TxtStrNu) ;

	return 0 ;
}

//-------------------------------------------------------------------------------