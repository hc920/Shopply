// HelloWorldBHO.h : Declaration of the CHelloWorldBHO

#pragma once
#include "resource.h"       // main symbols



#include "HelloWorld_i.h"

#include <ShlGuid.h>
#include <ExDispid.h>
#include <MsHTML.h>

#include <Wininet.h>
#include <atlutil.h>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CHelloWorldBHO

class ATL_NO_VTABLE CHelloWorldBHO :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHelloWorldBHO, &CLSID_HelloWorldBHO>,
	public IObjectWithSiteImpl<CHelloWorldBHO>,
	public IDispatchImpl<IHelloWorldBHO, &IID_IHelloWorldBHO, &LIBID_HelloWorldLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,

	public IDispEventImpl<1, CHelloWorldBHO, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>

{
public:
	CHelloWorldBHO()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HELLOWORLDBHO)

DECLARE_NOT_AGGREGATABLE(CHelloWorldBHO)

BEGIN_COM_MAP(CHelloWorldBHO)
	COM_INTERFACE_ENTRY(IHelloWorldBHO)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(SetSite)(IUnknown *pUnkSite);
//	BEGIN_SINK_MAP(CHelloWorldBHO)
//		SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
//	END_SINK_MAP()

	BEGIN_SINK_MAP(CHelloWorldBHO)
		SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2)
	END_SINK_MAP()
	// DWebBrowserEvents2
//    void STDMETHODCALLTYPE OnDocumentComplete(IDispatch *pDisp, VARIANT *pvarURL); 
	void STDMETHODCALLTYPE OnNavigateComplete2(IDispatch* pDisp, VARIANT *pvarURL);
	void CallShopplyAPI_ProcessResponse(CString url);
private:
	CComPtr<IWebBrowser2>  m_spWebBrowser;
	BOOL m_fAdvised;
	

};

OBJECT_ENTRY_AUTO(__uuidof(HelloWorldBHO), CHelloWorldBHO)
