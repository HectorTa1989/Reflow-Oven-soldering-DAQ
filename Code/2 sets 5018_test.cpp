// APAX-5018.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "ADSDIO.h"

#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <ctime>
#include <direct.h>
//#include <chrono.h>
//#include "func.h"
//#include <iterator>
//#include <vector>

using namespace std;


#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

const int g_ChannelNum = 12; //the channel number of APAX-5018 is 12
double m_dScaledValue = 0.0;//scaled value
							//APAX 5018 range table
#define NEG_50_TO_50_MV		0x0101
#define NEG_100_TO_100_MV	0x0102
#define NEG_500_TO_500_MV	0x0104
#define NEG_1_TO_1_V		0x0140
#define NEG_2_5_TO_2_5_V	0x0141
#define _4_TO_20_MA			0x0180
#define NEG_20_TO_20_MA		0x0181
#define ZERO_TO_20_MA		0x0182

#define TYPE_J_NEG_210_TO_1200_C	0x0401
#define TYPE_K_NEG_270_TO_1372_C	0x0421
#define TYPE_T_NEG_270_TO_400_C		0x0441
#define TYPE_E_NEG_270_TO_1000_C	0x0461
#define TYPE_R_ZERO_TO_1768_C		0x0481
#define TYPE_S_ZERO_TO_1768_C		0x04A1
#define TYPE_B_200_TO_1820_C		0x04C1


bool EX_OpenLib(LONG *o_lDriverHandle)
{
	DWORD dVersion = 0; //the version of ADSIO.lib

						//initialize the driver
	if (ERR_SUCCESS != ADAMDrvOpen(o_lDriverHandle))
		return false;

	if (ERR_SUCCESS == SYS_GetVersion(*o_lDriverHandle, &dVersion))
		printf("ADSDIO.lib version is %d\n\n", dVersion);
	else
		return false;

	return true;
}

void EX_CloseLib(LONG *o_lDriverHandle)
{
	//terminate the driver
	if (NULL != o_lDriverHandle)
	{
		ADAMDrvClose(o_lDriverHandle);
		o_lDriverHandle = NULL;
	}
}

void EX_SetRange(LONG i_lDriverHandle, WORD i_wSlotID)
{
	//==============================
	// user can set parameters below
	//==============================
	WORD wChRange[g_ChannelNum] = { 0 };
	/*	wChRange[0] = NEG_50_TO_50_MV;
	wChRange[1] = TYPE_J_NEG_210_TO_1200_C;
	wChRange[2] = NEG_100_TO_100_MV;
	wChRange[3] = TYPE_K_NEG_270_TO_1372_C;
	wChRange[4] = NEG_500_TO_500_MV;
	wChRange[5] = TYPE_T_NEG_270_TO_400_C;
	wChRange[6] = NEG_1_TO_1_V;
	wChRange[7] = TYPE_E_NEG_270_TO_1000_C;
	wChRange[8] = NEG_2_5_TO_2_5_V;
	wChRange[9] = TYPE_R_ZERO_TO_1768_C;
	wChRange[10] = _4_TO_20_MA;
	wChRange[11] = TYPE_B_200_TO_1820_C;	*/

	for (int i = 0; i<g_ChannelNum; i++)
	{
		wChRange[i] = TYPE_K_NEG_270_TO_1372_C;
	}


	LONG lSetRangeResult = AIO_SetRanges(i_lDriverHandle, i_wSlotID, g_ChannelNum, wChRange);
	if (ERR_SUCCESS == lSetRangeResult)
	{
		printf("Succeed to set ranges.\n");
		//	Sleep(1000);
		Sleep(1);
	}
	else
		printf("Fail to set ranges, error code = %d\n", lSetRangeResult);
}
void EX_GetRange(LONG i_lDriverHandle, WORD i_wSlotID, struct SlotInfo *i_SlotInfo)
{
	LONG lGetRangeResult = SYS_GetSlotInfo(i_lDriverHandle, i_wSlotID, i_SlotInfo);
	WORD wRangeType = 0;
	int iCnt = 0;

	if (ERR_SUCCESS == lGetRangeResult)
	{
		printf("Succeed to get ranges.\n");


		for (iCnt = 0; iCnt < g_ChannelNum; iCnt++)
		{
			wRangeType = *(i_SlotInfo->wChRange + iCnt);

			switch (wRangeType)
			{
			case NEG_50_TO_50_MV:
				printf("The channel %d has a range of  +/- 50 mV.\n", iCnt);
				break;

			case NEG_100_TO_100_MV:
				printf("The channel %d has a range of +/- 100 mV.\n", iCnt);
				break;

			case NEG_500_TO_500_MV:
				printf("The channel %d has a range of +/- 500 mV.\n", iCnt);
				break;

			case NEG_1_TO_1_V:
				printf("The channel %d has a range of +/- 1 V.\n", iCnt);
				break;

			case NEG_2_5_TO_2_5_V:
				printf("The channel %d has a range of +/- 2.5 V.\n", iCnt);
				break;

			case _4_TO_20_MA:
				printf("The channel %d has a range of 4 ~ 20 mA.\n", iCnt);
				break;

			case NEG_20_TO_20_MA:
				printf("The channel %d has a range of +/- 20 mA.\n", iCnt);
				break;

			case ZERO_TO_20_MA:
				printf("The channel %d has a range of 0 ~ 20 mA.\n", iCnt);
				break;

			case TYPE_J_NEG_210_TO_1200_C:
				printf("The channel %d is type J thermocouple, with a range of -210 ~ 1200 'C.\n", iCnt);
				break;

			case TYPE_K_NEG_270_TO_1372_C:
				printf("The channel %d is type K thermocouple, with a range of -270 ~ 1372 'C.\n", iCnt);
				break;

			case TYPE_T_NEG_270_TO_400_C:
				printf("The channel %d is type T thermocouple, with a range of -270 ~ 400 'C.\n", iCnt);
				break;

			case TYPE_E_NEG_270_TO_1000_C:
				printf("The channel %d is type E thermocouple, with a range of -270 ~ 1000 'C.\n", iCnt);
				break;

			case TYPE_R_ZERO_TO_1768_C:
				printf("The channel %d is type R thermocouple, with a range of 0 ~ 1768 'C.\n", iCnt);
				break;

			case TYPE_S_ZERO_TO_1768_C:
				printf("The channel %d is type S thermocouple, with a range of 0 ~ 1768 'C.\n", iCnt);
				break;

			case TYPE_B_200_TO_1820_C:
				printf("The channel %d is type B thermocouple, with a range of 200 ~ 1820 'C.\n", iCnt);
				break;

			default:
				printf("The range of channel %d is unknown.\n", iCnt);
				break;
			}
		}
	}
	else
	{
		printf("Fail to get ranges, error code = %d\n", lGetRangeResult);
	}

	//	Sleep(3000);
	Sleep(1);
}

void EX_GetStatus(LONG i_lDriverHandle, WORD i_wSlotID)
{
	BYTE bChStatus[32] = { 0 };//variable array to hold the channel status
	int iCnt = 0;

	LONG lGetStatusResult = AIO_GetChannelStatus(i_lDriverHandle, i_wSlotID, bChStatus);
	if (ERR_SUCCESS == lGetStatusResult)
	{
		printf("Succeed to get channel status.\n");

		for (iCnt = 0; iCnt < g_ChannelNum; iCnt++)
		{
			switch (bChStatus[iCnt])
			{
			case 0:
				printf("The status of channel %d is \"None\"\n", iCnt);
				break;
			case 1:
				printf("The status of channel %d is \"Normal\"\n", iCnt);
				break;
			case 2:
				printf("The status of channel %d is \"Over Current\"\n", iCnt);
				break;
			case 3:
				printf("The status of channel %d is \"Under Current\"\n", iCnt);
				break;
			case 4:
				printf("The status of channel %d is \"Burn Out\"\n", iCnt);
				break;
			case 5:
				printf("The status of channel %d is \"Open Loop\"\n", iCnt);
				break;
			case 6:
				printf("The status of channel %d is \"Not Ready\"\n", iCnt);
				break;
			case 14:
				printf("The status of channel %d is \"CJC Temp Error\"\n", iCnt);
				break;
			default:
				printf("The status of channel %d is \"Unknown\"\n", iCnt);
				break;
			}
		}
		//Sleep(100);
		Sleep(1);
	}
	else
		printf("Fail to get channel status, error code = %d\n", lGetStatusResult);
}

void EX_SetIntegrationTime(LONG i_lDriverHandle, WORD i_wSlotID)
{
	//==============================
	// user can set parameter below
	//==============================
	DWORD dIntegration = 0;

	LONG lSetIntegrationTimeResult = AI_SetIntegrationTime(i_lDriverHandle, i_wSlotID, dIntegration);
	if (ERR_SUCCESS == lSetIntegrationTimeResult)
	{
		printf("Succeed to set integration time.\n");
		//Sleep(3000);
		Sleep(1);
	}
	else
		printf("Fail to set integation time, error code = %d\n", lSetIntegrationTimeResult);

}

void EX_SetBurnoutMode(LONG i_lDriverHandle, WORD i_wSlotID)
{
	//==============================
	// user can set parameter below
	//==============================
	//set dwBurnoutMode = 0      for "down scale"
	//set dwBurnoutMode = 0xFFFF for "up scale"

	DWORD dwBurnoutMode = 0xFFFF; //up scale

	LONG lSetBurnoutModeResult = AI_SetBurnoutValue(i_lDriverHandle, i_wSlotID, dwBurnoutMode);
	if (ERR_SUCCESS == lSetBurnoutModeResult)
	{
		printf("Succeed to set burnout detect mode.\n");
		//Sleep(3000);
		Sleep(1);
	}
	else
		printf("Fail to set burnout detect mode, error code = %d\n", lSetBurnoutModeResult);

}

void EX_GetBurnoutMode(LONG i_lDriverHandle, WORD i_wSlotID)
{

	DWORD dwBurnoutMode = 0;
	LONG lGetBurnoutModeResult = AI_GetBurnoutValue(i_lDriverHandle, i_wSlotID, &dwBurnoutMode);
	if (ERR_SUCCESS == lGetBurnoutModeResult)
	{
		if (dwBurnoutMode > 0)
		{
			printf("The burnout detect mode is \" %s \".\n", "Up Scale");
		}
		else
			printf("The burnout detect mode is \" %s \".\n", "Down Scale");

		//	Sleep(3000);
		Sleep(1);
	}
	else
		printf("Fail to get burnout detect mode, error code = %d\n", lGetBurnoutModeResult);

}

void EX_ScaleRawValue(WORD i_wRangeType, WORD i_wRawValue, double *o_dScaledValue, char *o_cUnit)
{
	switch (i_wRangeType)
	{
	case NEG_50_TO_50_MV:
		*o_dScaledValue = -50.0 + (i_wRawValue / 65535.0) * 100.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "mV");
		break;

	case NEG_100_TO_100_MV:
		*o_dScaledValue = -100.0 + (i_wRawValue / 65535.0) * 200.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "mV");
		break;

	case NEG_500_TO_500_MV:
		*o_dScaledValue = -500.0 + (i_wRawValue / 65535.0) * 1000.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "mV");
		break;

	case NEG_1_TO_1_V:
		*o_dScaledValue = -1.0 + (i_wRawValue / 65535.0) * 2.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "V");
		break;

	case NEG_2_5_TO_2_5_V:
		*o_dScaledValue = -2.5 + (i_wRawValue / 65535.0) * 5.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "V");
		break;

	case _4_TO_20_MA:
		*o_dScaledValue = 4.0 + (i_wRawValue / 65535.0) * 16.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "mA");
		break;

	case NEG_20_TO_20_MA:
		*o_dScaledValue = -20.0 + (i_wRawValue / 65535.0) * 40.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "mA");
		break;

	case ZERO_TO_20_MA:
		*o_dScaledValue = (i_wRawValue / 65535.0) * 20.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "mA");
		break;

	case TYPE_J_NEG_210_TO_1200_C:
		*o_dScaledValue = -210.0 + (i_wRawValue / 65535.0) * 1410.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "'C");
		break;

	case TYPE_K_NEG_270_TO_1372_C:
		*o_dScaledValue = -270.0 + (i_wRawValue / 65535.0) * 1642.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "'C");
		break;

	case TYPE_T_NEG_270_TO_400_C:
		*o_dScaledValue = -270.0 + (i_wRawValue / 65535.0) * 670.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "'C");
		break;

	case TYPE_E_NEG_270_TO_1000_C:
		*o_dScaledValue = -270.0 + (i_wRawValue / 65535.0) * 1270.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "'C");
		break;

	case TYPE_R_ZERO_TO_1768_C:
	case TYPE_S_ZERO_TO_1768_C:
		*o_dScaledValue = (i_wRawValue / 65535.0) * 1768.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "'C");
		break;

	case TYPE_B_200_TO_1820_C:
		*o_dScaledValue = 200.0 + (i_wRawValue / 65535.0) * 1620.0;
		strcpy_s(o_cUnit, sizeof(o_cUnit), "'C");
		break;

	default:
		strcpy_s(o_cUnit, sizeof(o_cUnit), "UN");

		break;
	}
}

void EX_GetValue(LONG i_lDriverHandle, WORD i_wSlotID, struct SlotInfo &i_SlotInfo)
{

	//==============================
	// user can set parameter below
	//==============================
	WORD wChannel = 0;// the channel ID from 0 to 11

	WORD wRawValue = 0; //raw data
	double dScaledValue = 0.0;//scaled value
	WORD wRangeType = 0;
	char cUnit[12] = { 0 };

	wRangeType = *(i_SlotInfo.wChRange + wChannel); //get range type
	LONG lGetValueResult = AIO_GetValue(i_lDriverHandle, i_wSlotID, wChannel, &wRawValue); //get raw value
	if (ERR_SUCCESS == lGetValueResult)
	{
		EX_ScaleRawValue(wRangeType, wRawValue, &dScaledValue, cUnit);//get scale value
		if (strcmp(cUnit, "UN") != 0)
		{
			printf("Channel %d raw data is 0x%04X, scaled value is %.4f %s.\n", wChannel, wRawValue, dScaledValue, cUnit);
			//Sleep(3000);
			Sleep(3);
		}
		else
			printf("Channel %d range is unknown.\n", wChannel);
	}
	else
		printf("Fail to get value, error code = %d\n", lGetValueResult);


}
void EX_GetMultiValue(LONG i_lDriverHandle, WORD i_wSlotID, struct SlotInfo &i_SlotInfo)
{
	WORD wRawValue[g_ChannelNum] = { 0 }; //raw data

	WORD wRangeType = 0;
	char cUnit[12] = { 0 };

	int iCnt = 0;



	LONG lGetMultiValueResult = AIO_GetValues(i_lDriverHandle, i_wSlotID, wRawValue); //get raw value
	if (ERR_SUCCESS == lGetMultiValueResult)
	{

		for (iCnt = 0; iCnt < g_ChannelNum; iCnt++)
		{
			wRangeType = *(i_SlotInfo.wChRange + iCnt); //get range type
			EX_ScaleRawValue(wRangeType, wRawValue[iCnt], &m_dScaledValue, cUnit);//get scale value			
			if (strcmp(cUnit, "UN") != 0)
			{
				printf("Channel %d raw data is 0x%04X, scaled value is %.4f %s.\n", iCnt, wRawValue[iCnt], m_dScaledValue, cUnit);
				Sleep(10);
			}
			else
				printf("Channel %d range is unknown.\n", iCnt);
		}



	}
	else
		printf("Fail to get value, error code = %d\n", lGetMultiValueResult);


}

void EX_SetChMask(LONG i_lDriverHandle, WORD i_wSlotID)
{

	//==============================
	// user can set parameter below
	//==============================
	DWORD dwMask = 0x0000FFFF; //enable all channelS

	int iCnt = 0;
	LONG lSetMaskResult = AI_SetChannelMask(i_lDriverHandle, i_wSlotID, dwMask);
	if (ERR_SUCCESS == lSetMaskResult)
	{
		printf("Succeed to set channel mask.\n");
		//Sleep(1000);
		Sleep(1);
	}
	else
		printf("Fail to set channel mask, error code = %d.\n", lSetMaskResult);
}

void EX_GetChMask(LONG i_lDriverHandle, WORD i_wSlotID, struct SlotInfo *i_SlotInfo)
{

	DWORD dwMask = 0;
	int iCnt = 0;

	LONG lGetMaskResult = SYS_GetSlotInfo(i_lDriverHandle, i_wSlotID, i_SlotInfo);
	dwMask = i_SlotInfo->dwChMask;
	if (ERR_SUCCESS == lGetMaskResult)
	{
		printf("Succeed to get channel mask.\n");
		for (iCnt = 0; iCnt < g_ChannelNum; iCnt++)
		{
			if (dwMask & (0x01 << iCnt))
			{
				printf("Channel %d is enabled.\n", iCnt);
				//Sleep(100);
				Sleep(3);
			}
			else
				printf("Channel %d is disabled.\n", iCnt);
		}
	}
	else
		printf("Fail to get channel mask, error code = %d.\n", lGetMaskResult);
}

int main(int argc, char* argv[])
{

	//==============================
	// user can set SlotID below
	//==============================
	WORD wSlotID_1 = 1; //the slot ID which is ranged from 0 to 31
	WORD wSlotID_2 = 2;

	WORD wRawValue[g_ChannelNum] = { 0 }; //raw data
	double m_dScaledValue = 0.0;//scaled value
	WORD wRangeType = 0;
	char cUnit[12] = { 0 };
	int iCnt = 0;

	//system parameters
	bool bAdvStatus = false;//the loading status of ADSDIO.lib
	LONG lDriverHandle = NULL;//driver handler
	struct SlotInfo sSlotInfo; //slot information including channel number, type, value and range
	memset(&sSlotInfo, 0, sizeof(struct SlotInfo));

	struct SlotInfo sSlotInfo2; //slot information including channel number, type, value and range
	memset(&sSlotInfo2, 0, sizeof(struct SlotInfo));

	//module ID parameters
	DWORD	dModuleID = 0;
	DWORD	dHiWord = 0x0;
	DWORD	dLoWord = 0x0;
	LONG	lGetModuleResult1 = 0;

	DWORD	dModuleID2 = 0;
	DWORD	dHiWord2 = 0x0;
	DWORD	dLoWord2 = 0x0;
	LONG	lGetModuleResult2 = 0;

	char filename[50];

	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	timeinfo->tm_mon++;								// Because the range of tm_mon is 0~11, so we need to increment it by 1
	timeinfo->tm_year = timeinfo->tm_year + 1900;		// Because year counted since 1900

	clock_t start = clock();

	//  sprintf(filename2, "%04d%02d%02d_%02dh%02dm%02ds.csv", 
	//         timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
	

	//load library
	if (bAdvStatus = EX_OpenLib(&lDriverHandle))
	{
		printf("The slot 1 ID setting is %d.\n", wSlotID_1);
		printf("The slot 2 ID setting is %d.\n", wSlotID_2);

		//get module ID
		lGetModuleResult1 = SYS_GetModuleID(lDriverHandle, wSlotID_1, &dModuleID);
		lGetModuleResult2 = SYS_GetModuleID(lDriverHandle, wSlotID_2, &dModuleID2);

		//the moduleID for Apax-5018 is 0x50180000
		//dHiWord is 5018, dLoWord is 0000
		if (ERR_SUCCESS == lGetModuleResult1)
		{
			dHiWord = (dModuleID & 0xFFFF0000) >> 16;
			dLoWord = dModuleID & 0x0000FFFF;

			if (0 == dLoWord)
				printf("/*** APAX %x Module Sample Code ***/\n\n", dHiWord);
			else if (0 != (dLoWord & 0x0000FF00) && (dLoWord & 0x000000FF))
				printf("/*** APAX %x%c%c Module ***/\n\n", dHiWord, dLoWord >> 8, (dLoWord & 0x000000FF));
			else
				printf("/*** APAX %x%c Module ***/\n\n", dHiWord, dLoWord >> 8);
		}
		else
			printf("Fail to get module 1 ID, error code = %d\n", lGetModuleResult1);

		Sleep(1);

		//the moduleID for Apax-5018 is 0x50180000
		//dHiWord2 is 5018, dLoWord2 is 0000
		if (ERR_SUCCESS == lGetModuleResult1)
		{
			dHiWord2 = (dModuleID2 & 0xFFFF0000) >> 16;
			dLoWord2 = dModuleID2 & 0x0000FFFF;

			if (0 == dLoWord2)
				printf("/*** APAX %x Module Sample Code ***/\n\n", dHiWord2);
			else if (0 != (dLoWord2 & 0x0000FF00) && (dLoWord2 & 0x000000FF))
				printf("/*** APAX %x%c%c Module ***/\n\n", dHiWord2, dLoWord2 >> 8, (dLoWord2 & 0x000000FF));
			else
				printf("/*** APAX %x%c Module ***/\n\n", dHiWord2, dLoWord2 >> 8);
		}
		else
			printf("Fail to get module 2 ID, error code = %d\n", lGetModuleResult2);

		Sleep(1);


	}
	else
	{
		printf("Fail to load ADSDIO library.\n");
	}

	// get slot info
	if (bAdvStatus)
	{

		LONG lGetSlotInfo_1 = SYS_GetSlotInfo(lDriverHandle, wSlotID_1, &sSlotInfo);
		LONG lGetSlotInfo_2 = SYS_GetSlotInfo(lDriverHandle, wSlotID_2, &sSlotInfo2);
		if (ERR_SUCCESS != lGetSlotInfo_1)
			printf("Fail to get slot 1 information, error code = %d\n", lGetSlotInfo_1);
		else if (ERR_SUCCESS != lGetSlotInfo_2)
			printf("Fail to get slot 2 information, error code = %d\n", lGetSlotInfo_2);
		Sleep(1);
	}

	//=================================================================================
	//
	//		Selected items functions
	//
	//=================================================================================

	//set AI channel mask example (set channel enable or disable)
	if (bAdvStatus)
	{
		printf("\n*** Set AI channel mask ***\n");
		EX_SetChMask(lDriverHandle, wSlotID_1);
		EX_SetChMask(lDriverHandle, wSlotID_2);
	}

	//get AI channel mask example
	if (bAdvStatus)
	{
		printf("\n*** Get AI channel mask ***\n");
		EX_GetChMask(lDriverHandle, wSlotID_1, &sSlotInfo);
		EX_GetChMask(lDriverHandle, wSlotID_2, &sSlotInfo2);
	}

	//=================================================================================
	//
	//		Module setting functions
	//
	//=================================================================================


	//set AI integation time
	if (bAdvStatus)
	{
		printf("\n*** Set AI integation time ***\n");
		EX_SetIntegrationTime(lDriverHandle, wSlotID_1);
		EX_SetIntegrationTime(lDriverHandle, wSlotID_2);
	}

	//set AI burnout detection mode
	if (bAdvStatus)
	{
		printf("\n*** Set AI burnout detection mode ***\n");
		EX_SetBurnoutMode(lDriverHandle, wSlotID_1);
		EX_SetBurnoutMode(lDriverHandle, wSlotID_2);
	}

	//get AI burnout detection mode
	if (bAdvStatus)
	{
		printf("\n*** Get AI burnout detection mode ***\n");
		EX_GetBurnoutMode(lDriverHandle, wSlotID_1);
		EX_GetBurnoutMode(lDriverHandle, wSlotID_2);
	}

	//set AI channel ranges example
	if (bAdvStatus)
	{
		printf("\n*** Set AI channel ranges ***\n");
		EX_SetRange(lDriverHandle, wSlotID_1);
		EX_SetRange(lDriverHandle, wSlotID_2);
	}

	//get AI channel ranges example
	if (bAdvStatus)
	{
		printf("\n*** Get AI channel ranges ***\n");
		EX_GetRange(lDriverHandle, wSlotID_1, &sSlotInfo);
		EX_GetRange(lDriverHandle, wSlotID_2, &sSlotInfo2);
	}

	//get AI channel status example
	if (bAdvStatus)
	{
		printf("\n*** Get AI channel status ***\n");
		EX_GetStatus(lDriverHandle, wSlotID_1);
		EX_GetStatus(lDriverHandle, wSlotID_2);
	}

	//get AI single channel value example
	if (bAdvStatus)
	{
		printf("\n*** Get AI single channel value ***\n");
		EX_GetValue(lDriverHandle, wSlotID_1, sSlotInfo);
		EX_GetValue(lDriverHandle, wSlotID_2, sSlotInfo2);
	}

	//get AI multiple channel values example
	if (bAdvStatus)
	{
		printf("\n*** Get AI multiple channel values ***\n");
		
		sprintf(filename, "%04d%02d%02d_%02dh.csv\n",
			timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour);
		printf("\nFilename: %s\n", filename);


		ofstream dest2;
		dest2.open(filename, ios_base::app | ios_base::out);
		dest2 << "Date" << "," << "Time" << "," << "milisecond" << "," << "Channel 0" << "," << "Channel 1" << "," << "Channel 2" << "," << "Channel 3" << "," << "Channel 4" << "," << "Channel 5" << "," << "Channel 6" << "," << "Channel 7" << "," << "Channel 8" << "," << "Channel 9" << "," << "Channel 10" << "," << "Channel 11" << "," << "Channel 12" << "Channel 13" << "Channel 14" << "Channel 15" << "Channel 16" << "Channel 17" << "Channel 18" << "Channel 19" << "Channel 20" << "Channel 21" << "Channel 22" << "Channel 23" << endl;

		while (timeinfo->tm_min < 60)
		{
			clock_t timeElapsed = clock() - start;
			unsigned secElapsed = timeElapsed / CLOCKS_PER_SEC;
			unsigned msElapsed = timeElapsed / CLOCKS_PER_MS;

			sprintf(filename, "%04d%02d%02d_%02dh.csv",
				timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour);
			printf("\nFilename: %s", filename);

			ofstream dest;
			dest.open(filename, ios_base::app | ios_base::out);
			if (timeinfo->tm_min > 59 && msElapsed < 5)
				dest << "Date" << "," << "Time" << "," << "milisecond" << "," << "Channel 0" << "," << "Channel 1" << "," << "Channel 2" << "," << "Channel 3" << "," << "Channel 4" << "," << "Channel 5" << "," << "Channel 6" << "," << "Channel 7" << "," << "Channel 8" << "," << "Channel 9" << "," << "Channel 10" << "," << "Channel 11" << "," << "Channel 12" << "Channel 13" << "Channel 14" << "Channel 15" << "Channel 16" << "Channel 17" << "Channel 18" << "Channel 19" << "Channel 20" << "Channel 21" << "Channel 22" << "Channel 23" << endl;

			// Get Analog Data ------------------------------
			//EX_GetMultiValue(lDriverHandle, wSlotID_1, sSlotInfo);
			LONG lGetMultiValueResult_1 = AIO_GetValues(lDriverHandle, wSlotID_1, wRawValue); //get raw value
			if (ERR_SUCCESS == lGetMultiValueResult_1)
			{


				/*if (timeinfo->tm_min == 59 && timeinfo->tm_sec == 59 && msElapsed > 950)
				{
				dest.close();
				dest.open(filename, ios_base::app | ios_base::out);
				dest << "Date" << "," << "Time" << "," << "milisecond" << "," << "Channel 0" << "," << "Channel 1" << "," << "Channel 2" << "," << "Channel 3" << "," << "Channel 4" << "," << "Channel 5" << "," << "Channel 6" << "," << "Channel 7" << "," << "Channel 8" << "," << "Channel 9" << "," << "Channel 10" << "," << "Channel 11" << "," << "Channel 12" << "Channel 13" << "Channel 14" << "Channel 15" << "Channel 16" << "Channel 17" << "Channel 18" << "Channel 19" << "Channel 20" << "Channel 21" << "Channel 22" << "Channel 23" << endl;

				}*/
				while (msElapsed >= 1000)
					msElapsed -= 1000;
				while ((timeinfo->tm_sec + secElapsed) > 59)
				{
					timeinfo->tm_sec -= 60;
					//		secElapsed = 0;
					timeinfo->tm_min++;
				}
				while (timeinfo->tm_min > 59)
				{
					timeinfo->tm_min -= 60;
					timeinfo->tm_hour++;

				}
				while (timeinfo->tm_hour > 23)
				{
					timeinfo->tm_hour -= 24;
					timeinfo->tm_mday++;
				}

				dest << timeinfo->tm_year << "-" << timeinfo->tm_mon << "-" << timeinfo->tm_mday << "," << timeinfo->tm_hour << "h" << timeinfo->tm_min << "m" << timeinfo->tm_sec + secElapsed << "s" << ",";
				dest << msElapsed << "ms" << ",";
				for (int iCnt = 0; iCnt < g_ChannelNum; iCnt++)
				{
					wRangeType = *(sSlotInfo.wChRange + iCnt); //get range type
					EX_ScaleRawValue(wRangeType, wRawValue[iCnt], &m_dScaledValue, cUnit);//get scale value			
					if (strcmp(cUnit, "UN") != 0)
					{
						printf("Channel %d raw data is 0x%04X, scaled value is %.4f %s.\n", iCnt, wRawValue[iCnt], m_dScaledValue, cUnit);
						dest << m_dScaledValue << ",";
						Sleep(0.5);
					}
				}
			}
			else
				printf("Fail to get Slot 1 value, error code = %d\n", lGetMultiValueResult_1);

			//EX_GetMultiValue(lDriverHandle, wSlotID_2, sSlotInfo2);
			LONG lGetMultiValueResult_2 = AIO_GetValues(lDriverHandle, wSlotID_2, wRawValue); //get raw value
			if (ERR_SUCCESS == lGetMultiValueResult_2)
			{
			
				for (int iCnt = 0; iCnt < g_ChannelNum; iCnt++)
				{
					wRangeType = *(sSlotInfo2.wChRange + iCnt); //get range type
					EX_ScaleRawValue(wRangeType, wRawValue[iCnt], &m_dScaledValue, cUnit);//get scale value			
					if (strcmp(cUnit, "UN") != 0)
					{
						printf("Channel %d raw data is 0x%04X, scaled value is %.4f %s.\n", iCnt, wRawValue[iCnt], m_dScaledValue, cUnit);
						dest << m_dScaledValue << ",";
						Sleep(0.5);
					}
				}
			}
			else
				printf("Fail to get Slot 2 value, error code = %d\n", lGetMultiValueResult_2);
			dest << endl;
			dest.close();
			//dest2.close();

		}
	}

	//close library
	EX_CloseLib(&lDriverHandle);
	printf("\n/***        END         ***/ \n\n");
	system("pause");
	return 0;
}

