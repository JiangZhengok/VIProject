#include <vector>
#include <iostream>
#include <iomanip>
#include "stimpublish/STIM300Driver.h"
#include "stimpublish/MyPortSerial.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

vector<unsigned char> vbuff(38);
uint8_t STIMFrame[L];
bool IdenFlag = false;

unsigned int crc32_mpeg_2(unsigned char *data, unsigned int lenght)
{
	unsigned char i;
	unsigned int crc = 0xffffffff;  //Initial value
	while(lenght-- )
	{
		crc ^= (unsigned int)(*data++) << 24;  //crc ^= (unsigned int)(data*)<<24; data++;
		for(i = 0; i < 8; ++i)
		{
			if(crc & 0x80000000)
			crc = (crc << 1) ^ 0x04C11DB7;
			else 
			crc <<= 1;
		}
	}
	return crc;
}

bool ReceiveCharfromSTIM300(void) 
{
	unsigned int num = 0;
	unsigned char RXBuff;
	
	if(Read_abit_fromeSerialPort(&RXBuff))
	{
		vbuff.push_back(RXBuff);
		vbuff.erase( vbuff.begin() );
		if( vbuff[0] == IDEN )
		{
			for(int i = 0; i < L; i++)
				STIMFrame[i] = vbuff[i];
			return true;
		}
		else return 0;
	}
	else return 0;
} 

bool IsDateTrue(const unsigned char (&frame)[L])
{
	uint8_t datagram[L - 4 + 2];
	unsigned int Checksum = 0;
	for(int i = 0; i < L - 4; i++)
		datagram[i] = frame[i];
	datagram[L - 4] = 0x00;  datagram[L - 4 + 1] = 0x00;
	Checksum = ( frame[CRC_BYTE] << 24) + ( frame[CRC_BYTE+1] << 16)
			+ ( frame[CRC_BYTE+2] << 8) + ( frame[CRC_BYTE+3]);
	if(crc32_mpeg_2(datagram, L-4+2) == Checksum)
		return true;
	else
		return false;
}

bool DecodeSTIMDatagram(const unsigned char (&frame)[L], STIMData &dat)
{
	if(IsDateTrue(frame))
	{
		dat.gyro.x = (double)((int)( (frame[G_BEG_BYTE]<<24)
									+(frame[G_BEG_BYTE+1]<<16)
									+(frame[G_BEG_BYTE+2] << 8))>>8) *  M_PI / 180 / G_DEGREE_S;
		dat.gyro.y = (double)((int)( (frame[G_BEG_BYTE+3]<<24)
									+(frame[G_BEG_BYTE+3+1]<<16)
									+(frame[G_BEG_BYTE+3+2] << 8))>>8) *  M_PI / 180 / G_DEGREE_S;
		dat.gyro.z = (double)((int)( (frame[G_BEG_BYTE+6]<<24)
									+(frame[G_BEG_BYTE+6+1]<<16)
									+(frame[G_BEG_BYTE+6+2] << 8))>>8) *  M_PI / 180 / G_DEGREE_S;
		dat.gyro_ST = frame[G_BEG_BYTE+6+2+1];
	
		dat.acc.x = (double)((int)((frame[A_BEG_BYTE]<<24) 
								 + (frame[A_BEG_BYTE+1]<<16)
								 + (frame[A_BEG_BYTE+2]<<8))>>8) * SAMPLE_RATE /  A_M_S_SA;
		dat.acc.y = (double)((int)((frame[A_BEG_BYTE+3]<<24) 
								 + (frame[A_BEG_BYTE+3+1]<<16)
								 + (frame[A_BEG_BYTE+3+2]<<8))>>8)  * SAMPLE_RATE /  A_M_S_SA;
		dat.acc.z = (double)((int)((frame[A_BEG_BYTE+6]<<24) 
								 + (frame[A_BEG_BYTE+6+1]<<16)
								 + (frame[A_BEG_BYTE+6+2]<<8))>>8) * SAMPLE_RATE /  A_M_S_SA;
		dat.acc_ST = frame[A_BEG_BYTE+6+2+1];
	
		dat.incli.x = (double)((int)( (frame[I_BEG_BYTE] << 24)
									+ (frame[I_BEG_BYTE+1] << 16)
									+ (frame[I_BEG_BYTE+2] << 8))>>8) * SAMPLE_RATE / I_M_S_SA;
		dat.incli.y = (double)((int)( (frame[I_BEG_BYTE+3] << 24)
									+ (frame[I_BEG_BYTE+3+1] << 16)
									+ (frame[I_BEG_BYTE+3+2] << 8))>>8) * SAMPLE_RATE / I_M_S_SA;
		dat.incli.z = (double)((int)( (frame[I_BEG_BYTE+6] << 24)
									+ (frame[I_BEG_BYTE+6+1] << 16)
									+ (frame[I_BEG_BYTE+6+2] << 8))>>8) * SAMPLE_RATE / I_M_S_SA;
		dat.incli_ST = frame[I_BEG_BYTE+6+2+1];
	
		dat.counter = frame[COUNT_BYTE];
	
		dat.latency = (frame[LATENCY_BEG_BYTE] << 8) + frame[LATENCY_BEG_BYTE + 1];
		//cout << (int)dat.gyro_ST << endl;
		//cout << (int)dat.acc_ST << endl;
		//cout << "*******************" << endl;
		if( ((dat.gyro_ST == 0x20) || (dat.gyro_ST == 0x00))&&
		    ((dat.acc_ST == 0x20) || (dat.acc_ST == 0x00)) &&
	        ((dat.incli_ST == 0x20) || (dat.incli_ST == 0x00)) ) 
			return true;
		else
			return false;
	}
	else
		return false;
}

bool STIMInit(string usb)
{
	return PortIint(usb, BAUDRATE_beingUsed);
}


stimlogger logger;
bool stim_log_display(stimdata &stimdat)
{
	
	if(ReceiveCharfromSTIM300())
	{
		logger.isTruef = DecodeSTIMDatagram(STIMFrame, stimdat);	
		if( logger.isTruef )
		{
			if(logger.isBeganRe == false)
			{
				logger.isBeganRe = true;
				logger.last_stimCount = stimdat.counter;
				logger.numFrames = 0;
			}
			++logger.numFrames;

			if(stimdat.counter >= logger.last_stimCount)
				logger.stimCountInterval = stimdat.counter - logger.last_stimCount;
			else
				logger.stimCountInterval = 256 + stimdat.counter - logger.last_stimCount;
			
			logger.last_stimCount = stimdat.counter;		
			logger.stimTolCount += logger.stimCountInterval * STIM_COUNTING_INTERVAL_us;
					
// 			std::cout
// 			<< " | Gx: " << left << setw(13) << stimdat.gyro.x
// 			<< " | Gy: " << left << setw(13) << stimdat.gyro.y
// 			<< " | Gz: " << left << setw(13) << stimdat.gyro.z
// 			<< " | Ax: " << left << setw(13) << stimdat.acc.x
// 			<< " | Ay: " << left << setw(13) << stimdat.acc.y
// 			<< " | Az: " << left << setw(13) << stimdat.acc.z 
// 			<< " | Cn: " << logger.stimCountInterval 
// 			<< endl;
			return true;
		}
		return false;
	}
	return false;
}
