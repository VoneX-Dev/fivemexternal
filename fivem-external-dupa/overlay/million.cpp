#include "million.h"

#pragma warning (disable : 4244) // O_o

struct tab_anim
{
    float active_text_anim;
    float active_rect_alpha;
};

bool figma_frame::tab(const char* label, bool selected)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x + 10, pos.y + 23));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_anim> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end())
    {
        anim.insert({ id, {0.5f, 0.0f} });
        it_anim = anim.find(id);
    }

    it_anim->second.active_text_anim = ImLerp(it_anim->second.active_text_anim, selected ? 1.0f : hovered ? 0.8f : 0.5f, 0.06f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.active_rect_alpha = ImLerp(it_anim->second.active_rect_alpha, selected ? 1.0f : 0.0f, 0.08f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(ImVec2(rect.Min.x, rect.Max.y + 10), ImVec2(rect.Max.x, rect.Max.y + 12), ImColor(157 / 255.0f, 176 / 255.0f, 242 / 255.0f, it_anim->second.active_rect_alpha), 2.0f, ImDrawFlags_RoundCornersTop);
    window->DrawList->AddText(ImVec2((rect.Min.x + rect.Max.x) / 2 - (label_size.x) / 2 - 1, (rect.Min.y + rect.Max.y + 10) / 2.f - (label_size.y / 2.f)), ImColor(1.0f, 1.0f, 1.0f, it_anim->second.active_text_anim), label);

    return pressed;
}
