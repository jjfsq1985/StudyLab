// dllmain.h : 模块类的声明。

class CsocxModule : public ATL::CAtlDllModuleT< CsocxModule >
{
public :
	DECLARE_LIBID(LIBID_socxLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SOCX, "{7B9B9906-9AA0-408C-BE8F-7438383C8EB3}")
};

extern class CsocxModule _AtlModule;
