

#include "stdafx.h"
#include "kavser\canlib.h"

#pragma warning(disable : 4996)

#ifdef _WINDOWS

	int myprintf(const char * _Format, ...);
	int myfprintf(__inout FILE * _File, const char * _Format, ...);
	void progress_Changed(unsigned value);


	#define printf  myprintf
	#define fprintf myfprintf
#else
	#define progress_Changed(x)
#endif


MsgBuffer::MsgBuffer()
{
	pWriteIndex = (VCI_CAN_OBJ*)m_buf;//��ʼ��ָ�룬дָ��ָ�򻺳���ͷ
	pReadIndex = (VCI_CAN_OBJ*)m_buf;//��ʼ��ָ�룬��ָ��ָ�򻺳���ͷ
	memset(m_buf,0,sizeof(VCI_CAN_OBJ)*MAX_PACKAGE);
	InitializeCriticalSection(&m_section);
	//InitializeCriticalSectionAndSpinCount(&m_section,2000);
}


MsgBuffer::~MsgBuffer()
{
	LeaveCriticalSection(&m_section);
	DeleteCriticalSection(&m_section);
}

void MsgBuffer::Push(VCI_CAN_OBJ& msgpg)
{
	EnterCriticalSection(&m_section);
	if (pWriteIndex < (VCI_CAN_OBJ*)m_buf + sizeof(m_buf)/sizeof(VCI_CAN_OBJ))
	{//��ֹԽ��
		*pWriteIndex = msgpg;//���뻺����
		pWriteIndex++;
	}
	else
	{
		pWriteIndex = (VCI_CAN_OBJ*)m_buf;
		*pWriteIndex = msgpg;//���뻺����
		pWriteIndex++;
	}
	LeaveCriticalSection(&m_section);
}


bool MsgBuffer::Pop(VCI_CAN_OBJ& msgpg)
{
	if(Readable())//�Ƿ��а��ɶ�
	{
		EnterCriticalSection(&m_section);
		if (pReadIndex > (VCI_CAN_OBJ*)m_buf + sizeof(m_buf)/sizeof(VCI_CAN_OBJ))
		{
			pReadIndex = (VCI_CAN_OBJ*)m_buf;
			msgpg = *pReadIndex;
			pReadIndex++;
		}
		else
		{
			msgpg = *pReadIndex;
			pReadIndex++;
		}
		LeaveCriticalSection(&m_section);
		return true;
	}
	return false;
}

void MsgBuffer::ClearBuff()
{
	EnterCriticalSection(&m_section);
	pWriteIndex = (VCI_CAN_OBJ*)m_buf;//��ʼ��ָ�룬дָ��ָ�򻺳���ͷ
	pReadIndex = (VCI_CAN_OBJ*)m_buf;//��ʼ��ָ�룬��ָ��ָ�򻺳���ͷ
	memset(m_buf,0,sizeof(VCI_CAN_OBJ)*MAX_PACKAGE);
	LeaveCriticalSection(&m_section);
}


//******************************************************************
// FUNCTION:	-	Readable
// RETURNS:		-	
// PARAMETERS:	-	
// COMMENTS:	-	�Ƿ������ݿɶ�
//******************************************************************
bool MsgBuffer::Readable()
{
	EnterCriticalSection(&m_section);
	if (pReadIndex != pWriteIndex)
	{
		LeaveCriticalSection(&m_section);
		return true;
	}
	LeaveCriticalSection(&m_section);
	return false;
}


CDeviceCAN::CDeviceCAN()
{
	m_deviceindex = 0;
	SetUSBCANConnectState(USBCANCLOSE);
	m_canindex = 1;//����CAN �˿�
}

CDeviceCAN::~CDeviceCAN()
{
	 HINSTANCE CANDll = NULL;
	 CloseCAN();
	CANDll = GetModuleHandle(TEXT("ControlCAN"));
	if(CANDll != NULL)
		FreeLibrary(CANDll);//�ͷŶ�̬����
	 CANDll = GetModuleHandle(TEXT("PCAN_USB"));
	if(CANDll != NULL)
		FreeLibrary(CANDll);//�ͷŶ�̬����
}

 int CDeviceCAN::GetCANBPS()
 {
	return m_canBaudrate;
 }

 void CDeviceCAN::SetCANBPS(int canbps)
 {
	m_canBaudrate = canbps;
 }

 int CDeviceCAN::GetUSBCANConnectState()
 {
	return m_USBCANState;
 }

 void CDeviceCAN::SetUSBCANConnectState(int connectstate)
 {
	m_USBCANState = connectstate;
 }

int CDeviceCAN::InitCAN()
{
	return STATUS_OK;
}

int CDeviceCAN::StartCAN()
{
	return STATUS_OK;
}
 
int CDeviceCAN::OpenCAN()
{	
	int stat;

	canInitializeLibrary();
	canhandle = canOpenChannel(0, canOPEN_EXCLUSIVE);
	if (canhandle < 0) {
		AfxMessageBox("�򿪶˿ڴ���");	
		return STATUS_ERR;
	}
	Baudrates PCANBaudrate;
	if(m_canBaudrate == 500)
	{
		PCANBaudrate = BAUD_500K;
	}
	else
	{
		PCANBaudrate = BAUD_250K;
	}
	stat = canSetBusParams(canhandle, PCANBaudrate, 0, 0, 1, 1, 0);

	if (stat < 0) 
	{
		AfxMessageBox("���ò�������");	
			return STATUS_ERR;
	}
		stat = canBusOn(canhandle);
	if (stat < 0) {
		AfxMessageBox("�������ߴ���");
			return STATUS_ERR;
	}
	return STATUS_OK;

}

int CDeviceCAN::ClearBuffer()
{
	return STATUS_ERR;
}

int CDeviceCAN::CloseCAN()
{
	(void)canBusOff(canhandle);
	canClose(canhandle);
	return STATUS_OK;
}


int CDeviceCAN::CAN_Send(VCI_CAN_OBJ *msg)
{
	canWrite(canhandle, msg->ID, msg->Data, msg->DataLen, canMSG_EXT);
	return STATUS_OK;
}

void  CDeviceCAN::StartReceive()
{
	 AfxBeginThread(CTPprotocol::ReceiveThread,this); 
}

int CDeviceCAN::GetMsg(st_can_msg& msg)
{
	VCI_CAN_OBJ canobj;
	if(DeviceCan.m_MsgBuffer.Pop(canobj))
	{	
		memcpy(&msg.u8_msgDat,canobj.Data,8);//ȡ����
		msg.u8_len = canobj.DataLen;
		msg.u32_id = canobj.ID;
		return 0;
	}
	return -1;
}
