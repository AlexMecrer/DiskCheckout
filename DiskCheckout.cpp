#include "DiskCheckout.h"

VOID 
KDIskstartRoutine(
	PVOID StartContext
)
{
	PDISKEXTEND Me = (PDISKEXTEND)StartContext;
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL OldIrql;
	CHAR * Buffer = NULL;
	LARGE_INTEGER FileSize,Offset,Length = {0};
	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES attri;
	IO_STATUS_BLOCK StatusBlock;
	UNICODE_STRING FileName = RTL_CONSTANT_STRING(TmpFileName);
	InitializeObjectAttributes(&attri,&FileName,OBJ_KERNEL_HANDLE|OBJ_CASE_INSENSITIVE,NULL,NULL);
	FileSize.QuadPart=
	status=ZwCreateFile(&hFile,
		GENERIC_ALL|DELETE,
		&attri,
		&StatusBlock,
		&Me->DiskSize,
		FILE_ATTRIBUTE_HIDDEN,
		0,
		FILE_CREATE,
		FILE_DELETE_ON_CLOSE,
		NULL,
		NULL);
	if (!NT_SUCCESS(status))
	{
		Me->PsOffFalg = TRUE;
	}
	while (!Me->PsOffFalg)
	{
		if (Me->R.RequitNumber>0)
		{
			PLIST_ENTRY Entry = RemoveHeadList(&(Me->RequitList));
			PIRP pirp = (PIRP)CONTAINING_RECORD(Entry,DISKEXTEND,RequitList);
			InterlockedAdd(&Me->R.RequitNumber,-1);
			PIO_STACK_LOCATION spirp = IoGetCurrentIrpStackLocation(pirp);
			if (pirp->MdlAddress == NULL)
			{
				Buffer = (CHAR*)pirp->AssociatedIrp.SystemBuffer;
			}
			else
			{
				Buffer = (CHAR*)MmGetSystemAddressForMdlSafe(pirp->MdlAddress,NormalPagePriority);
			}
			if (spirp->MajorFunction == IRP_MJ_READ)
			{
				Offset = spirp->Parameters.Read.ByteOffset;
				Length.QuadPart = spirp->Parameters.Read.Length;
			}
			else if (spirp->MajorFunction == IRP_MJ_WRITE)
			{
				
			}
		}
	}
	
	PsTerminateSystemThread(status);
	return;
}

NTSTATUS
DiskCheckAddDevice(
	_DRIVER_OBJECT * DriverObject,
	_DEVICE_OBJECT * PhysicalDeviceObject
)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT FilteObject,NextDevice=NULL;
	PDISKEXTEND Me = NULL;
	HANDLE ThreadHandle;
	PARAM param;
	PAGED_CODE();
	status=IoCreateDevice(DriverObject,sizeof(DISKEXTEND),NULL,PhysicalDeviceObject->DeviceType,
		PhysicalDeviceObject->Characteristics,FALSE,&FilteObject);
	RtlZeroMemory(&param,sizeof(PARAM));
	if (NT_SUCCESS(status))
	{
		NextDevice=IoAttachDeviceToDeviceStack(FilteObject,PhysicalDeviceObject);
		if (NextDevice != NULL)
		{
			Me = (PDISKEXTEND)FilteObject->DeviceExtension;
			RtlZeroMemory(Me,sizeof(DISKEXTEND));
			Me->FilteObject = FilteObject;
			Me->NextObject = NextDevice;
			InitializeListHead(&(Me->RequitList));
			KeInitializeSpinLock(&Me->R.mutex);
			Me->R.RequitNumber = 0;
			Me->PsOffFalg = FALSE;
			param.Me = Me;
			status=PsCreateSystemThread(&ThreadHandle,GENERIC_ALL,NULL,NULL,NULL,KDIskstartRoutine,&param);
			goto RET;
		}
	}
CLEAN:
	if (status == STATUS_SUCCESS)
	{
		IoDeleteDevice(FilteObject);
	}
	FilteObject = NULL;
RET:
	return status;
}

NTSTATUS
DiskCheckDispath(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP Irp
)
{
	NTSTATUS status = STATUS_SUCCESS;
	PAGED_CODE();
	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(DeviceObject,Irp);
}
