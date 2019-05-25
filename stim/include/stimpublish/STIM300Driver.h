#include <stdint.h>

#ifndef __STIM300DRIVER__
#define __STIM300DRIVER__
#define L (38)
#define IDEN (0x93)
#define G_BEG_BYTE (1)
#define A_BEG_BYTE (11)
#define I_BEG_BYTE (21)
#define COUNT_BYTE (31)
#define LATENCY_BEG_BYTE (32)
#define CRC_BYTE (34)

#define STIM_COUNTING_INTERVAL_us (500)

#define BAUDRATE_beingUsed (460800)

#define US_PER_SEC (1000000)
#define NS_PER_SEC (1000000000)

#define G_DEGREE_S (0x4000)  //2 ^ 14

#define A_M_S_SA (0x400000)  //2 ^ 22

#define I_M_S_SA (0X2000000) //2 ^ 25

#define SAMPLE_RATE (250)



struct axes{
	double x;
	double y;
	double z;
};

typedef struct stimdata{
	struct axes gyro;
	struct axes acc;
	struct axes incli;
	
	unsigned char gyro_ST;
	unsigned char acc_ST;
	unsigned char incli_ST;
	
	unsigned char counter;
	
	uint16_t latency;
} STIMData;

typedef struct {
	uint64_t stimTolCount;
	uint64_t numFrames;
	bool isTruef;
	bool isBeganRe;
	uint8_t last_stimCount;
	uint16_t stimCountInterval;
}stimlogger;

extern bool IdenFlag;
extern uint8_t STIMFrame[L];

extern bool ReceiveCharfromSTIM300(void);
bool DecodeSTIMDatagram(const unsigned char (&frame)[L], STIMData &dat);
extern bool STIMInit(std::string usb);
extern bool stim_log_display(stimdata &stimdat);

extern stimlogger logger;
#endif

