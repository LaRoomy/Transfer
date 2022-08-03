#pragma once
#include"framework.h"
#include"Resource.h"

#include <StringClass.h>
#include <SingleLineEdit.h>
#include <ComboBox.h>
#include <textField.h>

#include "DataExchange/DataExchange.h"

constexpr auto DEFAULT_INFO_TEXT = L"Status: Bereit\0";
constexpr auto SETUP_PAGE_CLASS = L"setuppageclass186\0";
constexpr auto DEFAULT_FILE_NAME = L"Datei1\0";
constexpr auto EXTENSION_FILTER = L"All Files\0*.*\0\0";

#define			CTRLID_COMEDIT				218
#define			CTRLID_BAUDRATECOMBO		219
#define			CTRLID_PARITYCOMBO			220
#define			CTRLID_DATABITSCOMBO		221
#define			CTRLID_STOPBITSCOMBO		222
#define			CTRLID_OPENBUTTON			223
#define			CTRLID_SAVEBUTTON			224
#define			CTRLID_SENDBUTTON			225
#define			CTRLID_RECEIVEBUTTON		226
#define			CTRLID_DATATRANSFERINFOTXT	227

#define			WNDID_EDITBOX				300

#define			NO_TRANSMSN					-1
#define			CLSE_ONTERMINATE			-2


class  ConfigProperty
	: public customButtonEventSink,
	public comboBoxEventSink,
	public singleLineEditEventSink,
	public ITransmissionReportProtocol
{
public:
	 ConfigProperty();
	~ ConfigProperty();

	void Release() {
		delete this;
	}

	HRESULT CreateConfigWindow(HINSTANCE hInst, HWND parent);
	HRESULT CreateEditBox();
	HRESULT CreateSubControls();

	LRESULT onResizeWindow(HWND hWndMain);
	LRESULT onMainWndPaint(HWND hWnd);
	LRESULT onMainWndCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
	LRESULT onDestroy();
	LRESULT onClose();
	LRESULT onExternalDestroy(HWND hWnd);

	// button eventhandler
	void onCustomButtonClick(cObject sender, CTRLID ID) {
		this->onButtonClick(
			reinterpret_cast<CustomButton*>(sender), ID);
	}

	// comboBox eventhandler
	void onComboBoxSelectionChanged(cObject sender, int selIndex) {
		this->onComboSelChange(
			reinterpret_cast<comboBox*>(sender), selIndex);
	}

	// editbox events
	void onEditboxContentChanged(cObject sender, CTRLID ctrlID) {
		this->onEditContentChanged(
			reinterpret_cast<singleLineEdit*>(sender), ctrlID);
	}
	void onTabKeyWasPressed(cObject sender, CTRLID ctrlID) {
		UNREFERENCED_PARAMETER(sender);
		UNREFERENCED_PARAMETER(ctrlID);
	}

	// transmission events
	void onTransmissionStart(int mode);
	void transmissionProgress(int currentProgress, int fullDataSize);
	void onTransmissionComplete(LPCTSTR data);
	void onTransmissionInterrupt(LPCTSTR data);

private:
	HWND configWnd;
	HWND parentWnd;
	HWND editBox;
	HINSTANCE hInstance;
	HFONT clientFont;
	Textfield* infoTextField = nullptr;
	CustomButton* sendButton = nullptr;
	CustomButton* receiveButton = nullptr;
	iString currentLoadedFileBuffer;

	SerialComm* serialComm = nullptr;

	bool editChangeBlockerEnabled;
	int currentTransmissionMode;

	HRESULT createConfigControls(LPRECT frameRect);
	void registerConfigWndClass();

	static LRESULT CALLBACK PageProc(HWND page, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT onConfigWndPaint(HWND hWnd);

	void onComboSelChange(comboBox* cBox, int selIndex);
	void onEditContentChanged(singleLineEdit* edit, CTRLID ctrlID);
	void onButtonClick(CustomButton* button, CTRLID ctrlID);

	void onOpenFile();
	void onSaveFile();
	void onSendData();
	void onReceiveData();

	void util_convertForEditboxConformity(LPPTSTR buffer);
	void util_updateContentHolder();
	void util_normalizeButtons();
};
