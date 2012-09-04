// dllmain.h : Declaration of module class.

class CHelloWorldModule : public ATL::CAtlDllModuleT< CHelloWorldModule >
{
public :
	DECLARE_LIBID(LIBID_HelloWorldLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_HELLOWORLD, "{3586A635-3F11-4291-9D92-FEFE1EED58A9}")
};

extern class CHelloWorldModule _AtlModule;
