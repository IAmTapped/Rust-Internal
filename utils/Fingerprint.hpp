#pragma once
#include <intrin.h>
#include <cctype>

struct SystemFingerprint
{
private:
	template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1)
	{
		static const char* digits = "0123456789ABCDEF";
		std::string rc(hex_len, '0');
		for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
			rc[i] = digits[(w >> j) & 0x0f];
		return rc;
	}
public:
	static constexpr int FingerprintSize = 16;
	unsigned char UniqueFingerprint[16];

	std::string ToString()
	{
		std::string OutString;
		for (int i = 0; i < FingerprintSize - 1; i++)
			OutString += n2hexstr(UniqueFingerprint[i]);
		return OutString;
	}

	static std::string Pad4Byte(std::string const& str)
	{
		const auto s = str.size() + (4 - str.size() % 4) % 4;

		if (str.size() < s)
			return str + std::string(s - str.size(), ' ');
		return str;
	}

	void InitializeFingerprint()
	{
		for (int i = 0; i < FingerprintSize - 1; i++)
			UniqueFingerprint[i] = ~i & 255;
	}

	__forceinline void Interleave(unsigned long Data)
	{
		*(unsigned long*)UniqueFingerprint
			= *(unsigned long*)UniqueFingerprint ^ Data + 0x2EF35C3D;
		*(unsigned long*)(UniqueFingerprint + 4)
			= *(unsigned long*)(UniqueFingerprint + 4) ^ Data + 0x6E50D365;
		*(unsigned long*)(UniqueFingerprint + 8)
			= *(unsigned long*)(UniqueFingerprint + 8) ^ Data + 0x73B3E4F9;
		*(unsigned long*)(UniqueFingerprint + 12)
			= *(unsigned long*)(UniqueFingerprint + 12) ^ Data + 0x1A044581;

		/* assure no reversal */
		unsigned long OriginalValue = *(unsigned long*)(UniqueFingerprint);
		*(unsigned long*)UniqueFingerprint ^= *(unsigned long*)(UniqueFingerprint + 12);
		*(unsigned long*)(UniqueFingerprint + 12) ^= OriginalValue * 0x3D05F7D1 + *(unsigned long*)UniqueFingerprint;
		UniqueFingerprint[0] = UniqueFingerprint[15] + UniqueFingerprint[14];
		UniqueFingerprint[14] = UniqueFingerprint[0] + UniqueFingerprint[15];
	}

	static __forceinline bool IsNumber(const std::string& s)
	{
		auto it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}

	static __forceinline std::string GetPhysicalDriveId(DWORD Id)
	{
		const auto H = CreateFileA((std::string(xorstr_("\\\\.\\PhysicalDrive")) + std::to_string(Id)).c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (H == INVALID_HANDLE_VALUE)
		{
			return xorstr_("Unknown");
		}

		std::unique_ptr<std::remove_pointer<HANDLE>::type, void(*)(HANDLE)> HDevice{ H, [](HANDLE handle) { CloseHandle(handle); } };

		STORAGE_PROPERTY_QUERY StoragePropQuery{};
		StoragePropQuery.PropertyId = StorageDeviceProperty;
		StoragePropQuery.QueryType = PropertyStandardQuery;

		STORAGE_DESCRIPTOR_HEADER StorageDescHeader{};
		DWORD dwBytesReturned = 0;
		if (!DeviceIoControl(HDevice.get(), IOCTL_STORAGE_QUERY_PROPERTY, &StoragePropQuery, sizeof(STORAGE_PROPERTY_QUERY),
			&StorageDescHeader, sizeof(STORAGE_DESCRIPTOR_HEADER), &dwBytesReturned, NULL))
		{
			return xorstr_("Unknown");
		}

		const auto OutBufferSize = StorageDescHeader.Size;
		std::unique_ptr<BYTE[]> OutBuffer{ new BYTE[OutBufferSize]{} };
		SecureZeroMemory(OutBuffer.get(), OutBufferSize);

		if (!DeviceIoControl(HDevice.get(), IOCTL_STORAGE_QUERY_PROPERTY, &StoragePropQuery, sizeof(STORAGE_PROPERTY_QUERY),
			OutBuffer.get(), OutBufferSize, &dwBytesReturned, NULL))
		{
			return xorstr_("Unknown");
		}

		const auto DeviceDescriptor = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(OutBuffer.get());
		const auto DwSerialNumber = DeviceDescriptor->SerialNumberOffset;
		if (DwSerialNumber == 0)
		{
			return xorstr_("Unknown");
		}

		const auto SerialNumber = reinterpret_cast<const char*>(OutBuffer.get() + DwSerialNumber);
		return SerialNumber;
	}

	static SystemFingerprint* CreateUniqueFingerprint()
	{
		using namespace winreg;

			SystemFingerprint* Fingerprint = new SystemFingerprint;
		Fingerprint->InitializeFingerprint();

		/* pass 1: cpuid */
		int cpuid_sum = 0;
		int cpuid_int[4] = { 0, 0, 0, 0 };
		__cpuid(cpuid_int, (int)0x80000001);
		cpuid_sum = cpuid_int[0] + cpuid_int[1] + (cpuid_int[2] | 0x8000) + cpuid_int[3];
		Fingerprint->Interleave(cpuid_sum);

		/* pass 2: hard disk serial number */
		DWORD HddNumber = 0;
		GetVolumeInformationA("C://", NULL, NULL, &HddNumber, NULL, NULL, NULL, NULL);
		Fingerprint->Interleave(HddNumber);

		/* pass 3: computer name */
		char ComputerName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD ComputerNameLength = sizeof(ComputerName);
		SecureZeroMemory(ComputerName, ComputerNameLength);
		GetComputerNameA(ComputerName, &ComputerNameLength);
		for (DWORD i = 0; i < ComputerNameLength; i += 4)
		{
			Fingerprint->Interleave(*(unsigned long*)&ComputerName[i]);
		}

		/* pass 4: reg info */
		RegKey SysInfoKey{ HKEY_LOCAL_MACHINE, xorstr_("SYSTEM\\CurrentControlSet\\Control\\SystemInformation"), KEY_READ };
		if (SysInfoKey.FindStringValue(xorstr_("ComputerHardwareId")))
		{
			auto CompHwid = Pad4Byte(SysInfoKey.GetStringValue(xorstr_("ComputerHardwareId")));
			for (size_t i = 0; i < CompHwid.size(); i += 4)
			{
				Fingerprint->Interleave(*(unsigned long*)&CompHwid[i]);
			}
		}

		RegKey BIOSKey{ HKEY_LOCAL_MACHINE, xorstr_("HARDWARE\\DESCRIPTION\\System\\BIOS"), KEY_READ };
		if (BIOSKey.FindStringValue(xorstr_("BIOSVendor")))
		{
			auto BiosVendor = Pad4Byte(BIOSKey.GetStringValue(xorstr_("BIOSVendor")));
			for (size_t i = 0; i < BiosVendor.size(); i += 4)
			{
				Fingerprint->Interleave(*(unsigned long*)&BiosVendor[i]);
			}
		}

		if (BIOSKey.FindStringValue(xorstr_("BIOSReleaseDate")))
		{
			auto BiosReleaseDate = Pad4Byte(BIOSKey.GetStringValue(xorstr_("BIOSReleaseDate")));
			for (size_t i = 0; i < BiosReleaseDate.size(); i += 4)
			{
				Fingerprint->Interleave(*(unsigned long*)&BiosReleaseDate[i]);
			}
		}

		if (BIOSKey.FindStringValue(xorstr_("SystemManufacturer")))
		{
			auto SystemManufacturer = Pad4Byte(BIOSKey.GetStringValue(xorstr_("SystemManufacturer")));
			for (size_t i = 0; i < SystemManufacturer.size(); i += 4)
			{
				Fingerprint->Interleave(*(unsigned long*)&SystemManufacturer[i]);
			}
		}

		if (BIOSKey.FindStringValue(xorstr_("SystemProductName")))
		{
			auto SystemProductName = Pad4Byte(BIOSKey.GetStringValue(xorstr_("SystemProductName")));
			for (size_t i = 0; i < SystemProductName.size(); i += 4)
			{
				Fingerprint->Interleave(*(unsigned long*)&SystemProductName[i]);
			}
		}

		RegKey CPUKey{ HKEY_LOCAL_MACHINE, xorstr_("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), KEY_READ };
		auto ProcessorNameString = Pad4Byte(CPUKey.GetStringValue(xorstr_("ProcessorNameString")));
		for (size_t i = 0; i < ProcessorNameString.size(); i += 4)
		{
			Fingerprint->Interleave(*(unsigned long*)&ProcessorNameString[i]);
		}

		/* pass 5: hard disk serial (IoDeviceControl) */
		auto Id = Pad4Byte(GetPhysicalDriveId(0));

		for (size_t i = 0; i < Id.size(); i += 4)
		{
			Fingerprint->Interleave(*(unsigned long*)&Id[i]);
		}

		/* complete! */
			return Fingerprint;
	}

	SystemFingerprint()
		: UniqueFingerprint{} {};
};