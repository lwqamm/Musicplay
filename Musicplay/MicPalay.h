#pragma once
#include "Stream.h"
class MicPalay
{
public:
	MicPalay();
	~MicPalay();
public:
	void paly_1();
	//����
	void paly(char* fileName, int strl);
	//��ͣ
	void pause();
	//��ȡ��ͣ״̬
	int Getpause();
	void Setmini(BOOL rt);
	//0, 0, 341, 224     52,31,182,150
	//��ȡ�߳̾��
	HANDLE GetThread();
	//��ȡ��ǰ����·��
	char* GetMp3Path();
	//��ò����б�����
	int GetListSize();
	//�ⲿϵ�Ż�ȡ����
	CString Getdxliststr(int dx);
	//ϵ�Ż�ȡ�������ⲿ��ȡ����ȫ·��
	CString Getdxlist(int dx);
	//ɾ��ָ������
	void    DeleteList(int dx);
	//����
	void resume();
	//ֹͣ
	void stop();
	//�˳�����
	void close();
	//��һ��
	void Previous();
		//��һ��
	void Next();
	//��Ӳ����б�
	BOOL AddList(CString str);
	//���ò������
	void SetHwnd(HWND hwnd);
	//��ר��
	void StretchBlt();
	//Ѱ��MP3�ļ���չ��
	BOOL myGetExtension(IN PVOID strbuff);
	//��ȡMP3��ǰ���ŵ��±�
	int GetMp3dx();
	//����б�
	 void  ClearList();
	 //������ģʽ
	 void Stretchmodel(int x=815,int y=623);
	 //����ģʽ��ť��ɫ
	 void SetModelColor(BOOL ret=FALSE);
	 //���ò���ģʽ
	 void SetModel(int ret=-1);
private:
	//�����ָ�
	int GetMp3String(PVOID path, int* vea);
	BOOL main(char* fileName);

	//�ͷ�Զ���ڴ�
	void VirFree(char* str);
	//�ļ��Ƿ����
	int file_exist(char* str);
	//�ͷŽ�����
	void Releaseplug();
	// �����Ƿ���� ����1
	int process_exist(char* pid);
	//MP3ת��
	BOOL MicPalay::ParseMP3(const char* mp3Path);

	//������ŵĴ�����Ϣ
	void SetMp3Eroo(char* str, int dx);
	//�ַ����ָ�
	/*
	���ܣ������б��ַ����ָ� ��|��
	����1�����ָ���ַ���
	����2���ָ��� ��|��
	����3���б����
	*/
	int SplitString(LPCTSTR lpszStr, LPCTSTR lpszSplit, CStringArray& rArrString);
	//��ȡ����ϵ��
	int Getlistdx(CString str);
	//��ȡ�����
	int __rand();
	//ȡ�ַ��ڴ泤��
	int mystrlen(PVOID str);
	//���������б��ļ�
	void  CreatFileTxt();
	//��ȡ�����б�
	CString GetList();
	//д�벥���б�
	void SetList();
	
private:
	BOOL              m_exe = FALSE; //������
	BOOL              m_dll = FALSE; //������dll
	CStringArray      m_list; //�������б�
	CString           m_list_Str;//��ini�ļ���ȡ���ַ�
	int               m_listMax=0;//�����б��������
	myimage           m_image; //ר���滭
	HWND              m_hwnd;  //�������ھ��
	BOOL m_Last = FALSE;    // ��һ��4
	BOOL m_Next = FALSE;	//��һ��5
	BOOL m_inside = FALSE;  //�ڲ�����
	int  m_int = 0;
	int  m_model = 0;       //����˳��
	BOOL m_modelColor=FALSE;      //ģʽ��ť��ɫ
	private:
		void mymode(CDC* dc,POINT* pt,int cons);
private:
	int          eroo = 0; //���Ŵ������
	char         m_mp3Path[MAX_PATH] = { 0 };//���ڲ��ŵ�����·��
	int          m_listdx=-1;
	int          m_Previous=-1;//��¼��һ��
	BOOL         m_text0 = FALSE;//����������
	HANDLE       m_Thread = 0;//���Ÿ������߳�
	BOOL         m_lock = TRUE;   //���ſ�����
	int          a1 = 0;
	int          a2 = 0;//��¼�ϴλ���λ��
	BOOL        m_pause = FALSE;//�Ƿ���ͣ
	BOOL        m_resume = FALSE;//�Ƿ����
	BOOL        m_stop = FALSE;  //�Ƿ�ֹͣ
	int        m_paly = 0;
	BOOL       m_close = FALSE;  //�˳�����״̬
	int        m_addres = 0;     //�ⲿ���������ڴ泤��
	BOOL       m_mini = FALSE;   //��С��״̬
	BOOL       m_ret = FALSE;
	
};

