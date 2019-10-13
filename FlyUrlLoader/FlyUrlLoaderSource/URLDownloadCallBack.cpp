//-----------------------------------------------------------------------------
// URLDownloadCallBack.cpp
// А. Николенко 03.05.2019
// 
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <FlyUrlLoader.h>
#include <FlyUrlLoaderSource\\URLDownloadCallBack.h>

//-----------------------------------------------------------------------------
CUrlDownloadCallBack::CUrlDownloadCallBack(
	unsigned int ID, 
	int(*pUserCB)(unsigned int ID, float p))
{
	pUserCallBack = pUserCB;
	hWndInternal = nullptr;
	posWasBreak = -100.0f;
	bAbort = FALSE;
	cRef = 0;
	IDL = ID;
}

CUrlDownloadCallBack::~CUrlDownloadCallBack()
{
}

//-----------------------------------------------------------------------------
//Имплементация QueryInterface
STDMETHODIMP CUrlDownloadCallBack::QueryInterface(REFIID riid, void **ppv)
{
	HRESULT hr = E_NOINTERFACE;

	if (!ppv) return E_POINTER;

	*ppv = NULL;
	if (riid == IID_IUnknown || riid == IID_IBindStatusCallback)
	{
		*ppv = (IBindStatusCallback *)this;
		AddRef();
		hr = S_OK;
	}
	return hr;
}

//-----------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE CUrlDownloadCallBack::AddRef(void)
{
	return ++cRef;
}

//-----------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE CUrlDownloadCallBack::Release()
{
	--cRef;
	if (cRef > 0) return cRef;
	//delete this;
	return 0;
}

//-----------------------------------------------------------------------------
STDMETHODIMP CUrlDownloadCallBack::OnStartBinding(DWORD grfBSCOption, IBinding *pib)
{
	return E_NOTIMPL;
}

STDMETHODIMP CUrlDownloadCallBack::OnStopBinding(HRESULT hresult, LPCWSTR szError)
{
	return E_NOTIMPL;
}

STDMETHODIMP CUrlDownloadCallBack::OnObjectAvailable(REFIID riid, IUnknown *punk)
{
	return E_NOTIMPL;
}

STDMETHODIMP CUrlDownloadCallBack::GetPriority(LONG *pnPriority)
{
	return E_NOTIMPL;
}

STDMETHODIMP CUrlDownloadCallBack::OnLowResource(DWORD dwReserved)
{
	return E_NOTIMPL;
}

bool CUrlDownloadCallBack::isWasBreak()
{
	if (posWasBreak >= 0) return true;
	return false;
}

float CUrlDownloadCallBack::WhereWasBreak()
{
	return posWasBreak;
}

//-----------------------------------------------------------------------------
STDMETHODIMP CUrlDownloadCallBack::OnProgress(
	ULONG ulProgress,
	ULONG ulProgressMax,
	ULONG ulStatusCode,
	LPCWSTR szStatusText)
{
	char cMessage[128];	cMessage[0] = 0;

	switch (ulStatusCode) {
	case BINDSTATUS_CONNECTING:
		lstrcpyn(cMessage, "Соединение...", sizeof(cMessage));
		cout << cMessage << endl;
		break;
	case BINDSTATUS_SENDINGREQUEST:
	case BINDSTATUS_REDIRECTING:
		break;
	case BINDSTATUS_BEGINDOWNLOADDATA:
		lstrcpyn(cMessage, "Закачка...", sizeof(cMessage));
		cout << cMessage << endl;
		//HWND hWndPr;
		//hWndPr = GetDlgItem(hWndInternal, IDC_PROGRESS1);
		//ShowWindow(hWndPr, SW_SHOW);
		break;
	case BINDSTATUS_ENDDOWNLOADDATA:
		cout << endl << endl;
		break;
	case BINDSTATUS_DECODING:
	case BINDSTATUS_DOWNLOADINGDATA:
		break;
	case BINDSTATUS_USINGCACHEDCOPY:
		lstrcpyn(cMessage, "Закачка из кэша...", sizeof(cMessage));
		cout << cMessage << endl;
		break;
	default:
		break;
	}

	if (ulStatusCode == BINDSTATUS_BEGINDOWNLOADDATA ||
		ulStatusCode == BINDSTATUS_DOWNLOADINGDATA) {
		//конвертируем прогресс в проценты
		float nNewPos = (ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA) ? 100.0f
						: (ulProgressMax ? 
						((float)ulProgress * 100.0f / (float)ulProgressMax) 
						: 0.0f);
		if (pUserCallBack) {
			if (pUserCallBack(IDL, nNewPos)<0) bAbort = true;
		} else {
			cout << "\r     " << nNewPos << "%    ";
		}

		if (hWndInternal) {
			//SendDlgItemMessage(hWndInternal, IDC_PROGRESS1, PBM_SETPOS, (WPARAM)nNewPos, 0);
			//SetDlgItemText(hWndInternal, IDC_STATIC2, cMessage);
		}
	}

	//Прерывание закачки
	if (bAbort) {
		bAbort = false;
		return E_ABORT;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
STDMETHODIMP CUrlDownloadCallBack::GetBindInfo(
	DWORD    *pgrfBINDF, 
	BINDINFO *pbindInfo)
{
	if (!pgrfBINDF || !pbindInfo || !pgrfBINDF)
		return E_POINTER;
	if (pbindInfo->cbSize<sizeof(BINDINFO))
		return E_INVALIDARG;
	if (pbindInfo->dwBindVerb != BINDVERB_POST && 
		pbindInfo->dwBindVerb != BINDVERB_GET)
		return E_UNEXPECTED;
	return E_NOTIMPL;
}

//-----------------------------------------------------------------------------
STDMETHODIMP CUrlDownloadCallBack::OnDataAvailable(
	DWORD     grfBSCF, 
	DWORD     dwSize,
	FORMATETC *pformatetc,
	STGMEDIUM *pstgmed)
{
	return E_NOTIMPL;
}

//-----------------------------------------------------------------------------
bool CUrlDownloadCallBack::SetWindow(HWND hWnd)
{
	if (!hWnd) {
		hWndInternal = NULL;
		return FALSE;
	}
	hWndInternal = hWnd;
	return TRUE;
}

//-----------------------------------------------------------------------------
bool CUrlDownloadCallBack::GetWindow(HWND *phWnd)
{
	if (!phWnd) return FALSE;
	*phWnd = hWndInternal;
	return TRUE;
}

//-----------------------------------------------------------------------------
bool CUrlDownloadCallBack::Abort(void)
{
	bAbort = TRUE;
	return TRUE;
}

//-----------------------------------------------------------------------------
