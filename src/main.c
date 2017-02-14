#include <efi.h>
#include <efilib.h>
#include "main.h"
#include "cpu.h"
#include "memory.h"
#include "gui.h"
#include "lib/types.h"
#include "lib/string.h"
#include "drivers/io.h"
#include "drivers/display.h"
#include "drivers/keyboard.h"

#include "pictures/cpu.h"
#include "pictures/ram.h"

EFI_STATUS efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

	/*
	 * UEFI boot loader
	 */

	InitializeLib(ImageHandle, SystemTable);
	EFI_STATUS status=EFI_SUCCESS;

	uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);

	// Getting memory informations
	// http://stackoverflow.com/questions/17591351/converting-efi-memory-map-to-e820-map
	
	uint64_t totalMemory=0;
	uint64_t freeMemory=0;
	uint64_t firstAddress=0;
	uint64_t lastAddress=0;

    uint64_t MemMapSize=sizeof(EFI_MEMORY_DESCRIPTOR)*16;
    uint64_t MemMapSizeOut=MemMapSize;
    uint64_t MemMapKey=0;
    uint64_t MemMapDescriptorSize=0;
    uint32_t MemMapDescriptorVersion=0;
    uint64_t DescriptorCount=0;
    uint8_t *buffer=NULL;
    EFI_MEMORY_DESCRIPTOR *MemoryDescriptorPtr=NULL;

	// Discover memory map size
    do {

        buffer = AllocatePool(MemMapSize);
        if (buffer == NULL) {
			break;
        }

        status = SystemTable->BootServices->GetMemoryMap(&MemMapSizeOut, (EFI_MEMORY_DESCRIPTOR*)buffer,
			&MemMapKey, &MemMapDescriptorSize, &MemMapDescriptorVersion);

        if (status != EFI_SUCCESS) {
            FreePool(buffer);
            MemMapSize += sizeof(EFI_MEMORY_DESCRIPTOR)*16;
        }

    } while (status != EFI_SUCCESS);

	// Discover memory map and calcul total/free memory
    if (buffer != NULL) {

        DescriptorCount = MemMapSizeOut / MemMapDescriptorSize;
        MemoryDescriptorPtr = (EFI_MEMORY_DESCRIPTOR*)buffer;

        for (int i=0; i<DescriptorCount; i++) {

            MemoryDescriptorPtr = (EFI_MEMORY_DESCRIPTOR*)(buffer + (i*MemMapDescriptorSize));

            if (MemoryDescriptorPtr->Type == EfiLoaderData
		        || MemoryDescriptorPtr->Type == EfiBootServicesData
		        || MemoryDescriptorPtr->Type == EfiRuntimeServicesData
		        || MemoryDescriptorPtr->Type == EfiConventionalMemory) {

				freeMemory += MemoryDescriptorPtr->NumberOfPages*EFI_PAGE_SIZE;
				if(firstAddress==0) firstAddress=MemoryDescriptorPtr->PhysicalStart;
				lastAddress = MemoryDescriptorPtr->PhysicalStart + MemoryDescriptorPtr->NumberOfPages * EFI_PAGE_SIZE;

			}

			totalMemory += MemoryDescriptorPtr->NumberOfPages*EFI_PAGE_SIZE;

        }

        FreePool(buffer);

    }

	// Initialize framebuffer (GOP)
    static EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
	static EFI_GUID GopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    uefi_call_wrapper(SystemTable->BootServices->LocateProtocol, 3, &GopGuid, NULL, (void **)&Gop);

	// Set desired resolution
    for (int i = 0; i < Gop->Mode->MaxMode; i++) {

        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
        uint64_t infoSize;
        Gop->QueryMode(Gop, i, &infoSize, &info);

		if(info->HorizontalResolution>=KAOS_SCREEN_WIDTH && info->VerticalResolution>=KAOS_SCREEN_HEIGHT) {
			Gop->SetMode(Gop, i);
			break;
		}

		FreePool(info);

    }


	// Exiting UEFI land
	uefi_call_wrapper(SystemTable->BootServices->ExitBootServices, 2, ImageHandle, MemMapKey);


	/*
	 * Kernel main. In the futur, must be loaded from external file...
	 */

	// Initialize the quick'n'dirty memory manager
	init_memory_manager(totalMemory, freeMemory, lastAddress);
	
	// Make a string with OS name and version
	wchar_t *osName = kmalloc(200 * sizeof(wchar_t));
	wchar_t *osMajor = kmalloc(3 * sizeof(wchar_t));
	wchar_t *osMinor = kmalloc(3 * sizeof(wchar_t));
	wchar_t *osRevision = kmalloc(3 * sizeof(wchar_t));
	strf(osName, 7, L"KaOS v", itoa(KAOS_VERSION_MAJOR, osMajor, 10), L".", 
		itoa(KAOS_VERSION_MINOR, osMinor, 10), L".", 
			itoa(KAOS_VERSION_REVISION, osRevision, 10), L", the Karrot OS !");

	// Make a string with CPU informations
	wchar_t *strCpu = kmalloc(50 * sizeof(wchar_t));
	strf(strCpu, 2, detect_cpu(), L" CPU @ 0 Mhz");

	// Make a string with memory informations
	wchar_t *strMem = kmalloc(50 * sizeof(wchar_t));
	wchar_t *memTotal = kmalloc(20 * sizeof(wchar_t));
	wchar_t *memFree = kmalloc(20 * sizeof(wchar_t));

	uint32_t gbuff1[Gop->Mode->FrameBufferSize*sizeof(uint32_t)];
	int i=0;

	wchar_t c1=0;
	wchar_t c2=' ';

	while(1) {

		// Make a string with current memory status
		strf(strMem, 5, itoa(get_total_memory()/1024/1024, memTotal, 10), L" Mo", 
			L" / ", itoa(get_free_memory()/1024/1024, memFree, 10), L" Mo");

		// Draw background
		drawFillRectangle(gbuff1, 0, 0, KAOS_SCREEN_WIDTH, KAOS_SCREEN_HEIGHT, 0x00babdb6);

		// Draw a window
		drawWindow(gbuff1, 50, 130, 800, 480, 0x00ffffff, osName);
		drawPicture(picture_cpu, gbuff1, 60, 170, picture_cpu_width, picture_cpu_height, ALPHA_COLOR);
		drawPicture(picture_ram, gbuff1, 60, 212, picture_ram_width, picture_ram_height, ALPHA_COLOR);
		drawString(gbuff1, 102, 178, 0x00000000, strCpu);
		drawString(gbuff1, 102, 220, 0x00000000, strMem);
		drawString(gbuff1, 60, 236+(i*16), 0x00cc0000, L"Memory stress test in progress, please wait for the world's end...\n");
		i++;
		if(i>22) i=0;

		drawRectangle(gbuff1, 300, 170, 200, 20, 0x006e3800);
		c1=scancodeToChar(getScancode());
		if(c1!=0) c2=c1;
		drawChar(gbuff1, 304, 174, 0x00000000, c2);

		// Display the entire frame
		blitBufferToScreen(Gop->Mode->FrameBufferBase, gbuff1);

		// Memory overflow test... due to the stupid memory allocator, this must kill the kernel's code in memory
		char *stress=kmalloc(20000);
		for(uint64_t m=0; m<20000; m+=1000) stress[m]='A';

	}

}

