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
	CString queryurl = pvarURL->bstrVal;
	if ( queryurl.GetLength() == 0 )
		return;
	else
		CallShopplyAPI_ProcessResponse(queryurl);
}


void CHelloWorldBHO::CallShopplyAPI_ProcessResponse(CString url)
{		
	//Request Headers	
	CString strHeaders = _T("Content-Type: application/json\r\nAccept: application/json\r\nTenant-Id: 853e99f0affb11e1afa60800200c9a66\r\n\r\n");  
	int header_length = strHeaders.GetLength();	


	DWORD url_length = INTERNET_MAX_URL_LENGTH;
	TCHAR encodedUrl[INTERNET_MAX_URL_LENGTH];

	AtlEscapeUrl(url, encodedUrl, &url_length, INTERNET_MAX_URL_LENGTH, ATL_URL_ENCODE_PERCENT);

	//connect to the server
	HINTERNET hSession = InternetOpen(_T("HTTPPOST"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hSession) 
		return;
	HINTERNET hConnect = InternetConnect(hSession, _T("api.shop-o-saur.us"), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if ( !hConnect )
		return;
	
	PCTSTR accept[2]={_T("*/*"), NULL};
	CString ObjectName = _T("/website/deals/info?pageUrl=");
	ObjectName += encodedUrl;

	HINTERNET hRequest = HttpOpenRequest(hConnect, _T("GET"), ObjectName, NULL, NULL, accept, 0, 0);
	if ( !hRequest )
		return;
	bool send_ok = HttpSendRequest(hRequest, (LPCWSTR)strHeaders, header_length, NULL, 0);
	if ( !send_ok )
		return;

	DWORD BytesRead = 0;
	char* data = NULL;
	DWORD dataSize  = 0;
	char *szBuffer = NULL;
	CString output;
	//Process Response
	do 
	{ 
		// Read in a temporary buffer
		szBuffer = new char[1024];
		ZeroMemory(szBuffer, 1024);
		InternetReadFile(hRequest, (LPVOID)szBuffer, 1024, &BytesRead); 
		// If we have received anything add it to the global buffer and increase the byte-counter 
		char* tempData = new char[dataSize+BytesRead];
		tempData[dataSize+BytesRead] = '\0';
		ZeroMemory(tempData, dataSize+BytesRead);
		memcpy(tempData, data, dataSize);
		memcpy(tempData + dataSize, szBuffer, BytesRead);
//		delete[] data;
		data = tempData;
		dataSize += BytesRead;
	} while ( BytesRead != 0 );
	
	//json parser
	char* pch1 = strstr(data, "sosu-uid");
	char* pch2 = strstr(pch1+12, "\"");
	//pch1+12 ~ pch2-1
	char* sosu_uid = new char[pch2-pch1-12];	
    memcpy(sosu_uid, &data[pch1+12-data], pch2-pch1-12);
	sosu_uid[pch2-pch1-12] = '\0';

	char* pch3 = strstr(data, "response");
	char* pch4 = strstr(pch3+1, "merchantExists");
	if ( (pch4 == NULL) || (*(pch4+17) == 'f') )
		return;
	//merchantExists is true
	char* coupons, *deals, *promos;
	if ( *(pch4+17) == 't' )
	{
		char* pch5 = strstr(data, "\"coupons\":") ;
		char* pch6 = strstr(data, "\"deals\":");
		char* pch7 = strstr(data, "\"promos\":");

		if ( pch5 != NULL )
		{
			char* pch52 = strstr(pch5+11, "]");
			//pch5+11 ~ pch52
			coupons = new char[pch52-pch5-10];
			memcpy(coupons, &data[pch5+11-data], pch52-pch5-10);
			coupons[pch52-pch5-10] = '\0';

		}
		if ( pch6 != NULL )
		{
			char* pch62 = strstr(pch6+9, "]");
			//pch6+9 ~ pch62
			deals = new char[pch62-pch6-8];
			memcpy(deals, &data[pch6+9-data], pch62-pch6-8);
			deals[pch62-pch6-8] = '\0';
		}
		if ( pch7 != NULL )
		{
			char* pch72 = strstr(pch7+10, "]");
			//pch7+10 ~ pch72
			promos = new char[pch72-pch7-9];
			memcpy(promos, &data[pch7+10-data], pch72-pch7-9);
			promos[pch72-pch7-9] = '\0';
		}
	}
	InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);
}
