//-------------------------------------------------------------------------------
// ZCPrintManagerPNU.cpp
// �. ��������� 12.10.2018
// ���������������� ��
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>
#include <FlyTime.h>			// �� ��� �����
#include <FlyHtmlWriter.h>		// ���������������� � ������� Html

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
		case _SK_ASK1975: sprintf(NameSK, "��� 1975�.\0" ) ; break ;
		case _SK_ASK2000: sprintf(NameSK, "��� 2000�.\0" ) ; break ; 
		case _SK_ASKTE:   sprintf(NameSK, "��� ��\0" ) ; break ; 
		case _SK_ASKTES:  sprintf(NameSK, "��� ���\0" ) ; break ; 
		case _SK_OSK1975: sprintf(NameSK, "�� 1975�.\0" ) ; break ; 
		case _SK_OSK2000: sprintf(NameSK, "�� 2000�.\0" ) ; break ; 
		case _SK_OSKTE:   sprintf(NameSK, "�� ��\0" ) ; break ; 
		case _SK_OSKTES:  sprintf(NameSK, "�� ���\0" ) ; break ; 
		case _SK_GSK:     sprintf(NameSK, "���\0" ) ; break ; 
	}

	ZNU	 TNU ;
	DATE dk;
	TIME vk ;
	TA_DMB(0, &dk, &vk, &NU.t) ;

	if (VarPrint==1) {
		// ������
		TNU = NU;
		sprintf(NameSK, "��-2000�.\0" ) ;
		ConvertSkNu(TNU, _SK_ASK2000) ; 
		C00[0] = TNU.t ;
		memcpy(&C00[1],TNU.X,6*sizeof(double));

		fprintf(fp, "%s", Separator) ;
		sprintf(TxtStrNu,"��-%d  ��-%ld/%d/%d/%d %s  ���-%d ",
		NU.Key.ka, NU.Key.nom, NU.Key.type, NU.Key.mod, NU.Key.bc, NameSK, NU.numlsf);
		fprintf(fp, "%s\n", TxtStrNu) ;
		fprintf(fp, "%s", Separator) ;

		C00[0]=NU.t ;
		memcpy(C00+1, &TNU.X, 6*sizeof(double)) ;	

		sprintf(TxtStrNu,"����    %02d.%02d.%04d       �����    %02d:%02d:%07.4lf      �����   %6d",
		dk.d, dk.m, dk.g, vk.h, vk.m, (double)(vk.s + vk.d), TNU.vit) ;
		fprintf(fp, "%s\n", TxtStrNu) ;
		fprintf(fp, "%s", Separator) ;

		sprintf(TxtStrNu,"X   %13.6lf  ��  Y   %13.6lf  ��  Z   %13.6lf  �� ", C00[1]*10000.,C00[2]*10000.,C00[3]*10000.);
		fprintf(fp, "%s\n", TxtStrNu) ;
		sprintf(TxtStrNu,"Vx  %13.6lf �/�  Vy  %13.6lf �/�  Vz  %13.6lf �/� ", C00[4]*k2,C00[5]*k2,C00[6]*k2);
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
		// ����
		// �������� ��
		sprintf(TxtStrNu,"��-%d ��-%ld/%d/%d/%d ��-%s  ���-%d  ",
		NU.Key.ka, NU.Key.nom, NU.Key.type, NU.Key.mod, NU.Key.bc, (VarPrint==2 ? "1975�.":"2000�."), NU.numlsf) ;
		fprintf(fp, "%s\n", TxtStrNu) ;

		sprintf(TxtStrNu,"����%4.2d.%2.2d.%4.4d  �����  %02d:%02d:%07.4lf  ����� %d",
		dk.d, dk.m, dk.g, vk.h, vk.m, (double)(vk.s + vk.d), Vit);
		fprintf(fp, "%s\n", TxtStrNu) ;

		double* C = (VarPrint==2 ? C75:C00) ;
		sprintf(TxtStrNu,"X%16.6lf ��   Y%16.6lf ��   Z%16.6lf ��   ", C[1]*k1, C[2]*k1, C[3]*k1) ;
		fprintf(fp, "%s\n", TxtStrNu) ;
		sprintf(TxtStrNu,"Vx%15.9lf ��/� Vy%15.9lf ��/� Vz%15.9lf ��/� ", C[4]*k2/1000., C[5]*k2/1000., C[6]*k2/1000.) ;
		fprintf(fp, "%s\n", TxtStrNu) ;

		TxtStrNu[0]='\0';
		if (NU.Sb>0) sprintf(TxtStrNu,"S��� %9.6lf (��.�2)  ", NU.Sb) ;
		if (NU.Kp>0) sprintf(TxtStrNu+strlen(TxtStrNu),"���� %9.4lf  ", NU.Kp) ;
		if (NU.M0>0) sprintf(TxtStrNu+strlen(TxtStrNu),"����� %9.4lf (��)  ", NU.M0+NU.MF) ;
		if (TxtStrNu[0]!=0) fprintf(fp, "%s\n\n", TxtStrNu) ; 

		return 0;
	} 

	//-------------------------------------------------------------------------
	// ����������� ��
	//-------------------------------------------------------------------------
	if (HtmlCreator) {
		// ������ ��������� ��������� ������� ����� �������� ��������
		int j = HtmlCreator->CountItems();
		// ����� �� � ��������� ������� ������ ������
		(*HtmlCreator)(j+1).n = "br";
		// �������� ������ �� ������� ��������� �� �������� ����� ������ �������
		Z�HtmlItem& I = (*HtmlCreator)(j);

		// !!!! ����� ����� !!!!
		// ��� ��������� ������ �� ������� html-��������� ������ ��������� 
		// �������� ����� ��������� � ������� ������������ ���� ��� ������������
		// ���������� ������. 
		// ������� ������� � ���, ��� ��� �������� ������ �������� ������ 
		// ���������� �� ��������������, ��� ��� ��������� html-��������� ������
		// ������ ��������� ��� vector � ��� �������� ������ �������� ���������
		// �������� ����������������� ������ vector-� ...
		// ��� �������� ���:
		//		
		//		int j = HtmlCreator->CountItems();
		//		// �������� �������� � �������� j
		//		Z�HtmlItem& I = (*HtmlCreator)(j);
		//		// �������� �������� � �������� j+1 � ������������ ��������� 
		//		// ������ �� ����
		//		(*HtmlCreator)(j+1).n = "br";
		//		// !!!! � ��� ������ ��������� �������� �� ����� ���������� 
		//		// ������ ����� ������� ������ !!!!
		//		I.n = "table";			// I ��� �� �������������� ������
		//
		// ������� ��������� ������ ��� ��� ��������� �������������� �����
		// ��������� ������������ ... ���� ����������
		//
		
		// �� ������ I ������ ������� 
		I.n = "table";
		I[0] << "class\n NU";

		//---------------------------------------------------------------------
		// �������� ������ ������ ��������
		j = 0;
		I(j).n = "tr";
		I(j)(0).n = "th";
		sprintf(TxtStrNu, "�� ��-%d\0", NU.Key.ka);
		I(j)(0).txt = TxtStrNu;
		I(j)(0)[0] << "colspan \n 5";
		j++;

		I(j)(0).n = "td";
		I(j)(0)[0] << "colspan \n 5";
		I(j)(0)[1] << "style \n text-align : left";
		sprintf(TxtStrNu, "�����-%ld ���-%d ���-%d ��-%d  ����� ���-%d<br>����  %2.2d.%2.2d.%4.4d   �����  %02d:%02d:%07.4lf   ����� %-6d", 
		NU.Key.nom, NU.Key.type, NU.Key.mod, NU.Key.bc, NU.numlsf,
		dk.d, dk.m, dk.g, vk.h, vk.m, (double)(vk.s + vk.d), Vit);
		I(j)(0).txt = TxtStrNu;
		j++;

		//---------------------------------------------------------------------
		// �������� ������ ������ ��������
		I(j).n = "tr";
		I(j)(0).n = "th";
		I(j)(0)[0] << "colspan \n 2";
		I(j)(0).txt = "�����, ��";
		I(j)(1).n = "th";
		I(j)(1).txt = "S<sub>���</sub>, ��*�2";
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
		I(j)(1).txt = "���-2000";

		I(j)(2).n   = "th";
		I(j)(2).txt = "���    ";

		I(j)(3).n   = "th";
		I(j)(3).txt = "���-����";
		j++;

		//---------------------------------------------------------------------
		I(j).n = "tr";
		I(j)(0).n   = "th";
		I(j)(0).txt = "X,<br>Y,<br>Z,<br>V<sub>x</sub>,<br>V<sub>y</sub>,<br>V<sub>z</sub>,";

		I(j)(1).n   = "th";
		I(j)(1).txt = "��<br>��<br>��<br>�/���<br>�/���<br>�/���";

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
		I(j)(1).txt = "���-2000";

		I(j)(2).n   = "th";
		I(j)(2).txt = "���-����";

		I(j)(3).n   = "th";
		I(j)(3).txt = "";
		j++;

		//---------------------------------------------------------------------------
		I(j).n = "tr";
		I(j)(0).n   = "th";
		I(j)(0).txt = "a,<br>e<br>i,<br><font face=\"symbol\">W</font>,<br><font face=\"symbol\">w</font>, <br>u,";

		I(j)(1).n   = "th";
		I(j)(1).txt = "��<br> <br>����<br>����<br>����<br>����";

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
	sprintf(TxtStrNu, "__�� ��-%d\0", NU.Key.ka);
	if (Title && Title[0]!='\0') sprintf( &TxtStrNu[strlen(TxtStrNu)], "%s\0", Title) ;
	int lenstring = strlen(TxtStrNu) ;
	while (lenstring<60) { TxtStrNu[lenstring] = '_' ; lenstring++ ; }
	TxtStrNu[lenstring] = '\0' ;
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu," �����-%ld ���-%d ���-%d ��-%d  ����� ���-%d", NU.Key.nom, NU.Key.type, NU.Key.mod, NU.Key.bc, NU.numlsf) ;
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," ����  %2.2d.%2.2d.%4.4d   �����  %02d:%02d:%07.4lf   ����� %-6d", dk.d, dk.m, dk.g, vk.h, vk.m, (double)(vk.s+vk.d), Vit);
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu, "____________________________________________________________");
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu,"             ���-2000             ���            ���-����");
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," X,   ��%15.6lf%18.6lf%18.6lf",C00[1]*k1,CG[1]*k1,CTE[1]*k1);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Y,   ��%15.6lf%18.6lf%18.6lf",C00[2]*k1,CG[2]*k1,CTE[2]*k1);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Z,   ��%15.6lf%18.6lf%18.6lf",C00[3]*k1,CG[3]*k1,CTE[3]*k1);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Vx, �/�%15.6lf%18.6lf%18.6lf",C00[4]*k2,CG[4]*k2,CTE[4]*k2);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Vy, �/�%15.6lf%18.6lf%18.6lf",C00[5]*k2,CG[5]*k2,CTE[5]*k2);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," Vz, �/�%15.6lf%18.6lf%18.6lf",C00[6]*k2,CG[6]*k2,CTE[6]*k2);
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu, "____________________________________________________________");
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu,"             ���-2000          ���-����");
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," a,   ��%15.6lf   %15.6lf", K00[1]*k1, KTE[1]*k1);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," e,     %18.9lf   %15.9lf", K00[2], KTE[2]);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," i, ����%15.6lf   %15.6lf", K00[3]*k3, KTE[3]*k3);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," W, ����%15.6lf   %15.6lf", K00[4]*k3, KTE[4]*k3);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," w, ����%15.6lf   %15.6lf", K00[5]*k3, KTE[5]*k3);
	fprintf(fp, "%s\n", TxtStrNu) ;
	sprintf(TxtStrNu," u, ����%15.6lf   %15.6lf", K00[6]*k3, KTE[6]*k3);
	fprintf(fp, "%s\n", TxtStrNu) ;

	sprintf(TxtStrNu, "____________________________________________________________");
	fprintf(fp, "%s\n", TxtStrNu) ;

	TxtStrNu[0]='\0';
	if (NU.Sb>0) sprintf(TxtStrNu,"S��� %9.6lf (��.�2)  ", NU.Sb) ;
	if (NU.Kp>0) sprintf(TxtStrNu+strlen(TxtStrNu),"���� %9.4lf  ", NU.Kp) ;
	if (NU.M0>0) sprintf(TxtStrNu+strlen(TxtStrNu),"����� %9.4lf (��)  ", NU.M0+NU.MF) ;
	if (TxtStrNu[0]!=0) fprintf(fp, "%s\n", TxtStrNu) ; 

	sprintf(TxtStrNu, "____________________________________________________________");
	fprintf(fp, "%s\n\n", TxtStrNu) ;

	return 0 ;
}

//-------------------------------------------------------------------------------