//-------------------------------------------------------------------------------
// ZMessageManager.cpp : 
// А. Николенко 17.10.2018
//-------------------------------------------------------------------------------
#include <stdafx.h>
#include <FlyCoreSource\\ZMessageManager.h>

ZMessageManager MsgManager ;

//-------------------------------------------------------------------------------
ZMessageManager::ZMessageManager()
{
	indexMsg =-1 ; 
}

//-------------------------------------------------------------------------------

ZMessageManager::~ZMessageManager()
{
	Clear() ;
}

//-------------------------------------------------------------------------------
void  ZMessageManager::Clear() 			
{
	indexMsg =-1 ;
	MessageList.clear() ;
}

//-------------------------------------------------------------------------------
int ZMessageManager::Size() 
{
	int N = MessageList.size() ; return N ;
}

//-------------------------------------------------------------------------------
void  ZMessageManager::Add(char* Msg) 
{
	MessageList.push_back(Msg) ;
}

//-------------------------------------------------------------------------------
char* ZMessageManager::First()
{
	int N = MessageList.size() ; if (N<1) return nullptr ;
	indexMsg = 0 ;
	return (char*)MessageList[indexMsg].c_str() ;
}

//-------------------------------------------------------------------------------
char* ZMessageManager::Next() 
{
	int N = MessageList.size() ; if (N<1 || indexMsg >= N-1) return nullptr ;
	indexMsg++ ;
	return (char*)MessageList[indexMsg].c_str() ;
}

//-------------------------------------------------------------------------------
char* ZMessageManager::Prev() 
{
	int N = MessageList.size() ; if (N<1 || indexMsg <= 0) return nullptr ;
	indexMsg-- ;
	return (char*)MessageList[indexMsg].c_str() ;
}

//-------------------------------------------------------------------------------
char* ZMessageManager::Last() 
{
	int N = MessageList.size() ; if (N<1) return nullptr ;
	indexMsg = N-1 ;
	return (char*)MessageList[indexMsg].c_str() ;
}

//-------------------------------------------------------------------------------
ZMessageManager& operator << (ZMessageManager& MM, char* Msg) 
{
	MM.Add(Msg) ;
	return MM ;
}
void operator >> (char* Msg, ZMessageManager& MM) 
{
	MM.Add(Msg) ;
}
void operator >> (ZMessageManager& MM, FILE* pf) 
{
	int N = MM.Size() ; if (!N || !pf) return ; 
	fprintf(pf, "\n") ;
	char* S = MM.First() ; 
	for (int i=0 ; i<N ; i++) {
		if (S) fprintf(pf, "%s\n", S) ;
		S = MM.Next() ; 
	}
}
void operator << (FILE* pf, ZMessageManager& MM) 
{
	int N = MM.Size() ; if (!N || !pf) return ; 
	fprintf(pf, "\n") ;
	char* S = MM.Last() ; 
	for (int i=0 ; i<N ; i++) {
		if (S) fprintf(pf, "%s\n", S) ;
		S = MM.Prev() ; 
	}
}

//-------------------------------------------------------------------------------




