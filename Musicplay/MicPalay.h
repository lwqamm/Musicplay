#pragma once
#include "Stream.h"
class MicPalay
{
public:
	MicPalay();
	~MicPalay();
public:
	void paly_1();
	//播放
	void paly(char* fileName, int strl);
	//暂停
	void pause();
	//获取暂停状态
	int Getpause();
	void Setmini(BOOL rt);
	//0, 0, 341, 224     52,31,182,150
	//获取线程句柄
	HANDLE GetThread();
	//获取当前播放路径
	char* GetMp3Path();
	//获得播放列表数量
	int GetListSize();
	//外部系号获取歌曲
	CString Getdxliststr(int dx);
	//系号获取歌曲和外部获取歌曲全路径
	CString Getdxlist(int dx);
	//删除指定歌曲
	void    DeleteList(int dx);
	//继续
	void resume();
	//停止
	void stop();
	//退出程序
	void close();
	//上一首
	void Previous();
		//下一首
	void Next();
	//添加播放列表
	BOOL AddList(CString str);
	//设置操作句柄
	void SetHwnd(HWND hwnd);
	//画专辑
	void StretchBlt();
	//寻找MP3文件扩展名
	BOOL myGetExtension(IN PVOID strbuff);
	//获取MP3当前播放的下标
	int GetMp3dx();
	//清空列表
	 void  ClearList();
	 //画播放模式
	 void Stretchmodel(int x=815,int y=623);
	 //设置模式按钮颜色
	 void SetModelColor(BOOL ret=FALSE);
	 //设置播放模式
	 void SetModel(int ret=-1);
private:
	//歌名分割
	int GetMp3String(PVOID path, int* vea);
	BOOL main(char* fileName);

	//释放远程内存
	void VirFree(char* str);
	//文件是否存在
	int file_exist(char* str);
	//释放解码插件
	void Releaseplug();
	// 进程是否存在 返回1
	int process_exist(char* pid);
	//MP3转码
	BOOL MicPalay::ParseMP3(const char* mp3Path);

	//输出播放的错误信息
	void SetMp3Eroo(char* str, int dx);
	//字符串分割
	/*
	功能：播放列表字符串分割 “|”
	参数1：被分割的字符串
	参数2：分隔符 “|”
	参数3：列表参数
	*/
	int SplitString(LPCTSTR lpszStr, LPCTSTR lpszSplit, CStringArray& rArrString);
	//获取歌曲系号
	int Getlistdx(CString str);
	//获取随机数
	int __rand();
	//取字符内存长度
	int mystrlen(PVOID str);
	//创建播放列表文件
	void  CreatFileTxt();
	//读取播放列表
	CString GetList();
	//写入播放列表
	void SetList();
	
private:
	BOOL              m_exe = FALSE; //解码插件
	BOOL              m_dll = FALSE; //解码插件dll
	CStringArray      m_list; //播放器列表
	CString           m_list_Str;//从ini文件读取的字符
	int               m_listMax=0;//播放列表最大数量
	myimage           m_image; //专辑绘画
	HWND              m_hwnd;  //操作窗口句柄
	BOOL m_Last = FALSE;    // 上一首4
	BOOL m_Next = FALSE;	//下一首5
	BOOL m_inside = FALSE;  //内部调用
	int  m_int = 0;
	int  m_model = 0;       //播放顺序
	BOOL m_modelColor=FALSE;      //模式按钮颜色
	private:
		void mymode(CDC* dc,POINT* pt,int cons);
private:
	int          eroo = 0; //播放错误代码
	char         m_mp3Path[MAX_PATH] = { 0 };//正在播放的音乐路径
	int          m_listdx=-1;
	int          m_Previous=-1;//记录上一首
	BOOL         m_text0 = FALSE;//不带参数打开
	HANDLE       m_Thread = 0;//播放歌曲的线程
	BOOL         m_lock = TRUE;   //播放控制锁
	int          a1 = 0;
	int          a2 = 0;//记录上次滑块位置
	BOOL        m_pause = FALSE;//是否暂停
	BOOL        m_resume = FALSE;//是否继续
	BOOL        m_stop = FALSE;  //是否停止
	int        m_paly = 0;
	BOOL       m_close = FALSE;  //退出进程状态
	int        m_addres = 0;     //外部传进来的内存长度
	BOOL       m_mini = FALSE;   //最小化状态
	BOOL       m_ret = FALSE;
	
};

