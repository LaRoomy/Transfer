#pragma once
#include"..//framework.h"
#include"..//cObject.h"

#include <StringClass.h>
#include <new>
#include <cListView.h>
#include <CustomButton.h>

#define		SEND_					1240
#define		RECEIVE_				1241
#define		SEND_BUFFER				1242
#define		RECEIVE_BUFFER			1243
#define		TERMINATE				1252
#define		INTERRUPT_TRANSMISSION	1253
#define		TRANSMISSION_COMPLETE	1254

#define		MAX_STRINGLENGTH			56
#define		UNDEF						-1
#define		COMMONARRAYSIZE				512
#define		DEFAULT_INPUTBUFFER_SIZE	500000
#define		BAUDRATE_STRUCTSIZE		15

const GUID GUID_CLASS_COMPORT = { 0x86e0d1e0, 0x8089, 0x11d0, 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 };

static const DWORD _baud[] = {	CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_14400,
								CBR_19200, CBR_38400, CBR_56000, CBR_57600, CBR_115200, CBR_128000, CBR_256000	};

static const BYTE parity[] = {	NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY };

static const BYTE stoppbts[] = {	ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS };

typedef struct String_List{

	int NUM_STRINGS;
	WCHAR Stringlist[ 65 ][ MAX_STRINGLENGTH ];

}STRINGSTOCOMBO, *PSTRINGSTOCOMBO;

typedef struct _TRANSMISSON_THREAD_DATA{

	int MODE;
	int additional;
	LONG_PTR serialCom;

}TRANSMISSION_THREAD_DATA, *LPTRANSMISSION_THREAD_DATA;

typedef struct _SERIAL_CONFIG{

	int Active_port;
	int baud_index;
	int parity_index;
	int databit_index;
	int stopbit_index;
	int DTR_control_index;
	int RTS_control_index;
	TCHAR XonChar;
	TCHAR XoffChar;
	TCHAR ErrorChar;
	TCHAR EofChar;
	TCHAR EventChar;
	int RI_Timeout;
	int RT_Timeout_cst;
	int RT_Timeout_mpl;
	int WT_Timeout_cst;
	int WT_Timeout_mpl;
	BOOL DSR_Sense;
	BOOL Paritycheck;
	BOOL Abort_on_error;
	BOOL ErrorChar_replace;
	BOOL CTS_Flow;
	BOOL DSR_Flow;
	BOOL XON_XOFF;

	_SERIAL_CONFIG& operator= (const _SERIAL_CONFIG& sc)
	{
		this->Active_port = sc.Active_port;
		this->baud_index = sc.baud_index;
		this->parity_index = sc.parity_index;
		this->databit_index = sc.databit_index;
		this->stopbit_index = sc.stopbit_index;
		this->DTR_control_index = sc.DTR_control_index;
		this->RTS_control_index = sc.RTS_control_index;
		this->XoffChar = sc.XoffChar;
		this->XonChar = sc.XonChar;
		this->ErrorChar = sc.ErrorChar;
		this->EofChar = sc.EofChar;
		this->EventChar = sc.EventChar;
		this->RI_Timeout = sc.RI_Timeout;
		this->RT_Timeout_cst = sc.RT_Timeout_cst;
		this->RT_Timeout_mpl = sc.RT_Timeout_mpl;
		this->WT_Timeout_cst = sc.WT_Timeout_cst;
		this->WT_Timeout_mpl = sc.WT_Timeout_mpl;
		this->DSR_Sense = sc.DSR_Sense;
		this->Paritycheck = sc.Paritycheck;
		this->Abort_on_error = sc.Abort_on_error;
		this->ErrorChar_replace = sc.ErrorChar_replace;
		this->CTS_Flow = sc.CTS_Flow;
		this->DSR_Flow = sc.DSR_Flow;
		this->XON_XOFF = sc.XON_XOFF;

		return *this;
	}

}SERIAL_CONFIG, *PSERIAL_CONFIG;

typedef struct _INOUTINFO {
	int mode;
	int totalDataSize;//(in bytes)
	int processedDataSize;//(in bytes)

}INOUTINFO, *LPINOUTINFO;


__interface ITransmissionReportProtocol {
public:
	void onTransmissionStart(int mode);
	void transmissionProgress(int currentProgress, int fullDataSize);
	void onTransmissionComplete(LPCTSTR data);
	void onTransmissionInterrupt(LPCTSTR data);
};

class SerialComm : public ClsObject<SerialComm>
{
public:			
	SerialComm();										// the default constructor is for the config setter/getter methods only, do not use for data transmission
	SerialComm(HWND Parent, LPCTSTR Root_Directory);	// the custom constructor must be used for the data transmission
	~SerialComm();

	HRESULT InitDataTransmission(int Mode, LPCTSTR directBuffer);

	void CancelTransmission();
	void TerminateTransmission();

	bool HasActiveTransmission() {
		return this->threadActive ? true : false;
	}

	int GetTransmissionDataLength() {
		return this->IO_Info.processedDataSize;
	}

	void SetInputBufferSize(DWORD bufferSize) {
		this->inputBufferSize = bufferSize;
	}

	void SetTransmissionEventProtocolHandler(ITransmissionReportProtocol* pProtocol) {
		this->transmissionReport = pProtocol;
	}

	// setter + getter for the saved configuration:
	HRESULT setConfiguration(const TCHAR* path, PSERIAL_CONFIG config_in);
	HRESULT getConfiguration(const TCHAR* path, PSERIAL_CONFIG config_out);
	HRESULT setDefaultConfiguration(const TCHAR* path);

	// implementation of the virtual ClsObject methods
	const wchar_t* ToString();
	void FromString(const wchar_t* stringRepresentation);

private:
	HWND ParentWindow;
	int threadInterruptCtrl;
	BOOL threadActive;
	DWORD inputBufferSize;
	WCHAR *configFileTargetDirectory;
	WCHAR *IOBuffer;

	PSERIAL_CONFIG Configuration;
	INOUTINFO IO_Info;
	ITransmissionReportProtocol* transmissionReport;

	static DWORD WINAPI TransmissionProc(LPVOID);

	BOOL startTransmissionThread(int);
	HRESULT serialInit(LPDCB,LPCOMMTIMEOUTS,LPOVERLAPPED);
	DWORD bufferConvert(char**);
	DWORD bufferReconvert(char*);

	void OnInterrupt(int Mode);
	void FinishTransmission();
	void stepProgress();
	void setDefault(void);
	
	HRESULT LoadTransmissionParameter(void);
	HRESULT SaveTransmissionParameter(void);

	BOOL outputProcess(HANDLE,LPOVERLAPPED);
	BOOL inputProcess(HANDLE,LPOVERLAPPED);
};