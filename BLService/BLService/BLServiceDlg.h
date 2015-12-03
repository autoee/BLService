
// BLServiceDlg.h : ͷ�ļ�
//

#pragma once

// CBLServiceDlg �Ի���
class CBLServiceDlg : public CDialogEx
{
// ����
public:
	CBLServiceDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CBLServiceDlg();//��������
// �Ի�������
	enum { IDD = IDD_BLSERVICE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void Log(LPCTSTR pszText);
	void LogResetReplaceLine();
	void ProgressChanged(unsigned value);
	void LogClear();
	void EnableInputs(bool enable);
	void EnableControlBtn(bool enable);
	bool m_ClickedConnectBtn;//�����������±�־
	enum {OFFLINE,ONLINE};
	int m_Connect;
	int m_selFrom;
	int m_selTo;
public:
	CEdit m_logCtrl;
	CComboBox m_cbfileS19;
	// CComboBox m_cbCANType;
	CComboBox m_cbCANSpeed;
	// CComboBox m_cbModule;
	CButton m_btConnect;
	CButton m_BtOpenS19;
	CButton m_btErase;
	CButton m_btAutoProg;
	CEdit m_edbImageSize;
	CProgressCtrl m_pbProgress;
public:
	afx_msg void OnBnClickedBtOpens19();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedFlErase();
	afx_msg void OnBnClickedFlAutoprog();
	afx_msg void OnCbnSelchangeComboCanport();
	afx_msg void OnCbnSelchangeComboCanspeed();
	int GetConnectState();
	void SetConnectState(int connect);
	static UINT  ConnectThread(void *param);
	static UINT  EraseThread(void *param);
	static UINT  AutoprogThread(void *param);
};

class CBusy
{
protected:
	CWaitCursor* m_pwc;
	CWnd* m_wnd;

public:
	CBusy();
	~CBusy();
};


extern "C" {
	int myprintf(const char * _Format, ...);
	int myfprintf(__inout FILE * _File, const char * _Format, ...);
	void progress_Changed(unsigned value);
}
