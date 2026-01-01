#include "ReShadeManager.h"

ReShadeManager& ReShadeManager::Instance() {
    static ReShadeManager instance;
    return instance;
}

bool ReShadeManager::Init() {
    if (m_pSharedData) return true;

    m_hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, TEXT("OptiScaler_ReShade_Data"));
    if (m_hMapFile == nullptr) return false;

    m_pSharedData = (OptiScalerReShadeData*)MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0, sizeof(OptiScalerReShadeData));
    return m_pSharedData != nullptr;
}

void ReShadeManager::Shutdown() {
    if (m_pSharedData) {
        UnmapViewOfFile(m_pSharedData);
        m_pSharedData = nullptr;
    }
    if (m_hMapFile) {
        CloseHandle(m_hMapFile);
        m_hMapFile = nullptr;
    }
}
