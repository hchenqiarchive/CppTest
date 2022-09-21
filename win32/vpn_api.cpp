#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

#include <stdio.h>

#include <windows.h>
#include "ras.h"
#include "raserror.h"


#pragma comment(lib, "rasapi32.lib")


template<class T> using ref_ptr = T*;


struct VpnInfo {
	std::wstring name;
	ref_ptr<void> connection_handle = nullptr;
};


std::vector<VpnInfo> EnumerateVpn() {
	DWORD ret = 0, size = 0, ras_entry_count = 0;
	ret = RasEnumEntries(nullptr, nullptr, nullptr, &size, &ras_entry_count);
	if (ret != ERROR_BUFFER_TOO_SMALL) { return {}; }

	std::vector<RASENTRYNAME> ras_entry_list(ras_entry_count); ras_entry_list[0].dwSize = sizeof(RASENTRYNAME);
	ret = RasEnumEntries(nullptr, nullptr, ras_entry_list.data(), &size, &ras_entry_count);
	if (ret != ERROR_SUCCESS) { return {}; }

	std::vector<VpnInfo> vpn_list; vpn_list.reserve(ras_entry_count);
	for (auto& ras_entry : ras_entry_list) {
		vpn_list.emplace_back(ras_entry.szEntryName);
	}
	return vpn_list;
}

std::vector<VpnInfo> EnumerateVpnConnected() {
	DWORD ret = 0, size = 0, ras_connection_count = 0;
	ret = RasEnumConnections(nullptr, &size, &ras_connection_count);
	if (ret != ERROR_BUFFER_TOO_SMALL) { return {}; }

	std::vector<RASCONN> ras_connection_list(ras_connection_count); ras_connection_list[0].dwSize = sizeof(RASCONN);
	ret = RasEnumConnections(ras_connection_list.data(), &size, &ras_connection_count);
	if (ret != ERROR_SUCCESS) { return {}; }

	std::vector<VpnInfo> vpn_connected_list; vpn_connected_list.reserve(ras_connection_count);
	for (auto& ras_connection : ras_connection_list) {
		vpn_connected_list.emplace_back(ras_connection.szEntryName, ras_connection.hrasconn);
	}
	return vpn_connected_list;
}

void ConnectVpn(VpnInfo& vpn) {
	if (vpn.connection_handle != nullptr) { return; }
	RASDIALPARAMS ras_entry; ras_entry.dwSize = sizeof(RASDIALPARAMS);
	wcscpy_s(ras_entry.szEntryName, vpn.name.c_str());
	BOOL password_saved;
	RasGetEntryDialParams(nullptr, &ras_entry, &password_saved);
	RasDial(nullptr, nullptr, &ras_entry, 0, nullptr, (HRASCONN*)&vpn.connection_handle);
}

void DisConnectVpn(const VpnInfo& vpn) {
	RasHangUp((HRASCONN)vpn.connection_handle);
}


int main() {
	std::vector<VpnInfo> vpn_list;
	vpn_list = EnumerateVpn();
	for (auto& info : vpn_list) {
		std::wcout << info.name << std::endl;
	}

	ConnectVpn(vpn_list[0]);

	vpn_list = EnumerateVpnConnected();
	for (auto& info : vpn_list) {
		std::wcout << info.name << std::endl;
	}

	DisConnectVpn(vpn_list[0]);

	vpn_list = EnumerateVpnConnected();
	for (auto& info : vpn_list) {
		std::wcout << info.name << std::endl;
	}
}
