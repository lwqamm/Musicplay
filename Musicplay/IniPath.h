#pragma once
#define INIPATH(X) inipa(X)
/*ȡ����Ŀ¼*/
CString QuYunxunmulu();//ȡ����Ŀ¼

VOID Write_config(CString str, CString maiStr);

CString inipa(int veal);

CString Read_config(CString lpAppName);

BOOL FreeMyResource(UINT uiResouceName, char* lpszResourceType, char* lpszSaveFileName);
