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
	AimMap->BeUse = (PCHAR)ExAllocatePoolWithTag(NonPagedPool,1024,'Use');
	if (AimMap->Table == NULL||AimMap->BeUse==NULL)
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
	PLENGTHINFO Length
)
/*
如果是部分使用那么被使用的长度将会被Length返回
*/
{
	LENGTHINFO BeforeHalf,EndHalf;
	BeforeHalf = EndHalf = {0};
	PCHAR Tmp;
	PULONG64 Tmp1;
	if (AimMap->BeUse[GETGB(Startoffset.QuadPart)])
	{
		SET_LENGTH_VALUE(BeforeHalf,Startoffset.QuadPart);
		SET_LENGTH_VALUE(EndHalf,Startoffset.QuadPart+Length->TotalSize);
		if (BeforeHalf.GbSize == EndHalf.GbSize)
		{
			if (!AimMap->BeUse[BeforeHalf.GbSize])
			{
				return NOUSE;
			}
			if (BeforeHalf.MbSize == EndHalf.MbSize)
			{
				if (!AimMap->Table[BeforeHalf.GbSize]->BeUse[BeforeHalf.MbSize])
				{
					return NOUSE;
				}
				if (BeforeHalf.KbSize == EndHalf.KbSize)
				{
					if (!AimMap->Table[BeforeHalf.GbSize]->MbTable[BeforeHalf.MbSize]->BeUse[BeforeHalf.KbSize])
					{
						return NOUSE;
					}
					Tmp = AimMap->Table[BeforeHalf.GbSize]->MbTable[BeforeHalf.MbSize]->KbTable[BeforeHalf.KbSize]->ByteTable;
					Tmp = Tmp + BeforeHalf.ByteSize;
					Tmp1 = (PULONG64)Tmp;
					ULONG Size = EndHalf.ByteSize - BeforeHalf.ByteSize;
					for (int i = 0; i < Size / sizeof(ULONG64); i++)
					{
						if ((Tmp1[i] != -1)&&((i+1)*sizeof(ULONG64)>Size)&&(Tmp1[i]==Size%sizeof(ULONG64)))
						{
							return FULLUSE;
						}
					}
				}
			}
		}
	}
	return NOUSE;
}

VOID 
DCBitMaskBit(
	PVOID AimMap,
	ULONG Length
)
{
	memset(AimMap,-1,Length);//Bug
	return;
}
