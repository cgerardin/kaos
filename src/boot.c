/*
 * UEFI boot loader
 */

#include "boot.h"

#include "kernel.h" // Temporary workaround for call kmain. Must be replaced by loading kernel from disk

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

    InitializeLib(ImageHandle, SystemTable);
    EFI_STATUS status = EFI_SUCCESS;

    uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1, SystemTable->ConOut);

    // Getting memory informations
    uint64_t totalMemory = 0;
    uint64_t freeMemory = 0;
    uint64_t firstAddress = 0;
    uint64_t lastAddress = 0;

    uint64_t MemMapSize = sizeof (EFI_MEMORY_DESCRIPTOR)*16;
    uint64_t MemMapSizeOut = MemMapSize;
    uint64_t MemMapKey = 0;
    uint64_t MemMapDescriptorSize = 0;
    uint32_t MemMapDescriptorVersion = 0;
    uint64_t DescriptorCount = 0;
    uint8_t *buffer = NULL;
    EFI_MEMORY_DESCRIPTOR *MemoryDescriptorPtr = NULL;

    // Discover memory map size
    do {

        buffer = AllocatePool(MemMapSize);
        if (buffer == NULL) {
            break;
        }

        status = SystemTable->BootServices->GetMemoryMap(&MemMapSizeOut, (EFI_MEMORY_DESCRIPTOR*) buffer,
                &MemMapKey, &MemMapDescriptorSize, &MemMapDescriptorVersion);

        if (status != EFI_SUCCESS) {
            FreePool(buffer);
            MemMapSize += sizeof (EFI_MEMORY_DESCRIPTOR)*16;
        }

    } while (status != EFI_SUCCESS);

    // Discover memory map and calcul total/free memory
    if (buffer != NULL) {

        DescriptorCount = MemMapSizeOut / MemMapDescriptorSize;
        MemoryDescriptorPtr = (EFI_MEMORY_DESCRIPTOR*) buffer;

        for (int i = 0; i < DescriptorCount; i++) {

            MemoryDescriptorPtr = (EFI_MEMORY_DESCRIPTOR*) (buffer + (i * MemMapDescriptorSize));

            if (MemoryDescriptorPtr->Type == EfiLoaderData
                    || MemoryDescriptorPtr->Type == EfiBootServicesData
                    || MemoryDescriptorPtr->Type == EfiRuntimeServicesData
                    || MemoryDescriptorPtr->Type == EfiConventionalMemory) {

                freeMemory += MemoryDescriptorPtr->NumberOfPages*EFI_PAGE_SIZE;
                if (firstAddress == 0) firstAddress = MemoryDescriptorPtr->PhysicalStart;
                lastAddress = MemoryDescriptorPtr->PhysicalStart + MemoryDescriptorPtr->NumberOfPages * EFI_PAGE_SIZE;

            }

            totalMemory += MemoryDescriptorPtr->NumberOfPages*EFI_PAGE_SIZE;

        }

        FreePool(buffer);

    }

    // Initialize framebuffer (GOP)
    static EFI_GRAPHICS_OUTPUT_PROTOCOL *framebuffer;
    static EFI_GUID GopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    uefi_call_wrapper(SystemTable->BootServices->LocateProtocol, 3, &GopGuid, NULL, (void **) &framebuffer);

    // Set desired resolution
    for (int i = 0; i < framebuffer->Mode->MaxMode; i++) {

        EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
        uint64_t infoSize;
        framebuffer->QueryMode(framebuffer, i, &infoSize, &info);

        if (info->HorizontalResolution >= KAOS_SCREEN_WIDTH && info->VerticalResolution >= KAOS_SCREEN_HEIGHT) {
            framebuffer->SetMode(framebuffer, i);
            break;
        }

        FreePool(info);

    }

    // Exiting UEFI land
    uefi_call_wrapper(SystemTable->BootServices->ExitBootServices, 2, ImageHandle, MemMapKey);

    // Loading kernel (Temporary workaround before loading a real executable from disk)
    kmain(totalMemory, freeMemory, lastAddress, framebuffer);

    return EFI_SUCCESS;
}

