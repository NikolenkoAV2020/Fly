//-------------------------------------------------------------------------------
// ZCPrintManager.cpp
// �. ��������� 03.08.2018
// ���������������� ����������� ��������
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>

//-------------------------------------------------------------------------------
// ���������� Fly
#include <FlyMMath.h>	// �������������� �������
#include <FlyTime.h>	// �� ��� �����
// ����������� ���������� FlyHtmlWriter ��� ����������������
// � ������� Html
#define IMPORT_FLY_HTML_WRITER
#include <FlyHtmlWriter.h>

//-------------------------------------------------------------------------------
#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>
#include <FlyCoreSource\\ZCPrintManager.h>
#include <FlyCoreSource\\ZCPrintFormat.h>

//-------------------------------------------------------------------------------
void SmartPrintfI(FILE*pf, char* sf, int I, bool isTwoDest) {
	if (isTwoDest || pf) fprintf(pf, sf, I);
	if (isTwoDest ||!pf) printf(sf, I);
}
void SmartPrintfL(FILE*pf, char* sf, long L, bool isTwoDest) {
	if (isTwoDest || pf) fprintf(pf, sf, L);
	if (isTwoDest ||!pf) printf(sf, L);
}
void SmartPrintfD(FILE*pf, char* sf, double D, bool isTwoDest) {
	if (isTwoDest || pf) fprintf(pf, sf, D);
	if (isTwoDest ||!pf) printf(sf, D);
}
void SmartPrintfS(FILE*pf, char* sf, char* S, bool isTwoDest) {
	if (isTwoDest || pf) fprintf(pf, sf, S);
	if (isTwoDest ||!pf) printf(sf, S);
}
void SmartPrintfS(FILE*pf, char* S, bool isTwoDest) {
	if (isTwoDest || pf) fprintf(pf, S);
	if (isTwoDest ||!pf) printf(S);
}

//-------------------------------------------------------------------------------
ZCPrintManager::ZCPrintManager() 
{
	memset(FileSticker, 0, sizeof(FileSticker));
	memset(PSB, 0, sizeof(PSB));
	memset(PVB, 0, sizeof(PSB));
	HtmlCreator = nullptr;
	fullTextFileName = "";
	fullHtmlFileName = "";
}

//-------------------------------------------------------------------------------
ZCPrintManager::~ZCPrintManager() 
{
	localFp = nullptr;
	if (HtmlCreator) {
		delete HtmlCreator; HtmlCreator = nullptr;
	}
}

//-------------------------------------------------------------------------------
const char* ZCPrintManager::GetTextFileName()
{
	return (fullTextFileName.c_str());
}

const char* ZCPrintManager::GetHtmlFileName()
{
	return (fullHtmlFileName.c_str());
}

//-------------------------------------------------------------------------------
void ZCPrintManager::ShowTextFile()
{
	if (!fullTextFileName.length()) return;
	string cmd = "";
	cmd += fullTextFileName;
	system(cmd.c_str());
}

void ZCPrintManager::ShowHtmlFile()
{
	if (!fullHtmlFileName.length()) return;
	string cmd = "";
	cmd += fullHtmlFileName;
	system(cmd.c_str());
}

//-------------------------------------------------------------------------------
void ZCPrintManager::GetPrintLongName(ZMSC* pKA, 
	char* DirMame,	// ������� � ������� �������� ��������
	char* FileName, // ��� ����� ��� ���� ���������� � �������
	char* LNF,		// ������ ��� ���������� ����� �� ��������
	char* LNF2,		// ������ ��� ����� �� �������� �� ��� ����������
	char* LNF3,		// ��� ����� �� �������� �� ��� ���� � ����������
	char* LND)		// ������ ������� � ������� ����������� ����
{
	char NF[512] ;
	// ��� ��������� �� ���������
	sprintf(NF, "%s\0", FileName ? FileName:"������� ��") ;
	// �������� � ��� ��������� ����� ��
	if (pKA) sprintf(&NF[strlen(NF)], "_%d\0", pKA->GetNumKA()) ;	
	// �������� � ��� ��������� ������
	if (strlen(CommonFileSticker)) 
		sprintf(&NF[strlen(NF)], "_%s\0", CommonFileSticker);
	if (strlen(FileSticker)) 
		sprintf(&NF[strlen(NF)], "_%s\0", FileSticker) ;	
	// ������������ ������� ����� ����� (����+����)
	char ND[256] ;	
	mkdir(DIRRESULT);
	sprintf(ND, "%s\0", DIRRESULT) ; 
	if (DirMame) {
		sprintf(&ND[strlen(ND)], "\\%s\0", DirMame);
		mkdir(ND);
	}
	if (LNF ) sprintf(LNF,  "%s\\%s.txt\0", ND, NF) ;
	if (LNF2) sprintf(LNF2, "%s\\%s\0", ND, NF);
	if (LNF3) sprintf(LNF3, "%s\0", NF);
	if (LND ) sprintf(LND,  "%s\0", ND) ;
}

//-------------------------------------------------------------------------------
int ZCPrintManager::OpenPrintLongName(ZMSC* pKA, char* DirMame, char* FileName, FILE** pfp, char* FullName) 
{
	if (!pfp) assert(0) ;
	char LNF[512], LNF2[512], LNF3[512], LND[512];
	GetPrintLongName(pKA, DirMame, FileName, LNF, LNF2, LNF3, LND) ;
	if (FullName) sprintf(FullName, "%s\0", LNF);

	// �������� ���������� �����
	*pfp = fopen(LNF, "w") ; 
	if (!(*pfp)) return-1 ;
	localFp = *pfp ;

	fullTextFileName = LNF2; fullTextFileName += ".txt";

	// �������� ������� Html-����������������
	if (HtmlCreator) delete HtmlCreator;
	HtmlCreator = new ZCHtmlBuilder;
	// �������� Html-�����
	if (HtmlCreator) {
		// ���� HtmlCreator �� ����� �� �� �� ���� �������� �� ������ ��,
		// ��� �� �������� ��������� �������, ��� ��� ��������� �������� 
		// ����� ������ � ����� ������

		// ������������� Html-����������

		// ���������� ������������ ������� ������ � ��������� ���������
		HtmlCreator->S.cssLocation = 1;
		// ������������ ������� ������ CSS 
		HtmlCreator->BuildStyles();
		// ������������� � ��������� ������ ����� ��� ���� ����������
		// ������� Fly
		//HtmlCreator->UseDefaultStyles();

		// ������������� �������� ��������� 
		HtmlCreator->Init(LNF3, nullptr, 0, nullptr, LNF2);
		fullHtmlFileName = LNF2; fullHtmlFileName += ".html";
	}
	
	return 0 ;
}

//-------------------------------------------------------------------------------
// var	- ������� ����������������
//	  0 - �� ��������
//	  1 - �� �������
int ZCPrintManager::PrePrint(ZMSC* pKA, char* DirMame, char* FileName, FILE** pfp, char* FullName)
{ 	
	_getsystime(&start_TM) ;
	startTIME = time( nullptr ) ;
	startTIME = clock( ) ;

	MPF.Formated() ;
	MPF.FirstLineOn() ;

	return(OpenPrintLongName(pKA, DirMame, FileName, pfp ? pfp : &localFp, FullName));
}  

//-------------------------------------------------------------------------------
int ZCPrintManager::PrintHeaderDoc(char* hd, FILE* fp)
{ 	
	if (!fp) fp = localFp ;
	if (!fp) return-1 ;

	// � ��������� ��������
	fprintf(fp, "%s\0", hd) ;

	// � Html
	if (HtmlCreator) {
		int n = HtmlCreator->CountItems();
		(*HtmlCreator)(n).n = "h3";
		(*HtmlCreator)(n).txt = hd;
		(*HtmlCreator)(n+1).n = "hr";
	}

	return 0 ; 
}  

//-------------------------------------------------------------------------------
// ���������������� �������� ������
int ZCPrintManager::PrintText(const char* text, char countCR, FILE* fp)
{
	if (!fp) fp = localFp;
	if (!fp) return-1;

	// � ��������� ��������
	fprintf(fp, "%s\n", text);
	for (int i = 0; i < countCR; i++) fprintf(fp, "\n");

	// � Html
	if (HtmlCreator) {
		int n = HtmlCreator->CountItems();
		(*HtmlCreator)(n).n = "p class=simpletext";
		(*HtmlCreator)(n).txt = text;
		if (countCR) {
			string cr = "";
			for (int i = 0; i < countCR; i++) cr += "<br>";
			(*HtmlCreator)(n).txt += cr;
	}	}
	return 0;
}

//-------------------------------------------------------------------------------
int ZCPrintManager::PrintMSC(ZMSC* pKA, ZSC_SI* SI, ZSC_VI* VI, FILE* fp)
{
	if (!fp) fp = localFp ;
	if (!fp) return-1 ;
	if (MPK.isAflat) PrintAflatMSC(pKA, SI, VI, fp);
	else PrintApeakMSC(pKA, SI, VI, fp);
	return 0 ;
}

//-------------------------------------------------------------------------------
void ZCPrintManager::SetPrintFileSticker(char* FS) 
{
	FileSticker[0] = '\0';
	if (FS)	sprintf(FileSticker, "%s\0", FS) ;
}

//-------------------------------------------------------------------------------
int ZCPrintManager::PrintLineMSC(FILE* fp)
{
	if ( !fp ) fp = localFp ;
	if ( !fp ) return-1 ;
	if (MPK.isAflat) PrintAflatLineMSC(fp);
	else PrintApeakLineMSC(fp);
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZCPrintManager::PostPrint(FILE** pfp, int cointiter)
{
	FILE* fp ;
	if (!pfp || !(*pfp)) fp = localFp ;
	else fp = *pfp ;
	if (!fp) return-1 ;

	PrintMSC(nullptr, nullptr, nullptr, fp);

	// ������ ������� ������ ������� ������ 
	char s1[128];
	sprintf(s1, "%02d:%02d:%02d %02d:%02d:%04d\0",
		start_TM.tm_hour, start_TM.tm_min, start_TM.tm_sec, start_TM.tm_mday,
		start_TM.tm_mon, start_TM.tm_year + 1900);

	// ������ ������������ ������� ������ 
	char s2[128];
	endTIME = time(nullptr);
	endTIME = clock();
	double dt = (endTIME - startTIME) / 1e3;
	sprintf(s2, "%0.3lf ���.\0", dt);

	// ���������������� � ��������� ��������
	// :)))) "���������������� � ��������" - �������� ...
	fprintf(fp, "\n\n\n");
	char s3[64];
	if (cointiter>0) {
		sprintf(s3, "%d\0", cointiter);
		fprintf(fp, " ��������        %s\n", s3);
	}

	fprintf(fp, " ������ �������  %s\n", s1);
	fprintf(fp, " �����           %s\n", s2);

	// � Html
	if (HtmlCreator) {
		int n = HtmlCreator->CountItems();
		(*HtmlCreator)(n).n = "br";
		(*HtmlCreator)(n + 1).n = "br";
		Z�HtmlItem& I = (*HtmlCreator)(n + 2);

		I.n = "table";
		I[0] << "class\n simple";

		int j = 0;
		if (cointiter>0) {
			I(j).n = "tr";
			I(j)(0).n = "td";
			I(j)(0).txt = "��������:";
			I(j)(1).n = "td";
			I(j)(1).txt = s3;
			j++;
		}

		I(j).n = "tr";
		I(j)(0).n = "td";
		I(j)(0).txt = "������ �������:";
		I(j)(1).n = "td";
		I(j)(1).txt = s1;
		j++;

		I(j).n = "tr";
		I(j)(0).n = "td";
		I(j)(0).txt = "�����:";
		I(j)(1).n = "td";
		I(j)(1).txt = s2;

		// "����� ���� ������ � �������� Html"
		HtmlCreator->Flash();
		// "��������" ��� ������ �� ����� - ������� ���.
		// ... "������� �� �����, ������� ������ ..." ���-���-���.
		delete HtmlCreator; HtmlCreator = nullptr;
	}

	return 0 ;
}

//-------------------------------------------------------------------------------
int ZCPrintManager::ClosePrint(FILE** pfp)
{
	FILE* fp;
	if (!pfp || !(*pfp)) fp = localFp;
	else fp = *pfp;

	if (fp) fclose(fp);
	if (!pfp || !(*pfp)) localFp = 0;
	else *pfp = 0;
	return 0;
}

//-------------------------------------------------------------------------------
