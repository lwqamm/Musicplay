#include "pch.h"

#include "MicPalay.h"

#include "IniPath.h"
#include <iostream>
#include <io.h>
#include <tlhelp32.h>

#include "resource.h"
// 头文件
#include <chrono>


#pragma warning(disable:4996)
#pragma comment( lib, "res/bassx64.lib" )


MicPalay* mp3this = 0;
int micza = 0;

extern   BOOL MP3Btn;           //MP3鼠标左键按下
extern   BOOL MP3BtnUP;			//MP3鼠标左键按下
extern   int Mp3Pos(int dx, int pos);//设置进度条进度
extern void mp3paly(int dx);          //设置播放按钮样式
extern void SetListColor1(int dx,int ret=TRUE);//设置播放光标
extern void Rwlist1();            //重置播放列表
extern BOOL   Paint ;          //是否在画图
extern CDC* g_dc ;             //画图对象

char* g_fileName = 0;

MicPalay::MicPalay()
{

	mp3this = this;
	Releaseplug();
	m_image.Setmyimage(0);
	CString str = INIPATH(0) + "\\data";
	if (!file_exist(str.GetBuffer()))
	{
		str.ReleaseBuffer();
		CreatFileTxt();
	}
	str.ReleaseBuffer();
	m_list_Str = GetList();
    m_listMax=SplitString(m_list_Str,"|", m_list);
	str=Read_config("当前系号");
	m_listdx = StrToInt(str);
	str = Read_config("歌曲模式");
	m_model= StrToInt(str);
	BASS_Init(-1, 44100, 0, 0, NULL);//bass初始化
	//Write_config(m_list_Str, "播放列表");
		//Write_config(str, "当前系号");
	//Write_config(str, "歌曲模式");
}

MicPalay::~MicPalay()
{
	
}
void MicPalay::SetList()
{
	CString str = INIPATH(0) + "\\data";
	//char* pszFileName = "C:\\myfile.txt";

	//https://blog.csdn.net/dldw8816/article/details/113616034
	CStdioFile myFile;

	CFileException fileException;

	if (myFile.Open(str.GetBuffer(), CFile::typeText | CFile::modeCreate | CFile::modeReadWrite), &fileException)

	{
		//str.ReleaseBuffer();
		//myFile.WriteString("第1行\n");


		//CString strOrder;

		//strOrder.Format("%d,%.3f", 66, 88.88);

		myFile.WriteString(m_list_Str);

	}

	else

	{

		TRACE("Can't open file %s,error=%u\n", str, fileException.m_cause);

	}
	myFile.Close();
}
CString MicPalay::GetList()
{
	
	CString str = INIPATH(0) + "\\data";
	CStdioFile myFile;

	CFileException fileException;

	if (myFile.Open(str.GetBuffer(), CFile::typeText | CFile::modeRead), &fileException)

	{
		str.ReleaseBuffer();
		myFile.SeekToBegin();

		CString str1;

		myFile.ReadString(str1);

		CString str2;

		myFile.ReadString(str2);

		//AfxMessageBox(str1 + str2);
		str = str1 + str2;
	}

	else

	{
		str.ReleaseBuffer();
		//TRACE("Can't open file %s,error=%u\n", pszFileName, fileException.m_cause);

	}

	myFile.Close();
	return str;
}

void MicPalay::ClearList()
{
	m_list_Str = "";
	m_listMax = 0;
	m_list.RemoveAll();
	SetList();
	Rwlist1();
	quit();
	StretchBlt();
	return;
}

int MicPalay::GetMp3dx()
{
	if (m_paly)

	{
		return m_listdx;
	}
	return -1;
}
int MicPalay::mystrlen(PVOID str)
{
	PUCHAR p = (PUCHAR)str;
	int a = 0;
	while (1)
	{
		if (p[a] == '\0' || (PVOID)(p + a) == nullptr)
		{
			return a;
		}
		a++;
	}
	return 0;
}
void MicPalay::CreatFileTxt()
{
	CString str = INIPATH(0)+"\\data";
	char* pszFileName = str.GetBuffer();
	CStdioFile myFile;
	CFileException fileException;
	if (myFile.Open(pszFileName, CFile::typeText | CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate), &fileException)
	{
		//CString strOrder;			//如果文件存在则打开，否则创建
		//strOrder.Format("%d,%.3f\t", 66, 88.88);
		///myFile.WriteString(strOrder);
		myFile.Close();
	}
	else
	{
		TRACE("Can't open file %s,error=%u\n", pszFileName, fileException.m_cause);	//异常处理
	}
	//原文链接：https ://blog.csdn.net/u012987386/article/details/71088296/
}
//寻找文件扩展名
BOOL MicPalay::myGetExtension(IN PVOID strbuff)
{
	PUCHAR p = (PUCHAR)strbuff;
	ULONG strl = mystrlen((char*)strbuff);
	char Extension[100] = {0};

	for (ULONG i = 0; i < strl; i++)
	{
		//       0123456
		//       123.mp3
		//      7
		if (p[strl - (i + 1)] == '.')
		{
			//7-(i+1)	
			if (Extension != NULL)
			{
				memcpy(Extension, p + (strl - i), 3);
			}
			break;
		}
	}
	if ( (strcmp(Extension,"MP3")==0|| strcmp(Extension, "mp3") == 0)&&
		strlen(Extension)==3)
	{
		return TRUE;
	}
	return FALSE;
}

int MicPalay::SplitString(LPCTSTR lpszStr, LPCTSTR lpszSplit, CStringArray& rArrString)
{
	rArrString.RemoveAll();
	CString szStr = lpszStr;
	szStr.TrimLeft();
	szStr.TrimRight();

	if (szStr.GetLength() == 0)
	{
		return 0;
	}

	CString szSplit = lpszSplit;

	if (szSplit.GetLength() == 0)
	{
		rArrString.Add(szStr);
		return 1;
	}

	CString s;
	int n;

	do
	{
		n = szStr.Find(szSplit);

		if (n > 0)
		{
			rArrString.Add(szStr.Left(n));
			szStr = szStr.Right(szStr.GetLength() - n - szSplit.GetLength());
			szStr.TrimLeft();
		}
		else if (n == 0)
		{
			rArrString.Add(_T(""));
			szStr = szStr.Right(szStr.GetLength() - szSplit.GetLength());
			szStr.TrimLeft();
		}
		else
		{
			if ((szStr.GetLength() > 0))
			{
				if (myGetExtension(szStr.GetBuffer()))
				{
					rArrString.Add(szStr);
				}
				szStr.ReleaseBuffer();
			}

			break;
		}
	} while (1);

	return (int)rArrString.GetSize();
}

int MicPalay::__rand()
{
	//srand(time(0)); //播撒随机种子
	int a=0, b=0;
	
	if (m_Next)
	{
		m_Next = FALSE;
		if (m_listdx<m_listMax-2)
		{
			a = m_listdx + 1;
			b = m_listMax - 1;
		}
		else if(m_listdx == m_listMax - 1)
		{
			a = 0;
			b = m_listdx - 1;
		}
		else 
		{
			a = m_listdx + 1;
			b = m_listMax - 1;
		}
	}
	else if (m_Last)
	{
		m_Last = FALSE;
		if (m_listdx==0)
		{
			a =  1;
			b = m_listMax - 1;
		}
		else if (m_listdx == 1)
		{
			a = 0;
			b = m_listMax - 1;
		}
		else
		{
			a = 0;
			b = m_listdx + 1;
		}
	}
	else 
	{

		if (m_listdx == m_listMax - 1)
		{
			a = 0;
			b = m_listdx - 1;
		}
		if (m_listdx == 0)
		{
			a = 1;
			b = m_listMax - 1;
		}
		if (m_listdx >= 1 && m_listdx < m_listMax - 2)
		{
			a = m_listdx + 1;
			b = m_listMax - 1;
		}
	}

	while (1)
	{

		int a1 = rand();
		if (a1<=b&&a1>=a)
		{
			return a1;
		}
	}

}

int MicPalay::Getlistdx(CString str)
{
	for (int i = 0; i < m_listMax; i++)
	{
		if (str ==m_list[i] )
		{
			return i;
		}
	}

	return -1;
}

CString MicPalay::Getdxlist(int dx)
{
	if (dx<0||dx>=m_listMax)
	{
		return "";
	}
	return m_list[dx];
}

int MicPalay::GetMp3String(PVOID path,int* vea)
{
	PUCHAR p = (PUCHAR)path;
	int size = strlen((char*)path);
	int ret = 0;
	for (int i = 0; i < size; i++)
	{
		if (size - (i + 1) < 0)
		{
			return -1;
		}
		if (ret == 0)
		{
			if (p[size - (i + 1)] == '.')
			{
				*(int*)vea = i;
				ret++;
			}
			//  12\345.mp3    4      7
		}
		if (ret)
		{
			if (p[size - (i + 1)] == '\\')
			{
				int bb = size - (size - i);
				ret = bb - (*(int*)vea + 1);
				*(int*)vea = ret;
				return size - i;
			}
		}
	}
	return -1;
}

CString MicPalay::Getdxliststr(int dx)
{
	char ch[MAX_PATH] = { 0 };
	int tsize = 0;
	CString str = Getdxlist(dx);
	int Min = GetMp3String(str.GetBuffer(),&tsize);
	str.ReleaseBuffer();
	if (Min == -1)
	{
		strcpy(ch, "未知歌曲");
	}
	else
	{
		memcpy(ch, str.GetBuffer() + Min, tsize);
		str.ReleaseBuffer();
	}
	str = ch;
	return str;
}

void MicPalay::DeleteList(int dx)
{
	m_list_Str = "";
	CString str = "";
	for (int i = 0; i < m_listMax; i++)
	{
		if (i==dx)
		{
			continue;
		}
		str = Getdxlist(i);
		if (m_list_Str=="")
		{
			m_list_Str += str;
			continue;
		}
		m_list_Str += "|"+str;
	}
	//Write_config(m_list_Str, "播放列表");
	SetList();
	m_listMax = 0;
	m_list.RemoveAll();
	m_listMax = SplitString(m_list_Str, "|", m_list);
	m_listdx -1;
	if (m_listdx<0)
	{
		m_listdx = 0;
	}
	Rwlist1();
	SetListColor1(m_listdx);

}

BOOL MicPalay::AddList(CString str)
{
	int  dx = Getlistdx(str);
	if (dx>=0)
	{
		return FALSE;
	}
	if (m_list_Str == "")
	{
		m_list_Str=str;
	}
	else
	{
		m_list_Str += "|" + str;
		//Write_config(m_list_Str, "播放列表");
	}
	m_listMax = SplitString(m_list_Str, "|", m_list);
	SetList();
	Rwlist1();
	
	return TRUE;
}

void MicPalay::SetHwnd(HWND hwnd)
{
	m_hwnd = hwnd;
	m_image.Setmyimage(hwnd);
}
//文件是否存在
int MicPalay::file_exist(char* str)
{

	if (access(str, 0) == NULL)
	{
		return 1;
	}
	return 0;
}

void MicPalay::Releaseplug()
{
	//CString str = "";
	//str = INIPATH(3) + "lame.exe";
	//if (!file_exist(str.GetBuffer()))  // 是否有重名文件夹
	//{
	//	str.ReleaseBuffer();
	//	FreeMyResource(IDR_EXE, "plug", str.GetBuffer());
	//	str.ReleaseBuffer();
	//	if (file_exist(str.GetBuffer()))  // 是否有重名文件夹
	//	{
	//		m_exe = TRUE;
	//	}
	//}
	//else
	//{
	//	m_exe = TRUE;
	//}
	//str.ReleaseBuffer();
	//str = "";
	//str = INIPATH(4) + "lame_enc.dll";
	//if (!file_exist(str.GetBuffer()))  // 是否有重名文件夹
	//{
	//	str.ReleaseBuffer();
	//	FreeMyResource(IDR_DLL, "plug", str.GetBuffer());
	//	str.ReleaseBuffer();
	//	if (file_exist(str.GetBuffer()))  // 是否有重名文件夹
	//	{
	//		m_dll = TRUE;
	//	}
	//}
	//else
	//{
	//	m_dll = TRUE;
	//}

}

// 进程是否存在 返回1
int MicPalay::process_exist(char* pid)
{
	int ret = 0;
	char pid_path[64] = { 0 };
	struct stat stat_buf;
	if (!pid)
		return 0;
	snprintf(pid_path, 64, "/proc/%s/status", pid);
	if (stat(pid_path, &stat_buf) == 0)
		ret = 1;
	return ret;
}

//MP3转码
BOOL MicPalay::ParseMP3(const char* mp3Path)
{
	if (!(m_exe == TRUE && m_dll == TRUE))
	{
		return FALSE;
	}

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;
	CString str;
	//str.Format(" %s %s", mp3Path, INIPATH(2).GetBuffer());
	CString str1 = INIPATH(3) + "lame.exe";
	TCHAR Name[MAX_PATH * 2] = { 0 };
	wsprintf(Name, "%s", str1.GetBuffer());
	TCHAR lpCmd[MAX_PATH * 2] = { 0 };
	wsprintf(lpCmd, " %s %s", mp3Path, INIPATH(2).GetBuffer());
	BOOL ret = CreateProcess(Name, lpCmd,
		NULL, NULL, FALSE, CREATE_NO_WINDOW,
		NULL, NULL, &si, &pi);

	if (!ret)
	{
		return FALSE;
	}
	str.ReleaseBuffer();
	//str.Format("%d",pi.dwProcessId);
	while (1)
	{

		BOOL piret = FALSE;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			return 0;
		}
		PROCESSENTRY32 pt;
		pt.dwSize = sizeof(PROCESSENTRY32); //第一次使用必须初始化成员
		BOOL bRet = Process32First(hSnapshot, &pt);
		while (bRet)
		{
			if (pt.th32ProcessID == pi.dwProcessId)
			{
				piret = TRUE;
				break;
			}
			bRet = Process32Next(hSnapshot, &pt);
		}
		CloseHandle(hSnapshot);
		if (piret == FALSE)
		{
			break;
		}
		Sleep(20);
	}
	
	if (file_exist(INIPATH(2).GetBuffer()))
	{
		return TRUE;
	}

	return FALSE;
}

void WINAPI main_1()
{
	micza = 1;
	mp3this->paly_1();
}

void MicPalay::paly(MYLISTSTR vear)
{
	if (vear.lx>= STARTPARSTR&& vear.lx<= BOTTONPARSTR)
	{
		int dx = -1;
		switch (vear.lx)
		{
		case STARTPARSTR:
		{
			
			   if (vear.MP3str.GetLength()<=6)
			   {
				   Mp3Pos(0, 0);
				   mp3paly(1);
				   return;
			   }
				dx = Getlistdx(vear.MP3str);
				if (dx == -1)
				{

					AddList(vear.MP3str);
					m_listdx = m_listMax - 1;
				}
				else if(dx>=0&&dx<= m_listMax - 1)
				{
					
					m_listdx = dx;
				}
				else
				{
					Mp3Pos(0, 0);
					mp3paly(1);
					return;
				}
				m_addres = STARTPARSTR;
				if (m_paly)
				{
					quit();
				}
				if (m_Thread > 0)
				{
					return;
				}
			break;
		}
		case SENDPARSTR:
		{
			if (vear.MP3str.GetLength() <= 6)
			{
				Mp3Pos(0, 0);
				mp3paly(1);
				return;
			}
			dx = Getlistdx(vear.MP3str);
			if (dx == -1)
			{

				AddList(vear.MP3str);
				m_listdx = m_listMax - 1;
			}
			else if (dx >= 0 && dx <= m_listMax - 1)
			{

				m_listdx = dx;
			}
			else
			{
				Mp3Pos(0, 0);
				mp3paly(1);
				return;
			}
			m_addres = STARTPARSTR;
			m_memlen = vear.memlen;
			if (m_paly)
			{
				quit();
			}
			if (m_Thread>0)
			{
				return;
			}
			break;
		}
		case LISTPARSTR:
		{
	     if (vear.dx >= 0 && vear.dx <= m_listMax - 1)
	     {

		m_listdx = vear.dx;
	     }
	    else
	    {
		Mp3Pos(0, 0);
		mp3paly(1);
		return;
	    }
        m_addres = LISTPARSTR;
	     if (m_paly)
	     {
		quit();
	     }
		 if (m_Thread > 0)
		 {
			 return;
		 }
			break;
		}
		case  BOTTONPARSTR:
		{
			if (m_paly == 2 || m_paly == 3)
			{
				if (m_pause == FALSE)
				{
					pause();
					return;
				}
				resume();
				return;
			}
			else if (m_paly == 1)
			{
				pause();
				return;
			}

			if (m_Thread > 0)
			{
				return;
			}
			break;
		}
		default:
			break;
		}
	}

	if (m_inside|| m_Thread==0)
	{
		m_Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main_1, 0, 0, 0);
		return;
	}


	return ;
}
void MicPalay::paly_1()
{
	if (micza == 0)
	{
		MessageBox(0, "非法调用！", "MP3友情提示", MB_OKCANCEL);
		return;
	}
	micza = 0;
	int dex =-1;
	m_stop = TRUE;
	m_quit = FALSE;
	while (m_stop)
	{
		m_int++;

		//有外部参数传入
		if (m_addres <= BOTTONPARSTR && m_addres >= STARTPARSTR)
		{
			m_addres = -1;
			CString str = Getdxlist(m_listdx);
			if (str.GetLength()<=6)
			{
				mp3paly(1);
				Mp3Pos(0, 0);
				return;
			}
			if (!main(str.GetBuffer()))
			{
				DeleteList(m_listdx);
			}
			continue;
		}
		if (m_int == 1 && m_listMax >= 1)
		{
			if (m_listdx>= 0&& m_listdx<=m_listMax-1)
			{
				CString str = Getdxlist(m_listdx);

				if (str.GetLength() > 6)
				{
					if (!main(str.GetBuffer()))
					{
						DeleteList(Getlistdx(m_mp3Path));
					}
					str.ReleaseBuffer();
				}

				if (m_model == 3 && !m_Last && !m_Next)
				{
					m_Thread = 0;
					m_int = 0;
					return;
				}
			}
			else
			{
				m_Thread = 0;
				m_int = 0;
				return;
			}
			Sleep(200);
			continue;
		}
		if (m_Last)//上一曲
		{
			if (m_paly)
			{
				quit();
				while (m_paly)
				{
					if (m_paly == 0)
					{
						break;
					}
					Sleep(1);
				}
			}
			
			
		}
		//下一曲
		if (m_Next) 
		{
			if (m_paly)
			{
				quit();
				while (m_paly)
				{
					if (m_paly == 0)
					{
						break;
					}
					Sleep(1);
				}
			}
			
		}
		//出错了
		if (m_listMax<=0)
		{
			mp3paly(1);
			Mp3Pos(0, 0);
			break;
		}
		int aa0 = 0;
		CString str = "";
		switch (m_model)
		{
		case 0:
		{
			//顺序播放
			if (m_Next && aa0 == 0)
			{
				aa0 = 1;
				m_Next = FALSE;
				if (m_listdx == m_listMax - 1)
				{
					dex = 0;
				}
				else
				{
					dex = m_listdx + 1;
				}
			}
			if (m_Last && aa0 == 0)
			{
				aa0 = 1;
				m_Last = FALSE;
				if (m_listdx == 0)
				{
					dex = m_listMax - 1;
				}
				else
				{
					dex = m_listdx - 1;
				}
			}
			else if (aa0 != 1)
			{
				if (m_listdx == m_listMax - 1|| m_int==1)
				{
					if (m_int == 1)
					{
						dex = m_listdx;
					}
					else
					{
						dex = 0;
					}
					
				}
				else
				{
					dex = m_listdx + 1;
				}
			}
			aa0 = 0;
			break;
		}
		case 1://随机播放
		{
			//顺序播放
			dex= __rand();
			break;
		}
		case 2://单曲循环播放
		{
			
			if (m_Next && aa0 == 0)
			{
				m_Next = FALSE;
				if (m_listdx ==m_listMax-1)
				{
					dex = 0;
				}
				else
				{
					dex = m_listdx+1;
				}
			}else if (m_Last && aa0 == 0)
			{
				aa0 = 1;
				m_Last = FALSE;
				if (m_listdx == 0)
				{
					dex = m_listMax - 1;
				}
				else
				{
					dex = m_listdx - 1;
				}
			}
			else if(aa0 != 1)
			{
				dex = m_listdx;
			}
			aa0 = 0;
			break;
		}
		case 3://单曲播放
		{
			if (m_Next && aa0 == 0)
			{
				aa0 = 1;
				m_Next = FALSE;
				if (m_listdx == m_listMax - 1)
				{
					dex = 0;
				}
				else
				{
					dex = m_listdx + 1;
				}
			}else if (m_Last&& aa0 == 0)
			{
				aa0 = 1;
				m_Last = FALSE;
				if (m_listdx == 0)
				{
					dex = m_listMax - 1;
				}
				else
				{
					dex = m_listdx - 1;
				}
			}
			else if(aa0 != 1)
			{
				dex = m_listdx;
			}
			aa0 = 0;
			if (dex >= 0 && dex <= m_listMax - 1)
			{
				str = Getdxlist(dex);

				if (str.GetLength() > 6)
				{
					if (!main(str.GetBuffer()))
					{
						DeleteList(Getlistdx(m_mp3Path));
					}
					str.ReleaseBuffer();
				}
				if (m_model == 3 && !m_Last && !m_Next)
				{
					m_Thread = 0;
					m_int = 0;
					return;
				}
			}
			else
			{
				m_Thread = 0;
				m_int = 0;
				return;
			}
			Sleep(200);
		   continue;
			break;
		}
		default:
			break;
		}
		aa0 = 0;
		if (dex >= 0 && dex <= m_listMax - 1)
		{
			str = Getdxlist(dex);
			if (str.GetLength() > 6)
			{
				if (!main(str.GetBuffer()))
				{
					DeleteList(Getlistdx(m_mp3Path));
				}
				str.ReleaseBuffer();
			}

			if (m_model == 3 && !m_Last && !m_Next)
			{
				m_Thread = 0;
				m_int = 0;
				return;
			}
		}
		else
		{
			m_Thread = 0;
			m_int = 0;
			return;
		}
		Sleep(200);
	}
	m_Thread = 0;
}
void MicPalay::Previous()
{
	if (m_listMax <= 0)
	{
		return;
	}
	MYLISTSTR vea;
	//vea.lx = BOTTONPARSTR;
	//m_addres = BOTTONPARSTR;
	if (m_Thread == 0)
	{
		m_inside = TRUE;
		paly(vea);
		m_inside = FALSE;
		return;
	}
	if (m_listMax <= 1)
	{
		return;
	}
	if (m_paly)
	{
		quit();
	}

	m_Last = TRUE;
	return;
}
void MicPalay::Next()
{
	if (m_listMax <= 0)
	{
		return;
	}
	MYLISTSTR vea;
	
	if (m_Thread == 0)
	{
		m_inside = TRUE;
		paly(vea);
		m_inside = FALSE;
		return;
	}
	if (m_listMax<=1)
	{
		return;
	}
	if (m_paly)
	{
		quit();
	}
	
	m_Next = TRUE;
	return;
}
void MicPalay::pause()
{
	m_pause = TRUE;
	mp3paly(1);
}

HANDLE MicPalay::GetThread()
{
	return m_Thread;
}

char* MicPalay::GetMp3Path()
{
	return  m_mp3Path;
}

int MicPalay::Getpause()
{
	return m_paly;
}
int MicPalay::GetListSize()
{
	return m_listMax;
}

void MicPalay::resume()
{
	m_resume = TRUE;
	mp3paly(2);
}

void MicPalay::quit()
{
	m_quit = TRUE;
}

void MicPalay::stop()
{
	if (m_paly)
	{
		quit();
		m_int = 0;
		m_stop = FALSE;
	}
	
}
void MicPalay::close()
{
	m_close = TRUE;
}

void MicPalay::VirFree(char* str, int len)
{

	VirtualFree(str, m_addres, MEM_COMMIT);	
}

void MicPalay::Setmini(BOOL rt)
{
	m_mini = rt;
	while (m_ret==FALSE)
	{
		if (rt==FALSE)
		{
			m_ret = FALSE;
			return;
		}
		Sleep(1);
	}
	m_ret = FALSE;
	return;
}

void MicPalay::SetMp3Eroo(char* str, int dx)
{
	CDC dcMem;

	CBitmap bmp;  //内存中承载临时图象的位图
	CRect rc;
	GetClientRect(m_hwnd, &rc);

	dcMem.CreateCompatibleDC(g_dc);  //依附窗口DC创建兼容内存DC
	//创建兼容位图(必须用pDC创建，否则画出的图形变成黑色)
	bmp.CreateCompatibleBitmap(g_dc, rc.Width(), rc.Height());
	CBitmap* pOldBit = dcMem.SelectObject(&bmp);
	//按原来背景填充客户区，不然会是黑色
	dcMem.FillSolidRect(rc, RGB(0xF0, 0xF0, 0xF0));
	if (dx==0)
	{
		g_dc->StretchBlt(408, 544,400,20,&dcMem, 408, 544, 400, 20,SRCCOPY);
		//将内存DC上的图象拷贝到前台
//绘图完成后的清理
		dcMem.DeleteDC();      //删除DC
		bmp.DeleteObject();  //删除位图
		//g_dc->DeleteDC();
		return;
	}
	COLORREF	tco = dcMem.SetTextColor(RGB(0xec, 0x41, 0x41));
	int a= dcMem.SetBkMode(0);
	dcMem.TextOut(408, 545, str);
	g_dc->StretchBlt(408, 544, 400, 20, &dcMem, 408, 544, 400, 20, SRCCOPY);
	//g_dc->DeleteDC();
	dcMem.SetTextColor(tco);
	dcMem.SetBkMode(a);
	dcMem.DeleteDC();      //删除DC
	bmp.DeleteObject();  //删除位图
	dcMem.DeleteDC();
	return;
}

void MicPalay::mymode(CDC* dc, POINT* pt, int cons)
{
	for (size_t i = 0; i < cons; i++)
	{
		if (pt[i].x<0|| pt[i].y < 0)
		{
			return;
		}
	}
	for (size_t i = 0; i < cons; i++)
	{
		if (i==0)
		{
			dc->MoveTo(pt[i]);
		}
		dc->LineTo(pt[i]);
	}
	return;
}

void MicPalay::SetModel(int ret)
{
	if (ret==-1)
	{
		if (m_model >= 3)
		{
			m_model = 0;
			SetModelColor();
			CString str;
			str.Format("%d", m_model);
			Write_config(str, "歌曲模式");
			return;
		}
		m_model += 1;
		SetModelColor();
		CString str;
		str.Format("%d", m_model);
		Write_config(str, "歌曲模式");
		return;
	}
	m_model = ret;
	SetModelColor();
	CString str;
	str.Format("%d", m_model);
	Write_config(str, "歌曲模式");
	return;
}

void MicPalay::SetModelColor(BOOL ret)
{
	m_modelColor = ret;
	Stretchmodel();
}

void MicPalay::Stretchmodel(int x,int y )
{
	CDC dcMem;

	CBitmap bmp;  //内存中承载临时图象的位图
	CRect rc;
	GetClientRect(m_hwnd, &rc);

	dcMem.CreateCompatibleDC(g_dc);  //依附窗口DC创建兼容内存DC
	//创建兼容位图(必须用pDC创建，否则画出的图形变成黑色)
	bmp.CreateCompatibleBitmap(g_dc, rc.Width(), rc.Height());
	CBitmap* pOldBit = dcMem.SelectObject(&bmp);
	//按原来背景填充客户区，不然会是黑色
	dcMem.FillSolidRect(rc, RGB(0xF0, 0xF0, 0xF0));
	int crColor = 0;
	COLORREF obCo = 0;
	if (m_modelColor==TRUE)
	{
		//a2e2ff
		crColor = RGB(0xa2, 0xe2, 0xff);
	}
	else
	{
		crColor = RGB(0x7c, 0x7c, 0x7c);
	}
	CPen pen(PS_SOLID, 2, crColor);
	CPen* obpen =dcMem.SelectObject(&pen);
	POINT pt[20] = {0};
	switch (m_model)
	{
	case  0:
	{
		 pt[0] = {x+18,y-13};          // 815 623== 833,610
		 pt[1] = { x+15,y-3 };           // 830,620
		 mymode(&dcMem, pt, 2);
		pt[2] = { x+20,y+2 };		//    833,625
		pt[3] = { x +24,y -2};		//    839,621
		pt[4] = { x + 28,y+1 };	//    845,624
		mymode(&dcMem, pt+2, 3);
		dcMem.Arc(x - 7, y - 8,  x + 25, y + 15,  x+17, y-6,  x+23 , y );
		//顺序播放 857,623  35c1ff =815 623
		 obCo = dcMem.SetTextColor(RGB(0x35, 0xc1, 0xff));
		dcMem.SetBkMode(0);
		dcMem.TextOut(x + 42, y + 1, "顺序播放", strlen("顺序播放"));
		dcMem.SetTextColor(obCo);
		break;
	}
	case  1:   //随机播放 857,623  35c1ff
	{
		pt[0] = { x ,y};          // 815 623== 833,610
		pt[1] = { x + 6,y };           // 830,620
		pt[2] = { x + 16,y + 15 };		//    833,625
		pt[3] = { x + 21,y +15 };		//    839,621
		pt[4] = { x + 18,y + 15 };	//    845,624
		pt[5] = { x + 18,y + 17 };	//    845,624
		mymode(&dcMem, pt , 6);
		pt[6] =        { x ,y + 15 };          // 815 623== 833,610
		pt[7] =        { x + 6,y + 15 };           // 830,620
		pt[8] =        { x + 16,y };		//    833,625
		pt[9] =        { x + 21,y  };		//    839,621
		pt[10] =       { x + 18,y };	//    845,624
		pt[11] =       { x + 18,y-3 };	//    845,624
		mymode(&dcMem, pt + 6, 6);

		//随机播放 857,623  35c1ff =815 623
		 obCo = dcMem.SetTextColor(RGB(0x35,0xc1,0xff));
		dcMem.SetBkMode(0);
		dcMem.TextOut(x + 42, y +1, "随机播放", strlen("随机播放"));
		dcMem.SetTextColor(obCo);
		break;
	}
	case  2:
	{
		pt[0] = { x + 18,y - 13 };          // 815 623== 833,610
		pt[1] = { x + 15,y - 3 };           // 830,620
		mymode(&dcMem, pt, 2);
		pt[2] = { x + 20,y + 2 };		//    833,625
		pt[3] = { x + 24,y - 2 };		//    839,621
		pt[4] = { x + 28,y + 1 };	//    845,624
		mymode(&dcMem, pt + 2, 3);

	obCo=dcMem.SetTextColor(crColor);
	    dcMem.SetBkMode(0);
		dcMem.TextOut(x+7, y-5,"1",1);
		dcMem.SetTextColor(obCo);

		dcMem.Arc(x - 7, y - 8, x + 25, y + 15, x + 17, y - 6, x + 23, y);
		//单曲循环 857,623  35c1ff =815 623
		 obCo = dcMem.SetTextColor(RGB(0x35, 0xc1, 0xff));
		dcMem.SetBkMode(0);
		dcMem.TextOut(x + 42, y + 1, "单曲循环", strlen("单曲循环"));
		dcMem.SetTextColor(obCo);
		break;
	}
	case  3:
	{
		pt[0] = { x ,y +8 };          // 815 623== 833,610
		pt[1] = { x + 20,y+8 };           // 830,62
		pt[2] = { x + 17,y + 5 };
		mymode(&dcMem, pt , 3);
		//单曲播放 857,623  35c1ff =815 623
		 obCo = dcMem.SetTextColor(RGB(0x35, 0xc1, 0xff));
		dcMem.SetBkMode(0);
		dcMem.TextOut(x + 42, y + 1, "单曲播放", strlen("单曲播放"));
		dcMem.SetTextColor(obCo);
		break;
	}
	default:
		break;
	}
	g_dc->StretchBlt(x-8, y-10, 150, 40, &dcMem, x - 8, y - 10, 150, 40, SRCCOPY);

	dcMem.SelectObject(obpen);
	dcMem.DeleteDC();      //删除DC
	bmp.DeleteObject();  //删除位图
	dcMem.DeleteDC();
}
void MicPalay::StretchBlt()
{
	if (m_paly==0&&strlen(m_mp3Path)<=6)
	{
		m_image.StretchBlt(g_dc, 15, 580, 85);
		return;
	}
	else if (m_paly != 0)
	{
		m_image.GetMp3ParSing(m_mp3Path, g_dc, 15, 580, 85);
		return;
	}
	return;
}

int asd = 0;

BOOL MicPalay::main(char* fileName)
{
	if (fileName!=NULL && strlen(fileName) <= 6&& myGetExtension(fileName))
	{	
		return FALSE;
	}
	if (eroo == 0)
	{
		memset(m_mp3Path, 0, sizeof(m_mp3Path));
		wsprintf(m_mp3Path, "%s", fileName);
	}
	else
	{
		asd = 1;
		eroo = 0;
	}

	 m_stream = BASS_StreamCreateFile(FALSE, m_mp3Path, 0, 0, BASS_SAMPLE_LOOP);
	 BASS_ChannelFlags(m_stream, 0, BASS_SAMPLE_LOOP);//设置是否重复播放
	 /*   if chkLoop.Checked then
	BASS_ChannelFlags(hs, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP)
   else
	BASS_ChannelFlags(hs, 0, BASS_SAMPLE_LOOP);
	*/
	CString str=Read_config("歌曲路径");
	if (str== m_mp3Path)
	{
		m_text0 = TRUE;
	}
	else
	{
		m_text0 = FALSE;
	}
	QWORD pos = BASS_ChannelGetLength(m_stream, BASS_POS_BYTE);
	if (m_text0)
	{
		CString sstr = Read_config("歌曲播放进度");
		if (sstr != "")
		{
			QWORD pos1 = StrToInt(sstr);
			if (pos1 >= 100 && pos1 <= pos)
			{
				/* 设置播放进度 */
				BASS_ChannelSetPosition(m_stream, pos1, BASS_POS_BYTE);
			}	
		}
	

		m_text0 = FALSE;
	}


	pos = pos / 100;

	Write_config("", "歌曲路径");
	Write_config("", "歌曲播放进度");
	Write_config("", "进度条进度");

	if (pos == 0)
	{
		
		//memset(m_mp3Path, 0, sizeof(m_mp3Path));
		return FALSE;
	}

	m_lock = TRUE;
	int a1 = 0;
	int a2 = 0;//记录上次滑块位置
	Write_config(m_mp3Path, "歌曲路径");

	if ( BASS_ChannelPlay(m_stream, FALSE))
	{
		m_lock = TRUE;
		m_paly = 1;
	}
	else
	{
		m_lock = FALSE;
		m_paly = 0;
	}
	m_image.GetMp3ParSing(m_mp3Path, g_dc, 15, 580, 85);
	
	str.Format("%d", m_listdx);
	Write_config(str, "当前系号");
	mp3paly(2);
	Mp3Pos(0, 0);
	m_lock = TRUE;
	m_paly = 1;
	//Rwlist1();
	m_listdx = Getlistdx(m_mp3Path);
	SetListColor1(m_listdx,TRUE);
	while (m_lock)
	{

		QWORD tlen = BASS_ChannelGetPosition(m_stream, BASS_POS_BYTE);
		QWORD tlen1 = tlen / pos; 
		QWORD tlen2 = tlen % pos;
		if (tlen2 >= pos / 2) { tlen = tlen1 + 1; }
		else { tlen = tlen1; }
		if (tlen > 100) { tlen = 100; }
		if (tlen > a1 )
		{
			Mp3Pos(0, tlen);
		}
		if (!m_mini && !Paint)
		{
			if (MP3Btn && a1 == 0)
			{
				a1 = Mp3Pos(1, 0);
				MP3Btn = FALSE;
			}
			if (MP3BtnUP)
			{
				tlen = Mp3Pos(1, 0);
				if (tlen > a1 || tlen < a1)
				{
					Mp3Pos(0, tlen);
					BASS_ChannelPause(m_stream);//音乐暂停
					
					BASS_ChannelSetPosition(m_stream, tlen*pos, BASS_POS_BYTE);
					
					BASS_ChannelPlay(m_stream, FALSE);
					Sleep(300);
				}
				MP3BtnUP = FALSE;
				MP3Btn = FALSE;
				a1 = 0;
			}
		}
		
		int rett = BASS_ChannelIsActive(m_stream);
	
		if (rett == BASS_ACTIVE_STOPPED)
		{
			m_lock = FALSE;
			m_paly = 0;
			BASS_ChannelStop(m_stream);
			mp3paly(1);	//AfxMessageBox("播放完毕");
			Mp3Pos(0, 0);	return TRUE;
			Write_config("0", "歌曲播放进度");
			Write_config("0", "进度条进度");
			m_Previous = m_listdx;
			SetListColor1(m_listdx, FALSE);
			BASS_StreamFree(m_stream);
			break;
		}

		if (m_mini || Paint)
		{
			if (!Paint)
			{
				m_ret = TRUE;
			}
			else
			{
				m_image.GetMp3ParSing(m_mp3Path, g_dc, 15, 580, 85);
			}
			while (m_mini || Paint)
			{
				m_paly = 3;
				if (m_pause)//暂停
				{
					//m_pause = FALSE;
					//m_resume = FALSE;
					BASS_ChannelPause(m_stream);
					break;
				}
				 rett = BASS_ChannelIsActive(m_stream);
				if (rett == BASS_ACTIVE_STOPPED)
				{
					BASS_ChannelStop(m_stream);
					m_lock = FALSE;
					m_paly = 0;
					mp3paly(1);
					Mp3Pos(0, 0);
					Write_config("0", "歌曲播放进度");
					Write_config("0", "进度条进度");
					m_Previous = m_listdx;
					SetListColor1(m_listdx, FALSE);
					BASS_StreamFree(m_stream);
					break;
				}

				if (m_resume)
				{
					
					m_pause = FALSE;
					m_resume = FALSE;
					break;
				}
				if (m_quit)
				{
					BASS_ChannelStop(m_stream);
					m_pause = FALSE;
					m_resume = FALSE;
					m_quit = FALSE;
					m_lock = FALSE;
					Mp3Pos(0, 0);
					mp3paly(1);
					a2 = tlen;
					CString str;
					str.Format("%d", tlen * pos);
					Write_config(str, "歌曲播放进度");
					str.Format("%d", tlen);
					Write_config(str, "进度条进度");
					m_Previous = m_listdx;
					SetListColor1(m_listdx, FALSE);
					BASS_StreamFree(m_stream);
					break;
				}
				if (m_close)
				{
					BASS_ChannelStop(m_stream);
					BASS_StreamFree(m_stream);
					m_pause = FALSE;
					m_resume = FALSE;
					m_quit = FALSE;
					m_lock = FALSE;
					Mp3Pos(0, 0);
					mp3paly(1);

					a2 = tlen;
					CString str;
					str.Format("%d", tlen * pos);
					Write_config(str, "歌曲播放进度");
					str.Format("%d", tlen);
					Write_config(str, "进度条进度");
					DeleteFile(INIPATH(2).GetBuffer());
					m_Previous = m_listdx;
					m_paly = 0;
					BASS_StreamFree(m_stream);
					::PostMessage(m_hwnd, WM_QUIT, 0, 0);//关闭程序
					break;
				}
				Sleep(1);
			}


		}

		if (m_pause)//暂停
		{
			m_paly = 2;
			BASS_ChannelPause(m_stream);//暂停
			while (m_pause)//暂停
			{
				if (m_mini)
				{
					break;
				}
				if (m_resume)
				{
					BASS_ChannelPlay(m_stream, 0);
					m_pause = FALSE;
					m_resume = FALSE;
					break;
				}
				if (m_quit)
				{
					BASS_ChannelStop(m_stream);
					m_pause = FALSE;
					m_resume = FALSE;
					m_quit = FALSE;
					m_lock = FALSE;
					Mp3Pos(0, 0);
					mp3paly(1);
					a2 = tlen;
					CString str;
					str.Format("%d", tlen * pos);
					Write_config(str, "歌曲播放进度");
					str.Format("%d", tlen);
					Write_config(str, "进度条进度");
					m_Previous = m_listdx;
					SetListColor1(m_listdx, FALSE);
					BASS_StreamFree(m_stream);
					break;
				}
				if (m_close)
				{
					BASS_ChannelStop(m_stream);
					m_pause = FALSE;
					m_resume = FALSE;
					m_quit = FALSE;
					m_lock = FALSE;
					Mp3Pos(0, 0);
					mp3paly(1);

					a2 = tlen;
					CString str;
					str.Format("%d", tlen* pos);
					Write_config(str, "歌曲播放进度");
					str.Format("%d", tlen);
					Write_config(str, "进度条进度");
					DeleteFile(INIPATH(2).GetBuffer());
					m_Previous = m_listdx;
					m_paly = 0;
					BASS_StreamFree(m_stream);
					::PostMessage(m_hwnd, WM_QUIT, 0, 0);//关闭程序;
					break;
				}
				Sleep(1);
			}
		}
		
		if (m_quit)
		{
			BASS_ChannelStop(m_stream);
			m_pause = FALSE;
			m_resume = FALSE;
			m_quit = FALSE;
			m_lock = FALSE;
			m_paly = FALSE;
			Mp3Pos(0, 0);
			mp3paly(1);
			a2 = tlen;
			CString str;
			str.Format("%d", tlen * pos);
			Write_config(str, "歌曲播放进度");
			str.Format("%d", tlen);
			Write_config(str, "进度条进度");
			m_Previous = m_listdx;
			SetListColor1(m_listdx, FALSE);
			BASS_StreamFree(m_stream);
			m_paly = 0;
			break;
		}
		if (m_close)
		{
			BASS_ChannelStop(m_stream);
			m_pause = FALSE;
			m_resume = FALSE;
			m_quit = FALSE;
			m_lock = FALSE;
			Mp3Pos(0, 0);
			mp3paly(1);
			a2 = tlen;
			CString str;
			str.Format("%d", tlen * pos);
			Write_config(str, "歌曲播放进度");
			str.Format("%d", tlen);
			Write_config(str, "进度条进度");
			DeleteFile(INIPATH(2).GetBuffer());
			m_Previous = m_listdx;
			m_paly = 0;
			BASS_StreamFree(m_stream);
			::PostMessage(m_hwnd, WM_QUIT, 0, 0);//关闭程序
			break;
		}
		//保存歌曲进度
		if (tlen > a2 || tlen < a2)
		{
			a2 = tlen;
			CString str;
			str.Format("%d", tlen * pos);
			Write_config(str, "歌曲播放进度");
			str.Format("%d", tlen);
			Write_config(str, "进度条进度");
		}
		m_paly = 1;
		Sleep(50);
	}
	m_paly = 0;
	SetListColor1(m_listdx, FALSE);
	return TRUE;
}


