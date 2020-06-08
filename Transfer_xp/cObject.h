#pragma once

#ifndef _COBJECT_H_
#define	_COBJECT_H_

#define		WM_GETWNDINSTANCE		WM_USER+100			// TODO -> CHANGE
#define		WM_INTERRUPT			WM_USER+101			// ???
#define		WM_TRANSMISSIONCOMPLETE	WM_USER+102			// ???
#define		WM_EXDESTROY			WM_USER+103			// ???

#include<stdexcept>
using namespace std;

class DataAccessViolationException
	: public runtime_error {
private:
	DWORD dwError = EXCEPTION_ACCESS_VIOLATION;
public:
	DataAccessViolationException(const char* errorMessage)
	: runtime_error(errorMessage) {}

	DWORD GetExceptionCode() const { return this->dwError; }
};

// TODO: add more exception classes

typedef LONG_PTR cObject;
typedef LPTSTR* LPPTSTR;

template<class C> class ClsObject
{
public:
	ClsObject(){}
	~ClsObject(){}

	void Release(){
		delete (C*)this;
	}
	void Release_delayed(int delay_ms) {

		this->delayTime = delay_ms;
		createReleaseThread();
	}

	virtual const wchar_t* ToString() = 0;
	virtual void FromString(const wchar_t* stringRepresentation) = 0;

private:
	int delayTime = 0;
	

	void createReleaseThread()
	{
		DWORD threadId;
		HANDLE hThread;

		hThread = CreateThread(nullptr, 0, ClsObject::delayProc, reinterpret_cast<LPVOID>(this), 0, &threadId);
		if (hThread != nullptr)
		{
			WaitForSingleObject(hThread, 10);
			CloseHandle(hThread);
		}
	}

	static DWORD __stdcall delayProc(LPVOID lParam)
	{
		auto _this = reinterpret_cast<ClsObject*>(lParam);
		if (_this != nullptr)
		{
			int time = _this->delayTime;
			Sleep(time);

			_this->Release();
		}
		return 0;
	}

protected:
	TCHAR* clsStringArg = nullptr;
};
#endif // _COBJECT_H_
