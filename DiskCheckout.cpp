#include "DiskCheckout.h"

NTSTATUS 
DiskCheckAddDevice(
	_DRIVER_OBJECT * DriverObject,
	_DEVICE_OBJECT * PhysicalDeviceObject
)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT FilteObject,NextDevice=NULL;
	PDISKEXTEND Me = NULL;
	PAGED_CODE();
	status=IoCreateDevice(DriverObject,sizeof(DISKEXTEND),NULL,PhysicalDeviceObject->DeviceType,
		PhysicalDeviceObject->Characteristics,FALSE,&FilteObject);
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
		}
	}
CLEAN:
	IoDeleteDevice(FilteObject);
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
