//                    ELECTRO ENGINE
// Copyright(c) 2021 - Electro Team - All rights reserved
#include "epch.hpp"
#include "PanelManager.hpp"

namespace Electro
{
    void PanelManager::PushPanel(const String& name, IPanel* panel, bool* showSwitch, void* initValue)
    {
        mPanelMap[name] = { showSwitch, panel };
        panel->Init(initValue);
    }

    IPanel* PanelManager::GetPanel(const String& name)
    {
        for (auto& [panelName, panel] : mPanelMap)
        {
            if (panelName == name)
                return panel.Data2;
        }
        ELECTRO_WARN("No Panel found with name %s!", name.c_str());
        return nullptr;
    }

    void PanelManager::RenderAllPanels()
    {
        for (auto& [name, panel] : mPanelMap)
        {
            bool* showSwitch = panel.Data1;
            IPanel* actualPanel = panel.Data2;
            if(*showSwitch)
                actualPanel->OnImGuiRender(showSwitch);
        }
    }
}