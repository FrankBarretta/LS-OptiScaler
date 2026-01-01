#pragma once
#include "FSR31Feature.h"
#include <upscalers/IFeature_Dx11wDx12.h>

#include "dx12/ffx_api_dx12.h"
#include "proxies/FfxApi_Proxy.h"

class FSR31FeatureDx11on12 : public FSR31Feature, public IFeature_Dx11wDx12
{
  private:
    bool _baseInit = false;
    NVSDK_NGX_Parameter* SetParameters(NVSDK_NGX_Parameter* InParameters);

  protected:
    bool InitFSR3(const NVSDK_NGX_Parameter* InParameters);

  public:
    std::string Name() const { return "FSR3 w/Dx12"; }

    FSR31FeatureDx11on12(unsigned int InHandleId, NVSDK_NGX_Parameter* InParameters);

    bool Init(ID3D11Device* InDevice, ID3D11DeviceContext* InContext, NVSDK_NGX_Parameter* InParameters) override;
    bool Evaluate(ID3D11DeviceContext* InDeviceContext, NVSDK_NGX_Parameter* InParameters) override;

    ~FSR31FeatureDx11on12()
    {
        // ALWAYS destroy the FFX context, even during shutdown
        // Not destroying it leaves orphaned resources that crash 1-2 seconds later
        // when FFX internal threads/callbacks access freed memory

        // Wait for GPU to complete all pending operations before destroying context
        if (Dx12Fence != nullptr && Dx12FenceEvent != nullptr && Dx12CommandQueue != nullptr)
        {
            UINT64 fenceValue = Dx12Fence->GetCompletedValue() + 1;
            Dx12CommandQueue->Signal(Dx12Fence, fenceValue);
            if (Dx12Fence->GetCompletedValue() < fenceValue)
            {
                Dx12Fence->SetEventOnCompletion(fenceValue, Dx12FenceEvent);
                WaitForSingleObject(Dx12FenceEvent, 5000); // 5 second timeout
            }
        }

        if (_context != nullptr)
            FfxApiProxy::D3D12_DestroyContext()(&_context, NULL);
    }
};
