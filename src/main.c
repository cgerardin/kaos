#include <efi.h>
#include <efilib.h>
#include <efiprot.h>
#include <wchar.h>
#include "main.h"
#include "logo.h"
#include "memory.h"
#include "lib/string.h"
#include "drivers/io.h"
#include "drivers/display.h"
#include "drivers/keyboard.h"

EFI_STATUS
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	/*
	 * UEFI boot loader
	 */

	InitializeLib(ImageHandle, SystemTable);

	uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);
	
	Print(LOGO);
	Print(L"\nWelcome to KaOS v%d.%d.%d bootloader !\n\n", KAOS_VERSION_MAJOR, KAOS_VERSION_MINOR, KAOS_VERSION_REVISION);
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
        if (gop_mode_info->HorizontalResolution == KAOS_SCREEN_WIDTH &&
              gop_mode_info->VerticalResolution == KAOS_SCREEN_HEIGHT &&
              gop_mode_info->PixelFormat        == KAOS_SCREEN_PIXEL_FORMAT)
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
	 
	int k,l;
	// Blank screen
	for(k=0; k<KAOS_SCREEN_WIDTH*KAOS_SCREEN_HEIGHT; k++) { putPixel(gop->Mode->FrameBufferBase, k, 0, 0x00000000); }
	
	// Draw a 50x20 green box...
	for(l=0; l<20; l++) {
		for(k=0; k<50; k++) {
			putPixel(gop->Mode->FrameBufferBase, 10+k, 10+l, 0x00336600);
		}
	}
	
	// Draw a 50x80 orange box...
	for(l=0; l<80; l++) {
		for(k=0; k<50; k++) {
			putPixel(gop->Mode->FrameBufferBase, 10+k, 30+l, 0x00ff8000);
		}
	}

	// ...the OS name and version...
	putString(gop->Mode->FrameBufferBase, 70, 55, 0x00ff8000, L"KaOS v0.1.1, the Karrot OS !\0");

	// ...a line...
	for(k=0; k<KAOS_SCREEN_WIDTH-20; k++) {
		putPixel(gop->Mode->FrameBufferBase, 10+k, 120, 0x00ff8000);
	}

	// At this point, we need a malloc() function. So we need a real memory manager !
	wchar_t *strbuffer = L"\0ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ"; // Temporary workaround

	// Displaying memory size, kernel last address, first free address
	// and total free memory, in the upper right corner
	extern int end;
	int memorySize=getCmosMemSize(); // (Mo : n/512+1)
	int lastKernelAddress=end;
	int firstFreeAddress=end+1;
	int freeMemorySize=0;

	for(l=0; l<64; l++) {
		for(k=0; k<72; k++) {
			putPixel(gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-104+k, 10+l, 0x004e9a06);
		}
	}
	putString(gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-100, 10, 0x00ffffff, itoa(memorySize/1000, strbuffer, 10));
	putString(gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-100, 26, 0x00ffffff, itoa(lastKernelAddress, strbuffer, 16));
	putString(gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-100, 42, 0x00ffffff, itoa(firstFreeAddress, strbuffer, 16));
	putString(gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-100, 58, 0x00ffffff, itoa(freeMemorySize, strbuffer, 16));

	// Read keyboard raw input
	int kposition=10;
	char key;
	wchar_t c=0;

	while(1) {

		key = getScancode();
		c=scancodeToChar(key);

		// Display charcode in the lower right corner
		for(l=0; l<16; l++) {
			for(k=0; k<36; k++) {
				putPixel(gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-54+k, KAOS_SCREEN_HEIGHT-150+l, 0x0075507b);
			}
		}
		putString(gop->Mode->FrameBufferBase, KAOS_SCREEN_WIDTH-50, KAOS_SCREEN_HEIGHT-150, 0x00ffffff, itoa(key, strbuffer, 16));
		
		// Display the char
		switch(c) {

			case '\n':
				kposition=10;
				break;

			case '\b':
				kposition-=8;
				for(l=0; l<16; l++) {
					for(k=0; k<8; k++) {
						putPixel(gop->Mode->FrameBufferBase, kposition+k, 262+l, 0x00000000);
					}
				}
				break;

			default:

				putChar(gop->Mode->FrameBufferBase, kposition, 262, 0x00ffffff, c);
				kposition+=8;

		}
	
	}

}

