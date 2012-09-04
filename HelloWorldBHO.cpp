// HelloWorldBHO.cpp : Implementation of CHelloWorldBHO

#include "stdafx.h"
#include "HelloWorldBHO.h"
#include <Wininet.h>
#include <atlutil.h>

// CHelloWorldBHO
//To connect the event handler to the browser through the event map
STDMETHODIMP CHelloWorldBHO::SetSite(IUnknown* pUnkSite)
{
    if (pUnkSite != NULL)
    {
        // Cache the pointer to IWebBrowser2.
        HRESULT hr = pUnkSite->QueryInterface(IID_IWebBrowser2, (void **)&m_spWebBrowser);
        if (SUCCEEDED(hr))
        {
            // Register to sink events from DWebBrowserEvents2.
            hr = DispEventAdvise(m_spWebBrowser);
            if (SUCCEEDED(hr))
            {
                m_fAdvised = TRUE;
            }
        }
    }
    else
    {
        // Unregister event sink.
        if (m_fAdvised)
        {
            DispEventUnadvise(m_spWebBrowser);
            m_fAdvised = FALSE;
        }

        // Release cached pointers and other resources here.
        m_spWebBrowser.Release();
    }

    // Call base class implementation.
    return IObjectWithSiteImpl<CHelloWorldBHO>::SetSite(pUnkSite);
}


void STDMETHODCALLTYPE CHelloWorldBHO::OnNavigateComplete2(IDispatch *pDisp, VARIANT *pvarURL)
{
	BSTR queryurl = pvarURL->bstrVal;
	if ( queryurl == _T(" ") )
		return;
	else
		CallShopplyAPI_ProcessResponse(queryurl);
}


void CHelloWorldBHO::CallShopplyAPI_ProcessResponse(BSTR url)
{
	//connect to the server
	HINTERNET Initialize, Connection, HttpRequest;
    DWORD dwBytes;

    char ch;
    Initialize = InternetOpen(_T("HTTPPOST"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_ASYNC);

    Connection = InternetConnect(Initialize, _T("http://api.shop-o-saur.us"), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

    HttpRequest = HttpOpenRequest(Connection, _T("POST"), _T("/website/deals/info?"), _T("HTTP/1.1"), 0, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_EXISTING_CONNECT, 0);

	//Request Headers, Body
	//json writer: convert data to json format
	//Serialize a string.
	DWORD length = INTERNET_MAX_URL_LENGTH;
	WCHAR pageUrl[INTERNET_MAX_URL_LENGTH];
	AtlEscapeUrl(url, pageUrl, &length, INTERNET_MAX_URL_LENGTH, ATL_URL_ENCODE_PERCENT);
	BSTR Tenant_Id = L"853e99f0affb11e1afa60800200c9a66";

	//Send Request


	//Process Response
    InternetCloseHandle(HttpRequest);
    InternetCloseHandle(Connection);
    InternetCloseHandle(Initialize);
}

/*
int encode_request_POST(char* url)
{
	int SIZE = 0;
    memcpy(&bufw[SIZE], "GET ", 4);
    SIZE += 4;
    int len1 = strlen(url);
    memcpy(&bufw[SIZE], url, len1);
    SIZE += len1;
    memcpy(&bufw[SIZE], " ", 1);
    SIZE += 1;

    char str[] = "HTTP/1.0\r\n";
    int len2 = strlen(str);
    memcpy(&bufw[SIZE], str, len2);
    SIZE += len2;

    char CRLF[] = "\r\n";
    int len3 = strlen(CRLF);
    memcpy(&bufw[SIZE], CRLF, len3);
    SIZE += len3;
    
    printf("%s\n", url);
    printf("%s", &bufw);
    return SIZE;
}
*/