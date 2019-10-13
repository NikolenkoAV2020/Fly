//-------------------------------------------------------------------------------
// ZIndex.cpp : 
//-------------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyMMath.h>

#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZIndex.h>

//-------------------------------------------------------------------------------
// Механизм индексирования данных
// Реализован подход к индексированию данных, называемый в литературе как
// "Кластерное индексирование" 
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
void IndexCluster::Clear() {
	D1 = D2 = 0 ;
	iFirst = iLast =-1 ;
	if (NextI) { delete((ZIndexes*)NextI) ; NextI = nullptr ; }
}

//-------------------------------------------------------------------------------
ZIndexes::ZIndexes(int ClusterSize)
{
	Clear() ;

	if (ClusterSize<0) IndexesClusterSize = IndexesClusterMaxSize ;
	else IndexesClusterSize = ClusterSize ;
}

//-------------------------------------------------------------------------------
ZIndexes::~ZIndexes()
{
	//if (NextI) { delete NextI ; NextI = nullptr ; }
	Clear() ;
}

//-------------------------------------------------------------------------------
void ZIndexes::Clear()
{
	I.clear() ;
	II.Clear() ;
	j =-1 ;
	ValueMax = ValueMin = 0 ;
}

//-------------------------------------------------------------------------------
void ZIndexes::AddValue(double d)
{
	// Индексирование
	j++ ;
	if (II.iFirst==-1) {
		if (IndexesClusterSize>IndexesClusterMinSize) {
			II.NextI = (ZIndexes*)new ZIndexes(IndexesClusterSize/10) ;
			if (!II.NextI) assert(0) ;
		}
		II.iFirst = j ;
		II.D1 = II.D2 = d ;
		if (!j) { ValueMax = ValueMin = d ; }
	} else {
		II.D1 = d<II.D1 ? d:II.D1 ;
		II.D2 = d>II.D2 ? d:II.D2 ;
		if (II.D1<ValueMin) ValueMin = II.D1 ;
		if (II.D2>ValueMax) ValueMax = II.D2 ;
	}

	II.iLast = j ;
	if (II.NextI) ((ZIndexes*)II.NextI)->AddValue(d) ;

	if (II.iLast-II.iFirst+1 >= IndexesClusterSize) {
		I.push_back(II) ;
		II.Clear() ;
	}	
}

//-------------------------------------------------------------------------------
int ZIndexes::FindFor(double d)
{
	// Проверка... попадаем ли в диапазон значений,
	if (d < ValueMin) return-1 ;
	if (d > ValueMax) return-2 ;
	//
	// Примерный индекс центрального участка
	int i = (int)((double)j*100.0*(d-ValueMin)/(ValueMax-ValueMin)) ;

	while (1) {
		if (I[i].D1>d) {
			// Смещение индекса участа влево (уменьшание)
			i--; 
			if (i<0) {
				// неправильно был построен индекс
				assert(0) ;
			}
		} else {
			if (I[i].D2<d) {
				// Смещение индекса участа влево (уменьшание)
				i++; 
				if (i>=j) {
					// неправильно был построен индекс нл всё рано
					// Возвращаем индекс самой правой (последней) точки
					return j ;
				}
			} else {
				if (I[i].NextI) {
					return(((ZIndexes*)I[i].NextI)->FindFor(d)) ;
				}
				return i ;
	}	}	}
}
//-------------------------------------------------------------------------------