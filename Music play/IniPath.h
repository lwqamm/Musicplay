#pragma once
#define INIPATH(X) inipa(X)
/*取运行目录*/
CString QuYunxunmulu();//取运行目录

VOID Write_config(CString str, CString maiStr);

CString inipa(int veal);

CString Read_config(CString lpAppName);

BOOL FreeMyResource(UINT uiResouceName, char* lpszResourceType, char* lpszSaveFileName);
