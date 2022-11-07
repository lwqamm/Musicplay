#pragma once
#pragma once
#include <windows.h> 
class CStream : public IStream
{
public:
	// 
	// IUnknown members 
	// 
	HRESULT __stdcall QueryInterface(REFIID iid, void** ppvObject);
	ULONG   __stdcall AddRef(void);
	ULONG   __stdcall Release(void);

	// 
	// ISequentialStream members 
	// 
	HRESULT __stdcall Read(void* pv, ULONG cb, ULONG* pcbRead);
	HRESULT __stdcall Write(const void* pv, ULONG cb, ULONG* pcbWritten);


	// 
	// IStream members 
	// 
	HRESULT __stdcall Seek(LARGE_INTEGER  dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition);
	HRESULT __stdcall SetSize(ULARGE_INTEGER libNewSize);
	HRESULT __stdcall CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten);
	HRESULT __stdcall Commit(DWORD grfCommitFlags);
	HRESULT __stdcall Revert(void);
	HRESULT __stdcall LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	HRESULT __stdcall UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	HRESULT __stdcall Stat(STATSTG* pstatstg, DWORD grfStatFlag);
	HRESULT __stdcall Clone(IStream** ppstm);


	// 
	// Constructor / Destructor 
	// 
	CStream(BYTE* pData, ULONG nSize);
	~CStream();

private:
	// 
	// private members and functions 
	// 
	LONG            m_lRefCount;


	STATSTG         m_statstg;          // each IStream needs one of these 
	ULARGE_INTEGER  m_nOffset;          // offset within the stream 
	ULARGE_INTEGER  m_nLength;          // length of the stream 
	BYTE* m_pData;            // stream data from 
};

 class myimage 
{
public:
	 myimage();
	 ~myimage();
private:
	 typedef struct _TID3InfoEx {
		 char* Title;          // TIT2: 标题，相当于ID3v1的Title
		 char* Artist;		  // TPE1： 艺术家相当于ID3v1的Artist 歌手
		 char* Album;		  // TALB： 专辑，相当于ID3v1的Album
		 char* Year;			  // TYER： 年代，相当于ID3v1的Year
		 char* Comment;		  // COMM： 注释，相当于ID3v1的Comment
		 char* TrackNum;		  // TRCK： 音轨（曲号），相当于ID3v1的Track
		 char* Portfolio;      // TBOS:         作品集部分 
		 char* Software;      //TSSE    [#TSEE Software  软件
		 char* Genre;		  // TCON： 流派（风格），见表2  流派 Genre:  
		 char* AlbumArtist;	  // 艺术家相当于ID3v1的Artist 唱片集艺术家 Artist1: 
		 char* Composer;		  // TCOM： 作曲家 作曲者 Composer:
		 char* OriginalArtist; // TOPE： 原艺术家 原创艺术家
		 char* Copyright;	  // TCOP： 版权          
		 char* URL;			  // WXXX： 用户定义的URL链接
		 char* Encoder;		  // TSSE： 编码使用的软件（硬件设置）        编码器
		 char* Publisher;	  // WPUB： 出版商官方网页              出版商
		 int BPM;	  // 
		 char* Pic;  //  APIC： 附加描述
		 char* reserved;	  //
	 } TID3InfoEx, * PTID3InfoEx;//
	 char* GetFrameInfoByEcoding(PVOID b, UCHAR conde);
	 ULONG GetJpeg(const PUCHAR p, int* size);
	 BOOL ReadFrame(PVOID fs, int size, OUT TID3InfoEx& Telb);
	 BOOL Mp3ParSing(const char* mp3Path, TID3InfoEx& Telb);
	 CStream GetCStream(PVOID pData, int nSize);
	 int  IsImageByHead(const PVOID pData);
	 void Deletemp3();
	 int filelength(FILE* fp);//计算文件长度
private:
	TID3InfoEx c_Telb;
	char m_mp3Path[MAX_PATH * 2] = {0};
	LPVOID lpVoid = NULL;//默认图片
	int    lpVoidSize = 0;
public:
	BOOL GetMp3ParSing(const char* mp3Path, CDC* mcdc,int x,int y,int w);
	BOOL StretchBlt(CDC* mcdc, int x, int y, int w);//画图
};
