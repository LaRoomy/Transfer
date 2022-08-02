#include"DataExchange.h"
#include"..\\BasicFPO.h"
#include"..\\HelperF.h"

SerialComm::SerialComm(HWND MainWnd, LPCTSTR Root_Directory)
	: ParentWindow(MainWnd),
	Configuration(nullptr),
	threadInterruptCtrl(0),
	IOBuffer(nullptr),
	threadActive(FALSE),
	transmissionReport(nullptr),
	inputBufferSize(DEFAULT_INPUTBUFFER_SIZE)
{
	this->configFileTargetDirectory = nullptr;
	CopyStringToPtr(Root_Directory, &this->configFileTargetDirectory);

	SecureZeroMemory(&this->IO_Info, sizeof(INOUTINFO));

	this->Configuration = new SERIAL_CONFIG;
	this->LoadTransmissionParameter();
}

SerialComm::SerialComm()
	: ParentWindow(nullptr),
	Configuration(nullptr),
	threadInterruptCtrl(0),
	IOBuffer(nullptr),
	threadActive(FALSE),
	transmissionReport(nullptr),
	configFileTargetDirectory(nullptr),
	inputBufferSize(DEFAULT_INPUTBUFFER_SIZE)
{
	this->Configuration = new SERIAL_CONFIG;
	SecureZeroMemory(this->Configuration, sizeof(SERIAL_CONFIG));
	this->Configuration->Active_port = 1; // default for first use!!!

	// use ONLY the getter and setter method for the configuration when creating an instance with the default constructor
	// DO NOTHING ELSE!!!!

	SecureZeroMemory(&this->IO_Info, sizeof(INOUTINFO));
}

SerialComm::~SerialComm(void)
{
	SafeDelete(&this->Configuration);
	SafeDeleteArray(&this->configFileTargetDirectory);
	SafeDeleteArray(&this->IOBuffer);
}

HRESULT SerialComm::LoadTransmissionParameter()
{
	HRESULT hr = S_OK;
	TCHAR *buffer = nullptr;

	auto bfpo = CreateBasicFPO();

	hr = (bfpo != nullptr) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		hr = bfpo->LoadBufferFmFileAsUtf8(&buffer, this->configFileTargetDirectory)
			? S_OK : E_FAIL;

		int xonchar, xoffchar, errorchar, eofchar, eventchar;

		if (SUCCEEDED(hr))
		{
			swscanf_s(buffer,
				L"%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n",
				&this->Configuration->Abort_on_error,
				&this->Configuration->Active_port,
				&this->Configuration->baud_index,
				&this->Configuration->CTS_Flow,
				&this->Configuration->databit_index,
				&this->Configuration->DSR_Flow,
				&this->Configuration->DSR_Sense,
				&this->Configuration->DTR_control_index,
				&this->Configuration->ErrorChar_replace,
				&this->Configuration->Paritycheck,
				&this->Configuration->parity_index,
				&this->Configuration->RI_Timeout,
				&this->Configuration->RTS_control_index,
				&this->Configuration->RT_Timeout_cst,
				&this->Configuration->RT_Timeout_mpl,
				&this->Configuration->stopbit_index,
				&this->Configuration->WT_Timeout_cst,
				&this->Configuration->WT_Timeout_mpl,
				&this->Configuration->XON_XOFF,
				&xonchar,
				&xoffchar,
				&errorchar,
				&eofchar,
				&eventchar
			);
			this->Configuration->XonChar = static_cast<WCHAR>(xonchar);
			this->Configuration->XoffChar = static_cast<WCHAR>(xoffchar);
			this->Configuration->ErrorChar = static_cast<WCHAR>(errorchar);
			this->Configuration->EofChar = static_cast<WCHAR>(eofchar);
			this->Configuration->EventChar = static_cast<WCHAR>(eventchar);
		}
		else
		{
			this->setDefault();
		}
		SafeDeleteArray(&buffer);
		SafeRelease(&bfpo);
	}
	return hr;
}

HRESULT SerialComm::SaveTransmissionParameter(void)
{
	HRESULT hr = S_OK;
	TCHAR *buffer = new TCHAR[2048];

	hr = StringCbPrintf(	buffer, sizeof( WCHAR ) * 2048,
							L"%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\n",
							this->Configuration->Abort_on_error,
							this->Configuration->Active_port,
							this->Configuration->baud_index,
							this->Configuration->CTS_Flow,
							this->Configuration->databit_index,
							this->Configuration->DSR_Flow,
							this->Configuration->DSR_Sense,
							this->Configuration->DTR_control_index,
							this->Configuration->ErrorChar_replace,
							this->Configuration->Paritycheck,
							this->Configuration->parity_index,
							this->Configuration->RI_Timeout,
							this->Configuration->RTS_control_index,
							this->Configuration->RT_Timeout_cst,
							this->Configuration->RT_Timeout_mpl,
							this->Configuration->stopbit_index,
							this->Configuration->WT_Timeout_cst,
							this->Configuration->WT_Timeout_mpl,
							this->Configuration->XON_XOFF,
							static_cast<int>(this->Configuration->XonChar),
							static_cast<int>(this->Configuration->XoffChar),
							static_cast<int>(this->Configuration->ErrorChar),
							static_cast<int>(this->Configuration->EofChar),
							static_cast<int>(this->Configuration->EventChar)
	);

	if (SUCCEEDED(hr))
	{
		auto bfpo = CreateBasicFPO();

		hr = (bfpo != nullptr)
			? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			bfpo->SaveBufferToFileAsUtf8(buffer, this->configFileTargetDirectory);

			SafeRelease(&bfpo);
		}
	}
	return hr;
}

void SerialComm::setDefault(void)
{
	this->Configuration->Active_port = 1;
	this->Configuration->baud_index = 4;
	this->Configuration->parity_index = 2;
	this->Configuration->databit_index = 2;
	this->Configuration->stopbit_index = 0;
	this->Configuration->XON_XOFF = FALSE;
	this->Configuration->DTR_control_index = 0;
	this->Configuration->RTS_control_index = 0;
	this->Configuration->CTS_Flow = FALSE;
	this->Configuration->DSR_Flow = FALSE;
	this->Configuration->Abort_on_error = FALSE;
	this->Configuration->ErrorChar_replace = TRUE;
	this->Configuration->Paritycheck = TRUE;
	this->Configuration->DSR_Sense = FALSE;
	this->Configuration->XonChar = '0';
	this->Configuration->XoffChar = '0';
	this->Configuration->EofChar = '0';
	this->Configuration->ErrorChar = '0';
	this->Configuration->EventChar = '0';
	this->Configuration->RI_Timeout = UNDEF;
	this->Configuration->RT_Timeout_cst = UNDEF;
	this->Configuration->RT_Timeout_mpl = UNDEF;
	this->Configuration->WT_Timeout_cst = UNDEF;
	this->Configuration->WT_Timeout_mpl = UNDEF;
}

HRESULT SerialComm::InitDataTransmission(int Mode, LPCTSTR dataOut)
{
	if(Mode == MODE_SEND)
	{
		if(dataOut == nullptr)
		{
			return E_FAIL;
		}
		else
		{
			size_t len;
			HRESULT hr;

			hr = StringCbLength(dataOut, sizeof( TCHAR )* STRSAFE_MAX_CCH, &len);
			if( SUCCEEDED( hr ))
			{
				this->IO_Info.totalDataSize = (int)(len / sizeof(TCHAR));
				this->IO_Info.processedDataSize = 0;

				this->IOBuffer = new WCHAR[(len + (10* sizeof( WCHAR )))];

				if(this->IOBuffer != nullptr)
				{
					hr = StringCbCopy(this->IOBuffer, (len + ( 10* sizeof( WCHAR ))), dataOut);
					if( SUCCEEDED( hr ))
					{
						hr = StringCbCat(this->IOBuffer, (len + ( 10 * sizeof( WCHAR ))), L"\n \n\0");	// *add two empty lines to make sure the whole data will be resolved by the machine
						if( FAILED( hr ))
						{
							return hr;
						}
					}
					else
						return E_FAIL;
				}
				else
					return E_FAIL;
			}
			else
				return E_FAIL;

		}
	}
	else if(Mode == MODE_RECEIVE)
	{
		this->IO_Info.processedDataSize = 0;
		this->IO_Info.totalDataSize = -1;// size unknown on receive-mode
	}
	else
		return E_FAIL;

	this->IO_Info.mode = Mode;

	HRESULT hr =
		this->startTransmissionThread(Mode)
		? S_OK : E_FAIL;

	if (SUCCEEDED(hr))
	{
		if (this->transmissionReport != nullptr)
		{
			this->transmissionReport->onTransmissionStart(Mode);
		}
	}
	return hr;
}

void SerialComm::TerminateTransmission()
{
	if (this->threadActive)
	{
		InterlockedExchange((LONG*)&this->threadInterruptCtrl, (LONG)TERMINATE);
	}
}


HRESULT SerialComm::setConfiguration(const TCHAR* path, PSERIAL_CONFIG config_in)
{
	HRESULT hr;

	if (path == nullptr)
	{
		hr = (this->configFileTargetDirectory != nullptr) ? S_OK : E_FAIL;
	}
	else
	{
		SafeDeleteArray(&this->configFileTargetDirectory);

		hr = (CopyStringToPtr(path, &this->configFileTargetDirectory) == TRUE)
			? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		this->Configuration->Abort_on_error = config_in->Abort_on_error;
		this->Configuration->Active_port = config_in->Active_port;
		this->Configuration->baud_index = config_in->baud_index;
		this->Configuration->CTS_Flow = config_in->CTS_Flow;
		this->Configuration->databit_index = config_in->databit_index;
		this->Configuration->DSR_Flow = config_in->DSR_Flow;
		this->Configuration->DSR_Sense = config_in->DSR_Sense;
		this->Configuration->DTR_control_index = config_in->DTR_control_index;
		this->Configuration->EofChar = config_in->EofChar;
		this->Configuration->ErrorChar = config_in->ErrorChar;
		this->Configuration->ErrorChar_replace = config_in->ErrorChar_replace;
		this->Configuration->EventChar = config_in->EventChar;
		this->Configuration->Paritycheck = config_in->Paritycheck;
		this->Configuration->parity_index = config_in->parity_index;
		this->Configuration->RI_Timeout = config_in->RI_Timeout;
		this->Configuration->RTS_control_index = config_in->RTS_control_index;
		this->Configuration->RT_Timeout_cst = config_in->RT_Timeout_cst;
		this->Configuration->RT_Timeout_mpl = config_in->RT_Timeout_mpl;
		this->Configuration->stopbit_index = config_in->stopbit_index;
		this->Configuration->WT_Timeout_cst = config_in->WT_Timeout_cst;
		this->Configuration->WT_Timeout_mpl = config_in->WT_Timeout_mpl;
		this->Configuration->XoffChar = config_in->XoffChar;
		this->Configuration->XonChar = config_in->XonChar;
		this->Configuration->XON_XOFF = config_in->XON_XOFF;

		return this->SaveTransmissionParameter();
	}
	return hr;
}

HRESULT SerialComm::getConfiguration(const TCHAR* path, PSERIAL_CONFIG config_out)
{
	HRESULT hr;

	hr = (path != nullptr) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		hr = (CopyStringToPtr(path, &this->configFileTargetDirectory) == TRUE)
			? S_OK : E_FAIL;

		if (FAILED(hr) && (this->configFileTargetDirectory != nullptr)) {
			iString tst(this->configFileTargetDirectory);
			if (tst.Equals(path)) {
				hr = S_OK;
			}
		}

		if (SUCCEEDED(hr))
		{
			hr =
				this->LoadTransmissionParameter();

			config_out->Abort_on_error = this->Configuration->Abort_on_error;
			config_out->Active_port = this->Configuration->Active_port;
			config_out->baud_index = this->Configuration->baud_index;
			config_out->CTS_Flow = this->Configuration->CTS_Flow;
			config_out->databit_index = this->Configuration->databit_index;
			config_out->DSR_Flow = this->Configuration->DSR_Flow;
			config_out->DSR_Sense = this->Configuration->DSR_Sense;
			config_out->DTR_control_index = this->Configuration->DTR_control_index;
			config_out->EofChar = this->Configuration->EofChar;
			config_out->ErrorChar = this->Configuration->ErrorChar;
			config_out->ErrorChar_replace = this->Configuration->ErrorChar_replace;
			config_out->EventChar = this->Configuration->EventChar;
			config_out->Paritycheck = this->Configuration->Paritycheck;
			config_out->parity_index = this->Configuration->parity_index;
			config_out->RI_Timeout = this->Configuration->RI_Timeout;
			config_out->RTS_control_index = this->Configuration->RTS_control_index;
			config_out->RT_Timeout_cst = this->Configuration->RT_Timeout_cst;
			config_out->RT_Timeout_mpl = this->Configuration->RT_Timeout_mpl;
			config_out->stopbit_index = this->Configuration->stopbit_index;
			config_out->WT_Timeout_cst = this->Configuration->WT_Timeout_cst;
			config_out->WT_Timeout_mpl = this->Configuration->WT_Timeout_mpl;
			config_out->XoffChar = this->Configuration->XoffChar;
			config_out->XonChar = this->Configuration->XonChar;
			config_out->XON_XOFF = this->Configuration->XON_XOFF;
		}
	}
	return hr;
}

HRESULT SerialComm::setDefaultConfiguration(const TCHAR * path)
{
	if (path != nullptr)
	{
		SERIAL_CONFIG config;
		SecureZeroMemory(&config, sizeof(SERIAL_CONFIG));

		config.baud_index = 4;
		config.databit_index = 2;
		config.parity_index = 2;
		config.stopbit_index = 0;
		config.Active_port = 1;

		config.EofChar = L'0';
		config.ErrorChar = L'0';
		config.EventChar = L'0';
		config.XoffChar = L'0';
		config.XonChar = L'0';

		config.Paritycheck = TRUE;
		config.ErrorChar_replace = TRUE;

		config.RI_Timeout = 50;
		config.RT_Timeout_mpl = 2000;
		config.RT_Timeout_cst = 0;
		config.WT_Timeout_mpl = 12000;
		config.WT_Timeout_cst = -1;

		return this->setConfiguration(path, &config);
	}
	return E_POINTER;
}

const wchar_t* SerialComm::ToString()
{
	SafeDeleteArray(&this->clsStringArg);

	HRESULT hr = S_OK;
	this->clsStringArg = new TCHAR[2048];

	hr = StringCbPrintf(this->clsStringArg, sizeof(WCHAR) * 2048,
		L"%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\r\n%i\n",
		this->Configuration->Abort_on_error,
		this->Configuration->Active_port,
		this->Configuration->baud_index,
		this->Configuration->CTS_Flow,
		this->Configuration->databit_index,
		this->Configuration->DSR_Flow,
		this->Configuration->DSR_Sense,
		this->Configuration->DTR_control_index,
		this->Configuration->ErrorChar_replace,
		this->Configuration->Paritycheck,
		this->Configuration->parity_index,
		this->Configuration->RI_Timeout,
		this->Configuration->RTS_control_index,
		this->Configuration->RT_Timeout_cst,
		this->Configuration->RT_Timeout_mpl,
		this->Configuration->stopbit_index,
		this->Configuration->WT_Timeout_cst,
		this->Configuration->WT_Timeout_mpl,
		this->Configuration->XON_XOFF,
		static_cast<int>(this->Configuration->XonChar),
		static_cast<int>(this->Configuration->XoffChar),
		static_cast<int>(this->Configuration->ErrorChar),
		static_cast<int>(this->Configuration->EofChar),
		static_cast<int>(this->Configuration->EventChar)
	);
	return this->clsStringArg;
}

void SerialComm::FromString(const wchar_t* stringRepresentation)
{
	if (stringRepresentation != nullptr)
	{
		int xonchar, xoffchar, errorchar, eofchar, eventchar;

		swscanf_s(stringRepresentation,
			L"%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n",
			&this->Configuration->Abort_on_error,
			&this->Configuration->Active_port,
			&this->Configuration->baud_index,
			&this->Configuration->CTS_Flow,
			&this->Configuration->databit_index,
			&this->Configuration->DSR_Flow,
			&this->Configuration->DSR_Sense,
			&this->Configuration->DTR_control_index,
			&this->Configuration->ErrorChar_replace,
			&this->Configuration->Paritycheck,
			&this->Configuration->parity_index,
			&this->Configuration->RI_Timeout,
			&this->Configuration->RTS_control_index,
			&this->Configuration->RT_Timeout_cst,
			&this->Configuration->RT_Timeout_mpl,
			&this->Configuration->stopbit_index,
			&this->Configuration->WT_Timeout_cst,
			&this->Configuration->WT_Timeout_mpl,
			&this->Configuration->XON_XOFF,
			&xonchar,
			&xoffchar,
			&errorchar,
			&eofchar,
			&eventchar
		);
		this->Configuration->XonChar = static_cast<WCHAR>(xonchar);
		this->Configuration->XoffChar = static_cast<WCHAR>(xoffchar);
		this->Configuration->ErrorChar = static_cast<WCHAR>(errorchar);
		this->Configuration->EofChar = static_cast<WCHAR>(eofchar);
		this->Configuration->EventChar = static_cast<WCHAR>(eventchar);
	}
	else
	{
		this->setDefault();
	}
}


HRESULT SerialComm::serialInit(LPDCB dcb, LPCOMMTIMEOUTS timeouts, LPOVERLAPPED ovl)
{
	// general:
	dcb->BaudRate = _baud[ this->Configuration->baud_index ];
	dcb->ByteSize = (BYTE)this->Configuration->databit_index + 5;
	dcb->StopBits = stoppbts[ this->Configuration->stopbit_index ];
	dcb->Parity = parity[ this->Configuration->parity_index ];

	dcb->fBinary = TRUE;

	dcb->fParity = this->Configuration->Paritycheck;
	dcb->fAbortOnError = this->Configuration->Abort_on_error;
	dcb->fErrorChar = this->Configuration->ErrorChar_replace;

	// characters:
	dcb->EofChar = (char)this->Configuration->EofChar;
	dcb->ErrorChar = (char)this->Configuration->ErrorChar;
	dcb->EvtChar = (char)this->Configuration->EventChar;
	dcb->XonChar = (char)this->Configuration->XonChar;
	dcb->XoffChar = (char)this->Configuration->XoffChar;

	// handshake:
	// if Configuration->XON_XOFF == 0 -> no handshake
	// if Configuration->XON_XOFF == 1 -> software handshake
	// if Configuration->XON_XOFF == 2 -> hardware handshake

	if( this->Configuration->XON_XOFF == 1 )
	{
		dcb->fInX = TRUE;
		dcb->fOutX = TRUE;

		dcb->XonLim = 50;
		dcb->XoffLim = 100;

		dcb->fDsrSensitivity = FALSE;
		dcb->fOutxCtsFlow = FALSE;
		dcb->fOutxDsrFlow = FALSE;
		
		dcb->fDtrControl = DTR_CONTROL_DISABLE;
		dcb->fRtsControl = RTS_CONTROL_DISABLE;

		dcb->fTXContinueOnXoff = TRUE;
	}
	else if (this->Configuration->XON_XOFF == 2)
	{
		dcb->fInX = FALSE;
		dcb->fOutX = FALSE;

		dcb->XonLim = 50;
		dcb->XoffLim = 100;

		dcb->fDsrSensitivity = TRUE;
		dcb->fOutxCtsFlow = TRUE;
		dcb->fOutxDsrFlow = TRUE;
		
		dcb->fDtrControl = this->Configuration->DTR_control_index;
		dcb->fRtsControl = this->Configuration->RTS_control_index;

		dcb->fTXContinueOnXoff = TRUE;
	}
	else
	{
		dcb->fInX = FALSE;
		dcb->fOutX = FALSE;

		dcb->fOutxCtsFlow = FALSE;
		dcb->fOutxDsrFlow = FALSE;
		dcb->fDsrSensitivity = FALSE;
		dcb->fAbortOnError = FALSE;
		dcb->fDtrControl = DTR_CONTROL_ENABLE;
		dcb->fRtsControl = RTS_CONTROL_ENABLE;
	}

	// init overlapped structure
	ovl->Internal = 0;
	ovl->InternalHigh = 0;
	ovl->Offset = 0;
	ovl->OffsetHigh = 0;

	// set timeouts
	timeouts->ReadIntervalTimeout = this->Configuration->RI_Timeout;
	timeouts->ReadTotalTimeoutMultiplier = this->Configuration->RT_Timeout_mpl;
	timeouts->ReadTotalTimeoutConstant = this->Configuration->RT_Timeout_cst;

	DWORD wttm = 12000;
	if ((dcb->BaudRate + 1) != 0)// prevent division by zero
		wttm = this->Configuration->WT_Timeout_mpl / (dcb->BaudRate + 1);

	timeouts->WriteTotalTimeoutMultiplier = (wttm);

	if( this->Configuration->WT_Timeout_cst < 2 )
	{
		timeouts->WriteTotalTimeoutConstant = timeouts->WriteTotalTimeoutMultiplier + 1;
	}
	else
	{
		timeouts->WriteTotalTimeoutConstant = this->Configuration->WT_Timeout_cst;
	}
	return S_OK;
}

void SerialComm::stepProgress()
{
	this->IO_Info.processedDataSize++;

	if (this->transmissionReport != nullptr)
	{
		this->transmissionReport->transmissionProgress(this->IO_Info.processedDataSize, this->IO_Info.totalDataSize);
	}
}

BOOL SerialComm::startTransmissionThread(int Mode)
{
	if (this->transmissionReport != nullptr)
	{
		// start the report
		this->transmissionReport->transmissionProgress(0, this->IO_Info.totalDataSize);
	}
	
	DWORD ThreadID;
	HANDLE hThread;
	LPTRANSMISSION_THREAD_DATA tinfo = nullptr;

	tinfo = new TRANSMISSION_THREAD_DATA;

	if(tinfo == nullptr)
	{
		return FALSE;
	}

	tinfo->MODE = Mode;
	tinfo->additional = 0;
	tinfo->serialCom = reinterpret_cast<LONG_PTR>(this);

	hThread =
		CreateThread(
			nullptr, 0,
			SerialComm::TransmissionProc,
			(LPVOID)tinfo,
			0,
			&ThreadID
		);
	if(hThread == nullptr)
	{
		return FALSE;
	}
	else
	{
		WaitForSingleObject(hThread, 50);
		CloseHandle(hThread);

		return TRUE;
	}
}

DWORD WINAPI SerialComm::TransmissionProc( LPVOID lParam )
{
	DWORD exitcode = 0;

	EnterCriticalSection(
		GetCriticalSection()
	);

	auto transmissionThreadData = reinterpret_cast<LPTRANSMISSION_THREAD_DATA>(lParam);
	if (transmissionThreadData != nullptr)
	{
		auto Mode = transmissionThreadData->MODE;
		auto _this_ =
			reinterpret_cast<SerialComm*>(transmissionThreadData->serialCom);

		auto hr = (_this_ != nullptr) ? S_OK : E_POINTER;
		if (SUCCEEDED(hr))
		{
			iString message;
			WCHAR PortID[20] = { 0 };

			InterlockedExchange((LONG*)&_this_->threadActive, (LONG)TRUE);

			hr =
				StringCbPrintf(
					PortID,
					sizeof(PortID),
					L"\\\\.\\COM%i\0",
					_this_->Configuration->Active_port
				);
			if (SUCCEEDED(hr))
			{
				DCB dcb;
				COMMTIMEOUTS timeouts;
				OVERLAPPED ovl;
				HANDLE hFile, hEvent;

				hFile =
					CreateFile(
						PortID,
						GENERIC_READ | GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						FILE_FLAG_OVERLAPPED,
						NULL
					);

				hr = (hFile == INVALID_HANDLE_VALUE) ? E_HANDLE : S_OK;
				if(FAILED(hr))
				{
					exitcode = 1;
				}
				else
				{
					SecureZeroMemory(&dcb, sizeof(DCB));
					dcb.DCBlength = sizeof(DCB);

					hr = GetCommState(hFile, &dcb) ? S_OK : E_FAIL;
					if(FAILED(hr))
					{
						exitcode = 2;
					}
					else
					{
						hr = _this_->serialInit(&dcb, &timeouts, &ovl);
						if (FAILED(hr))
						{
							exitcode = 3;
						}
						else
						{
							hr = SetCommState(hFile, &dcb) ? S_OK : E_FAIL;
							if(FAILED(hr))
							{
								exitcode = 4;
							}
							else
							{
								hr = GetCommTimeouts(hFile, &timeouts) ? S_OK : E_FAIL;
								if(FAILED(hr))
								{
									exitcode = 5;
								}
								else
								{
									hr = SetCommTimeouts(hFile, &timeouts) ? S_OK : E_FAIL;
									if(FAILED(hr))
									{
										exitcode = 6;
									}
									else
									{
										hEvent =
											CreateEvent(nullptr, TRUE, FALSE, nullptr);

										hr = (hEvent != nullptr) ? S_OK : E_HANDLE;
										if(FAILED(hr))
										{
											exitcode = 7;
										}
										else
										{
											ovl.hEvent = hEvent;

											SetCommMask(hFile, EV_RXCHAR);

											if (Mode == MODE_SEND)
											{
												if (!_this_->outputProcess(hFile, &ovl))
												{
													// send error
												}
											}
											else if (Mode == MODE_RECEIVE)
											{
												if (!_this_->inputProcess(hFile, &ovl))
												{
													// receive error
												}
											}

											// the "thread-active" indicator must be false, otherwise the finish-event won't be raised
											InterlockedExchange((LONG*)&_this_->threadActive, (LONG)FALSE);

											if ((_this_->threadInterruptCtrl == INTERRUPT_TRANSMISSION) || (_this_->threadInterruptCtrl == TERMINATE))
											{
												_this_->OnInterrupt(Mode);
											}
											else
											{
												_this_->FinishTransmission();
											}
											CloseHandle(hEvent);
										}
									}
								}
							}
						}
					}
					CloseHandle(hFile);
				}
			}
			InterlockedExchange((LONG*)&_this_->threadActive, (LONG)FALSE);
		}
		delete transmissionThreadData;

		LeaveCriticalSection(
			GetCriticalSection()
		);
	}
	return exitcode;
}

DWORD SerialComm::bufferConvert(char** low_Buffer)
{
	DWORD cbData = _lengthOfString(this->IOBuffer);

	if (ConvertWCHARtoCHAR(this->IOBuffer, low_Buffer) != TRUE)
	{
		return 0;
	}
	return cbData;
}

DWORD SerialComm::bufferReconvert(char* low_Buffer)
{
	SafeDeleteArray(&this->IOBuffer);

	if (ConvertCHARtoWCHAR(low_Buffer, &this->IOBuffer) != TRUE)
	{
		return  0;
	}
	else
	{
		return (DWORD)_lengthOfString(this->IOBuffer);
	}
}

BOOL SerialComm::outputProcess(HANDLE hFile, LPOVERLAPPED ovl)
{
	DWORD counter = 0;
	char *multiByteBuffer = nullptr;

	DWORD bytesToWrite = 
		this->bufferConvert(&multiByteBuffer);

	DWORD bytesWritten, Err, wait_result;

	while(counter < bytesToWrite)
	{
		this->stepProgress();

		// write char into output stream (WriteFile is used asynchronously, so it will return immediately)
		WriteFile(hFile, &multiByteBuffer[counter], 1, &bytesWritten, ovl);	// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
		counter++;

		// check if a termination of the output-loop is requested:
		if((this->threadInterruptCtrl == TERMINATE)
			||(this->threadInterruptCtrl == INTERRUPT_TRANSMISSION))
		{
			SafeDeleteArray(&multiByteBuffer);
			return TRUE;
		}

		// Check the last error value, it is expected that ERROR_IO_PENDING is returned
		// -> this is not an error, it indicates that the write operation is in progress
		Err =
			GetLastError();

		if(Err == ERROR_IO_PENDING)
		{
			// wait for the pending operation to complete
			while(1)
			{	
				// wait 100mSec for the event
				wait_result = WaitForSingleObject(ovl->hEvent, 100); 
				if(wait_result == WAIT_TIMEOUT)
				{
					// if a timeout occurs, check if a termination is requested:
					if((this->threadInterruptCtrl == INTERRUPT_TRANSMISSION)
						||(this->threadInterruptCtrl == TERMINATE))
					{
						SafeDeleteArray(&multiByteBuffer);
						return TRUE;
					}
				}
				else if(wait_result == WAIT_OBJECT_0)
				{
					// operation completed -> try to write the next char
					break;
				}
				else
				{
					// unexpected... (should not happen) ...could be an error
					break;
				}
			}
			if(!GetOverlappedResult(hFile, ovl, &bytesWritten, FALSE))	// https://docs.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-getoverlappedresult
			{
				Err =
					GetLastError(); 

				if((Err == ERROR_IO_PENDING)||(Err == ERROR_IO_INCOMPLETE))
				{ 
					// error
					SafeDeleteArray(&multiByteBuffer);
					return FALSE;
				}
			}
		}
		else
		{
			// (Err != ERROR_IO_PENDING) -> this could be an error, but don't have to be...
			SafeDeleteArray(&multiByteBuffer);
			return FALSE;
		}
	}
	// mark the transmission as completed
	InterlockedExchange((LONG*)&this->threadInterruptCtrl, (LONG)TRANSMISSION_COMPLETE);
	SafeDeleteArray(&multiByteBuffer);
	return TRUE;
}

BOOL SerialComm::inputProcess(HANDLE hFile, LPOVERLAPPED ovl)
{
	DWORD counter = 0;
	bool array_start = false, holding = true;
	char trans = 0;
	char *multiByteBuffer = nullptr;
	DWORD BytesRecieved, wait_result, Err;

	multiByteBuffer =
		new (std::nothrow) CHAR[static_cast<size_t>(this->inputBufferSize) + 1];

	if (multiByteBuffer == nullptr)
		return FALSE;

	while(holding)
	{
		// try to read a character out of the input stream
		while(ReadFile(hFile, &trans, 1, &BytesRecieved, ovl))	// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
		{
			// data received...

			if((trans != 0)&&(!array_start))
			{
				array_start = true;
			}
			if(array_start)
			{
				multiByteBuffer[counter] = trans;
				this->stepProgress();
				counter++;
			}
			// check if a termination of the stream is requested
			if((this->threadInterruptCtrl == TERMINATE)
				||(this->threadInterruptCtrl == INTERRUPT_TRANSMISSION))
			{
				multiByteBuffer[counter] = '\0';
				this->bufferReconvert(multiByteBuffer);
				SafeDeleteArray(&multiByteBuffer);
				return TRUE;
			}
		}
		// at this point ReadFile(..) returns false, so no data could be read out of the input stream
		// check the result of Read-Process
		Err = GetLastError();
		// ERROR_IO_PENDING is expected (this is not an error, is means that the read-operation is in progress)
		if (Err == ERROR_IO_PENDING)
		{
			// Wait in a loop for incoming data
			while(1)
			{
				//wait max 500mSec for the event
				wait_result = WaitForSingleObject(ovl->hEvent, 500);

				if (wait_result == WAIT_TIMEOUT)
				{	
					if(array_start)
					{
						holding = false;
						break;
					}
					// while waiting, check if a termination of the process is requested
					if( ( this->threadInterruptCtrl == INTERRUPT_TRANSMISSION ) ||
						( this->threadInterruptCtrl == TERMINATE ) )
					{
						if( this->threadInterruptCtrl == INTERRUPT_TRANSMISSION )
						{
							multiByteBuffer[counter] = '\0';
							this->bufferReconvert(multiByteBuffer);
							SafeDeleteArray(&multiByteBuffer);
						}
						else
						{
							// ...

							SafeDeleteArray(&multiByteBuffer);
						}
						return TRUE;
					}
				}
				else if(wait_result == WAIT_OBJECT_0)
				{
					// operation complete
					break;
				}
				else
				{
					// error?
					break;
				}
			}
			if(!holding)
			{
				break;
			}
			if(!GetOverlappedResult(hFile ,ovl, &BytesRecieved, FALSE))	// https://docs.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-getoverlappedresult
			{
				Err = GetLastError(); 
				if( ( Err == ERROR_IO_PENDING ) || ( Err == ERROR_IO_INCOMPLETE ) )
				{
					//error
					SafeDeleteArray(&multiByteBuffer);
					return FALSE;
				}
			}
			else
			{
				// this iteration catches the scenario when the overlapped-event is triggered after the invokation of ReadFile, but before the invokation of WaitForSingleObject.
				// if the characters wouldn't catched here, they were lost...
				if ((trans != 0) && (!array_start))
				{
					array_start = true;
				}
				if( array_start )
				{
					multiByteBuffer[counter] = trans;
					this->stepProgress();
					counter++;
				}
			}
		}
		else
		{
			SafeDeleteArray(&multiByteBuffer);
			return FALSE;
		}
	}
	multiByteBuffer[counter] = '\0';

	this->bufferReconvert( multiByteBuffer );
	this->threadInterruptCtrl = TRANSMISSION_COMPLETE;
	SafeDeleteArray(&multiByteBuffer);

	return TRUE;
}

void SerialComm::CancelTransmission()
{
	if(this->threadActive)
	{
		InterlockedExchange((LONG*)&this->threadInterruptCtrl, (LONG)INTERRUPT_TRANSMISSION);
	}
}

void SerialComm::FinishTransmission()
{
	if(this->threadActive)
	{
		InterlockedExchange((LONG*)&this->threadInterruptCtrl, (LONG)INTERRUPT_TRANSMISSION);
	}
	else
	{
		if( this->IOBuffer != nullptr )
		{
			if (this->transmissionReport != nullptr)
			{
				this->transmissionReport->onTransmissionComplete(this->IOBuffer);
			}
		}
	}
}

void SerialComm::OnInterrupt(int Mode)
{
	if(this->threadInterruptCtrl == TERMINATE)
	{
		if (this->transmissionReport != nullptr)
		{
			this->transmissionReport->onTransmissionInterrupt(nullptr);
		}
	}
	else if(this->threadInterruptCtrl == INTERRUPT_TRANSMISSION)
	{
		if(Mode == MODE_RECEIVE)
		{
			if (this->transmissionReport != nullptr)
			{
				this->transmissionReport->onTransmissionInterrupt(this->IOBuffer);
			}			
		}
		else if (Mode == MODE_SEND)
		{
			if (this->transmissionReport != nullptr)
			{
				this->transmissionReport->onTransmissionInterrupt(nullptr);
			}
		}
	}

	// reset interrupt control
	this->threadInterruptCtrl = 0;
}
