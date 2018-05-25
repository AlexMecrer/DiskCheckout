#include "DiskCheckout.h"

VOID KDIskstartRoutine(PVOID StartContext)
{
	PDISKEXTEND Me = (PDISKEXTEND)StartContext;
	while (!Me->PsOffFalg)
	{
		if (Me->R.RequitNumber != 0)
		{
			
		}
	}
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
			KeInitializeMutex(&(Me->R.mutex),0);
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
