struct USBINFO {
      char fileHex[64];
      unsigned int nFXType; //0 for FX2. 1 for FX3
      unsigned int nUnitType;
      unsigned int nVID;
      unsigned int nPID;
      unsigned int nUnit;
      long int nIntTime;
};

extern int bwtekTestUSB(int nUSBTiming, int nPixelNo, int nInputMode, int nChannel, void *pExtraData);
extern int bwtekDataReadUSB(int  nTriggerMode, unsigned short* pArray, int nChannel);
extern int bwtekSetTimeUSB( long  lTime,int nChannel);
extern int bwtekCloseUSB(int nChannel);
extern int bwtekReadEEPROMUSB(char *str, int nChannel);
extern int bwtekReadCCodeUSB(unsigned char *str, int nChannel=0);
extern int bwtekSetTimingsUSB(long  lTriggerExit, int nMuliply, int nChannel);

////// only for BRC115///////////////////////////
extern int bwtekFrameDataReadUSB(int FrameNum,int  nTriggerMode, unsigned short *pArray, int DeviceIndex=0);
extern int bwtekDSPDataReadUSB(int AveNum,int Smoothing,int DarkCompensate,int nTriggerMode, unsigned short* pArray,int DeviceIndex=0);
extern int GetChannelNo(int *nNo,int DeviceIndex );
extern int SetChannelNo(int nNo,int DeviceIndex);
extern int bwtekReadVIDPIDUSB(int *nVID, int *nPID, int DeviceIndex);
extern int bwtekSetTTLOut(int nNo, int nSetValue, int nInverse, int DeviceIndex=0  );
extern int bwtekGetTTLIn(int nNo, int *nValue, int DeviceIndex=0  );
extern int bwtekGetAnalogIn(int nNo, int *nValue, double *fValue,int DeviceIndex=0  );
extern int bwtekSetAnalogOut(int nNo, int nSetValue, int DeviceIndex=0  );
