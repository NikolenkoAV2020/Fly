//------------------------------------------------------------------------
// ModPUManager.cpp
// А. Николенко 13.08.2018
//------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyMMath.h>	// Математические функции
#include <FlyTime.h>	// Всё про время

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ModSK.h>
#include <FlyCoreSource\\ModPUManager.h>

//-------------------------------------------------------------------------------
// Менеджер программ управления
//-------------------------------------------------------------------------------
ZPUManager::ZPUManager( )
{
	Clear() ;
}

//-------------------------------------------------------------------------------
ZPUManager::~ZPUManager( ) 
{
	Clear() ;
}

//-------------------------------------------------------------------------------
void ZPUManager::Clear( ) 
{
	for (int i=0 ; i<MAX_PU_COUNT_MOD ; i++) PU[i].Clear() ;
	Count = 0 ;
	ClearIndex() ;
}

//-------------------------------------------------------------------------------
void ZPUManager::Init()
{
	ClearIndex() ;
}

//-------------------------------------------------------------------------------
void ZPUManager::ClearIndex( ) 
{
	memset(IndexVN,0, MAX_PU_COUNT_MOD*sizeof(int)) ;
	memset(IndexVP,0, MAX_PU_COUNT_MOD*sizeof(int)) ;
	memset(IndexV, 0, MAX_PU_COUNT_MOD*sizeof(int)) ;
	memset(IndexU, 0, MAX_PU_COUNT_MOD*sizeof(int)) ;
	TIndex = 0 ;
	NextTimePoint = 0 ;
	PrevTimePoint = 0 ;
	TimePointKey  = 0 ;

	PrevIDMSG = 0 ;
	NextIDMSG = 0 ;
	CurIDMSG  = 0 ;
}

//-------------------------------------------------------------------------------
void ZPUManager::Indexing(double t, int dir) 
{
	// Необходимо найти индексы
	//   1. включения и участка соответствующих времени t				IndexV[i] и IndexU[i] 
	//      если 0, то значит в данный момент времени t включения программы управления i нет
	//   2. ближайшего справа включения									IndexVN[i]
	//      если 0, то включение не найдено, если -1, то включения нет
	//   3. ближайшего слева включения									IndexVP[i]
    //      если 0, то включение не найдено, если -1, то включения нет
	//
	// Если для данного момента времени 
	// необходимо учитывать работу ДУ, то в результате работы поцедуры 
	//		PU[ i ].IsOn = true 
	//		IndexV[i] > 0 
	//		IndexU[i] > 0 
	TimePointKey = 0 ;
	PrevIDMSG    = 0 ;
	NextIDMSG    = 0 ;
	CurIDMSG     = 0 ;
	// Перебор программ управления
	for (int i=0 ; i<MAX_PU_COUNT_MOD ; i++) {
		PU[ i ].IsOn = false ; 
		if ( PU[ i ].isEmpty( ) ) {
			IndexVN[i] = 0 ;
			IndexVP[i] = 0 ;
			IndexV[i]  = 0 ;
			IndexU[i]  = 0 ;
			break ;
		}

		double eps = 1e-13 ;
		double tn, tk ;
		int j0 = 0 ;				// Начальный индекс для перебора вкючений
		int jm = PU[i].Size() ;		// Конечный индекс для перебора вкючений
		if (TIndex) {
			if (t >= TIndex) j0 = (IndexV[i] ? (IndexV[i]-1):(IndexVN[i]>0 ? (IndexVN[i]-1):0)) ;
			else j0 = (IndexV[i] ? (IndexV[i]-1):(IndexVP[i]>0 ? (IndexVP[i]-1):0)) ;
		}
		j0 = (j0>0?(j0-1):0) ;

		IndexVP[i] = 0 ;	// Индекс включения предыдущего для ПУ i	
		IndexVN[i] = 0 ;	// Индекс включения следующего (или текущего) для ПУ i
		// Перебор включений
		for (int j=j0 ; j<jm ; j++) {
			tn = PU[i].V[j].TS() ;	// Время начала включения
			tk = PU[i].V[j].TE() ;	// Время конца включения 
			if (t>tk) {
				IndexVP[i] = j+1 ;
				PrevTimePoint = (!i || PrevTimePoint<tk) ? tk:PrevTimePoint ;
				TimePointKey|= 1 ; 
				PrevIDMSG = MMSG_TDU_OFF ;
			}
			if (!IndexVN[i] && t<tn) {
				IndexVN[i] = j+1 ;
				NextTimePoint = (!i || NextTimePoint>tn) ? tn:NextTimePoint ;
				TimePointKey|= 2 ; 
				NextIDMSG = MMSG_TDU_ON ;
				break ;
			}
			if (fabs(t-tn)<eps || fabs(t-tk)<eps || (t-tn)*(t-tk)<0) {
				// Найдено включение которое содержит момент времени t
				PU[ i ].IsOn = true ; 
				IndexV[i] = j+1 ;
				IndexU[i] = 0 ;
				int km = PU[i].V[j].Size() ; 
				// Перебор участков
				for (int k=0 ; k<km ; k++) {
					tn = PU[i].V[j].U[k].TS() ;
					tk = PU[i].V[j].U[k].TE() ;
					if (fabs(t-tn)<eps || fabs(t-tk)<eps ||	(t-tn)*(t-tk)<0) {
						// Найден участок который содержит момент времени t
						if (dir>0) {
							if (fabs(t-tn)<eps) {
								if (k==0) {		
									// Начало включения								
									CurIDMSG = MMSG_TDU_ON ;
									PU[ i ].IsOn = true ; 
								} else {			
									// Начало очередного участка
									CurIDMSG = MMSG_TDU_STEPON ;
									PU[ i ].IsOn = true ; 
							}	}
							if (fabs(t-tk)<eps) {
								if (k==km-1) {	
									// Конец включения
									CurIDMSG = MMSG_TDU_OFF ;
									// Так-как достигнут конец последнего участка данного включения данной ПУ
									// то указываем, что ПУ не активна
									PU[ i ].IsOn = false ; 
								} else {			
									// Конец очередного участка
									CurIDMSG = MMSG_TDU_STEPOFF ;
									PU[ i ].IsOn = true ;
									// Необходимо проверить следующий участок
									continue ;
						}	}	}
						if (dir<0) {
							if (fabs(t-tn)<eps) {
								if (k==0) { 
									// Начало включения
									CurIDMSG = MMSG_TDU_ON ;
									// Так-как прогноз в обратном направлении (dir<0),
									// то указываем, что ПУ не активна
									PU[ i ].IsOn = false ; 
								} else {			
									// Начало очередного участка
									CurIDMSG = MMSG_TDU_STEPON ;
									PU[ i ].IsOn = true ; 
							}	}
							if (fabs(t-tk)<eps) {
								if (k==km-1) {	
									// Конец включения
									CurIDMSG = MMSG_TDU_OFF ;
									PU[ i ].IsOn = true ; 
								} else {			
									// Конец очередного участка
									CurIDMSG = MMSG_TDU_STEPOFF ;
									PU[ i ].IsOn = true ; 
						}	}	}
						if (PU[ i ].IsOn) { 
							IndexU[i] = k+1 ;
							PrevTimePoint = (!i || PrevTimePoint<tn) ? tn:PrevTimePoint ;
							NextTimePoint = (!i || NextTimePoint>tk) ? tk:NextTimePoint ;
							TimePointKey |= 4 ; 
						} 
						break ;
					} // Конец проверки участка	
				} // Конец перебора участков
				if (IndexU[i]==0) {
					IndexV[i] = 0 ;
					PU[ i ].IsOn = false ; 
				}
				break ;
			} // Конец проверки включения	
		} // Конец перебора включений
		if (!IndexVN[i]) IndexVN[i] =-1 ;
		if (!IndexVP[i]) IndexVP[i] =-1 ;
		TIndex = t ;
	} // Конец перебора программ управления
}

//-------------------------------------------------------------------------------
int ZPUManager::TimeCheckForStep(double t, int dir, long* IDDUMSG, double* maxstep)
{
	if (IDDUMSG) *IDDUMSG = 0 ; 

	// Поск ближайших временных меток по программе управления
	// Поиск ближайших слева и спарва точек разрыва
	// Определение текущей точки как разрыв
	Indexing(t, dir) ;

	if(CurIDMSG) {
		// Текущая точка определена как точка разрыва ПЧ СДУ
		// Возвращаем идентификатор найденного разрыва
		if (IDDUMSG) *IDDUMSG = CurIDMSG ; 
	}

	// Если найденв точка разрыва справа и направление прогноза вперёд 
	// или точка разрыва слева и прогноз назад, то ограничиваем максимальный
	// шаг интегрирования таким образом, чтобы не проскочит соответствующую
	// временную метку
	if ((TimePointKey&2 || TimePointKey&4) && dir>0) {
		if (maxstep) {
			if (t+*maxstep>NextTimePoint) *maxstep = NextTimePoint-t ;
	}	}	
	if ((TimePointKey&1 || TimePointKey&4) && dir<0) {
		if (maxstep) {
			if ((t-*maxstep)<PrevTimePoint) *maxstep = t-PrevTimePoint ;
	}	}	

	return 0 ;
}

//-------------------------------------------------------------------------------
void ZPUManager::GetTimePoint( int& Key, double& PrevT, double& NextT, long& P_IDMSG, long& N_IDMSG ) 
{
	NextT = NextTimePoint ;
	PrevT = PrevTimePoint ;
	Key   = TimePointKey ;
	P_IDMSG = PrevIDMSG ;
	N_IDMSG = NextIDMSG ;
}

//-------------------------------------------------------------------------------
bool ZPUManager::isEmpty ()  { return (Count?false:true) ; }
void ZPUManager::operator << (ZPU &I) { Add(I) ; }
int ZPUManager::Size() { return Count ; }

//-------------------------------------------------------------------------------
int ZPUManager::Add(ZPU &I) 
{
	int i ;
	for (i=0 ; i<MAX_PU_COUNT_MOD; i++) {
		if (PU[i].isEmpty()) {
			PU[i]=I ;
			ClearIndex() ;
			Count++ ;
			break ;
	}	}
	if (i == MAX_PU_COUNT_MOD) {
		AddErrorMSG( "Программа управления не установлена, колличество програии управления больше максимально допустимого" ) ; 
		return 1 ;
	}
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZPUManager::Set(ZPU &I, int ind) 
{
	if (ind<0 || ind>=MAX_PU_COUNT_MOD) {
		AddErrorMSG( "Программа управления не установлена, неверный индекс" ) ; 
		return 1 ;
	}
	if (PU[ind].isEmpty()) Count++ ;
	else PU[ind].Clear() ;			
	PU[ind] = I ;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZPUManager::Del(int ind) 
{
	if (ind<0) {
		for (int i=0 ; i<MAX_PU_COUNT_MOD ; i++) PU[i].Clear() ;
		return 0 ;
	}
	if (ind>=MAX_PU_COUNT_MOD) {
		AddErrorMSG( "Программа управления не удалена из оперативного массива, неверный индекс" ) ; 
		return 1 ;
	}
	PU[ind].Clear() ;
	Count-- ;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZPUManager::CalcRLN(double t, double m, double* arln, double* pMs) 
{ 
	memset(arln, 0, 3*sizeof(double)) ;
	if (pMs) *pMs=0 ;
	// Поиск актуального по времени участка каждой из программ управления
	//Indexing(t) ;
	// Рассчёт ускорений для каждой актуальной в данный момент времени ПУ
	double a[3] ;
	int    i, j ;
	// Перебор всех загруженных программ управления
	for (i=0 ; i<MAX_PU_COUNT_MOD ; i++) {
		if (PU[i].isEmpty() || !PU[i].IsOn) {
			if (i) break ;
			else return-1 ;
		}
		// Номер включения (отсчитывается от 1)
		int NumV = IndexV[i] ;
		if (NumV>0) {
			// Номер участка (отсчитывается от 1)
			int NumU = IndexU[i] ;
			if (NumU>0) {
				// Если участок есть то расчитываем укорения
				// Функция CalcARLN класса участка программы управления
				// Вычисляет ускорения в м/сек2 !!!!!
				double ms ;
				PU[i].V[NumV-1 ].U[NumU-1].CalcARLN(t, m, a, &ms) ;
				// Суммирование ускорений по всем актуальным 
				// на данном шаге программам управления
				for (j=0 ; j<3 ; j++) arln[j]+= a[j] ;
				// Суммирование секундного расхода топлива по всем актуальным 
				// на данном шаге программам управления
				if (pMs) *pMs+= ms ;
	}	}	}
	// Перевод ускорений из м/сек2 в дткм/ссс2
	for (j=0 ; j<3 ; j++) arln[j]*= 8.64*86.4 ;
	return 0 ; 
} 

//-------------------------------------------------------------------------------