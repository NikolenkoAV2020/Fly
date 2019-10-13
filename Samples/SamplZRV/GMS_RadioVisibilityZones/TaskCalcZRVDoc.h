//-------------------------------------------------------------------------------
// TaskCalcZRVDoc.h
// А. Николенко 11.12.2018
//
// Функции документирования расчитанных данных ЗРВ в файл
//------------------------------------------------------------------------------- 
#ifndef TASK_CALC_ZRV_DOC_H
#define TASK_CALC_ZRV_DOC_H

class ZModZRV ;

//-------------------------------------------------------------------------------
// Документирование - заголовок документа расчёта ЗРВ
void DocHeaderZRV(FILE* fp, ZModZRV* KA=NULL, int CountKA=0, int* NipIndexes=NULL, int CountNip=0) ;
// Документирование заголовка таблицы ЗРВ
void DocTablHeaderZRV(FILE* fp) ;
// Завершение документирование ЗРВ
void DocEndZRV(FILE* fp) ;
// Документирование одной ЗРВ
void DocZRV(ZZRV& zrv, FILE* fp) ;
void DocZRV(ZZRV& zrv, char* S, char* Sscr) ;
// Накапливание массива ЗРВ для последующего документирование
void AccumulationZRVData(ZZRV& zrv) ;
// Сортировка и документирование накопленного массива ЗРВ
void ResetZRVData(FILE* fp, ZModZRV* KA=NULL, int CountKA=0) ;
// Создание каталога для документирования ЗРВ и ЦУ
void MakeZRVDir(char* LDir, char* SDir) ;

//------------------------------------------------------------------------------- 
#endif // #ifndef TASK_CALC_ZRV_DOC_H