
#include "pch.h"
#include "IniPath.h"


CString g_str;
/*ȡ����Ŀ¼*/
CString QuYunxunmulu()//ȡ����Ŀ¼
{

	CString strPath;
	char strBuff[512];
	GetCurrentDirectory(512, strBuff);//ȡ����Ŀ¼
	strPath.Format("%s", strBuff);
	return strPath;
}
/*����:���������ļ�
����1:Ҫд�����ֵ
����2:ini·��
����3:ini����ֵ
����4:iniKey
*/
VOID Write_config(CString str,  CString maiStr)
{

	//��D��д��������ݿ�����ini�ļ���Ϣ��Ĭ����������
	CString Id = "����";
	CString iniPath = INIPATH(1);
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����
	BOOL ifFind = finder.FindFile(iniPath);
	if (!ifFind)
	{
		::WritePrivateProfileString(maiStr, Id, str, iniPath);
	}
	//iniPath = QuYunxunmulu() + iniPath;
	::WriteProfileString(maiStr, Id, str);
	::WritePrivateProfileString(maiStr, Id, str, iniPath);

}

CString inipa(int veal)
{


	CString iniPath;
	if (iniPath == "")
	{
		g_str = QuYunxunmulu();
	}
	iniPath = g_str;

	CString iniexe = ".\\plug.ini";
	if (veal==2)
	{
		iniPath = iniPath + "\\plug\\temp";
	}
	else
	{
		iniPath = iniPath + "\\plug";
	}
	
	if (!PathIsDirectory(iniPath.GetBuffer()))  // �Ƿ��������ļ���
	{
		iniPath.ReleaseBuffer();
		::CreateDirectory(iniPath.GetBuffer(), 0);
	}
	iniPath.ReleaseBuffer();
	if (veal == 0|| veal == 2)
	{
		if (veal==2)
		{
			return iniPath + "\\temp1.mp3";
		}
		return iniPath;
	}
	return iniPath + iniexe;


}
/*���������ļ��ַ���*/
CString Read_config(CString lpAppName)
{

	CString lpKeyName = "����";
	CString strStudName;
	char utbuff[512] = { 0 };
	//GetProfileSection();
	::GetPrivateProfileString(lpAppName.GetBuffer(), lpKeyName.GetBuffer(), NULL,
		utbuff, sizeof(utbuff), INIPATH(1));
	lpAppName.ReleaseBuffer();
	lpKeyName.ReleaseBuffer();
	strStudName.Format("%s", utbuff);
	return strStudName;

}
// CPeProcApp ��ʼ��

/*
���ܣ�����Դ�����ļ�
����1����ԴID
����2����Դ����
����3�������ļ���ȫ·��
*/
BOOL FreeMyResource(UINT uiResouceName, char* lpszResourceType, char* lpszSaveFileName)
{
	HRSRC hRsrc = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(uiResouceName), CString(lpszResourceType));
	//LPTSTR szBuffer = new TCHAR[1024];//�����Ƕ��建������С

	if (hRsrc == NULL)
	{
		//FreeRes_ShowError("FindResource");
		return FALSE;
	}
	DWORD dwSize = ::SizeofResource(NULL, hRsrc);
	if (0 >= dwSize)
	{
		//FreeRes_ShowError("SizeofResource");
		return FALSE;
	}

	HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		//FreeRes_ShowError("LoadResource");
		return FALSE;
	}

	LPVOID lpVoid = ::LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		//FreeRes_ShowError("LockResource");
		return FALSE;
	}


	FILE* fp = NULL;
	fopen_s(&fp, lpszSaveFileName, "wb+");
	if (NULL == fp)
	{
		//FreeRes_ShowError("LockResource");
		return FALSE;
	}
	fwrite(lpVoid, sizeof(char), dwSize, fp);
	fclose(fp);

	return TRUE;
}

/// ///////////////////////////////////

