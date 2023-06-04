#include "common.hpp"
 
c_memory mem = c_memory("csgo.exe");
 
#ifdef _DEBUG
#error "you must build with release"
#endif // _DEBUG

int main()
{   
    std::thread(overlay::create).detach();
    printf("created overlay!\n");

    if (!mem.attach())
    {
        printf("waiting for csgo.exe...\n");

        while (!mem.attach()) 
            Sleep(500);
    }

 

    printf("found csgo!\n");
    printf("process id -> %i\n", mem.data().th32ProcessID);

    while (!g_game_base)
        g_game_base = reinterpret_cast<std::uintptr_t>(mem.get_module("csgo.exe").modBaseAddr);

    printf("csgo.exe -> 0x%x\n", g_game_base);

    while (!g_client_base)
        g_client_base = reinterpret_cast<std::uintptr_t>(mem.get_module("client.dll").modBaseAddr);

    printf("client.dll -> 0x%x\n", g_client_base);

    overlay::set_rect(0, 0, 1920, 1080);
 
    while (true)
    {
        overlay::set_visible(GetForegroundWindow() == FindWindow("Valve001", 0));
        overlay::clear();

        auto local_player = sdk::get_local_player();
        if (!local_player)
            continue;

        auto glow_manager = sdk::get_glow_manager();
        if (!glow_manager)
            continue;

        auto local_team = sdk::get_local_player()->team();
 
        for (int idx = 0; idx < 64; idx++)
        {
            auto entry = sdk::c_player::find(idx);
            if (!entry->is_valid())
                continue;

            auto health = entry->health();
            if (!health || entry->life_state() != 0)
                continue;

            if (entry->team() == local_team)
                continue;
 
            auto pos3d = entry->origin();
            if (!pos3d.x && !pos3d.y && !pos3d.z)
                continue;

            pos3d.z -= 5.f;

            auto pos = sdk::w2s(pos3d);
            if (!pos.x && !pos.y)
                continue;

            auto head3d = sdk::vec3_t{ pos3d.x, pos3d.y, pos3d.z + 80.f };

            auto head = sdk::w2s(head3d);
            if (!head.x && !head.y)
                continue;

            float health_percent = health / 100.f;

            overlay::rect_filled({ pos.x - 30.f, pos.y + 10.f }, { pos.x + 30.f, pos.y + 20.f }, sdk::color_t{ 255, 125, 125, 255 });
            overlay::rect_filled({ pos.x - 30.f, pos.y + 10.f }, { pos.x - 30.f + (30.f * 2.f * health_percent), pos.y + 20.f }, sdk::color_t{ 125, 255, 125, 255 });
            overlay::rect({ pos.x - 30.f, pos.y + 10.f }, { pos.x + 30.f, pos.y + 20.f }, sdk::color_t{ 1, 1, 1, 255 });

            auto h = pos.y - head.y;
            overlay::rect({ pos.x - (h / 3.5f), pos.y }, { pos.x + (h / 3.5f), head.y }, sdk::color_t{ 255, 255, 255, 255 });
            overlay::line({ sdk::get_screen_size().x / 2.f, sdk::get_screen_size().y }, { pos.x, pos.y + 20.f }, sdk::color_t{ 255, 255, 255, 255 });
        }

        overlay::render();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
 