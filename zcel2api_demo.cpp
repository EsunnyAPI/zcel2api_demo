#include "zcel2api/ZceLevel2ApiInterface.h"
#include <iostream>
#include <windows.h>
using namespace std;


//====================================================
//
// 请配置以下项目
//
//Level2服务 IP地址
const char* ZCEL2_SERVICE_IP         = "";
//Level2服务 端口
const int   ZCEL2_SERVICE_PORT       = 0; 
//Level2服务 用户名
const char* ZCEL2_SERVICE_USERNAME   = "请替换为您的用户名";
//Level2服务 密码
const char* ZCEL2_SERVICE_PASSWORD   = "请替换为您的密码";
//====================================================

// 实现IZCEL2QuoteNotify接口，用于接收Level2API的通知
class ZCEL2Quote  : public IZCEL2QuoteNotify
{
public:

	virtual void ZCEL2QUOTE_CALL OnRspLogin(const ZCEL2QuotRspLoginField& loginMsg)
	{
		if(loginMsg.ErrorCode == ZCEErrCode_Success)
		{
			cout << "登陆成功, 时间：" << loginMsg.TimeStamp << endl;
		}
		else
		{	
			cout << "登陆失败, 原因：" << loginMsg.ErrorCode << " , " << loginMsg.ErrorText << endl;
		}
	}
	
	virtual void ZCEL2QUOTE_CALL OnRecvQuote(const ZCEL2QuotSnapshotField& snapshot)
	{
		cout << "行情更新:" 
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
		cout << "组合行情更新:" 
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
		cout << "连接断开" << endl;
	}

};



int main()
{
	//创建notify 实例
	ZCEL2Quote notify;

	//创建ZCEL2QuotClient实例
	IZCEL2QuoteClient* pZCEL2QuoteClient = CreateZCEL2QuotClient(&notify);
	if(NULL == pZCEL2QuoteClient)
	{
		cerr << "ZCEL2API创建失败" <<endl;
		return -1;
	}

	//查看版本信息
	cout << "ZCEL2API Version:" << pZCEL2QuoteClient->GetAPIVersion() << endl;

	ZCEINT4 iErr = ZCEErrCode_Success;

	//设置服务器IP
	iErr = pZCEL2QuoteClient->SetService(ZCEL2_SERVICE_IP,ZCEL2_SERVICE_PORT);
	if(ZCEErrCode_Success != iErr)
	{
		cerr << "SetService Err: "<< iErr << endl;
		return -1;
	}

	//建立连接
	iErr = pZCEL2QuoteClient->ConnectService();
	if(ZCEErrCode_Success != iErr)
	{
		cerr << "ConnectService Err"<< endl;
		return -1;
	}

	//登陆
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