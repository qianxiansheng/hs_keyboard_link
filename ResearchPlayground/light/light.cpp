#include "light.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

const char* lights[] = {
    u8"随波逐流-1",
    u8"随波逐流-2",
    u8"随波逐流-3",
    u8"随波逐流-4",
    u8"常亮模式",
    u8"呼吸模式",
    u8"蛇形跑马",
    u8"两头跑马",
    u8"正弦光波",
    u8"踏雪无痕",
    u8"单色瀑布",
    u8"穿插对跑",
    u8"斜拉变化",
    u8"一触即发",
    u8"如影随形",
    u8"涟漪扩散",
};
int lightSize = sizeof(lights) / sizeof(lights[0]);

void ShowLightWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
	if (!ImGui::Begin(WINNAME_LIGHT, p_open, window_flags)) {
		ImGui::End();
		return;
	}

    auto dockNode = ImGui::GetCurrentWindow()->DockNode;
    if (dockNode) dockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	ImGui::Text(u8"灯效设置");


    static int selected = -1;
    for (int n = 0; n < lightSize; n++)
    {
        if (ImGui::Selectable(lights[n], selected == n))
            selected = n;
    }

	ImGui::End();
}
