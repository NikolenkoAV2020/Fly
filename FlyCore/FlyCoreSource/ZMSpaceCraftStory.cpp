//-------------------------------------------------------------------------------
// ZMSpaceCraftStory.cpp
// А. Николенко 03.08.2018
//-------------------------------------------------------------------------------
#pragma once
#include <stdafx.h>
#include <FlyCoreSource\\ZMSpaceCraft.h>

//-------------------------------------------------------------------------------
int ZMSC::Story(int HStory)  
{
	void* Params[6]     = {&Vitok75, &FSC, &FSE, &FSP, &FSR, &FV};
	int   SizeParams[6] = {sizeof(long), sizeof(FSC), sizeof(FSE), sizeof(FSP), 
						   sizeof(FSR), sizeof(FV)};
	return(ZCMemoryKeeper::Story(HStory, Params, SizeParams, 6)) ;  
}

//-------------------------------------------------------------------------------
int ZMSC::ReStory(int HStory)  
{
	int    rc = 0 ;
	void* Params[6]     = {&Vitok75, &FSC, &FSE, &FSP, &FSR, &FV};
	int   SizeParams[6] = {sizeof(long), sizeof(FSC), sizeof(FSE), sizeof(FSP), 
						   sizeof(FSR), sizeof(FV)};
	rc = ZCIntegrator::ReStory(HStory, Params, SizeParams, 6) ; RRC ; 
	double* X = &((*this)[1]) ;
	double  t = (*this)[0] ;
	rc = OnMessage(MMSG_RESTORY, &t, X) ;
	return rc ;
}

//-------------------------------------------------------------------------------
void ZMSC::FreeStory(int* HStory)  
{
	ZCIntegrator::FreeStory(HStory) ;
	if (!HStory) HStoryClarify = 0 ;  
	else { if (*HStory==HStoryClarify) HStoryClarify = 0 ; }
}

//-------------------------------------------------------------------------------
void ZMSC::FastStory()  
{
	memcpy(FastX, m_a, NE()*sizeof(double)) ;
	memcpy(FastGI[0], GI[2*MAX_ADAMS_N], NE()*sizeof(double)) ;
	memcpy(FastGI[1], GI[2*MAX_ADAMS_N-1], NE()*sizeof(double)) ;
}

//-------------------------------------------------------------------------------
void ZMSC::FastReStory()  
{
	memcpy(m_a, FastX, NE()*sizeof(double)) ;
	memcpy(GI[2*MAX_ADAMS_N], FastGI[0], NE()*sizeof(double)) ;
	memcpy(GI[2*MAX_ADAMS_N-1], FastGI[1], NE()*sizeof(double)) ;
}

//-------------------------------------------------------------------------------