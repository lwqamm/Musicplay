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
		 char* Title;          // TIT2: ���⣬�൱��ID3v1��Title
		 char* Artist;		  // TPE1�� �������൱��ID3v1��Artist ����
		 char* Album;		  // TALB�� ר�����൱��ID3v1��Album
		 char* Year;			  // TYER�� ������൱��ID3v1��Year
		 char* Comment;		  // COMM�� ע�ͣ��൱��ID3v1��Comment
		 char* TrackNum;		  // TRCK�� ���죨���ţ����൱��ID3v1��Track
		 char* Portfolio;      // TBOS:         ��Ʒ������ 
		 char* Software;      //TSSE    [#TSEE Software  ���
		 char* Genre;		  // TCON�� ���ɣ���񣩣�����2  ���� Genre:  
		 char* AlbumArtist;	  // �������൱��ID3v1��Artist ��Ƭ�������� Artist1: 
		 char* Composer;		  // TCOM�� ������ ������ Composer:
		 char* OriginalArtist; // TOPE�� ԭ������ ԭ��������
		 char* Copyright;	  // TCOP�� ��Ȩ          
		 char* URL;			  // WXXX�� �û������URL����
		 char* Encoder;		  // TSSE�� ����ʹ�õ������Ӳ�����ã�        ������
		 char* Publisher;	  // WPUB�� �����̹ٷ���ҳ              ������
		 int BPM;	  // 
		 char* Pic;  //  APIC�� ��������
		 char* reserved;	  //
	 } TID3InfoEx, * PTID3InfoEx;//
	 char* GetFrameInfoByEcoding(PVOID b, UCHAR conde);
	 ULONG GetJpeg(const PUCHAR p, int* size);
	 BOOL ReadFrame(PVOID fs, int size, OUT TID3InfoEx& Telb);
	 BOOL Mp3ParSing(const char* mp3Path, TID3InfoEx& Telb);
	 CStream GetCStream(PVOID pData, int nSize);
	 int  IsImageByHead(const PVOID pData);
	 void Deletemp3();
	 int filelength(FILE* fp);//�����ļ�����
private:
	TID3InfoEx c_Telb;
	char m_mp3Path[MAX_PATH * 2] = {0};
	LPVOID lpVoid = NULL;//Ĭ��ͼƬ
	int    lpVoidSize = 0;
public:
	BOOL GetMp3ParSing(const char* mp3Path, CDC* mcdc,int x,int y,int w);
	BOOL StretchBlt(CDC* mcdc, int x, int y, int w);//��ͼ
};
