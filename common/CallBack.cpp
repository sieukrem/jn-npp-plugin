#include "CallBack.h"
#include "MyActiveSite.h"

CallBack::CallBack(unsigned int stacksize, IDispatchEx* cfg, unsigned int num):CComDispatch(){
	m_StackSize = stacksize;
	m_Cfg = cfg;
	cfg->AddRef();
	m_Num = num;
	*CallBack::GetCallBack(num) = this;
}

size_t CallBack::Call(void* params){
	VARIANT result;

	VARIANT var[1];
	var[0].vt = VT_BSTR;
	var[0].bstrVal = SysAllocStringByteLen(((LPCSTR)params),m_StackSize); 

	MyActiveSite::callMethod(TEXT("cmd"), m_Cfg, var, 1, &result);

	SysFreeString(var[0].bstrVal);

	VARTYPE type = VT_UI4;
	HRESULT hr = VariantChangeType(&result, &result, 0, type);
	
	DWORD dwResult = 0;
	dwResult = result.uintVal;

	return dwResult;
}

CallBack::~CallBack(){
	m_Cfg->Release();
	*CallBack::GetCallBack(m_Num) = NULL;
}

CallBack::CBP* CallBack::Proc(){
	// say compiler it should create 12 function stubs
	static CallBack::CBP* cbps[]={
		(CallBack::CBP*)&CallBack::CallBackProc<0>,
		(CallBack::CBP*)&CallBack::CallBackProc<1>,
		(CallBack::CBP*)&CallBack::CallBackProc<2>,
		(CallBack::CBP*)&CallBack::CallBackProc<3>,
		(CallBack::CBP*)&CallBack::CallBackProc<4>,
		(CallBack::CBP*)&CallBack::CallBackProc<5>,
		(CallBack::CBP*)&CallBack::CallBackProc<6>,
		(CallBack::CBP*)&CallBack::CallBackProc<7>,
		(CallBack::CBP*)&CallBack::CallBackProc<8>,
		(CallBack::CBP*)&CallBack::CallBackProc<9>,
		(CallBack::CBP*)&CallBack::CallBackProc<10>,
		(CallBack::CBP*)&CallBack::CallBackProc<11>,
	};

	return cbps[m_Num];
}
CallBack** CallBack::GetCallBack(unsigned int index){
	static CallBack* callBacks[12]={0};
	return &callBacks[index];
}


CallBack* CallBack::RegisterCallBack( IDispatchEx *cfg, unsigned int stacksize){
	for(int i=0; i<12; i++){
		if (*CallBack::GetCallBack(i)!=NULL)
			continue;

		return new CallBack(stacksize, cfg, i);
	}

	return NULL;
}
