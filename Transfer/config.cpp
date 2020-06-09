#include"config.h"

ConfigProperty::ConfigProperty()
	: configWnd(nullptr),
	editChangeBlockerEnabled(false),
	infoTextField(nullptr),
	currentTransmissionMode(NO_TRANSMSN),
	editBox(nullptr),
	hInstance(nullptr),
	parentWnd(nullptr),
	sendButton(nullptr),
	receiveButton(nullptr),
	serialComm (nullptr)
{
	this->clientFont = CreateCFont(18, FW_BOLD, APPLICATION_PRIMARY_FONT);
}

ConfigProperty::~ConfigProperty()
{
	SafeRelease(&this->serialComm);
	DeleteObject(this->clientFont);
}

HRESULT ConfigProperty::CreateConfigWindow(HINSTANCE hInst, HWND parent)
{
	this->parentWnd = parent;
	this->hInstance = hInst;

	HRESULT hr;

	RECT wndRect = { 0, 0, 400, 230 };

	this->registerConfigWndClass();

	hr = this->createConfigControls(&wndRect);
	if (SUCCEEDED(hr))
	{
		iString path(
			GetGlobalAppPath()
		);
		path.Append(L"\\comset.dat");

		this->serialComm =
			new SerialComm(parent, path.GetData());

		hr = (this->serialComm != nullptr)
			? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			this->serialComm->SetTransmissionEventProtocolHandler(
				dynamic_cast<ITransmissionReportProtocol*>(this)
			);

			// ...
		}
	}
	return hr;
}

HRESULT ConfigProperty::CreateEditBox()
{
	RECT configRect, parentRect;
	GetClientRect(this->configWnd, &configRect);
	GetClientRect(this->parentWnd, &parentRect);

	iString savePath(
		GetGlobalAppPath()
	);
	savePath.Append(L"\\temp_content.dat");

	auto bfpo = CreateBasicFPO();
	if (bfpo != nullptr)
	{
		TCHAR* buffer = nullptr;

		if (bfpo->LoadBufferFmFileAsUtf8(&buffer, savePath.GetData()))
		{
			if (buffer != nullptr)
			{
				this->currentLoadedFileBuffer = buffer;
			}
			SafeDeleteArray(&buffer);
		}
		bfpo->Release();
	}

	this->editBox =
		CreateWindow(
			L"EDIT",
			nullptr,
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE,
			configRect.right, 30, parentRect.right - configRect.right, parentRect.bottom - 30,
			this->parentWnd,
			IDTOHMENU(WNDID_EDITBOX),
			this->hInstance,
			nullptr
		);

	auto hr =
		(this->editBox != nullptr) ? S_OK : E_FAIL;

	if (SUCCEEDED(hr))
	{
		HFONT font = CreateCFont(18, FW_SEMIBOLD, L"Lucida Sans Typewriter");
		SendMessage(this->editBox, WM_SETFONT, reinterpret_cast<WPARAM>(font), (LPARAM)TRUE);

		if (this->currentLoadedFileBuffer.GetLength() > 0)
		{
			SetWindowText(
				this->editBox,
				this->currentLoadedFileBuffer.GetData()
			);
		}
	}
	return hr;
}

HRESULT ConfigProperty::CreateSubControls()
{
	HRESULT hr;
	POINT pos;
	SIZE size;
	RECT rcParent;
	iString ctrlText;

	GetClientRect(this->parentWnd, &rcParent);

	pos.x = 402;
	pos.y = 0;
	size.cx = (rcParent.right - 402) / 2;
	if (size.cx > 100)
		size.cx = 100;
	size.cy = 30;

	auto openButton =
		new CustomButton(this->parentWnd, BUTTONMODE_TEXT, &pos, &size, CTRLID_OPENBUTTON, this->hInstance);

	hr = (openButton != nullptr) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ctrlText.Replace(L"Öffnen");

		openButton->setAppearance_onlyText(ctrlText, FALSE);
		openButton->setBorder(TRUE, OUTLINECOLOR);
		openButton->setColors(BUTTONCOLOR, COLOR_BUTTON_MOUSE_OVER, COLOR_BUTTON_PRESSED);
		openButton->setTextColor(TEXTCOLOR);
		openButton->setEventListener(dynamic_cast<customButtonEventSink*>(this));

		hr = openButton->Create();
		if (SUCCEEDED(hr))
		{
			pos.x += (size.cx + 2);

			auto saveButton =
				new CustomButton(this->parentWnd, BUTTONMODE_TEXT, &pos, &size, CTRLID_SAVEBUTTON, this->hInstance);

			hr = (saveButton != nullptr) ? S_OK : E_FAIL;
			if (SUCCEEDED(hr))
			{
				ctrlText.Replace(L"Speichern");

				saveButton->setAppearance_onlyText(ctrlText, FALSE);
				saveButton->setBorder(TRUE, OUTLINECOLOR);
				saveButton->setColors(BUTTONCOLOR, COLOR_BUTTON_MOUSE_OVER, COLOR_BUTTON_PRESSED);
				saveButton->setTextColor(TEXTCOLOR);
				saveButton->setEventListener(dynamic_cast<customButtonEventSink*>(this));

				hr = saveButton->Create();
				if (SUCCEEDED(hr))
				{
					pos.x = 30;
					pos.y = 280;
					size.cx = 160;
					size.cy = 50;

					sendButton =
						new CustomButton(this->parentWnd, BUTTONMODE_ICONTEXT, &pos, &size, CTRLID_SENDBUTTON, this->hInstance);

					hr = (sendButton != nullptr) ? S_OK : E_FAIL;
					if (SUCCEEDED(hr))
					{
						ctrlText.Replace(L"Senden");

						sendButton->setAppearance_IconText(IDI_SEND, 42, ctrlText);
						sendButton->setBorder(TRUE, OUTLINECOLOR);
						sendButton->setColors(BUTTONCOLOR, COLOR_BUTTON_MOUSE_OVER, COLOR_BUTTON_PRESSED);
						sendButton->setTextColor(TEXTCOLOR);
						sendButton->setEventListener(dynamic_cast<customButtonEventSink*>(this));

						hr = sendButton->Create();
						if (SUCCEEDED(hr))
						{
							pos.x = 210;

							receiveButton =
								new CustomButton(this->parentWnd, BUTTONMODE_ICONTEXT, &pos, &size, CTRLID_RECEIVEBUTTON, this->hInstance);

							hr = (receiveButton != nullptr) ? S_OK : E_FAIL;
							if (SUCCEEDED(hr))
							{
								ctrlText.Replace(L"Empfangen");

								receiveButton->setAppearance_IconText(IDI_RECEIVE, 42, ctrlText);
								receiveButton->setBorder(TRUE, OUTLINECOLOR);
								receiveButton->setColors(BUTTONCOLOR, COLOR_BUTTON_MOUSE_OVER, COLOR_BUTTON_PRESSED);
								receiveButton->setTextColor(TEXTCOLOR);
								receiveButton->setEventListener(dynamic_cast<customButtonEventSink*>(this));

								hr = receiveButton->Create();
								if (SUCCEEDED(hr))
								{
									pos.x = 30;
									pos.y = 350;
									size.cx = 340;
									size.cy = 30;


									this->infoTextField = new Textfield();
									
									hr = (infoTextField != nullptr) ? S_OK : E_FAIL;
									if (SUCCEEDED(hr))
									{
										infoTextField->setColors(TEXTCOLOR, BACKGROUNDCOLOR);
										infoTextField->setText(DEFAULT_INFO_TEXT);
										infoTextField->setTextfieldId(CTRLID_DATATRANSFERINFOTXT);
										infoTextField->setFont(this->clientFont);
										infoTextField->setCreationFlags(Textfield::CF_USEFIXEDSIZE);

										hr = infoTextField->Create(this->hInstance, this->parentWnd, &pos, &size);

										if (SUCCEEDED(hr))
										{
											// ...
										}
									}

								}
							}

						}
					}

				}
			}
		}
	}
	return S_OK;
}

LRESULT ConfigProperty::onResizeWindow(HWND hWndMain)
{
	RECT rcMain, rcConfig;
	GetClientRect(hWndMain, &rcMain);
	GetClientRect(this->configWnd, &rcConfig);

	MoveWindow(
		this->editBox,
		rcConfig.right, 30,
		rcMain.right - rcConfig.right,
		rcMain.bottom - 30,
		TRUE
	);
	return static_cast<LRESULT>(0);
}

LRESULT ConfigProperty::onMainWndCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == EN_CHANGE)
	{
		// editbox content changed
		this->util_updateContentHolder();
	}

	int wmId = LOWORD(wParam);

	// Menüauswahl analysieren:
	switch (wmId)
	{
		case IDM_OPEN:
			this->onOpenFile();
			break;
		case IDM_SAVEAS:
			this->onSaveFile();
			break;
		case IDM_SEND:
			this->onSendData();
			break;
		case IDM_RECEIVE:
			this->onReceiveData();
			break;
		case IDM_ABOUT:
			DialogBox(this->hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, ConfigProperty::About);
			break;
		case IDM_EXIT:
			this->onClose();
			break;
		default:
			return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
	}
	return static_cast<LRESULT>(0);
}

LRESULT ConfigProperty::onDestroy()
{
	if(this->currentLoadedFileBuffer.GetLength() > 0)
	{ 
		iString savePath(
			GetGlobalAppPath()
		);

		savePath.Append(L"\\temp_content.dat");

		auto bfpo = CreateBasicFPO();
		if (bfpo != nullptr)
		{
			bfpo->SaveBufferToFileAsUtf8(this->currentLoadedFileBuffer.GetData(), savePath.GetData());

			bfpo->Release();
		}
	}

	// terminate message loop
	PostQuitMessage(0);

	return static_cast<LRESULT>(0);
}

LRESULT ConfigProperty::onClose()
{
	bool destroy = true;

	if (this->serialComm != nullptr)
	{
		if (this->serialComm->HasActiveTransmission())
		{
			this->currentTransmissionMode = CLSE_ONTERMINATE;
			this->serialComm->TerminateTransmission();
			destroy = false;
		}
	}
	if (destroy)
	{
		DestroyWindow(this->parentWnd);
	}
	return static_cast<LRESULT>(0);
}

LRESULT ConfigProperty::onExternalDestroy(HWND hWnd)
{
	DestroyWindow(hWnd);
	return static_cast<LRESULT>(0);
}

void ConfigProperty::registerConfigWndClass()
{
	WNDCLASSEX wcx;

	if (!GetClassInfoEx(this->hInstance, SETUP_PAGE_CLASS, &wcx))
	{
		wcx.cbSize = sizeof(WNDCLASSEX);
		wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = sizeof(LONG_PTR);
		wcx.lpfnWndProc = ConfigProperty::PageProc;
		wcx.hInstance = this->hInstance;
		wcx.hIcon = nullptr;
		wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcx.hbrBackground = nullptr;
		wcx.lpszClassName = SETUP_PAGE_CLASS;
		wcx.lpszMenuName = nullptr;
		wcx.hIconSm = nullptr;

		RegisterClassEx(&wcx);
	}
}

LRESULT CALLBACK ConfigProperty::PageProc(HWND page, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ConfigProperty* pProp = nullptr;

	if (msg == WM_CREATE)
	{
		auto pcr = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pProp = reinterpret_cast<ConfigProperty*>(pcr->lpCreateParams);
		SetWindowLongPtr(page, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pProp));
		return 1;
	}
	else
	{
		pProp = reinterpret_cast<ConfigProperty*>(GetWindowLongPtr(page, GWLP_USERDATA));

		if (pProp != nullptr)
		{
			switch (msg)
			{
				case WM_PAINT:
					return pProp->onConfigWndPaint(page);
				case WM_GETWNDINSTANCE:
					return reinterpret_cast<LRESULT>(pProp);
				default:
					return DefWindowProc(page, msg, wParam, lParam);
			}
		}
		return DefWindowProc(page, msg, wParam, lParam);
	}
}

LRESULT ConfigProperty::onConfigWndPaint(HWND hWnd)
{
	HDC hdc;
	RECT rc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);
	if (hdc)
	{
		GetClientRect(hWnd, &rc);

		HBRUSH bkgnd = CreateSolidBrush(BACKGROUNDCOLOR);
		if (bkgnd)
		{
			FillRect(hdc, &rc, bkgnd);

			HGDIOBJ original;
			HPEN pen;

			TextOutDC(
				hdc,
				L"Schnittstellenkonfiguration",
				20,
				20,
				this->clientFont, TEXTCOLOR
			);
			TextOutDC(
				hdc,
				L"Baudrate",
				60,
				92,
				this->clientFont, TEXTCOLOR
			);
			TextOutDC(
				hdc,
				L"Parität",
				60,
				122,
				this->clientFont, TEXTCOLOR
			);
			TextOutDC(
				hdc,
				L"Datenbits",
				60,
				152,
				this->clientFont, TEXTCOLOR
			);
			TextOutDC(
				hdc,
				L"Stoppbits",
				60,
				182,
				this->clientFont, TEXTCOLOR
			);
			pen = CreatePen(PS_SOLID, 1, TEXTCOLOR);
			if (pen)
			{
				original = SelectObject(hdc, pen);

				MoveToEx(hdc, 20, 40, nullptr);
				LineTo(hdc, 390, 40);

				MoveToEx(hdc, 60, 88, nullptr);
				LineTo(hdc, 370, 88);

				MoveToEx(hdc, 60, 208, nullptr);
				LineTo(hdc, 370, 208);

				SelectObject(hdc, original);
				DeleteObject(pen);
			}
			DeleteObject(bkgnd);
		}
		EndPaint(hWnd, &ps);
	}
	return static_cast<LRESULT>(0);
}

LRESULT ConfigProperty::onMainWndPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	RECT rc;
	HDC hdc = BeginPaint(hWnd, &ps);

	HBRUSH brush = CreateSolidBrush(BACKGROUNDCOLOR);
	if (brush != nullptr)
	{
		GetClientRect(hWnd, &rc);
		FillRect(hdc, &rc, brush);

		auto hPen = CreatePen(PS_SOLID, 1, OUTLINECOLOR);
		if (hPen)
		{
			auto hgdiobj =
				SelectObject(hdc, this->clientFont);
			
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, TEXTCOLOR);

			TextOut(hdc, 20, 240, L"Datentransfer", 13);

			SelectObject(hdc, hPen);

			MoveToEx(hdc, 20, 260, nullptr);
			LineTo(hdc, 390, 260);

			SelectObject(hdc, hgdiobj);

			DeleteObject(hPen);
		}
		DeleteObject(brush);
	}
	EndPaint(hWnd, &ps);

	return static_cast<LRESULT>(0);
}

void ConfigProperty::onComboSelChange(comboBox* cBox, int selIndex)
{
	if (cBox != nullptr)
	{
		iString path(
			GetGlobalAppPath()
		);
		path.Append(L"\\comset.dat");

		SerialComm dTraf;
		SERIAL_CONFIG config;

		dTraf.getConfiguration(path.GetData(), &config);

		auto ctrlID = cBox->getCtrlId();

		switch (ctrlID)
		{
		case CTRLID_BAUDRATECOMBO:
			config.baud_index = selIndex;
			break;
		case CTRLID_PARITYCOMBO:
			config.parity_index = selIndex;
			break;
		case CTRLID_STOPBITSCOMBO:
			config.stopbit_index = selIndex;
			break;
		case CTRLID_DATABITSCOMBO:
			config.databit_index = selIndex;
			break;
		default:
			break;
		}
		dTraf.setConfiguration(path.GetData(), &config);
	}
}

void ConfigProperty::onEditContentChanged(singleLineEdit* edit, CTRLID ctrlID)
{
	if (ctrlID == CTRLID_COMEDIT)
	{
		if (!edit->isContentInvalid())
		{
			auto content =
				edit->getContent();

			if (content.GetLength() > 0)
			{
				iString path(
					GetGlobalAppPath()
				);
				path.Append(L"\\comset.dat");

				SerialComm dTraf;
				SERIAL_CONFIG config;

				dTraf.getConfiguration(path.GetData(), &config);

				auto comPort = _wtoi(content.GetData());

				config.Active_port = comPort;

				dTraf.setConfiguration(path.GetData(), &config);
			}
		}
	}
}

void ConfigProperty::onButtonClick(CustomButton* button, CTRLID ctrlID)
{
	UNREFERENCED_PARAMETER(button);

	switch(ctrlID)
	{
	case CTRLID_OPENBUTTON:
		this->onOpenFile();
		break;
	case CTRLID_SAVEBUTTON:
		this->onSaveFile();
		break;
	case CTRLID_SENDBUTTON:
		this->onSendData();
		break;
	case CTRLID_RECEIVEBUTTON:
		this->onReceiveData();
		break;
	default:
		break;
	}
}

void ConfigProperty::onOpenFile()
{
	auto bfpo = CreateBasicFPO();
	if (bfpo != nullptr)
	{
		TCHAR* pathBuffer = new TCHAR[2048];

		if (pathBuffer != nullptr)
		{
			SecureZeroMemory(pathBuffer, (2048) * sizeof(TCHAR));

			OPENFILENAME ofn;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = this->parentWnd;
			ofn.hInstance = this->hInstance;
			ofn.lpstrFilter = nullptr;
			ofn.lpstrCustomFilter = nullptr;
			ofn.nMaxCustFilter = 0;
			ofn.nFilterIndex = 0;
			ofn.lpstrFile = pathBuffer;
			ofn.nMaxFile = 2047;
			ofn.lpstrFileTitle = nullptr;
			ofn.lpstrInitialDir = nullptr;
			ofn.lpstrTitle = L"Datei öffnen";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
			ofn.nFileOffset = 0;
			ofn.nFileExtension = 0;
			ofn.lpstrDefExt = nullptr;

			if (GetOpenFileName(&ofn) != FALSE)
			{
				if (ofn.lpstrFile != nullptr)
				{
					TCHAR* buffer = nullptr;

					if (bfpo->LoadBufferFmFileAsUtf8(&buffer, ofn.lpstrFile))
					{
						this->currentLoadedFileBuffer = buffer;

						this->util_convertForEditboxConformity(&buffer);
						SetWindowText(this->editBox, buffer);

						SafeDeleteArray(&buffer);
					}
				}
			}
			SafeDeleteArray(&pathBuffer);
		}
		bfpo->Release();
	}
}

void ConfigProperty::onSaveFile()
{
	auto bfpo = CreateBasicFPO();
	if (bfpo != nullptr)
	{
		TCHAR *pathBuffer = new TCHAR[2048];

		if (pathBuffer != nullptr)
		{
			SecureZeroMemory(pathBuffer, (2048) * sizeof(TCHAR));
			StringCbCopy(
				pathBuffer,
				sizeof(TCHAR) * (size_t)_lengthOfString(DEFAULT_FILE_NAME),
				DEFAULT_FILE_NAME
			);

			OPENFILENAME ofn;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = this->parentWnd;
			ofn.hInstance = this->hInstance;
			ofn.lpstrFilter = EXTENSION_FILTER;
			ofn.lpstrCustomFilter = nullptr;
			ofn.nMaxCustFilter = 0;
			ofn.nFilterIndex = 0;
			ofn.lpstrFile = pathBuffer;
			ofn.nMaxFile = 2047;
			ofn.lpstrFileTitle = nullptr;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = nullptr;
			ofn.lpstrTitle = L"Datei öffnen\0";
			ofn.Flags = OFN_EXPLORER;
			ofn.nFileOffset = 0;
			ofn.nFileExtension = 0;
			ofn.lpstrDefExt = nullptr;

			if (GetSaveFileName(&ofn) != FALSE)
			{
				if (ofn.lpstrFile != nullptr)
				{
					if (bfpo->SaveBufferToFileAsUtf8(
						this->currentLoadedFileBuffer.GetData(),
						ofn.lpstrFile))
					{
						// ...
					}
				}
			}
			SafeDeleteArray(&pathBuffer);
		}
		bfpo->Release();
	}
}

void ConfigProperty::onSendData()
{
	if (this->currentTransmissionMode == NO_TRANSMSN)
	{
		iString path(
			GetGlobalAppPath()
		);
		path.Append(L"\\comset.dat");

		this->serialComm->SetTransmissionEventProtocolHandler(dynamic_cast<ITransmissionReportProtocol*>(this));

		auto hr = this->serialComm->InitDataTransmission(MODE_SEND, this->currentLoadedFileBuffer.GetData());
		if (SUCCEEDED(hr))
		{
			// ...
		}
	}
	else
	{
		// transmission is active
			//-> so this must be a cancel request
		this->serialComm->CancelTransmission();
	}
}

void ConfigProperty::onReceiveData()
{
	if (this->currentTransmissionMode == NO_TRANSMSN)
	{
		iString path(
			GetGlobalAppPath()
		);
		path.Append(L"\\comset.dat");

		this->serialComm->SetTransmissionEventProtocolHandler(dynamic_cast<ITransmissionReportProtocol*>(this));

		auto hr = this->serialComm->InitDataTransmission(MODE_RECEIVE, nullptr);
		if (SUCCEEDED(hr))
		{
			// ...
		}
	}
	else
	{
		// transmission is active
			//-> so this must be a cancel request
		this->serialComm->CancelTransmission();
	}
}

void ConfigProperty::util_convertForEditboxConformity(LPPTSTR buffer)
{
	if (buffer != nullptr)
	{
		auto pBuffer = *buffer;
		if (pBuffer != nullptr)
		{
			auto len = _lengthOfString(pBuffer);
			if (len > 0)
			{
				int extraChars = 0;

				for (int i = 0; i < len; i++)
				{
					if ((pBuffer[i] == L'\r') && (pBuffer[i + 1] != L'\n'))
					{
						extraChars++;
					}
				}

				if (extraChars > 0)
				{
					TCHAR* newBuffer = new TCHAR[(len + extraChars + 1)];
					if (newBuffer != nullptr)
					{
						auto newBufCounter = 0;

						for (int i = 0; i < len; i++)
						{
							newBuffer[newBufCounter] = pBuffer[i];

							if ((pBuffer[i] == L'\r') && (pBuffer[i + 1] != L'\n'))
							{
								newBufCounter++;
								newBuffer[newBufCounter] = L'\n';
							}
							newBufCounter++;
						}
						newBuffer[newBufCounter] = L'\0';

						delete[] *buffer;
						*buffer = newBuffer;
					}
				}
			}
		}
	}
}

void ConfigProperty::util_updateContentHolder()
{
	auto len = GetWindowTextLength(this->editBox);
	if (len > 0)
	{
		TCHAR* buffer = new TCHAR[len + 2];
		if (buffer != nullptr)
		{
			if (GetWindowText(this->editBox, buffer, len + 1) != 0)
			{
				this->currentLoadedFileBuffer.Replace(buffer);
			}
			SafeDeleteArray(&buffer);
		}
	}
}

void ConfigProperty::util_normalizeButtons()
{
		iString txt(L"Senden");
		this->sendButton->setAppearance_IconText(IDI_SEND, 42, txt);
		this->sendButton->Update();
		txt = L"Empfangen";
		this->receiveButton->setAppearance_IconText(IDI_RECEIVE, 42, txt);
		this->receiveButton->Update();
}

void ConfigProperty::onTransmissionStart(int mode)
{
	this->currentTransmissionMode = mode;

	if (mode == MODE_SEND)
	{
		iString txt(L"Abbrechen");
		this->sendButton->setAppearance_IconText(IDI_CANCEL, 48, txt);
		this->sendButton->Update();
	}
	else if (mode == MODE_RECEIVE)
	{
		iString txt(L"Abbrechen");
		this->receiveButton->setAppearance_IconText(IDI_CANCEL, 48, txt);
		this->receiveButton->Update();
	}
}

void ConfigProperty::transmissionProgress(int currentProgress, int fullDataSize)
{
	UNREFERENCED_PARAMETER(fullDataSize);

	iString txtDsp(L"Aktiv: ");
	if (this->currentTransmissionMode == MODE_SEND)
		txtDsp += L"Senden ";
	else if (this->currentTransmissionMode == MODE_RECEIVE)
		txtDsp += L"Empfangen ";

	txtDsp += currentProgress;
	txtDsp += L"Bytes";

	this->infoTextField->setText(txtDsp);
}

void ConfigProperty::onTransmissionComplete(LPCTSTR data)
{
	if (this->currentTransmissionMode == MODE_RECEIVE)
	{
		if (data != nullptr)
		{
			this->currentLoadedFileBuffer = data;
			SetWindowText(this->editBox, data);
		}
	}
	else if (this->currentTransmissionMode == CLSE_ONTERMINATE)
	{
		_DESTROYMAINWINDOW;
		return;
	}

	this->currentTransmissionMode = NO_TRANSMSN;
	this->infoTextField->setText(DEFAULT_INFO_TEXT);
	this->util_normalizeButtons();
}

void ConfigProperty::onTransmissionInterrupt(LPCTSTR data)
{
	if (this->currentTransmissionMode == MODE_RECEIVE)
	{
		if (data != nullptr)
		{
			if (this->serialComm != nullptr)
			{
				if (this->serialComm->GetTransmissionDataLength() > 0)
				{
					auto res =
						MessageBox(this->parentWnd, L"Die Übertragung wurde unterbrochen.\nSollen die bereits empfangenen Daten geladen werden?", L"Interrupt", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON1);

					if (res == IDYES)
					{
						this->currentLoadedFileBuffer = data;
						SetWindowText(this->editBox, data);
					}
				}
			}
		}
	}
	else if (this->currentTransmissionMode == CLSE_ONTERMINATE)
	{
		_DESTROYMAINWINDOW;
		return;
	}
	this->currentTransmissionMode = NO_TRANSMSN;
	this->infoTextField->setText(DEFAULT_INFO_TEXT);
	this->util_normalizeButtons();
}

HRESULT ConfigProperty::createConfigControls(LPRECT frameRect)
{
	HRESULT hr;

	this->configWnd = CreateWindow(
		SETUP_PAGE_CLASS,
		nullptr,
		WS_CHILD | WS_VISIBLE,
		frameRect->left,
		frameRect->top,
		frameRect->right,
		frameRect->bottom,
		this->parentWnd,
		reinterpret_cast<HMENU>(PAGE_ID_EXCHANGE),
		this->hInstance,
		reinterpret_cast<LPVOID>(this)
	);

	hr = (this->configWnd != nullptr) ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		RECT rc;
		GetClientRect(this->configWnd, &rc);

		this->editChangeBlockerEnabled = true;
		
		iString path(
			GetGlobalAppPath()
		);
		path.Append(L"\\comset.dat");

		SerialComm dTraf;
		SERIAL_CONFIG config;

		dTraf.getConfiguration(path.GetData(), &config);

		auto comEdit = new singleLineEdit(this->hInstance);

		hr = (comEdit != nullptr) ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			iString com(L"Schnittstelle COM");

			comEdit->setEventHandler(dynamic_cast<singleLineEditEventSink*>(this));
			comEdit->setType(
				SLE_TYPE_WITHDESCRIPTION,
				200
			);
			comEdit->setDescriptionText(com);
			comEdit->setEditFontProperties(
				iString(APPLICATION_PRIMARY_FONT),
				14,
				22
			);
			comEdit->setCtrlFont(
				CreateCFont(18, FW_MEDIUM, APPLICATION_PRIMARY_FONT)
			);
			comEdit->setBorder(true, TEXTCOLOR);
			comEdit->setAdditionalEditStyles(ES_CENTER);
			comEdit->setAlignment(DESC_ALIGN_RIGHT);
			comEdit->setSpacing(20);
			comEdit->setDimensions(20, 50, 250);
			comEdit->setColors(BACKGROUNDCOLOR, ACCENTCOLOR, TEXTCOLOR, TEXTCOLOR);
			comEdit->restrictedContent(SLE_RESTRICTTYPE_ONLYNUMBERS);
			comEdit->setValidContentRange(1, 1024);
			comEdit->setExtendedFlag(SLE_EX_NOEVENT_ON_INVALIDCONTENT);

			hr = comEdit->Create(this->configWnd, CTRLID_COMEDIT);
			if (SUCCEEDED(hr))
			{
				auto comPort = iString::FromInt(config.Active_port);
				comEdit->setContent(comPort->GetData());
				comPort->Release();

				auto BaudCombo =
					new comboBox(
						this->hInstance,
						this->configWnd,
						COMBOTYPE_DROPDOWNLIST,
						CTRLID_BAUDRATECOMBO,
						220,
						90,
						150,
						0
					);
				if (BaudCombo != nullptr)
				{
					for (int i = 0; i < BAUDRATE_STRUCTSIZE; i++)
					{
						auto str = iString::FromInt(_baud[i]);

						BaudCombo->Items->AddItem(str);
						SafeRelease(&str);
					}
					BaudCombo->setSelectedIndex(config.baud_index);
					BaudCombo->setEventHandler(dynamic_cast<comboBoxEventSink*>(this));
					BaudCombo->setFont(
						CreateCFont(18, FW_MEDIUM, APPLICATION_PRIMARY_FONT)
					);
				}
				else
					hr = E_FAIL;

				if (SUCCEEDED(hr))
				{
					auto ParityCombo =
						new comboBox(
							this->hInstance,
							this->configWnd,
							COMBOTYPE_DROPDOWNLIST,
							CTRLID_PARITYCOMBO,
							220,
							120,
							150,
							0
						);
					if (ParityCombo != nullptr)
					{
						iString str(L"Keine");
						ParityCombo->Items->AddItem(str);
						str.Replace(L"Ungerade");
						ParityCombo->Items->AddItem(str);
						str.Replace(L"Gerade");
						ParityCombo->Items->AddItem(str);
						str.Replace(L"Markierung");
						ParityCombo->Items->AddItem(str);
						str.Replace(L"Leerzeichen");
						ParityCombo->Items->AddItem(str);
						
						ParityCombo->setSelectedIndex(config.parity_index);
						ParityCombo->setEventHandler(dynamic_cast<comboBoxEventSink*>(this));
						ParityCombo->setFont(
							CreateCFont(18, FW_MEDIUM, APPLICATION_PRIMARY_FONT)
						);
					}
					else
						hr = E_FAIL;

					if (SUCCEEDED(hr))
					{
						auto DatabitCombo =
							new comboBox(
								this->hInstance,
								this->configWnd,
								COMBOTYPE_DROPDOWNLIST,
								CTRLID_DATABITSCOMBO,
								220,
								150,
								150,
								0
							);
						if (DatabitCombo != nullptr)
						{
							for (int i = 0; i < 4; i++)
							{
								auto str1 = iString::FromInt(i + 5);
								str1->Append(L" bits\0");

								DatabitCombo->Items->AddItem(str1);

								SafeRelease(&str1);
							}
							DatabitCombo->setSelectedIndex(config.databit_index);
							DatabitCombo->setEventHandler(dynamic_cast<comboBoxEventSink*>(this));
							DatabitCombo->setFont(
								CreateCFont(18, FW_MEDIUM, APPLICATION_PRIMARY_FONT)
							);
						}
						else
							hr = E_FAIL;

						if (SUCCEEDED(hr))
						{
							auto StopbitCombo =
								new comboBox(
									this->hInstance,
									this->configWnd,
									COMBOTYPE_DROPDOWNLIST,
									CTRLID_STOPBITSCOMBO,
									220,
									180,
									150,
									0
								);
							if (StopbitCombo != nullptr)
							{
								iString oneSb(L"1 bit\0");
								StopbitCombo->Items->AddItem(oneSb);
								iString onePointFiveSb(L"1.5 bits\0");
								StopbitCombo->Items->AddItem(onePointFiveSb);
								iString twoSb(L"2 bits\0");
								StopbitCombo->Items->AddItem(twoSb);

								StopbitCombo->setSelectedIndex(config.stopbit_index);
								StopbitCombo->setEventHandler(dynamic_cast<comboBoxEventSink*>(this));
								StopbitCombo->setFont(
									CreateCFont(18, FW_MEDIUM, APPLICATION_PRIMARY_FONT)
								);
							}
							else
								hr = E_FAIL;

							if (SUCCEEDED(hr))
							{
								// ....
							}
						}
					}
				}
			}
		}
	}
	return hr;
}

// Meldungshandler für Infofeld.
INT_PTR CALLBACK ConfigProperty::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

