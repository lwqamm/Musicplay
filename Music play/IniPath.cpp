
#include "pch.h"
#include "IniPath.h"


CString g_str;
/*取运行目录*/
CString QuYunxunmulu()//取运行目录
{

	CString strPath;
	char strBuff[512];
	GetCurrentDirectory(512, strBuff);//取运行目录
	strPath.Format("%s", strBuff);
	return strPath;
}
/*功能:操作配置文件
参数1:要写入的数值
参数2:ini路径
参数3:ini主键值
参数4:iniKey
*/
VOID Write_config(CString str,  CString maiStr)
{

	//向D盘写入程序数据库连接ini文件信息，默认设置如下
	CString Id = "数据";
	CString iniPath = INIPATH(1);
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用
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
	
	if (!PathIsDirectory(iniPath.GetBuffer()))  // 是否有重名文件夹
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
/*读出配置文件字符串*/
CString Read_config(CString lpAppName)
{

	CString lpKeyName = "数据";
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
// CPeProcApp 初始化

/*
功能：从资源保存文件
参数1：资源ID
参数2：资源类型
参数3：保存文件名全路径
*/
BOOL FreeMyResource(UINT uiResouceName, char* lpszResourceType, char* lpszSaveFileName)
{
	HRSRC hRsrc = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(uiResouceName), CString(lpszResourceType));
	//LPTSTR szBuffer = new TCHAR[1024];//这里是定义缓冲区大小

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

