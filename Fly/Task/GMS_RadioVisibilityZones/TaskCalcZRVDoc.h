//-------------------------------------------------------------------------------
// TaskCalcZRVDoc.h
// �. ��������� 11.12.2018
//
// ������� ���������������� ����������� ������ ��� � ����
//------------------------------------------------------------------------------- 
#ifndef TASK_CALC_ZRV_DOC_H
#define TASK_CALC_ZRV_DOC_H

class ZModZRV ;

//-------------------------------------------------------------------------------
// ���������������� - ��������� ��������� ������� ���
void DocHeaderZRV(FILE* fp, ZModZRV* KA=NULL, int CountKA=0, int* NipIndexes=NULL, int CountNip=0) ;
// ���������������� ��������� ������� ���
void DocTablHeaderZRV(FILE* fp) ;
// ���������� ���������������� ���
void DocEndZRV(FILE* fp) ;
// ���������������� ����� ���
void DocZRV(ZZRV& zrv, FILE* fp) ;
void DocZRV(ZZRV& zrv, char* S, char* Sscr) ;
// ������������ ������� ��� ��� ������������ ����������������
void AccumulationZRVData(ZZRV& zrv) ;
// ���������� � ���������������� ������������ ������� ���
void ResetZRVData(FILE* fp, ZModZRV* KA=NULL, int CountKA=0) ;
// �������� �������� ��� ���������������� ��� � ��
void MakeZRVDir(char* LDir, char* SDir) ;

//------------------------------------------------------------------------------- 
#endif // #ifndef TASK_CALC_ZRV_DOC_H