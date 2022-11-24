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
	void Setmyimage(HWND hwnd);
	~myimage();
private:
	typedef struct _TID3InfoEx {
		char Title[80] = { 0 };          // TIT2: ���⣬�൱��ID3v1��Title
		char Artist[60] = { 0 };		  // TPE1�� �������൱��ID3v1��Artist ����
		char Album[60] = { 0 };		  // TALB�� ר�����൱��ID3v1��Album
		char Year[30] = { 0 };			  // TYER�� ������൱��ID3v1��Year
		char Comment[MAX_PATH] = { 0 };		  // COMM�� ע�ͣ��൱��ID3v1��Comment
		char TrackNum[30] = { 0 };		  // TRCK�� ���죨���ţ����൱��ID3v1��Track
		char Portfolio[60] = { 0 };      // TBOS:         ��Ʒ������ 
		char Software[60] = { 0 };      //TSSE    [#TSEE Software  ���
		char   Genre[30] = { 0 };		  // TCON�� ���ɣ���񣩣�����2  ���� Genre:  
		char  AlbumArtist[60] = { 0 };	  // �������൱��ID3v1��Artist ��Ƭ�������� Artist1: 
		char  Composer[60] = { 0 };		  // TCOM�� ������ ������ Composer:
		char  OriginalArtist[60] = { 0 }; // TOPE�� ԭ������ ԭ��������
		char  Copyright[40] = { 0 };	  // TCOP�� ��Ȩ          
		char  URL[128] = { 0 };			  // WXXX�� �û������URL����
		char  Encoder[60] = { 0 };		  // TSSE�� ����ʹ�õ������Ӳ�����ã�        ������
		char  Publisher[60] = { 0 };	  // WPUB�� �����̹ٷ���ҳ              ������
		int BPM = 0;	  // 
		char* Pic = NULL;  //  APIC�� ��������
		char reserved[126] = { 0 };	  //
	} TID3InfoEx, * PTID3InfoEx;//
	////////////////////////////////////////
#pragma pack(1)
/// //////////////////////
	typedef struct _ID3V2Header
	{
		unsigned  char         Header[3]; // ID3V2.3��ʶ��"ID3"��Ascii�룬������Ϊû��ID3V2.3
		unsigned  char         Ver;       // �汾��,��03
		unsigned  char         Revision;  // ���汾�ţ���00
		unsigned  char         flag;      // ��־�ֽڣ�һ��û���壬��00
		unsigned  char         Size[4];      // ��ǩ���ݳ��ȣ���λ��ǰ����������ǩͷ��10���ֽ�
	}ID3V2Header, * PID3V2Header;
	///////////////////////////////////
	typedef struct _LABEL
	{
		UINT32 FrameID;  //֡��ʶ����Ascii�룬���ñ�ʶ�����������5
		char   Size[4];     //֡���ݼ����뷽ʽ�ĺϼƳ��ȣ���λ��ǰ
		//char tcxaa;     //ռλ
		char Flags[2];    //��־��ֻʹ����6λ�������6��һ�����0
		/*encode�� 4��������֡�������õı��뷽ʽ�����֡û�д���
		֡���ݡ��������������� 1 ���ֽ�*/
	}LABEL, * PLABEL;

	typedef  struct  _tagHeader
	{
		unsigned int sync : 11;                        //A ͬ����Ϣ
		unsigned int version : 2;                      //B �汾
		unsigned int layer : 2;                           //C��
		unsigned int  protection : 1;                  // D CRCУ��
		unsigned int bitrate_index : 4;              //C λ��
		unsigned int sampling_frequency : 2;         //D ����Ƶ�� ����λ��Hz��
		unsigned int padding : 1;                    //E ֡������
		unsigned int privateb : 1;                       //F ������
		unsigned int mode : 2;                         //G ����ģʽ
		unsigned int  extension : 2;                  //H ����ģʽ
		unsigned int copyright : 1;                    // I ��Ȩ
		unsigned int original : 1;                      //J ԭ���־
		unsigned int emphasis : 2;                     //Kǿ��ģʽ
	}HEADER, * PHEADER, * LPHEADER;

	typedef struct _MYMPEG
	{
		char MPEG = 0;              //MPEG ��Ƶ�汾ID;
		char Layer = 0;             //00 - ���� 01 - Layer III 10 - Layer II 11 - Layer I
		char CRC = 0;               //У��λ 0 - ����֡ͷ����16λ��2���ֽ�����CRCУ�� 1 - û��У��
		int kbps = 0;               // λ������
		int Hz = 0;                 //����Ƶ�ʣ���λ��Hz��
		char Fillbit = 0;           //���λ 0 �C û�����  1 �C �����һ������Ŀ�λ
		char Vocaltract = 0;        // ���� 00 ������ 01 ��������������������10 ˫��������������11 ��������������
		int Numberofsamples = 0;    //ÿ֡��������ʾһ֡�в����ĸ���

	}MYMPEG, * PMYMPEG;
#pragma pack()
	////////////////////////////
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
	char m_mp3Path[MAX_PATH * 2] = { 0 };
	LPVOID lpVoid = NULL;//Ĭ��ͼƬ
	int    lpVoidSize = 0;
	HWND m_hwnd;
	MYMPEG m_Mpeg = { 0 };
public:
	BOOL GetMp3ParSing(const char* mp3Path, CDC* mcdc, int x, int y, int w);
	BOOL StretchBlt(CDC* mcdc, int x, int y, int w);//��ͼ
	//����MP3
	BOOL ParseMP3(const char* mp3Path, PVOID* oubuff);

	BOOL myMp3ParSing(const char* mp3Path, PVOID* outbuff);
	//�ļ���������
	int GetMp3String(int* vea);

private:
	//����MP3���̺���
	BOOL ParseMP3_1(PVOID mp3Path, PVOID* oubuff);
	void SetMPEG(HEADER vea);
};

