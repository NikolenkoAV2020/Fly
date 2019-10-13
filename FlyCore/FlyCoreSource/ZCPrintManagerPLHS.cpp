//-------------------------------------------------------------------------------
// ZCPrintManagerPLHS.cpp
// �. ��������� 12.10.2018
// ���������������� ���������� ����� ���
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>

#include <FlyHtmlWriter.h>		// ���������������� � ������� Html

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ZCPrintManager.h>


//-------------------------------------------------------------------------------
int ZCPrintManager::PrintLSF(ZLSF& LS, int VarPrint, FILE* fp)
{
	if (!fp) fp = TextFile() ;
	if (!fp) return-1 ;

	char S[512] ;
	char pt[13][10] = {
		"�����","������","����","������","������","����",
		"������","����","������","��������","������","���"};
	VarPrint = 1 ; 
	if (VarPrint==1) {
		// ������ ����������������

		//-------------------------------------------------------------------------
		// � Html ��������
		if (HtmlCreator) {
			int j = 0;
			(*HtmlCreator)(HtmlCreator->CountItems()).n = "br";
			Z�HtmlItem& I = (*HtmlCreator)(HtmlCreator->CountItems());
			I.n = "table";
			I[0] << "class\n NU";

			//---------------------------------------------------------------------
			I(j).n = "tr";
			I(j)(0).n   = "th";
			sprintf(S, "��� - %d", LS.num);
			I(j)(0).txt = S;
			j++;

			//-----------------------------------------------------------------
			I(j).n = "tr";
			I(j)(0).n = "td";
			I(j)(0)[1] << "style \n text-align : left";
			sprintf(S, "��� ���-%2d (%02dx%02d)<br>", 

			LS.vgpz, LS.ngpz, (LS.mgpz>LS.ngpz ? LS.ngpz : LS.mgpz));
			sprintf(&S[strlen(S)], "��� ���-%2d ��� ���-%2d ���  ��-%2d<br>", 
			LS.vatm, LS.vsvd, LS.isDU);
			sprintf(&S[strlen(S)], "��� ���-%2d ��� ���-%2d ��� ���-%2d",
			LS.isSun, LS.isMoon, LS.vTide);

			I(j)(0).txt = S;
			j++;

			//-----------------------------------------------------------------
			S[0] = '\0';
			for (int i = 0; i<8; i++) {
				if (LS.isPlanet[i]) {
					int   n = strlen(S);
					char *ps = &S[n>0 ? n : 0];
					sprintf(ps, "%s%s\0", (n ? ", " : " "), pt[i + 3]);
				}
			}
			if (strlen(S)) {
				I(j).n = "tr";
				I(j)(0).n = "td";
				I(j)(0)[1] << "style \n text-align : left";
				I(j)(0).txt = S;
			}
		}

		//---------------------------------------------------------------------
		// � ��������� ��������
		sprintf(S,"______��� - %d", LS.num );
		int lenstring = strlen(S) ;
		while (lenstring<60) { sprintf(&S[lenstring], "_\0" ) ; lenstring++ ; } 
		S[lenstring] = '\0' ;
		fprintf(fp, "%s\n", S) ;
		sprintf(S, " ��� ���-%2d (%02dx%02d)", LS.vgpz, LS.ngpz, (LS.mgpz>LS.ngpz?LS.ngpz:LS.mgpz)) ;
		fprintf(fp, "%s\n", S) ;

		sprintf(S, " ��� ���-%2d ��� ���-%2d ���  ��-%2d ", LS.vatm, LS.vsvd, LS.isDU) ;
		fprintf(fp, "%s\n", S) ;

		sprintf(S, " ��� ���-%2d ��� ���-%2d ��� ���-%2d", LS.isSun, LS.isMoon, LS.vTide);
		fprintf(fp, "%s\n", S) ;

		S[0] = '\0' ;
		for (int i=0 ; i<8 ; i++) {
			if (LS.isPlanet[i]) {
				int   n  = strlen(S) ;
				char *ps =&S[n>0 ? n:0] ; 
				sprintf(ps, "%s%s\0", (n ? ", ":" "), pt[i+3]) ;
		}	}
		if (strlen(S)) fprintf(fp, "%s\n", S) ;		

		sprintf(S,"____________________________________________________________");
		fprintf(fp, "%s\n", S) ;		
		return 0 ;
	}
	return 0 ;
}

//-------------------------------------------------------------------------------