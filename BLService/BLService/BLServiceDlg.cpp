
// BLServiceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BLService.h"
#include "BLServiceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma warning(disable : 4996)

#define printf myprintf
#define fprintf myfprintf


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBLServiceDlg �Ի���




CBLServiceDlg::CBLServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBLServiceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CBLServiceDlg::~CBLServiceDlg()
{

}


void CBLServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG, m_logCtrl);
	DDX_Control(pDX, IDC_COMBO_S19, m_cbfileS19);
	DDX_Control(pDX, IDC_COMBO_CANSPEED, m_cbCANSpeed);
	DDX_Control(pDX, IDC_CONNECT, m_btConnect);
	DDX_Control(pDX, IDC_BT_OPENS19, m_BtOpenS19);
	DDX_Control(pDX, IDC_FL_AUTOPROG, m_btAutoProg);
	DDX_Control(pDX, IDC_EDIT_IMAGESIZE, m_edbImageSize);
	DDX_Control(pDX, IDC_PB_PROGRESS, m_pbProgress);
}

BEGIN_MESSAGE_MAP(CBLServiceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_OPENS19, &CBLServiceDlg::OnBnClickedBtOpens19)
	ON_BN_CLICKED(IDC_CONNECT, &CBLServiceDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_FL_AUTOPROG, &CBLServiceDlg::OnBnClickedFlAutoprog)
END_MESSAGE_MAP()


// CBLServiceDlg ��Ϣ�������
BOOL CBLServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetConnectState(OFFLINE);
	EnableControlBtn(false);
	m_logCtrl.LimitText(0);
	m_pbProgress.SetRange(0,100);
	
	m_selFrom = 0;
	m_selTo = 0;
	m_cbCANSpeed.SetCurSel(0);//250 speed preselected
	m_edbImageSize.SetWindowTextA("0");
	m_edbImageSize.EnableWindow(false);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CBLServiceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBLServiceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBLServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBLServiceDlg::LogClear()
{
	m_logCtrl.SetSel(0, INT_MAX);
	m_logCtrl.ReplaceSel("");	
}

void CBLServiceDlg::LogResetReplaceLine()
{
	m_logCtrl.SetSel(INT_MAX, INT_MAX);
	m_logCtrl.ReplaceSel("");
	m_logCtrl.GetSel(m_selFrom, m_selTo);
}


void CBLServiceDlg::Log(LPCTSTR pszText)
{
	CString str = pszText;
	if("prog" == str.Mid(0,4))
	{
		CString numstr = str.Mid(4,str.GetLength()-4);
		int prog = _ttoi(numstr);
		ProgressChanged(prog);
		return;
	}

	int pos;
	str.Replace("\r\n", "\n");

	pos = str.Find('\r');

	if(pos >= 0)
	{
		// replace last line !
		str.Replace("\r", "");
		str.Replace("\n", "\r\n");
		m_logCtrl.SetSel(m_selFrom, INT_MAX);
		m_logCtrl.ReplaceSel(str);
	}
	else
	{
		// add new line !!
		str.Replace("\n", "\r\n");
		m_logCtrl.SetSel(INT_MAX, INT_MAX);
		pos = str.Find("\r\n");
		if(pos >= 0)
		{
			m_logCtrl.ReplaceSel("");
			m_logCtrl.GetSel(m_selFrom, m_selTo);
		}
		m_logCtrl.ReplaceSel(str);
	}
	//m_logCtrl.RedrawWindow();
}


void CBLServiceDlg::ProgressChanged(unsigned value)
{
	m_pbProgress.SetPos(value);
	m_pbProgress.RedrawWindow();
}


CBLServiceDlg* GetMainWnd()
{
	return (CBLServiceDlg*)AfxGetMainWnd();
}

extern "C" {
	int myfprintf(__inout FILE * _File, const char * _Format, ...)
	{
		
		char tmpStr[1024];
		va_list marker;

		(void)_File;
		

		va_start(marker, _Format);
		vsnprintf(tmpStr, 1024, _Format, marker);
		va_end(marker);

		GetMainWnd()->Log(tmpStr);

		return 0;
	}

	int myprintf(const char * _Format, ...)
	{
		char tmpStr[1024];
		va_list marker;

		va_start(marker, _Format);
		vsnprintf(tmpStr, 1024, _Format, marker);
		va_end(marker);

		
		GetMainWnd()->Log(tmpStr);

		return 0;		
	}

	void progress_Changed(unsigned value)
	{
		GetMainWnd()->ProgressChanged(value);
	}
}

void CBLServiceDlg::EnableInputs(bool enable)
{
	m_cbfileS19.EnableWindow(enable);
	m_cbCANSpeed.EnableWindow(enable);
	if(enable)
		m_edbImageSize.SetWindowTextA("0x0 B, 0 B");
}

void CBLServiceDlg::EnableControlBtn(bool enable)
{
	m_btErase.EnableWindow(enable);
	m_btAutoProg.EnableWindow(enable);
}
int CBLServiceDlg::GetConnectState()
{
	return m_Connect;
}
void CBLServiceDlg::SetConnectState(int connect)
{
	m_Connect = connect;
}

void CBLServiceDlg::OnBnClickedBtOpens19()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString fileName,pathName;
	int idx;
	char tmpstr[1024];
	char tmpstrCom[512];
	
	CFileDialog dlg(TRUE,NULL,NULL,(OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT),_T("BIN files (*.bin)|*.bin|All files (*.*)|*.*||"));
	
	if(dlg.DoModal() == IDOK)
	{
		pathName = dlg.GetPathName();
		fileName=dlg.GetFileName();
		m_cbfileS19.AddString((LPCTSTR)pathName);
		idx = m_cbfileS19.FindString(0,pathName);
		m_cbfileS19.SetCurSel(idx);
		
//================================================
		m_cbfileS19.GetLBText(m_cbfileS19.GetCurSel(),tmpstr);
		if(g_binDownload.read_bin(tmpstr) < 0)
		{
			printf("BIN�ļ��򿪴���\n");
			EnableInputs(true);
			return;
		}
		// sprintf(tmpstrCom, "0x%04x", S19::S19_GetCRC());
		// m_edbCheckSum.SetWindowTextA(tmpstrCom);
		if(g_binDownload.GetTotal() > 1024)
			sprintf(tmpstrCom, "%d KB", g_binDownload.GetTotal()/1024);
		else
			sprintf(tmpstrCom, "%d B",  g_binDownload.GetTotal());
		m_edbImageSize.SetWindowTextA(tmpstrCom);
	}
}

void CBLServiceDlg::OnBnClickedConnect()
{
	CBusy busy;
	
	ProgressChanged(0);
	
	if(true == m_ClickedConnectBtn)
	{
		m_ClickedConnectBtn = false;
		// Connected state => Quit and run MCU application
		LogResetReplaceLine();
		m_btConnect.SetWindowTextA("Connect");
		EnableInputs(true);
		EnableControlBtn(false);
		//LogClear();
		prog.SetConnectState(CONNECTSTEP_INIT);
		if(GetConnectState() == OFFLINE)
			return;
		
		prog.DisConnect();
		SetConnectState(OFFLINE);
		return;
	}
	else
	{
		char tmpstr[1024];
		CString Strtemp;
		
		m_ClickedConnectBtn = true;
		
		if(m_cbCANSpeed.GetLBText(m_cbCANSpeed.GetCurSel(),tmpstr) < 0)
		{
			EnableInputs(true);
			printf("�������ò����ʣ�\n");
			return;
		}
		DeviceCan.SetDevType(0);
		m_cbCANSpeed.GetWindowText(Strtemp);
		DeviceCan.SetCANBPS(atoi(Strtemp));
		// prog.Set_Module(m_cbModule.GetCurSel());
		LogResetReplaceLine();
		//EnvionmentInit
		if(prog.OpenCAN() == -1)
			return;
		
		EnableInputs(false);
		prog.StartReceive();
		LogResetReplaceLine();
		m_btConnect.SetWindowTextA("Disconnect");
		EnableControlBtn(false);
		AfxBeginThread(ConnectThread,this);
		return;
	}
}


UINT  CBLServiceDlg::ConnectThread(void *param)
{
	CBLServiceDlg * self = (CBLServiceDlg *)param;
	
	self->Log("�����У��������ն�.....................\n");
	
	while(1)
	{
		if(self->m_ClickedConnectBtn == false)
			break;
		
		if(prog.Connect_detect() == 0)//�����ɹ�
		{
			self->EnableControlBtn(true);
			self->Log("�����ɹ�,��ѡ�����\n");
			self->SetConnectState(ONLINE);
			break;
		}
		Sleep(100);
	}
	return 0;
}


void CBLServiceDlg::OnBnClickedFlErase()
{
	CBusy busy;
	if(GetConnectState() == OFFLINE)
		return;

	EnableControlBtn(false);
	
	AfxBeginThread(EraseThread,this);
	return;
}

UINT  CBLServiceDlg::EraseThread(void *param)
{
	return 0;
}


void CBLServiceDlg::OnBnClickedFlAutoprog()
{
	CBusy busy;
	if(GetConnectState() == OFFLINE)
		return;
	
	char tmpstr[1024];
	
	if(m_cbfileS19.GetLBText(m_cbfileS19.GetCurSel(),tmpstr) < 0)
	{
		EnableInputs(true);
		printf("���ȴ�BIN�ļ�!\n");
		return;
	}

	if(prog.check_image() < 0)
	{
		EnableInputs(true);
		return;
	}

	EnableControlBtn(false);

	AfxBeginThread(AutoprogThread,this);
	
	return;
}

UINT  CBLServiceDlg::AutoprogThread(void *param)
{	
	CBLServiceDlg * self = (CBLServiceDlg *)param;

	self->Log("׼������.....................\n");
	if(prog.preProgramming()==0)
	{
		self->Log("��ʼ����.....................\n");
		if(prog.programming()==0)
		{
			self->Log("�������.....................\n");
			prog.postProgramming();
			self->Log("���سɹ�!\n");
		}else{
			self->Log("����ʧ��!\n");
			goto OutAutoprog;
		}
	}else{
		self->Log("����ʧ��!\n");
		goto OutAutoprog;
	}

//==================================================	
	//prog.send_cmd(0x2F,OD_CMD,OD_CMD_OFFLINE,0);//���ͽ���ָ��
	self->ProgressChanged(100);
	self->LogResetReplaceLine();
	self->EnableControlBtn(true);
	self->OnBnClickedConnect();
	return 0;

OutAutoprog:	
	self->ProgressChanged(100);
	self->LogResetReplaceLine();
	self->EnableControlBtn(true);
	return 0;
}


//***********************************************************************************************/
CBusy::CBusy()
{
	m_pwc = new CWaitCursor();
	m_wnd = AfxGetMainWnd();
	
	//if(m_wnd)
//		m_wnd->EnableWindow(FALSE);
}

CBusy::~CBusy()
{
	if(m_pwc)
		delete m_pwc;

	if(m_wnd)
	{
		MSG msg;
		while(PeekMessage(&msg, m_wnd->m_hWnd, WM_MOUSEFIRST, WM_MOUSELAST, TRUE))
			;
		//m_wnd->EnableWindow(TRUE);
	}
}



