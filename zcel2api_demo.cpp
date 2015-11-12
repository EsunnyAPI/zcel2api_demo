#include "zcel2api/ZceLevel2ApiInterface.h"
#include <iostream>
#include <windows.h>
using namespace std;


//====================================================
//
// ������������Ŀ
//
//Level2���� IP��ַ
const char* ZCEL2_SERVICE_IP         = "";
//Level2���� �˿�
const int   ZCEL2_SERVICE_PORT       = 0; 
//Level2���� �û���
const char* ZCEL2_SERVICE_USERNAME   = "���滻Ϊ�����û���";
//Level2���� ����
const char* ZCEL2_SERVICE_PASSWORD   = "���滻Ϊ��������";
//====================================================

// ʵ��IZCEL2QuoteNotify�ӿڣ����ڽ���Level2API��֪ͨ
class ZCEL2Quote  : public IZCEL2QuoteNotify
{
public:

	virtual void ZCEL2QUOTE_CALL OnRspLogin(const ZCEL2QuotRspLoginField& loginMsg)
	{
		if(loginMsg.ErrorCode == ZCEErrCode_Success)
		{
			cout << "��½�ɹ�, ʱ�䣺" << loginMsg.TimeStamp << endl;
		}
		else
		{	
			cout << "��½ʧ��, ԭ��" << loginMsg.ErrorCode << " , " << loginMsg.ErrorText << endl;
		}
	}
	
	virtual void ZCEL2QUOTE_CALL OnRecvQuote(const ZCEL2QuotSnapshotField& snapshot)
	{
		cout << "�������:" 
		<< snapshot.TimeStamp << ","
		<< snapshot.ContractID << ","
		<< snapshot.LastPrice << ","
		<< snapshot.BidPrice[0] << ","
		<< snapshot.AskPrice[0] << ","
		<< snapshot.BidLot[0] << ","
		<< snapshot.AskLot[0] << ","
		<< snapshot.TotalVolume << ","
		<< snapshot.OpenInterest
		<< endl;
	}

	virtual void ZCEL2QUOTE_CALL OnRecvCmbQuote(const ZCEL2QuotCMBQuotField& snapshot)
	{	
		cout << "����������:" 
		<< snapshot.TimeStamp << ","
		<< snapshot.ContractID << ","
		<< snapshot.BidPrice << ","
		<< snapshot.AskPrice << ","
		<< snapshot.BidLot << ","
		<< snapshot.AskLot << ","
		<< endl;
	}

	virtual void ZCEL2QUOTE_CALL OnConnectClose()
	{
		cout << "���ӶϿ�" << endl;
	}

};



int main()
{
	//����notify ʵ��
	ZCEL2Quote notify;

	//����ZCEL2QuotClientʵ��
	IZCEL2QuoteClient* pZCEL2QuoteClient = CreateZCEL2QuotClient(&notify);
	if(NULL == pZCEL2QuoteClient)
	{
		cerr << "ZCEL2API����ʧ��" <<endl;
		return -1;
	}

	//�鿴�汾��Ϣ
	cout << "ZCEL2API Version:" << pZCEL2QuoteClient->GetAPIVersion() << endl;

	ZCEINT4 iErr = ZCEErrCode_Success;

	//���÷�����IP
	iErr = pZCEL2QuoteClient->SetService(ZCEL2_SERVICE_IP,ZCEL2_SERVICE_PORT);
	if(ZCEErrCode_Success != iErr)
	{
		cerr << "SetService Err: "<< iErr << endl;
		return -1;
	}

	//��������
	iErr = pZCEL2QuoteClient->ConnectService();
	if(ZCEErrCode_Success != iErr)
	{
		cerr << "ConnectService Err"<< endl;
		return -1;
	}

	//��½
	ZCEL2QuotReqLoginField stUserMsg;
	memset(&stUserMsg, 0, sizeof(stUserMsg));
	strcpy(stUserMsg.UserNo, ZCEL2_SERVICE_USERNAME);
	strcpy(stUserMsg.Password, ZCEL2_SERVICE_PASSWORD);
	iErr = pZCEL2QuoteClient->Login(&stUserMsg);
	if(ZCEErrCode_Success != iErr)
	{
		cerr << "Login Err: "<< iErr << endl;
		return -1;
	}
	


	while(true)
	{
		Sleep(1000);
	}
	
	return 0;
}