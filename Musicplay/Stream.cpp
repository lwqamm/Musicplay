#ifdef __ATLMISC_H__   //如果包含了atlmisc.h
#define __ATLTYPES_H__	//则不编译atltypes.h
#endif
#include <atlimage.h>
#include "pch.h"
#include "Stream.h"
#include <Wmcodecdsp.h>
#include <stdio.h>
#include "resource.h"

#define DEFNAME L"Memory IStream" 

// 
//  Constructor 
// 
CStream::CStream(BYTE* pData, ULONG nSize)
{
	m_lRefCount = 1;
	m_pData = pData;

	// stream metrics 
	m_nOffset.QuadPart = 0;
	m_nLength.QuadPart = nSize;//len; 


						   // stream status 
	m_statstg.type = STGTY_STREAM;      // IStream object 
	m_statstg.cbSize.QuadPart = 0;//len;                // Set to the length of our stream object 
	m_statstg.grfLocksSupported = 0;                // Region locking not supported 
	m_statstg.grfMode = 0;              // access mode 
	m_statstg.clsid = CLSID_NULL;       // not used for IStreams 
	m_statstg.grfStateBits = 0;             // not used for IStreams 
	m_statstg.reserved = 0;             // reserved for 


	CoFileTimeNow(&m_statstg.ctime);                // creation time 
	CoFileTimeNow(&m_statstg.atime);                // last access time 
	CoFileTimeNow(&m_statstg.mtime);                // last modify time 
}

// 
//  Destructor 
// 
CStream::~CStream()
{
}

// 
//  IUnknown::AddRef 
// 
ULONG __stdcall CStream::AddRef(void)
{
	// increment object reference count 
	return InterlockedIncrement(&m_lRefCount);
}

// 
//  IUnknown::Release 
// 
ULONG __stdcall CStream::Release(void)
{
	// decrement object reference count 
	LONG count = InterlockedDecrement(&m_lRefCount);

	if (count == 0)
	{
		delete this;
		return 0;
	}
	else
	{
		return count;
	}
}

// 
//  IUnknown::QueryInterface 
// 
HRESULT __stdcall CStream::QueryInterface(REFIID iid, void** ppvObject)
{
	// check to see what interface has been requested 
	if (iid == IID_IStream || iid == IID_IUnknown || iid == IID_ISequentialStream)
	{
		AddRef();
		*ppvObject = this;
		return S_OK;
	}
	else
	{
		*ppvObject = 0;
		return E_NOINTERFACE;
	}
}

// 
//  ISequentialStream::Read 
// 
HRESULT __stdcall CStream::Read(void* pv, ULONG cb, ULONG* pcbRead)
{
	ULONG available;


	if (pv == 0)
		return STG_E_INVALIDPOINTER;

	available = min(cb, (ULONG)(m_nLength.QuadPart - m_nOffset.QuadPart));

	memcpy(pv, m_pData + m_nOffset.QuadPart, available);

	m_nOffset.QuadPart += available;

	if (pcbRead)
		*pcbRead = available;

	return S_OK;
}

// 
//  ISequentialStream::Write 
// 
HRESULT __stdcall CStream::Write(const void* pv, ULONG cb, ULONG* pcbWritten)
{
	if (pv == 0)
		return STG_E_INVALIDPOINTER;


	return E_NOTIMPL;
}

// 
//  IStream::Seek 
// 
HRESULT __stdcall CStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition)
{
	switch (dwOrigin)
	{
	case STREAM_SEEK_SET:   m_nOffset.QuadPart = dlibMove.QuadPart;                      break;
	case STREAM_SEEK_CUR:   m_nOffset.QuadPart = m_nOffset.QuadPart + dlibMove.QuadPart; break;
	case STREAM_SEEK_END:   m_nOffset.QuadPart = m_nLength.QuadPart - dlibMove.QuadPart; break;
	}

	if (plibNewPosition)
		*plibNewPosition = m_nOffset;


	return S_OK;
}

// 
//  IStream::SetSize 
// 
HRESULT __stdcall CStream::SetSize(ULARGE_INTEGER libNewSize)
{
	return S_OK;
}

// 
//  IStream::CopyTo 
// 
HRESULT __stdcall CStream::CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten)
{
	DWORD len, written;
	len = (ULONG)min(cb.QuadPart, m_nLength.QuadPart);
	pstm->Write(m_pData, len, &written);

	if (pcbRead)
		pcbRead->QuadPart = len;
	if (pcbWritten)
		pcbWritten->QuadPart = written;
	return S_OK;
}

// 
//  IStream::Commit 
// 
HRESULT __stdcall CStream::Commit(DWORD grfCommitFlags)
{
	// Transacted mode is not supported 
	return S_OK;
}

// 
//  IStream::Revert 
// 
HRESULT __stdcall CStream::Revert()
{
	// Transacted mode is not supported 
	return S_OK;
}

// 
//  IStream::LockRegion 
// 
HRESULT __stdcall CStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	// locking is not supported 
	return STG_E_INVALIDFUNCTION;
}

// 
//  IStream::UnlockRegion 
// 
HRESULT __stdcall CStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	// locking is not supported 
	return STG_E_INVALIDFUNCTION;
}

// 
//  IStream::Stat 
// 
HRESULT __stdcall CStream::Stat(STATSTG* pstatstg, DWORD grfStatFlag)
{
	if (pstatstg == 0)
		return STG_E_INVALIDPOINTER;


	// return our STATSTG to the caller 
	m_statstg.cbSize.QuadPart = m_nLength.QuadPart;
	*pstatstg = m_statstg;


	switch (grfStatFlag)
	{
	case STATFLAG_DEFAULT:
		// allocate a new buffer for the name 
		if ((pstatstg->pwcsName = (WCHAR*)CoTaskMemAlloc(sizeof(DEFNAME))) == 0)
			return STG_E_INSUFFICIENTMEMORY;


		lstrcpyW(pstatstg->pwcsName, DEFNAME);
		break;


	case STATFLAG_NONAME:
		pstatstg->pwcsName = 0;
		break;


	default:
		return STG_E_INVALIDFLAG;
	}


	return S_OK;
}

// 
//  IStream::Clone 
// 
HRESULT __stdcall CStream::Clone(IStream** ppstm)
{
	return E_NOTIMPL;
}

wchar_t* utf_8ToUnicode(char* u8s)
{
	int wcsLen = MultiByteToWideChar(CP_UTF8, NULL, u8s, strlen(u8s), NULL, NULL);
	wchar_t* wcString = (wchar_t*)malloc(wcsLen + 1);
	memset(wcString, 0, wcsLen + 1);
	MultiByteToWideChar(CP_UTF8, NULL, u8s, strlen(u8s), wcString, wcsLen);
	wcString[wcsLen] = '\0';
	return wcString;
}

//将宽字节wchar_t*转化为单字节char*    
inline char* UnicodeToAnsi(const wchar_t* szStr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = (char*)malloc(nLen);
	memset(pResult, 0, nLen);
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
	pResult[nLen] = '\0';
	return pResult;
}

//将单字节char*转化为宽字节wchar_t*    
wchar_t* AnsiToUnicode(const char* szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}

char* Utf8ToAnsi(char* pchUtf8)
{
	int uSize = 0;
	WCHAR* pwText = NULL;
	char* pchAnsic = NULL;
	int ansicStrLen = 0;

	if (NULL == pchUtf8)
	{
		return NULL;
	}

	do
	{
		uSize = MultiByteToWideChar(CP_UTF8, 0, pchUtf8, -1, NULL, 0);
		if (uSize <= 0)
		{
			break;
		}
		pwText = (wchar_t*)malloc((uSize + 1) * sizeof(wchar_t));
		if (!pwText)
		{
			break;
		}
		memset((void*)pwText, 0, sizeof(wchar_t) * (uSize + 1));
		MultiByteToWideChar(CP_UTF8, 0, pchUtf8, -1, pwText, uSize);

		//widechar to utf8
		ansicStrLen = WideCharToMultiByte(CP_ACP, 0, pwText, -1, NULL, 0, NULL, NULL);
		if (0 >= ansicStrLen)
		{
			break;
		}

		pchAnsic = (char*)malloc(ansicStrLen + 2);
		if (NULL == pchAnsic)
		{
			break;
		}
		memset(pchAnsic, 0, ansicStrLen + 2);

		ansicStrLen = WideCharToMultiByte(CP_ACP, 0, pwText, -1, pchAnsic, ansicStrLen, NULL, NULL);
		if (0 >= ansicStrLen)
		{
			free(pchAnsic);
			pchAnsic = NULL;
			break;
		}

	} while (0);

	if (NULL != pwText)
	{
		free(pwText);
	}

	return pchAnsic;
}

static char* AnsiToUtf8(char* pchAnsi)
{
	int uSize = 0;
	WCHAR* pwText = NULL;
	char* pchUtf8 = NULL;
	int nUtf8Len = 0;

	if (NULL == pchAnsi)
	{
		return NULL;
	}

	do
	{
		uSize = MultiByteToWideChar(CP_ACP, 0, pchAnsi, -1, NULL, 0);
		if (uSize <= 0)
		{
			break;
		}
		pwText = (wchar_t*)malloc((uSize + 1) * sizeof(wchar_t));
		if (!pwText)
		{
			break;
		}

		memset((void*)pwText, 0, sizeof(wchar_t) * (uSize + 1));
		MultiByteToWideChar(CP_ACP, 0, pchAnsi, -1, pwText, uSize);

		//widechar to utf8
		nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, pwText, -1, NULL, 0, NULL, NULL);
		if (0 >= nUtf8Len)
		{
			break;
		}

		pchUtf8 = (char*)malloc(nUtf8Len + 2);
		if (NULL == pchUtf8)
		{
			break;
		}
		memset(pchUtf8, 0, nUtf8Len + 2);

		nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, pwText, -1, pchUtf8, nUtf8Len, NULL, NULL);
		if (0 >= nUtf8Len)
		{
			free(pchUtf8);
			pchUtf8 = NULL;
			break;
		}


	} while (0);

	if (NULL != pwText)
	{
		free(pwText);
	}

	return pchUtf8;
}

void iso_8859_1_to_utf8(const char* str, int size, OUT char* str1)
{
	PUCHAR p = (PUCHAR)str;
	for (int i = 0; i < size; i++)
	{

		if (p[i] < 0x80 && p[i] > 0x0) {
			str1[i] = p[i];
		}
		else {
			str1[i] = (0xc0 | p[i] >> 6);
			str1[i] = (0x80 | (p[i] & 0x3f));
		}
	}
	return;
}
/// //////////////////////////////

myimage::myimage()
{
	c_Telb = { 0 };
	if (lpVoid == NULL)
	{
		HRSRC hRsrc = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_JPEG1), "JPEG");//IDR_JPEG1
		//LPTSTR szBuffer = new TCHAR[1024];//这里是定义缓冲区大小

		if (hRsrc == NULL)
		{
			//FreeRes_ShowError("FindResource");
			return;
		}
		DWORD dwSize = ::SizeofResource(NULL, hRsrc);
		if (0 >= dwSize)
		{
			//FreeRes_ShowError("SizeofResource");
			return;
		}

		HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
		if (NULL == hGlobal)
		{
			//FreeRes_ShowError("LoadResource");
			return;
		}

		lpVoid = ::LockResource(hGlobal);
		if (NULL == lpVoid)
		{
			//FreeRes_ShowError("LockResource");
			return;
		}
		lpVoidSize = dwSize;
	}
}

void myimage::Setmyimage(HWND hwnd)
{
	m_hwnd = hwnd;
}

myimage::~myimage()
{

}

char* myimage::GetFrameInfoByEcoding(PVOID b, UCHAR conde)
{
	char* str = NULL;
	if (b == NULL) { return 0; }
	switch (conde)
	{
	case 0:

		str = UnicodeToAnsi((wchar_t*)b);

		if (strlen(str) <= 0)
		{
			free(str);
			return 0;
		}

		return str;
	case 1:
	{
		str = UnicodeToAnsi((wchar_t*)b);

		if (strlen(str) <= 0)
		{
			free(str);
			return 0;
		}
		return str;
	}
	case 2:
	{
		str = UnicodeToAnsi((wchar_t*)b);

		if (strlen(str) <= 0)
		{
			free(str);
			return 0;
		}
		return str;
	}
	case 3:
	{
		str = Utf8ToAnsi((char*)b);
		if (strlen(str) <= 0)
		{
			free(str);
			return 0;
		}

		return str;
	}
	}


	return 0;
}

ULONG myimage::GetJpeg(const PUCHAR p, int* size)
{
	UCHAR p1[] = { 0xFF,0xD8 }, p2[] = { 0xFF,0xD9 };
	int plen = *(int*)size;

	int b = *(int*)size = 0;

	for (int i = 0; i < plen; i++)
	{
		if (b == 0)
		{
			if (p[i] == p1[0] && p[i + 1] == p1[1])
			{
				*(int*)size = i;
				b++;
			}
		}
		if (b)
		{
			if (p[i] == p2[0] && p[i + 1] == p2[1])
			{
				return i + 2;
			}
		}

	}

	return 0;
}

BOOL myimage::ReadFrame(PVOID fs, int size, OUT TID3InfoEx& Telb)
{
	BOOL ret = FALSE;
	int asi = 0;
	while (1)
	{
		LABEL label = { 0 };
		memcpy(&label, (PVOID)((ULONG64)fs + asi), sizeof(LABEL));

		int frmSize = 0;
		char FramID[5] = { 0 };
		memcpy(FramID, &label, 4);

		PUCHAR p = (PUCHAR)label.Size;
		frmSize = p[0] * 0x100000000 + p[1] * 0x10000 + p[2] * 0x100 + p[3];

		if (frmSize <= 0 || asi >= size)
		{
			//就说明真的没有信息了
			return ret;

		}
		//bFrame 用来保存帧的信息
		PUCHAR bFrame = (PUCHAR)malloc(frmSize - 1);

		if (!bFrame)
		{
			return ret;
		}
		memset(bFrame, 0, frmSize - 1);
		asi += sizeof(LABEL);
		memcpy(bFrame, (PVOID)((ULONG64)fs + asi + 1), frmSize - 1);

		memcpy(&label.Flags, (char*)((ULONG64)fs + asi), 1);
		char* str = NULL;

		int t_Stl;//拷贝长度
		asi += frmSize;//TPE1
		frmSize -= 1;
		int fotsize = 0;
		if (strcmp(FramID, "TIT2") == 0)//歌曲名
		{
			ret = TRUE;
			str = GetFrameInfoByEcoding(bFrame, label.Flags[0]);

			memset(Telb.Title, 0, sizeof(Telb.Title));
			int a1 = 0, a = 0;
			fotsize = frmSize - 1;
			for (size_t i = 0; i < fotsize; i++)
			{
				if (str[i] != '?')
				{
					memcpy(Telb.Title + strlen(Telb.Title), str + i, 1);
				}
			}
			//free(str);
			continue;

		}
		////////////////////////////////
		if (strcmp(FramID, "TPE1") == 0)//歌手
		{
			ret = TRUE;
			str = GetFrameInfoByEcoding(bFrame, label.Flags[0]);


			memset(Telb.Artist, 0, sizeof(Telb.Artist));
			int a1 = 0, a = 0;
			fotsize = frmSize - 1;
			for (size_t i = 0; i < fotsize; i++)
			{
				if (str[i] != '?')
				{
					memcpy(Telb.Artist + strlen(Telb.Artist), str + i, 1);
				}
			}

			//free(str);
			continue;
		}
		////////////////////////////////
		if (strcmp(FramID, "TALB") == 0)//“TALB” 专辑
		{
			//ret = TRUE;
			//str = GetFrameInfoByEcoding(bFrame, label.myFlags);

			//t_Stl = strlen(str);
			//if (t_Stl > sizeof(Telb.Album))
			//{
			//	t_Stl = sizeof(Telb.Album) - 1;
			//}
			//memcpy(Telb.Album, str, t_Stl);
			free(str);
			continue;
		}
		////////////////////////////////
		if (strcmp(FramID, "TRCK") == 0)//“TRCK” ' 音轨
		{
			//ret = TRUE;
			//str = GetFrameInfoByEcoding(bFrame, label.myFlags);
			// 
			//t_Stl = strlen(str);
			//if (t_Stl > sizeof(Telb.TrackNum))
			//{
			//	t_Stl = sizeof(Telb.TrackNum) - 1;
			//}
			//memcpy(Telb.TrackNum, str, t_Stl);
			free(str);
			continue;
		}
		////////////////////////////////
		if (strcmp(FramID, "TBOS") == 0)//“TBOS”  ' 作品集部分
		{
			//ret = TRUE;
			//str = GetFrameInfoByEcoding(bFrame, label.myFlags);
			//
			//t_Stl = strlen(str);
			//if (t_Stl > sizeof(Telb.Portfolio))
			//{
			//	t_Stl = sizeof(Telb.Portfolio) - 1;
			//}
			//memcpy(Telb.Portfolio, str, t_Stl);
			free(str);
			continue;
		}
		////////////////////////////////
		if (strcmp(FramID, "COMM") == 0)//“COMM”  ' 注释 Comment
		{
			//ret = TRUE;
			//str = GetFrameInfoByEcoding(bFrame, label.myFlags);
			//
			//t_Stl = strlen(str);
			//if (t_Stl > sizeof(Telb.Comment))
			//{
			//	t_Stl = sizeof(Telb.Comment) - 1;
			//}
			//memcpy(Telb.Comment, str, t_Stl);
			free(str);
			continue;
		}
		////////////////////////////////
		if (strcmp(FramID, "TSSE") == 0)//“TSSE” ' 编码使用的软件（硬件设置）
		{
			/*		ret = TRUE;
					str = GetFrameInfoByEcoding(bFrame, label.myFlags);

					t_Stl = strlen(str);
					if (t_Stl > sizeof(Telb.Software))
					{
						t_Stl = sizeof(Telb.Software) - 1;
					}
					memcpy(Telb.Software, str, t_Stl);*/
			free(str);
			continue;
		}
		////////////////////////////////
		if (strcmp(FramID, "APIC") == 0)//“APIC”  ' 附加描述 图片数据在此处
		{
			ret = TRUE;
			//Telb.Pic = (char*)bFrame;

			int ttin = (int)frmSize;
			ULONG	piclen = GetJpeg(bFrame, &ttin);
			if (piclen == 0)
			{
				continue;
			}

			Telb.BPM = piclen;

			Telb.Pic = (char*)(malloc(piclen + 1));
			if (!Telb.Pic)
			{
				//free(bFrame);
				continue;
			}
			memset(Telb.Pic, 0, piclen + 1);
			memcpy(Telb.Pic, bFrame + ttin, piclen);
			continue;
			IMediaObject;
		}

		free(bFrame);
	}

	return ret;
}

int myimage::filelength(FILE* fp)
{
	int num;
	fseek(fp, 0, SEEK_END);
	num = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return num;
}
int myimage::GetMp3String(int* vea)
{
	PUCHAR p = (PUCHAR)m_mp3Path;
	int size = strlen(m_mp3Path);
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
BOOL myimage::ParseMP3(const char* mp3Path, PVOID* oubuff)
{
	FILE* fp;
	fp = fopen(mp3Path, "rb");

	DWORD fileSize = filelength(fp);    //获得文件大小的低两字节
	if (!fileSize)
	{
		return FALSE;
	}
	UCHAR* fileBuff = (UCHAR*)malloc(fileSize);    //创建缓冲区
	if (!fileBuff)
	{
		return FALSE;
	}
	DWORD realRead = 0;
	BOOL ret = false;
	ret = fread(fileBuff, fileSize, 1, fp);
	fclose(fp);
	*(fileBuff + fileSize - 1) = '\0';
	if (ret)
	{
		//0x000000020678f756 ""
		ID3V2Header Header = { 0 };
		memcpy(&Header, fileBuff, sizeof(ID3V2Header));
		ULONG size = (Header.Size[0] & 0x7F) * 0x200000
			+ (Header.Size[1] & 0x7F) * 0x4000
			+ (Header.Size[2] & 0x7F) * 0x80
			+ (Header.Size[3] & 0x7F);
		UCHAR mp3ID[] = "ID3";
		if (memcmp((UCHAR*)fileBuff, mp3ID, 3) == 0 && size > 0)
		{
			size += sizeof(ID3V2Header);
			ret = ParseMP3_1(fileBuff + size, oubuff);
			if (!ret)
			{
				free(fileBuff);
				return FALSE;
			}
			MessageBox(NULL, "有效MP3", "MP3", MB_OK);
		}
		else
		{
			PUCHAR p = (PUCHAR)fileBuff + (fileSize - 128);
			if (p[0] == 'T' && p[1] == 'A' && p[2] == 'G')
			{
				MessageBox(NULL, "有效MP3", "MP3", MB_OK);

			}
			else {
				MessageBox(NULL, "无效MP3", "MP3", MB_OK);
				ret = FALSE;
			}
		}
	}
	//https://www.jianshu.com/p/58df71a19901
	free(fileBuff);
	return ret;
}
BOOL myimage::ParseMP3_1(PVOID mp3Path, PVOID* oubuff)
{
	HEADER heder = { 0 };
	if (!mp3Path)
	{
		return FALSE;
	}
	PUCHAR p = (PUCHAR)mp3Path;
	memcpy(&heder, p, sizeof(HEADER));
	SetMPEG(heder);
	return 1;

}
void myimage::SetMPEG(HEADER vea)  //初始化MPEG
{
	switch (vea.version)
	{
	case 0:
	{
		m_Mpeg.MPEG = 3;
		break;
	}
	case 1:
	{
		m_Mpeg.MPEG = 0;
		break;
	}
	case 2:
	{
		m_Mpeg.MPEG = 2;
		break;
	}
	case 3:
	{
		m_Mpeg.MPEG = 1;
		break;
	}
	default:
		break;
	}
	switch (vea.layer)
	{
	case 0:
	{
		m_Mpeg.Layer = 0;
		break;
	}
	case 1:
	{
		m_Mpeg.Layer = 3;
		break;
	}
	case 2:
	{
		m_Mpeg.Layer = 2;
		break;
	}
	case 3:
	{
		m_Mpeg.Layer = 1;
		break;
	}
	default:
		break;
	}
	if (vea.protection == 0)
	{
		m_Mpeg.CRC = 1;
	}
	else if (vea.protection == 1)
	{
		m_Mpeg.CRC = 0;
	}
	switch (vea.bitrate_index)
	{
	case  0:
	{
		m_Mpeg.kbps = 0;
		break;
	}
	case  1:  //0001
	{
		if (m_Mpeg.MPEG == 1)
		{
			m_Mpeg.kbps = 32;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 32;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 8;
		}
		break;
	}
	case  2:  //0010
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 64;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 48;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 40;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 48;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 16;
		}
		break;
	}
	case  3:  //0011
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 96;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 56;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 48;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 56;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 24;
		}
		break;
	}
	case  4:  //0100
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 128;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 64;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 56;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 64;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 32;
		}
		break;
	}
	case  5:  //0101
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 160;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 80;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 64;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 80;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 40;
		}
		break;
	}
	case  6:  //0110
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 192;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 96;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 80;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 96;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 48;
		}
		break;
	}
	case  7:  //0111
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 224;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 112;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 96;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 112;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 56;
		}
		break;
	}
	case  8:  //1000
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 256;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 128;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 112;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 128;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 64;
		}
		break;
	}
	case  9:  //1001
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 288;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 160;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 128;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 144;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 80;
		}
		break;
	}
	case  0xA:  //1010
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 320;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 192;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 160;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 160;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 96;
		}
		break;
	}
	case  0xB:  //1011
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 352;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 224;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 196;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 176;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 112;
		}
		break;
	}
	case  0xC:  //1100
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 384;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 256;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 224;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 192;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 128;
		}
		break;
	}
	case  0xD:  //1101
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 416;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 320;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 256;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 224;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 144;
		}
		break;
	}
	case  0xE:  //1110
	{
		if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 448;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 2)
		{
			m_Mpeg.kbps = 384;
		}
		else if (m_Mpeg.MPEG == 1 && m_Mpeg.Layer == 3)
		{
			m_Mpeg.kbps = 320;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && m_Mpeg.Layer == 1)
		{
			m_Mpeg.kbps = 256;
		}
		else if ((m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3) && (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3))
		{
			m_Mpeg.kbps = 160;
		}
		break;
	}
	case  0xF:  //1111
	{
		m_Mpeg.kbps = 1;
		break;
	}
	default:
		break;
	}
	switch (vea.sampling_frequency)
	{
	case 0: //00
	{
		if (m_Mpeg.MPEG == 1)
		{
			m_Mpeg.Hz = 44100;
		}
		else 	if (m_Mpeg.MPEG == 2)
		{
			m_Mpeg.Hz = 22050;
		}
		else 	if (m_Mpeg.MPEG == 3)
		{
			m_Mpeg.Hz = 11025;
		}
		break;
	}
	case 1: //01
	{
		if (m_Mpeg.MPEG == 1)
		{
			m_Mpeg.Hz = 48000;
		}
		else 	if (m_Mpeg.MPEG == 2)
		{
			m_Mpeg.Hz = 24000;
		}
		else 	if (m_Mpeg.MPEG == 3)
		{
			m_Mpeg.Hz = 12000;
		}
		break;
	}
	case 2: //10
	{
		if (m_Mpeg.MPEG == 1)
		{
			m_Mpeg.Hz = 32000;
		}
		else 	if (m_Mpeg.MPEG == 2)
		{
			m_Mpeg.Hz = 16000;
		}
		else 	if (m_Mpeg.MPEG == 3)
		{
			m_Mpeg.Hz = 8000;
		}
		break;
	}
	default:
		break;
	}
	if (vea.padding == 0)
	{
		m_Mpeg.Fillbit = 0;
	}
	else if (vea.padding == 1)
	{
		m_Mpeg.Fillbit = 1;
	}
	switch (vea.mode)
	{
	case 0: //00
	{
		m_Mpeg.Vocaltract = 0;
		break;
	}
	case 1: //01
	{
		m_Mpeg.Vocaltract = 1;
		break;
	}
	case 2: //10
	{
		m_Mpeg.Vocaltract = 2;
		break;
	}
	case 3: //11
	{
		m_Mpeg.Vocaltract = 3;
		break;
	}
	default:
		break;
	}
	if (m_Mpeg.MPEG == 1)
	{
		if (m_Mpeg.Layer == 1)
		{
			m_Mpeg.Numberofsamples = 384;
		}
		else if (m_Mpeg.Layer == 2 || m_Mpeg.Layer == 3)
		{
			m_Mpeg.Numberofsamples = 1152;
		}
	}
	if (m_Mpeg.MPEG == 2 || m_Mpeg.MPEG == 3)
	{
		if (m_Mpeg.Layer == 1)
		{
			m_Mpeg.Numberofsamples = 384;
		}
		else if (m_Mpeg.Layer == 2)
		{
			m_Mpeg.Numberofsamples = 1152;
		}
		else 	if (m_Mpeg.Layer == 3)
		{
			m_Mpeg.Numberofsamples = 576;
		}
	}
}
BOOL myimage::myMp3ParSing(const char* mp3Path, PVOID* outbuff)
{
	if (Mp3ParSing(mp3Path, c_Telb))
	{
		*(PVOID*)outbuff = &c_Telb;
		return TRUE;
	}
	*(PVOID*)outbuff = 0;
	return  FALSE;
}
BOOL myimage::Mp3ParSing(const char* mp3Path, TID3InfoEx& Telb)
{
	FILE* fp;
	fp = fopen(mp3Path, "rb");

	DWORD fileSize = filelength(fp);    //获得文件大小的低两字节
	if (!fileSize)
	{
		return FALSE;
	}
	UCHAR* fileBuff = (UCHAR*)malloc(fileSize);    //创建缓冲区
	if (!fileBuff)
	{
		return FALSE;
	}
	DWORD realRead = 0;
	BOOL ret = false;
	ret = fread(fileBuff, fileSize, 1, fp);
	fclose(fp);
	*(fileBuff + fileSize - 1) = '\0';
	if (ret)
	{
		ID3V2Header Header = { 0 };
		memcpy(&Header, fileBuff, sizeof(ID3V2Header));
		ULONG size = (Header.Size[0] & 0x7F) * 0x200000
			+ (Header.Size[1] & 0x7F) * 0x4000
			+ (Header.Size[2] & 0x7F) * 0x80
			+ (Header.Size[3] & 0x7F);
		UCHAR mp3ID[] = "ID3";
		if (memcmp((UCHAR*)fileBuff, mp3ID, 3) == 0 && size > 0)
		{
			//printf(" size=%d\n", size);
			UCHAR* TBuff = (UCHAR*)malloc(fileSize - sizeof(ID3V2Header));    //创建缓冲区
			if (!TBuff) { free(fileBuff); return FALSE; }
			memcpy(TBuff, fileBuff + sizeof(ID3V2Header), fileSize - sizeof(ID3V2Header));

			free(fileBuff);

			if (!ReadFrame(TBuff, size, Telb))
			{
				free(TBuff);
				return FALSE;
			}
			free(TBuff);
			return TRUE;
		}
	}
	free(fileBuff);
	return FALSE;
}

CStream myimage::GetCStream(PVOID pData, int nSize)
{
	//CStream stream((BYTE*)pData, nSize);
	return CStream((BYTE*)pData, nSize);
}

int myimage::IsImageByHead(const PVOID pData)
{
	if (!pData)
	{
		return -1;
	}
	PUCHAR  pp = (PUCHAR)pData;
	if (pp[0] == 0xFF && pp[1] == 0xD8 && pp[2] == 0xFF)// JPEG (jpg)，文件头：FFD8FF
	{
		return 1;
	}
	else if (pp[0] == 0x89 && pp[1] == 0x50 && pp[2] == 0x4E && pp[3] == 0x47)
	{
		return 2;// PNG (png)，文件头：89504E47
	}
	else if (pp[0] == 0x47 && pp[1] == 0x49 && pp[2] == 0x46 && pp[3] == 0x38)
	{
		return 3;// GIF(gif)，文件头：47494638
	}
	else if (pp[0] == 0x49 && pp[1] == 0x49 && pp[2] == 0x2A && pp[3] == 0x00)
	{
		return 4; //TIFF(tif)，文件头：49492A00
	}
	else if (pp[0] == 0x42 && pp[1] == 0x4D)
	{
		return 5;//Windows Bitmap (bmp)，文件头：424D
	}
	return -1;
}

//BOOL myimage::StretchBlt(CDC* mcdc, int x, int y, int w)
//{
//	ASSERT_VALID(mcdc);
//	if (!mcdc)
//		return FALSE;
//
//	int lx = IsImageByHead(c_Telb.Pic);
//	CImage image;
//	if (lx == 1)
//	{
//		CStream stream = GetCStream(c_Telb.Pic, c_Telb.BPM);
//		image.Load(&stream);
//		lx = 2;
//	}
//	else
//	{
//		if (lpVoid == NULL || lpVoidSize == 0)
//		{
//			lx == 0;
//		}
//		else
//		{
//			CStream stream = GetCStream(lpVoid, lpVoidSize);
//			image.Load(&stream);
//			lx = 2;
//		}
//
//	}
//	if (lx == 2)
//	{
//		// 创建内存DC
//		CDC memDC;
//		memDC.CreateCompatibleDC(NULL);
//
//		// 建立一个与屏幕显示兼容的位图
//		CBitmap memBitmap;
//
//		memBitmap.Attach(image);
//		BITMAP btSize;
//
//		memBitmap.GetBitmap(&btSize);
//		mcdc->SetStretchBltMode(HALFTONE); //效果最佳
//			// 将位图选入到内存DC中
//		CBitmap* oldBitmap = memDC.SelectObject(&memBitmap);
//		int w1 = 0, h1 = 0;
//		w1 = image.GetWidth();
//		h1 = image.GetHeight();
//		int h = 0;
//		/// //////////////////////////////////
//		float tw = 0;
//		if (w >= w1)
//		{
//			tw = (float)w1 / 100;
//			float ta1 = w - w1;
//			ta1 = ta1 / tw;
//			w = w1 + (ta1 * tw);
//			h = h1 + (ta1 * tw);
//		}
//		else
//		{
//			tw = (float)w1 / 100;
//			float ta1 = w1 - w;
//			ta1 = ta1 / tw;
//			w = w1 - (ta1 * tw);
//			h = h1 - (ta1 * tw);
//		}
//		/// //////////////////////////////////
//		mcdc->StretchBlt(x, y, w, h, &memDC, 0, 0, w1, h1, SRCCOPY);
//		// 清理
//		memDC.SelectObject(oldBitmap);
//		memBitmap.DeleteObject();
//		memDC.DeleteDC();
//	}
//
//
//	//          X    Y      W        H
//	CRect rect(119, 819, 119 + 600, 819 + 20);
//
//	CPen NewPen, * pOldPen;
//	NewPen.CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
//	pOldPen = mcdc->SelectObject(&NewPen);
//
//	CBrush NewBrush, * pOldBrush;
//	NewBrush.CreateSolidBrush(0xFFFFFF);//创建画刷
//	pOldBrush = mcdc->SelectObject(&NewBrush);//选择画刷，保存原来画刷
//
//	mcdc->Rectangle(rect);
//	mcdc->SelectObject(pOldPen);//恢复设备上下文
//	mcdc->SelectObject(pOldBrush);
//	//原文链接：https ://blog.csdn.net/qq_44643644/article/details/119059875
//
//
//	 ///画歌名
//	LOGFONT lf;
//	CFont fontTemp, * pFontOld;
//
//	mcdc->GetCurrentFont()->GetLogFont(&lf);
//	lstrcpy(lf.lfFaceName, _T("宋体"));
//	lf.lfWidth = 0;
//	lf.lfWeight = FW_HEAVY;
//	lf.lfHeight = 0xE;
//	fontTemp.CreateFontIndirect(&lf);      //创建字体
//	pFontOld = mcdc->SelectObject(&fontTemp);
//	mcdc->SetTextColor(RGB(0x0E, 0x0E, 0xFF));
//	mcdc->SetBkMode(0);
//	char ch[MAX_PATH] = { 0 };
//	if (strlen(c_Telb.Title) == NULL && strlen(c_Telb.Artist) == NULL)
//	{
//		int tsize = 0;
//		int Min = GetMp3String(&tsize);
//		if (Min == -1)
//		{
//			strcpy(ch, "未知歌曲");
//		}
//		else
//		{
//			memcpy(ch, m_mp3Path + Min, tsize);
//		}
//	}
//
//	if (strlen(c_Telb.Title) != NULL && strlen(c_Telb.Artist) == NULL)
//	{
//		strcpy(ch, "未知歌手-");
//		strcpy(ch + strlen(ch), c_Telb.Title);
//	}
//	else if (strlen(c_Telb.Title) == NULL && strlen(c_Telb.Artist) != NULL)
//	{
//		strcpy(ch, c_Telb.Artist);
//		strcpy(ch + strlen(ch), "-未知歌曲");
//	}
//	if (strlen(c_Telb.Title) != NULL && strlen(c_Telb.Artist) != NULL)
//	{
//		memcpy(ch, c_Telb.Artist, strlen(c_Telb.Artist));
//		ch[strlen(ch)] = '-';
//		memcpy(ch + strlen(ch), c_Telb.Title, strlen(c_Telb.Title));
//	}
//	mcdc->TextOut(120, 820, ch, strlen(ch));
//	//UpdateWindow(m_hwnd);
//
//	RestoreDC((HDC)mcdc, 0);
//
//	return TRUE;
//}

void myimage::Deletemp3()
{
	if (c_Telb.Album)
	{
		memset(c_Telb.Album, 0, sizeof(c_Telb.Album));
	}
	if (c_Telb.AlbumArtist)
	{

		memset(c_Telb.AlbumArtist, 0, sizeof(c_Telb.AlbumArtist));
	}
	if (c_Telb.Artist)
	{
		memset(c_Telb.Artist, 0, sizeof(c_Telb.Artist));
	}
	if (c_Telb.BPM)
	{
		c_Telb.BPM = 0;
	}
	if (c_Telb.Comment)
	{

		memset(c_Telb.Comment, 0, sizeof(c_Telb.Comment));
	}
	if (c_Telb.Composer)
	{

		memset(c_Telb.Composer, 0, sizeof(c_Telb.Composer));
	}
	if (c_Telb.Copyright)
	{

		memset(c_Telb.Copyright, 0, sizeof(c_Telb.Copyright));
	}
	if (c_Telb.Encoder)
	{

		memset(c_Telb.Encoder, 0, sizeof(c_Telb.Encoder));
	}
	if (c_Telb.Genre)
	{

		memset(c_Telb.Genre, 0, sizeof(c_Telb.Genre));
	}
	if (c_Telb.OriginalArtist)
	{

		memset(c_Telb.OriginalArtist, 0, sizeof(c_Telb.OriginalArtist));
	}
	if (c_Telb.Pic)
	{
		free(c_Telb.Pic);
		c_Telb.Pic = NULL;
	}
	if (c_Telb.Portfolio)
	{

		memset(c_Telb.Portfolio, 0, sizeof(c_Telb.Portfolio));
	}
	if (c_Telb.Publisher)
	{

		memset(c_Telb.Publisher, 0, sizeof(c_Telb.Publisher));
	}
	if (c_Telb.reserved)
	{

		memset(c_Telb.reserved, 0, sizeof(c_Telb.reserved));
	}
	if (c_Telb.Software)
	{

		memset(c_Telb.Software, 0, sizeof(c_Telb.Software));
	}
	if (c_Telb.Title)
	{
		memset(c_Telb.Title, 0, sizeof(c_Telb.Title));
	}
	if (c_Telb.TrackNum)
	{

		memset(c_Telb.TrackNum, 0, sizeof(c_Telb.TrackNum));
	}
	if (c_Telb.URL)
	{

		memset(c_Telb.URL, 0, sizeof(c_Telb.URL));
	}
	if (c_Telb.Year)
	{

		memset(c_Telb.Year, 0, sizeof(c_Telb.Year));
	}
	return;
}

BOOL myimage::StretchBlt(CDC* mcdc, int x, int y, int w)
{
	ASSERT_VALID(mcdc);
	if (!mcdc)
		return FALSE;

	int lx = IsImageByHead(c_Telb.Pic);
	CImage image;
	if (lx == 1)
	{
		CStream stream = GetCStream(c_Telb.Pic, c_Telb.BPM);
		image.Load(&stream);
		lx = 2;
	}
	else
	{
		if (lpVoid == NULL || lpVoidSize == 0)
		{
			lx == 0;
		}
		else
		{
			CStream stream = GetCStream(lpVoid, lpVoidSize);
			image.Load(&stream);
			lx = 2;
		}

	}
	CDC dcMem;

	CBitmap bmp;  //内存中承载临时图象的位图
	CRect rc;
	GetClientRect(m_hwnd, &rc);

	dcMem.CreateCompatibleDC(mcdc);  //依附窗口DC创建兼容内存DC
	//创建兼容位图(必须用pDC创建，否则画出的图形变成黑色)
	bmp.CreateCompatibleBitmap(mcdc, rc.Width(), rc.Height());
	CBitmap* pOldBit = dcMem.SelectObject(&bmp);
	//按原来背景填充客户区，不然会是黑色
	dcMem.FillSolidRect(rc, RGB(240, 240, 240));;
	dcMem.SetStretchBltMode(HALFTONE); //效果最佳
	int w1 = 0, h1 = 0;
	if (lx == 2)
	{
		// 创建内存DC
		CDC memDC;
		memDC.CreateCompatibleDC(NULL);

		// 建立一个与屏幕显示兼容的位图
		CBitmap memBitmap;

		memBitmap.Attach(image);
		BITMAP btSize;

		memBitmap.GetBitmap(&btSize);
		mcdc->SetStretchBltMode(HALFTONE); //效果最佳
			// 将位图选入到内存DC中
		CBitmap* oldBitmap = memDC.SelectObject(&memBitmap);
		
		w1 = image.GetWidth();
		h1 = image.GetHeight();
		int h = 0;
		/// //////////////////////////////////
		float tw = 0;
		if (w >= w1)
		{
			tw = (float)w1 / 100;
			float ta1 = w - w1;
			ta1 = ta1 / tw;
			w = w1 + (ta1 * tw);
			h = h1 + (ta1 * tw);
		}
		else
		{
			tw = (float)w1 / 100;
			float ta1 = w1 - w;
			ta1 = ta1 / tw;
			w = w1 - (ta1 * tw);
			h = h1 - (ta1 * tw);
		}
		/// //////////////////////////////////
		dcMem.StretchBlt(x, y, w, h, &memDC, 0, 0, w1, h1, SRCCOPY);
		dcMem.StretchBlt(580, 62, w1, h1, &memDC, 0, 0, w1, h1, SRCCOPY);
		// 清理
		memDC.SelectObject(oldBitmap);
		memBitmap.DeleteObject();
		memDC.DeleteDC();
	}

	//          X    Y      W        H
	CRect rect(110, 580, 110 + 300, 580 + 20);

	CPen NewPen, * pOldPen;
	NewPen.CreatePen(PS_SOLID, 3, RGB(240, 240, 240));
	pOldPen = dcMem.SelectObject(&NewPen);

	CBrush NewBrush, * pOldBrush;
	NewBrush.CreateSolidBrush(0xF0F0F0);//创建画刷
	pOldBrush = dcMem.SelectObject(&NewBrush);//选择画刷，保存原来画刷

	dcMem.Rectangle(rect);
	dcMem.SelectObject(pOldPen);//恢复设备上下文
	dcMem.SelectObject(pOldBrush);
	//原文链接：https ://blog.csdn.net/qq_44643644/article/details/119059875


	 ///画歌名
	LOGFONT lf;
	CFont fontTemp, * pFontOld;

	dcMem.GetCurrentFont()->GetLogFont(&lf);
	lstrcpy(lf.lfFaceName, _T("宋体"));
	lf.lfWidth = 0;
	lf.lfWeight = FW_HEAVY;
	lf.lfHeight = 0xE;
	fontTemp.CreateFontIndirect(&lf);      //创建字体
	pFontOld = dcMem.SelectObject(&fontTemp);
	dcMem.SetTextColor(RGB(0x0E, 0x0E, 0xFF));
	dcMem.SetBkMode(0);
	char ch[MAX_PATH] = { 0 };
	if (strlen(c_Telb.Title) == NULL && strlen(c_Telb.Artist) == NULL)
	{
		int tsize = 0;
		int Min = GetMp3String(&tsize);
		if (Min == -1)
		{
			strcpy(ch, "未知歌曲");
		}
		else
		{
			memcpy(ch, m_mp3Path + Min, tsize);
		}
	}

	if (strlen(c_Telb.Title) != NULL && strlen(c_Telb.Artist) == NULL)
	{
		strcpy(ch, "未知歌手-");
		strcpy(ch + strlen(ch), c_Telb.Title);
	}
	else if (strlen(c_Telb.Title) == NULL && strlen(c_Telb.Artist) != NULL)
	{
		strcpy(ch, c_Telb.Artist);
		strcpy(ch + strlen(ch), "-未知歌曲");
	}
	if (strlen(c_Telb.Title) != NULL && strlen(c_Telb.Artist) != NULL)
	{
		memcpy(ch, c_Telb.Artist, strlen(c_Telb.Artist));
		ch[strlen(ch)] = '-';
		memcpy(ch + strlen(ch), c_Telb.Title, strlen(c_Telb.Title));
	}
	dcMem.TextOut(110, 590, ch, strlen(ch));
	mcdc->StretchBlt(6, 574, 346, 90, &dcMem, 6, 578, 346, 90, SRCCOPY);
	mcdc->BitBlt(580, 62, w1, h1, &dcMem, 590, 62, SRCCOPY);
	//UpdateWindow(m_hwnd);
		//绘图完成后的清理
	dcMem.DeleteDC();      //删除DC
	bmp.DeleteObject();  //删除位图
	//RestoreDC((HDC)mcdc, 0);

	return TRUE;
}

BOOL myimage::GetMp3ParSing(const char* mp3Path, CDC* mcdc, int x, int y, int w)
{
	if (strlen(mp3Path) <= 6)
	{
		return FALSE;
	}
	if (strcmp(mp3Path, m_mp3Path) == 0)
	{
		return StretchBlt(mcdc, x, y, w);

	}
	Deletemp3();
	if (Mp3ParSing(mp3Path, c_Telb))
	{
		memset(m_mp3Path, 0, sizeof(m_mp3Path));
		memcpy(m_mp3Path, mp3Path, strlen(mp3Path));
		return	StretchBlt(mcdc, x, y, w);
		//return TRUE;
	}
	memset(m_mp3Path, 0, sizeof(m_mp3Path));
	memcpy(m_mp3Path, mp3Path, strlen(mp3Path));
	return	StretchBlt(mcdc, x, y, w);

}

