//-------------------------------------------------------------------------------
// ZCMemoryKeeper.cpp
// А. Николенко 03.08.2018
// Хранилка памяти - для сохранения сгостояний модели движения и последующего  
// их восстановления на различные моменты времени
//-------------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyMMath.h>

#include <FlyCoreSource\\ModType.h>
#include <FlyCoreSource\\ModGlobal.h>
#include <FlyCoreSource\\ZCMemoryKeeper.h>

//-------------------------------------------------------------------------------
ZCMemoryKeeper::ZCMemoryKeeper()
{
	m_SizeA = 0 ;
	m_A     = nullptr ;
}

//-------------------------------------------------------------------------------
ZCMemoryKeeper::~ZCMemoryKeeper()
{
	m_SizeA = 0 ;
	m_A     = nullptr ;
	FreeStory(nullptr) ;
}

//-------------------------------------------------------------------------------
void ZCMemoryKeeper::Bind(double* Q, int N)
{
	m_SizeA = N ;
	m_A     = Q ;
}

//-------------------------------------------------------------------------------
int ZCMemoryKeeper::FaindFreeStoryKey()
{
	int i, n = (int)m_sa.size() ;
	for(i=0 ; i<n ; i++) { if (!m_sa[i]) break ; }
	if (i<n) return(i+1) ;
	return 0 ;
}

//-------------------------------------------------------------------------------
int ZCMemoryKeeper::Story(int key, void** Params, int* SizeParams, int CountParams)
{
	if (!key || key>(int)m_sa.size()) {
		// Поиск индекса свободного буфера ;
		key = FaindFreeStoryKey( ) ;
		if (!key) {
			// Если такого буфера не найдено то добавляем новый, 
			// но он естественно будет нулевой длинны - память под данные ещё не выделена
			m_sa.push_back(0) ; key = (int)m_sa.size() ; 
	}	}

	int i ;
	// Подсчёт необходимого размера буфера
	int Count = (!Params || !SizeParams || !CountParams) ? 0:CountParams ;
	int Size = m_SizeA*sizeof(double)+sizeof(int)*(Count+1) ;
	for(i=0 ; i<Count ; i++) Size+= SizeParams[i] ;

	// Проверка размера буфера
	if (m_sa[key-1]) {
		int saSize ;
		memcpy(&saSize, m_sa[key-1], sizeof(int)) ;
		if ( saSize<Size ) {
			delete[] m_sa[key-1] ;
			m_sa[key-1] = nullptr ;
	}	}

	// Выделение памяти под буфер
	if (!m_sa[key-1]) {
		m_sa[key-1] = new char[Size+sizeof(int)] ;
		memcpy(m_sa[key-1], &Size, sizeof(int)) ;
	} 

	// Копирование в буфер вектора состояния в СК интегрирования
	int L = sizeof(int) ;
	int l = m_SizeA*sizeof(double) ;
	memcpy(&m_sa[key-1][L], (void*)m_A, l) ;
	L+= l ;  
	// Запись в буфер колличества массивов дополнительной информации 
	l = sizeof(int) ;
	memcpy(&m_sa[key-1][L], (void*)&Count, l) ;
	L+= l ;

	if (Count<=0) return 0 ;

	// Запись в буфер длинны каждого массива дополнительной информации 
	for(i=0 ; i<Count ; i++) {
		l = sizeof(int) ;
		memcpy(&m_sa[key-1][L], (void*)&SizeParams[i], l) ;
		L+= l ;
	}
	// Запись в буфер массивов дополнительной информации 
	for(i=0 ; i<Count ; i++) {
		l = SizeParams[i] ;
		memcpy(&m_sa[key-1][L], (void*)Params[i], l) ;
		L+= l ;
	}
	return key ;
}

//-------------------------------------------------------------------------------
int ZCMemoryKeeper::ReStory(int key, void** Params, int* SizeParams, int CountParams)
{
	// Проверка наличия такого буфера
	if (key>(int)m_sa.size() || !m_sa[key-1]) return 1 ;

	// Восстановление из буфера интегрируемого вектора состояния
	int L = sizeof(int) ;
	int l = m_SizeA*sizeof(double) ;
	memcpy(m_A, &m_sa[key-1][L], l) ;
	L+= l ;

	// Считывание из буфера колличества массивов дополнительной информации 
	int i ;
	int Count ; 
	l = sizeof(int) ;
	memcpy((void*)&Count, &m_sa[key-1][L], l) ;
	L+= l ;

	if (Count!=CountParams) return-10 ;  
	if (Count<=0) return 0 ; 

	// Считывание из буфера длинны каждого массива дополнительной информации 
	for(i=0 ; i<Count ; i++) {
		int s ;
		l = sizeof(int) ;
		memcpy((void*)&s, &m_sa[key-1][L], l) ;
		L+= l ;
		if (s!=SizeParams[i]) return -(11+i) ; 
		SizeParams[i] = s ;
	}
	// Считывание из буфера массивов дополнительной информации 
	for(i=0 ; i<Count ; i++) {
		l = SizeParams[i] ;
		memcpy((void*)Params[i], &m_sa[key-1][L], l) ;
		L+= l ;
	}
	return 0 ;
}

//-------------------------------------------------------------------------------
void ZCMemoryKeeper::FreeStory(int* key)
{
	if (!key) {
		for(int i=0 ; i<(int)m_sa.size() ; i++) { if (m_sa[i]) delete[] m_sa[i] ; m_sa[i] = 0 ; }
		m_sa.clear() ;
		return ;
	}
	if (*key>(int)m_sa.size() || !m_sa[*key-1]) { *key = 0 ; return ; }
	delete[] m_sa[*key-1] ; m_sa[*key-1] = 0 ; *key = 0 ;
}

//-------------------------------------------------------------------------------
