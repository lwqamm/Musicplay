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
		char Title[80] = { 0 };          // TIT2: 标题，相当于ID3v1的Title
		char Artist[60] = { 0 };		  // TPE1： 艺术家相当于ID3v1的Artist 歌手
		char Album[60] = { 0 };		  // TALB： 专辑，相当于ID3v1的Album
		char Year[30] = { 0 };			  // TYER： 年代，相当于ID3v1的Year
		char Comment[MAX_PATH] = { 0 };		  // COMM： 注释，相当于ID3v1的Comment
		char TrackNum[30] = { 0 };		  // TRCK： 音轨（曲号），相当于ID3v1的Track
		char Portfolio[60] = { 0 };      // TBOS:         作品集部分 
		char Software[60] = { 0 };      //TSSE    [#TSEE Software  软件
		char   Genre[30] = { 0 };		  // TCON： 流派（风格），见表2  流派 Genre:  
		char  AlbumArtist[60] = { 0 };	  // 艺术家相当于ID3v1的Artist 唱片集艺术家 Artist1: 
		char  Composer[60] = { 0 };		  // TCOM： 作曲家 作曲者 Composer:
		char  OriginalArtist[60] = { 0 }; // TOPE： 原艺术家 原创艺术家
		char  Copyright[40] = { 0 };	  // TCOP： 版权          
		char  URL[128] = { 0 };			  // WXXX： 用户定义的URL链接
		char  Encoder[60] = { 0 };		  // TSSE： 编码使用的软件（硬件设置）        编码器
		char  Publisher[60] = { 0 };	  // WPUB： 出版商官方网页              出版商
		int BPM = 0;	  // 
		char* Pic = NULL;  //  APIC： 附加描述
		char reserved[126] = { 0 };	  //
	} TID3InfoEx, * PTID3InfoEx;//
	////////////////////////////////////////
#pragma pack(1)
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
		char   Size[4];     //帧内容及编码方式的合计长度，高位在前
		//char tcxaa;     //占位
		char Flags[2];    //标志，只使用了6位，详见表6，一般均＝0
		/*encode　 4　　　　帧内容所用的编码方式。许多帧没有此项
		帧内容　　　　　　至少 1 个字节*/
	}LABEL, * PLABEL;

	typedef  struct  _tagHeader
	{
		unsigned int sync : 11;                        //A 同步信息
		unsigned int version : 2;                      //B 版本
		unsigned int layer : 2;                           //C层
		unsigned int  protection : 1;                  // D CRC校验
		unsigned int bitrate_index : 4;              //C 位率
		unsigned int sampling_frequency : 2;         //D 采样频率 （单位：Hz）
		unsigned int padding : 1;                    //E 帧长调节
		unsigned int privateb : 1;                       //F 保留字
		unsigned int mode : 2;                         //G 声道模式
		unsigned int  extension : 2;                  //H 扩充模式
		unsigned int copyright : 1;                    // I 版权
		unsigned int original : 1;                      //J 原版标志
		unsigned int emphasis : 2;                     //K强调模式
	}HEADER, * PHEADER, * LPHEADER;

	typedef struct _MYMPEG
	{
		char MPEG = 0;              //MPEG 音频版本ID;
		char Layer = 0;             //00 - 保留 01 - Layer III 10 - Layer II 11 - Layer I
		char CRC = 0;               //校验位 0 - 紧跟帧头后有16位即2个字节用作CRC校验 1 - 没有校验
		int kbps = 0;               // 位率索引
		int Hz = 0;                 //采样频率（单位：Hz）
		char Fillbit = 0;           //填充位 0 – 没有填充  1 – 填充了一个额外的空位
		char Vocaltract = 0;        // 声道 00 立体声 01 联合立体声（立体声）10 双声道（立体声）11 单声道（单声）
		int Numberofsamples = 0;    //每帧采样数表示一帧中采样的个数

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
	int filelength(FILE* fp);//计算文件长度
	


private:
	TID3InfoEx c_Telb;
	char m_mp3Path[MAX_PATH * 2] = { 0 };
	LPVOID lpVoid = NULL;//默认图片
	int    lpVoidSize = 0;
	HWND m_hwnd;
	MYMPEG m_Mpeg = { 0 };
public:
	BOOL GetMp3ParSing(const char* mp3Path, CDC* mcdc, int x, int y, int w);
	BOOL StretchBlt(CDC* mcdc, int x, int y, int w);//画图
	//解析MP3
	BOOL ParseMP3(const char* mp3Path, PVOID* oubuff);

	BOOL myMp3ParSing(const char* mp3Path, PVOID* outbuff);
	//文件歌名解析
	int GetMp3String(int* vea);

private:
	//解析MP3过程函数
	BOOL ParseMP3_1(PVOID mp3Path, PVOID* oubuff);
	void SetMPEG(HEADER vea);
};

