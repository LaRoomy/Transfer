#pragma once
#include"BasicFPO.h"
#include<new>

#define		EMPTY_DIR		5

class BasicFileNPathOperations : public BasicFPO
{
public:
	BasicFileNPathOperations();

	 void Release() { delete this; }
	 BOOL SaveBufferToFile(LPCTSTR buffer, LPCTSTR path) { return this->pSaveBuffer(buffer, path, CP_ACP); }
	 BOOL LoadBufferFmFile(TCHAR** buffer, LPCTSTR path) { return this->pLoadBuffer(buffer, path, CP_ACP); }
	 BOOL SaveBufferToFileAsUtf8(LPCTSTR buffer, LPCTSTR path){ return this->pSaveBuffer(buffer, path, CP_UTF8); }
	 BOOL LoadBufferFmFileAsUtf8(TCHAR** buffer, LPCTSTR path){ return this->pLoadBuffer(buffer, path, CP_UTF8); }
#ifndef COMPILE_FOR_WIN_XP
	 BOOL GetKnownFolderPath(TCHAR** path, REFKNOWNFOLDERID ID) { return this->pGetKnownFolder(path, ID); }
#endif //COMPILE_FOR_WIN_XP
	 BOOL RemoveFilenameFromPath(_Inout_ LPTSTR path) { return this->pRemoveFilename(path); }
	 BOOL RemoveFilenameFromPath(_In_ LPCTSTR path, _Outptr_ TCHAR** newPath_out) { return this->pRemoveFilename(path, newPath_out); }
	 BOOL GetFilenameOutOfPath(LPCTSTR path, TCHAR** filename_out, BOOL hideFileExt) {return this->pGetFilenameOutOfPath(path, filename_out, hideFileExt);}
	 BOOL IfFileExistsChangePath(TCHAR** path) { return this->pIfFileExistsChangePath(path); }
	 BOOL RemoveFileExtension(LPTSTR string, TCHAR** extension_out) { return this->pRemoveFileExt(string, extension_out); }
	 BOOL GetFileExtension(LPCTSTR string, TCHAR** extension_out) { return this->pGetFileExt(string, extension_out); }
	 BOOL CountDirectoryContent(LPCTSTR path, DWORD* files, DWORD* folders) { return this->pCountDirContent(path, files, folders); }
	 BOOL CheckForFileExist(LPCTSTR path) { return this->pCheckForFileExist(path); }
	 BOOL VerifyCommandline(LPCTSTR nCmdLine, TCHAR** Path_out) { return this->pVerifyCmdLine(nCmdLine, Path_out); }
	 BOOL VerifyFilename(LPCTSTR fileName) { return this->pVerifyFilename(fileName); }
	 BOOL GetFileTimes(LPCTSTR path, LPSYSTEMTIME created, LPSYSTEMTIME lastAccessed, LPSYSTEMTIME lastWritten);
	 BOOL SetFileTimes(LPCTSTR path, LPSYSTEMTIME created, LPSYSTEMTIME lastAccessed, LPSYSTEMTIME lastWritten);

private:
	BOOL pSaveBuffer(LPCTSTR, LPCTSTR, UINT);
	BOOL pLoadBuffer(TCHAR**, LPCTSTR, UINT);
#ifndef COMPILE_FOR_WIN_XP
	BOOL pGetKnownFolder(TCHAR**, REFKNOWNFOLDERID);
#endif //COMPILE_FOR_WIN_XP
	BOOL pRemoveFilename(LPTSTR);
	BOOL pRemoveFilename(LPCTSTR, TCHAR**);
	BOOL pGetFilenameOutOfPath(LPCTSTR, TCHAR**, BOOL);
	BOOL pIfFileExistsChangePath(TCHAR**);
	BOOL pRemoveFileExt(LPTSTR, TCHAR**);
	BOOL pGetFileExt(LPCTSTR, TCHAR**);
	BOOL pCountDirContent(LPCTSTR, DWORD*, DWORD*);
	BOOL pCheckForFileExist(LPCTSTR);
	BOOL pVerifyCmdLine(LPCTSTR, TCHAR**);
	BOOL pVerifyFilename(LPCTSTR);

	HRESULT CountNextLevel(HANDLE,LPWIN32_FIND_DATA,TCHAR*,int&,DWORD*,DWORD*);
	BOOL RemoveWildcard(TCHAR*);
};

BasicFPO* CreateBasicFPO() { return new BasicFileNPathOperations(); }

