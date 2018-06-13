#include "BitMap.h"

NTSTATUS 
DCBitMapInit(
	PDC_BITMAP AimMap,
	LARGE_INTEGER TotalSize
)
{
	ULONG GbSize = TotalSize.QuadPart >> 30;
	AimMap->Table = (PMBTABLE*)ExAllocatePoolWithTag(NonPagedPool,sizeof(PMBTABLE)*GbSize,'GbS');
	AimMap->Size = GbSize;
	if (AimMap->Table == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}
	RtlZeroBytes(AimMap->Table,sizeof(PMBTABLE)*GbSize);
	return STATUS_SUCCESS;
}

NTSTATUS 
DCBitMapDelect(
	PDC_BITMAP AimMap
)
{

	for (int i = 0; i < AimMap->Size; i++)
	{
		if (AimMap->Table[i]!=NULL)
		{
			for (int j = 0; j < AimMap->Table[i]->Size; j++)
			{
				if (AimMap->Table[i]->MbTable[j] != NULL)
				{
					for (int z = 0; AimMap->Table[i]->MbTable[j]->Size; z++)
					{
						if (AimMap->Table[i]->MbTable[j]->KbTable!= NULL)
						{
							ExFreePoolWithTag(AimMap->Table[i]->MbTable[j]->KbTable,'Gbs');//Bug
						}
					}
				}
			}
		}
	}
	return STATUS_SUCCESS;
}

USHORT 
DCBitMapQuery(
	PDC_BITMAP AimMap,
	LARGE_INTEGER Startoffset,
	PULONG Length
)
/*
如果是部分使用那么被使用的长度将会被Length返回
*/
{
	ULONG Index2,Index1,Index=Startoffset.QuadPart>>30;
	if (*Length < 1024)
	{
		if (AimMap->Table[Index])
		{
			Index1 = Startoffset.QuadPart >> 20 - Index << 10;
			if (AimMap->Table[Index]->MbTable[Index1])
			{
				Index2 = Startoffset.QuadPart >> 10 - (Startoffset.QuadPart >> 20) << 10;

			}
		}
	}
	return NOUSE;
}
