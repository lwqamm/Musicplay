#include "pch.h"
#ifdef __ATLMISC_H__   //如果包含了atlmisc.h
#define __ATLTYPES_H__	//则不编译atltypes.h
#endif
#include <atlimage.h>
#include "Stream.h"
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
////////////////////////////////////////

/// //////////////////////
typedef struct _ID3V2Header
{
	unsigned  char         Header[3]; // ID3V2.3标识符"ID3"的Ascii码，否则认为没有ID3V2.3
	unsigned  char         Ver;       // 版本号,＝03
	unsigned  char         Revision;  // 副版本号，＝00
	unsigned  char         flag;      // 标志字节，一般没意义，＝00
	unsigned  char         Size[4];      // 标签内容长度，高位在前，不包括标签头的10个字节
}ID3V2Header, * PID3V2Header;
///////////////////////////////////
typedef struct _LABEL
{
	UINT32 FrameID;  //帧标识符的Ascii码，常用标识符的意义见表5
	UINT32 Size;     //帧内容及编码方式的合计长度，高位在前
	USHORT Flags;    //标志，只使用了6位，详见表6，一般均＝0
	/*encode　 4　　　　帧内容所用的编码方式。许多帧没有此项
	帧内容　　　　　　至少 1 个字节*/
}LABEL, * PLABEL;
////////////////////////////
wchar_t* utf_8ToUnicode(char* u8s)
{
	int wcsLen = MultiByteToWideChar(CP_UTF8, NULL, u8s, strlen(u8s), NULL, NULL);
	wchar_t* wcString = new wchar_t[wcsLen + 1];
	memset(wcString, 0, wcsLen + 1);
	MultiByteToWideChar(CP_UTF8, NULL, u8s, strlen(u8s), wcString, wcsLen);
	wcString[wcsLen] = '\0';
	return wcString;
}

char* unicodeToAnsi(wchar_t* wcString)
{
	int len = WideCharToMultiByte(CP_ACP, NULL, wcString, -1, NULL, NULL, NULL, NULL);
	char* str = new char[len];
	memset(str, 0, len);
	WideCharToMultiByte(CP_ACP, NULL, wcString, -1, str, len, NULL, NULL);
	return str;
}

char* Utf8ToAnsi(char* u8s)
{
	int len;
	int tmp_len;
	char* unicode_data = NULL;
	char* ansi_data = NULL;
	len = strlen(u8s);
	tmp_len = MultiByteToWideChar(CP_UTF8, 0, u8s, len, (LPWSTR)&unicode_data, NULL) * 2;
	unicode_data = new char(tmp_len);
	memset(unicode_data, 0, tmp_len);
	len = MultiByteToWideChar(CP_UTF8, 0, u8s, len, (LPWSTR)&unicode_data, tmp_len);
	if (len < 1)
	{
		delete[] unicode_data;
		return 0;
	}
	tmp_len = WideCharToMultiByte(936, 0, (LPCWCH)unicode_data, len, ansi_data, 0, 0, 0);
	ansi_data = new char(tmp_len);
	memset(ansi_data, 0, tmp_len);
	WideCharToMultiByte(936, 0, (LPCWCH)unicode_data, len, ansi_data, tmp_len, 0, 0);
	return ansi_data;
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
	c_Telb = {0};
	if (lpVoid == NULL)
	{
		HRSRC hRsrc = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_JPEG1), CString("JPEG"));
		//LPTSTR szBuffer = new TCHAR[1024];//这里是定义缓冲区大小

		if (hRsrc == NULL)
		{
			//FreeRes_ShowError("FindResource");
			return ;
		}
		DWORD dwSize = ::SizeofResource(NULL, hRsrc);
		if (0 >= dwSize)
		{
			//FreeRes_ShowError("SizeofResource");
			return ;
		}
		
		HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
		if (NULL == hGlobal)
		{
			//FreeRes_ShowError("LoadResource");
			return ;
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

		str = unicodeToAnsi((wchar_t*)b);

		if (strlen(str) <= 0)
		{
			return 0;
		}

		break;
	case 1:
		str = unicodeToAnsi((wchar_t*)b);

		if (strlen(str) <= 0)
		{
			return 0;
		}
		break;
	case 2:
		str = unicodeToAnsi((wchar_t*)b);

		if (strlen(str) <= 0)
		{
			return 0;
		}
		break;
	case 3:
		str = Utf8ToAnsi((char*)b);
		if (strlen(str) <= 0)
		{
			return 0;
		}

		break;
	}
	if (!str)
	{
		return 0;
	}
	int len = strlen(str);
	if (len <= 0)
	{
		return 0;
	}
	memcpy(str, str + 1, len - 1);
	str[len - 1] = 0;
	return str;
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
		PUCHAR p = (PUCHAR)&label.Size;
		frmSize = p[0] * 0x1000000 + p[1] * 0x10000 + p[2] * 0x100 + p[3];
		if (frmSize == 0)
		{
			//就说明真的没有信息了
			break;
		}
		//bFrame 用来保存帧的信息
		PUCHAR bFrame = new UCHAR[frmSize];
		memset(bFrame, 0, frmSize);
		if (!bFrame)
		{
			return FALSE;
		}
		asi += 10;
		memcpy(bFrame, (PVOID)((ULONG64)fs + asi + 1), frmSize - 1);
		char* str = NULL;


		asi += frmSize;//TPE1
		if (strcmp(FramID, "TIT2") == 0)//歌曲名
		{
			ret = TRUE;
			str = GetFrameInfoByEcoding(bFrame, label.Flags);
			Telb.Title = str;

		}
		////////////////////////////////
		if (strcmp(FramID, "TPE1") == 0)//歌手
		{
			ret = TRUE;
			str = GetFrameInfoByEcoding(bFrame, label.Flags);
			Telb.Artist = str;
		}
		////////////////////////////////
		if (strcmp(FramID, "TALB") == 0)//“TALB” 专辑
		{
			ret = TRUE;
			str = GetFrameInfoByEcoding(bFrame, label.Flags);
			Telb.Album = str;
		}
		////////////////////////////////
		if (strcmp(FramID, "TRCK") == 0)//“TRCK” ' 音轨
		{
			ret = TRUE;
			str = GetFrameInfoByEcoding(bFrame, label.Flags);
			Telb.TrackNum = str;
		}
		////////////////////////////////
		if (strcmp(FramID, "TBOS") == 0)//“TBOS”  ' 作品集部分
		{
			ret = TRUE;
			str = GetFrameInfoByEcoding(bFrame, label.Flags);
			Telb.Portfolio = str;
		}
		////////////////////////////////
		if (strcmp(FramID, "COMM") == 0)//“COMM”  ' 注释 Comment
		{
			ret = TRUE;
			str = GetFrameInfoByEcoding(bFrame, label.Flags);
			Telb.Comment = str;
		}
		////////////////////////////////
		if (strcmp(FramID, "TSSE") == 0)//“TSSE” ' 编码使用的软件（硬件设置）
		{
			ret = TRUE;
			str = GetFrameInfoByEcoding(bFrame, label.Flags);
			Telb.Software = str;
		}
		////////////////////////////////
		if (strcmp(FramID, "APIC") == 0)//“APIC”  ' 附加描述 图片数据在此处
		{
			ret = TRUE;
			//Telb.Pic = (char*)bFrame;
			frmSize -= 1;
			ULONG	piclen = GetJpeg(bFrame, &frmSize);
			if (piclen == 0)
			{
				continue;
			}

			Telb.BPM = piclen;
			Telb.Pic = (char*)bFrame + frmSize;

			continue;
		}

		delete bFrame;
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
	ret=fread(fileBuff, fileSize, 1, fp);
	fclose(fp);
	*(fileBuff + fileSize - 1) = '\0';
	if (ret)
	{
		ID3V2Header Header = { 0 };
		memcpy(&Header, fileBuff, sizeof(ID3V2Header));
		ULONG size = (ULONG)(Header.Size[0] & 0x7F) * 0x200000 + (Header.Size[1] & 0x7F) * 0x400 + (Header.Size[2] & 0x7F) * 0x80 + (Header.Size[3] & 0x7F);
		UCHAR mp3ID[] = "ID3";
		if (memcmp((UCHAR*)fileBuff, mp3ID, 3) == 0 && size > 0)
		{
			//printf(" size=%d\n", size);
			UCHAR* TBuff = new UCHAR[fileSize - sizeof(ID3V2Header)];    //创建缓冲区
			if (!TBuff) { delete[] fileBuff; return FALSE; }
			memcpy(TBuff, fileBuff + sizeof(ID3V2Header), fileSize - sizeof(ID3V2Header));

			free(fileBuff);

			if (!ReadFrame(TBuff, size, Telb))
			{
				delete TBuff;
				return FALSE;
			}
			delete TBuff;
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

BOOL myimage::StretchBlt(CDC* mcdc, int x, int y, int w)
{
	int lx = IsImageByHead(c_Telb.Pic);
	CImage image;
	if (lx==1)
	{
		CStream stream = GetCStream(c_Telb.Pic, c_Telb.BPM);
		image.Load(&stream);
		lx = 2;
	}
	else
	{
		if (lpVoid==NULL|| lpVoidSize==0)
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
		int w1 = 0, h1 = 0;
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
		mcdc->StretchBlt(x, y, w, h, &memDC, 0, 0, w1, h1, SRCCOPY);
		// 清理
		memDC.SelectObject(oldBitmap);
		memBitmap.DeleteObject();
		memDC.DeleteDC();
	}

	if (c_Telb.Title!=NULL||c_Telb.Artist!=NULL)
	{	///画歌名
		LOGFONT lf;
		CFont fontTemp, * pFontOld;

		mcdc->GetCurrentFont()->GetLogFont(&lf);
		lstrcpy(lf.lfFaceName, _T("宋体"));
		lf.lfWidth = 0;
		lf.lfWeight = FW_HEAVY;
		lf.lfHeight = 0xE;
		fontTemp.CreateFontIndirect(&lf);      //创建字体
		pFontOld = mcdc->SelectObject(&fontTemp);
		mcdc->SetTextColor(RGB(0x0E, 0x0E, 0xFF));
		mcdc->SetBkMode(0);
		char ch[MAX_PATH] = { 0 };
		memcpy(ch, c_Telb.Artist, strlen(c_Telb.Artist));
		ch[strlen(ch)] = '-';
		memcpy(ch + strlen(ch), c_Telb.Title, strlen(c_Telb.Title));
		mcdc->TextOut(120, 820, ch, strlen(ch));
	}
	RestoreDC((HDC)mcdc, 0);
	return TRUE;
}

void myimage::Deletemp3()
{
	if (c_Telb.Album)
	{
		delete c_Telb.Album;
	}
	if (c_Telb.AlbumArtist)
	{
		delete c_Telb.AlbumArtist;
	}
	if (c_Telb.Artist)
	{
		delete c_Telb.Artist;
	}
	if (c_Telb.BPM)
	{
	  c_Telb.BPM=0;
	}
	if (c_Telb.Comment)
	{
		delete c_Telb.Comment;
	}
	if (c_Telb.Composer)
	{
		delete c_Telb.Composer;
	}
	if (c_Telb.Copyright)
	{
		delete c_Telb.Copyright;
	}
	if (c_Telb.Encoder)
	{
		delete c_Telb.Encoder;
	}
	if (c_Telb.Genre)
	{
		delete c_Telb.Genre;
	}
	if (c_Telb.OriginalArtist)
	{
		delete c_Telb.OriginalArtist;
	}
	if (c_Telb.Pic)
	{
		delete c_Telb.Pic;
	}
	if (c_Telb.Portfolio)
	{
		delete c_Telb.Portfolio;
	}
	if (c_Telb.Publisher)
	{
		delete c_Telb.Publisher;
	}
	if (c_Telb.reserved)
	{
		delete c_Telb.reserved;
	}
	if (c_Telb.Software)
	{
		delete c_Telb.Software;
	}
	if (c_Telb.Title)
	{
		delete c_Telb.Title;
	}
	if (c_Telb.TrackNum)
	{
		delete c_Telb.TrackNum;
	}
	if (c_Telb.URL)
	{
		delete c_Telb.URL;
	}
	if (c_Telb.Year)
	{
		delete c_Telb.Year;
	}
	return;
}

BOOL myimage::GetMp3ParSing(const char* mp3Path, CDC* mcdc, int x, int y, int w)
{
	if (strlen(mp3Path) <= 0)
	{
		return FALSE;
	}
	if (strcmp(mp3Path,m_mp3Path)==0)
	{
		return StretchBlt(mcdc,x, y,  w);
		
	}
	Deletemp3();
	if (Mp3ParSing(mp3Path,c_Telb) )
	{
		memcpy(m_mp3Path, mp3Path,strlen(mp3Path));
		return	StretchBlt(mcdc, x, y, w);
		//return TRUE;
	}
	return 0;
}
