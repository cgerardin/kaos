#include <efi.h>
#include <efilib.h>
#include <efiprot.h>
#include "main.h"
#include "logo.h"

EFI_STATUS
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	/*
	 * UEFI boot loader
	 */
    
    InitializeLib(ImageHandle, SystemTable);
    
    uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
	
	Print(LOGO);
	Print(L"\nWelcome to KaOS v0.0.1, the Karrot OS !\n\n");
	Print(L"Press any key to boot, [H] to halt the system, [R] to reboot...\n");
	
	EFI_INPUT_KEY Key;
	while ((SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);

	if(Key.UnicodeChar == 'h') {

		Print(L"System halted.");
		for(;;) __asm__("hlt");
		return EFI_SUCCESS;

	} else if(Key.UnicodeChar == 'r') {

		Print(L"Rebooting the system...\n");
		return EFI_SUCCESS;
	
	}
	
	// Exiting UEFI land
	
	uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
	Print(L"Booting the kernel...");

	unsigned long mapSize = 0;
	unsigned long mapKey;
	unsigned long descriptorSize;
	EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
	uint32_t descriptorVersion;

	uefi_call_wrapper(SystemTable->BootServices->GetMemoryMap, 5, &mapSize, memoryMap, NULL, &descriptorSize, NULL);
	mapSize += 2 * descriptorSize;

	uefi_call_wrapper(SystemTable->BootServices->AllocatePool, 3, EfiLoaderData, mapSize, (void **)&memoryMap);
	uefi_call_wrapper(SystemTable->BootServices->GetMemoryMap, 5, &mapSize, memoryMap, &mapKey, &descriptorSize, &descriptorVersion);
	uefi_call_wrapper(SystemTable->BootServices->ExitBootServices, 2, ImageHandle, mapKey);

	/*
	 * Kernel main. In the futur, must be loaded from external file...
	 */

	while(1);

}

