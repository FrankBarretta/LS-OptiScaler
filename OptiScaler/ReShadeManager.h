#pragma once
#include "pch.h"

struct OptiScalerReShadeData {
    uint64_t depth_resource;
    uint64_t mv_resource;
    float jitterX;
    float jitterY;
    uint32_t frame_index;
};

class ReShadeManager {
public:
    static ReShadeManager& Instance();
    bool Init();
    void Shutdown();
    bool IsConnected() const { return m_pSharedData != nullptr; }
    OptiScalerReShadeData* GetData() { return m_pSharedData; }

private:
    ReShadeManager() = default;
    ~ReShadeManager() = default;
    HANDLE m_hMapFile = nullptr;
    OptiScalerReShadeData* m_pSharedData = nullptr;
};
