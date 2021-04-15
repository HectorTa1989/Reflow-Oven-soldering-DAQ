void ADAMDrvOpen(LONG *);
void ADAMDrvClose(LONG *);
bool SYS_GetVersion(long, DWORD);
LONG AIO_SetRanges(long, WORD, WORD, WORD);
LONG SYS_GetSlotInfo(long, WORD, struct SlotInfo*);
LONG AIO_GetChannelStatus(long, WORD, BYTE*);
LONG AI_GetBurnoutValue(long, WORD, DWORD);
LONG AI_SetBurnoutValue(long, WORD, DWORD);
LONG AI_SetChannelMask(long, WORD, DWORD);
LONG AIO_GetValue(long, WORD, WORD, WORD);
LONG AIO_GetValues(long, WORD, WORD);
LONG AI_SetIntegrationTime(long, WORD, DWORD);
LONG SYS_GetModuleID(long, WORD, DWORD);

