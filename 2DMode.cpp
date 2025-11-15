#include "2DMode.h"
#include <vector>
#include <cmath>


struct Line
{
    ImVec2 start; // stored in grid space
    ImVec2 end;   // stored in grid space
    ImVec2 normal; // new
};

std::vector<Line> lines;
ImVec2 spawnpointV = ImVec2(0.0f, 0.0f);

// temp world bounds
float WL = -10.0f;
float WR = 10.0f;
float WT = -10.0f;
float WB = 10.0f;



Wall toWorldSpace(const Line& line, float spacing) {
    Wall worldLine;
    worldLine.start = glm::vec2(line.start.x / spacing, line.start.y / spacing);
    worldLine.end = glm::vec2(line.end.x / spacing, line.end.y / spacing);
    return worldLine;
}


void Mode2D::render2D()
{

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_MenuBar;
        //ImGuiWindowFlags_NoDocking; // Prevent undocking

    ImGui::Begin("Level Editor", nullptr, flags );



    if (ImGui::BeginMenuBar())
    {
        if (ImGui::Button("New"))
        {
            lines.clear();
        }

        ImGui::SameLine(); // you can put more buttons here
        if (ImGui::Button("Load"))
        {
            lvl.sectors.clear();
            Sector sector;
            ImVec2 dims = ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - 20);

            for (Line line : lines) {
                std::mt19937 rng(std::random_device{}());
                std::uniform_real_distribution<float> dist(0.0f, 1.0f);


				Wall wall = toWorldSpace(line, 40.0f);

                //wall.start = glm::vec2(line.end.x, line.end.y) * 0.1f;
                //wall.end = glm::vec2(line.start.x, line.start.y) * 0.1f;


                wall.r = dist(rng);
                wall.g = dist(rng);
                wall.b = dist(rng);

                sector.walls.push_back(std::move(wall));
            }

            lvl.sectors.push_back(std::move(sector));
        }

        ImGui::EndMenuBar();
    }



    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Invisible button to capture mouse input
    ImGui::InvisibleButton("canvas", canvas_size);
    bool is_hovered = ImGui::IsItemHovered();
    ImVec2 mouse = ImGui::GetIO().MousePos;


    // --- Panning ---
    static ImVec2 offset = ImVec2(0, 0);
    static ImVec2 last_mouse = mouse;
    if (is_hovered && ImGui::IsMouseDown(ImGuiMouseButton_Middle))
    {
        offset.x += mouse.x - last_mouse.x;
        offset.y += mouse.y - last_mouse.y;
    }

    //if (is_hovered && ImGui::IsMouseDown(ImGuiMouseButton_Right))
    //{
    //    spawnpointV.x = mouse.x;
    //    spawnpointV.y = mouse.y;

    //    spawnpoint.x = spawnpointV.x ;
    //    spawnpoint.y = spawnpointV.x;
    //}

    last_mouse = mouse;

    float spacing = 40.0f; // fixed spacing

    // --- Draw Dot Grid ---
    for (float x = fmodf(offset.x, spacing); x < canvas_size.x; x += spacing)
    {
        for (float y = fmodf(offset.y, spacing); y < canvas_size.y; y += spacing)
        {
            if (offset.x > 0 && offset.y > 0) {
				if (x == 0 || y == 0) continue;
            }
            draw_list->AddCircleFilled(
                ImVec2(canvas_pos.x + x, canvas_pos.y + y),
                2.0f,
                IM_COL32(200, 200, 200, 255)
            );
        }
    }

    // --- Line Placement ---
    static bool placing_line = false;
    static ImVec2 line_start;

    auto snap_to_grid = [&](const ImVec2& pos) -> ImVec2
        {
            ImVec2 local;
            local.x = pos.x - canvas_pos.x - offset.x;
            local.y = pos.y - canvas_pos.y - offset.y;

            ImVec2 snapped;
            snapped.x = round(local.x / spacing) * spacing;
            snapped.y = round(local.y / spacing) * spacing;
            return snapped;
        };

    // Compute normal of a line
    auto compute_normal = [](Line& line)
        {
            glm::vec2 e1 = glm::vec2(line.start.x, line.start.y) - glm::vec2(line.end.x, line.end.y);
            glm::vec2 normal = glm::normalize(glm::vec2(-e1.y, e1.x));
            line.normal = ImVec2(normal.x, normal.y);
        };

    if (is_hovered)
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            placing_line = true;
            line_start = snap_to_grid(mouse);
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && placing_line)
        {
            Line new_line;
            new_line.start = line_start;
            new_line.end = snap_to_grid(mouse);
            compute_normal(new_line); // compute the normal
            lines.push_back(new_line);
            placing_line = false;
        }
    }

    // --- Draw Lines ---
    for (const auto& line : lines)
    {
        // Line
        draw_list->AddLine(
            ImVec2(canvas_pos.x + offset.x + line.start.x,
                canvas_pos.y + offset.y + line.start.y),
            ImVec2(canvas_pos.x + offset.x + line.end.x,
                canvas_pos.y + offset.y + line.end.y),
            IM_COL32(255, 0, 0, 255),
            3.0f
        );

        // Draw normal for debugging (small green line at midpoint)
        ImVec2 mid = { (line.start.x + line.end.x) * 0.5f,
                       (line.start.y + line.end.y) * 0.5f };
        float normal_length = 20.0f;
        draw_list->AddLine(
            ImVec2(canvas_pos.x + offset.x + mid.x,
                canvas_pos.y + offset.y + mid.y),
            ImVec2(canvas_pos.x + offset.x + mid.x + line.normal.x * normal_length,
                canvas_pos.y + offset.y + mid.y + line.normal.y * normal_length),
            IM_COL32(0, 255, 0, 255),
            2.0f
        );
    }

    // Draw preview line
    if (placing_line)
    {
        ImVec2 snap_mouse = snap_to_grid(mouse);
        draw_list->AddLine(
            ImVec2(canvas_pos.x + offset.x + line_start.x,
                canvas_pos.y + offset.y + line_start.y),
            ImVec2(canvas_pos.x + offset.x + snap_mouse.x,
                canvas_pos.y + offset.y + snap_mouse.y),
            IM_COL32(255, 255, 0, 255),
            2.0f
        );
    }

    ImGui::End();
}
