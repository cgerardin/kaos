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
	
	// Initialize framebuffer (GOP)
	
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    uefi_call_wrapper(SystemTable->BootServices->LocateProtocol, 3, &gop_guid, NULL, (void **)&gop);
    
    // This code suck - Let's using hard-coded mode number at this time...
    //EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gop_mode_info;
    //UINTN size_of_info;
    /*UINTN mode_num;
    for (mode_num = 0; gop->QueryMode(gop, mode_num, &size_of_info, &gop_mode_info); mode_num++) {
        if (gop_mode_info->HorizontalResolution == DESIRED_HREZ &&
              gop_mode_info->VerticalResolution == DESIRED_VREZ &&
              gop_mode_info->PixelFormat        == DESIRED_PIXEL_FORMAT)
            break;
    }*/
    gop->SetMode(gop, 1); // 800x600
	
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
	 
	int k;
	// Blank screen
	for(k=0; k<DESIRED_HREZ*DESIRED_VREZ; k++) { putPixel(gop->Mode->FrameBufferBase, k, 0, 0x00ffffff); }
	
	// Draw a 100x100 orange box...
	for(k=0; k<100; k++) { 
		putPixel(gop->Mode->FrameBufferBase, 10+k, 10, 0x00ff8000);
		putPixel(gop->Mode->FrameBufferBase, 10+k, 110, 0x00ff8000);
		putPixel(gop->Mode->FrameBufferBase, 10, 10+k, 0x00ff8000);
		putPixel(gop->Mode->FrameBufferBase, 110, 10+k, 0x00ff8000);
	}
	
	while(1);

}

void putPixel(EFI_PHYSICAL_ADDRESS lfb_base_addr, uint32_t x, uint32_t y, uint32_t color) {

    uint32_t* at = (uint32_t*)lfb_base_addr;

    at += (x+y*DESIRED_HREZ);
    *at++ = color;

}

