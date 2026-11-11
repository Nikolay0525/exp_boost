// exp_boost: Kyubu port of the Cube-World-EXP-Buff mod.
//
// The old mod replaced the level curve (CalculateMaxExp) and patched three inlined copies of it.
// Here the XP *gained* is scaled instead, which is the same thing for the player (a x2 multiplier
// means a level takes half as many kills) and needs a single hook:
//
//   cube::World::killXpSite  (Cube.exe 0x59638D, Server.exe twin)
//   in applyHit's death branch, right after the game stored the kill XP as a float at [EBP-0x2F8].
//   Writing a different value there changes the XP of every player in the threat map.
//   This is the same site level_rework uses (see its main.cpp).
//
// Not scaled: the type-13 item XP (zone completion bonus, cube::GameController::itemXpSite).
#include "kyubu.hpp"
#include "cubeworld/cubeworld.h"

#include <cstring>

namespace {

volatile float g_multiplier = 3.0f;

void KYUBU_CALL kill_xp_site(KyubuCpuContext* ctx, void*) {
    float* xp = reinterpret_cast<float*>(ctx->ebp - 0x2F8);
    *xp = *xp * g_multiplier;
}

}  // namespace

struct ExpBoost {
    explicit ExpBoost(kyubu::Mod& m) : mod(m) {
        // Single-player Cube.exe initialises the mod once per side; hook only once. On Server.exe only the
        // server side exists, so it installs there.
        KyubuHostInfo host{};
        host.size = sizeof host;
        const bool server_host = m.api()->host_info(&host) == KY_OK && host.host == KY_HOST_SERVER;
        if (!server_host && m.side() != KY_SIDE_CLIENT) return;

        g_multiplier = static_cast<float>(m.config_float("exp_multiplier", 3.0));

        const uintptr_t site = m.symbol(CW_SYM_cube_World_killXpSite);
        if (!site) {
            m.error("exp_boost: symbol %s is unavailable on this build: kill XP stays vanilla", CW_SYM_cube_World_killXpSite);
            return;
        }
        KyubuId id = 0;
        const KyubuResult r = m.api()->hook_mid(m.handle(), site, &kill_xp_site, nullptr, &id);
        if (r != KY_OK) {
            m.error("exp_boost: could not hook the kill XP site (result %d): kill XP stays vanilla", static_cast<int>(r));
            return;
        }
        m.info("exp_boost: kill XP x%.2f (hook at 0x%08X, %s)", static_cast<double>(g_multiplier),
               static_cast<unsigned>(site), server_host ? "Server.exe" : "Cube.exe");

        // Changing exp_multiplier in the config applies without a restart.
        m.on("config.changed", [this](KyubuEvent& ev) {
            auto* c = kyubu::Mod::data<KyubuConfigChangedData>(ev);
            if (c && c->mod && c->key && std::strcmp(c->mod, "exp_boost") == 0 && std::strcmp(c->key, "exp_multiplier") == 0) {
                g_multiplier = static_cast<float>(mod.config_float("exp_multiplier", 3.0));
                mod.info("exp_boost: kill XP now x%.2f", static_cast<double>(g_multiplier));
            }
        });
    }
    kyubu::Mod& mod;
};

KYUBU_MOD("exp_boost", KY_SIDE_CLIENT | KY_SIDE_SERVER, ExpBoost)
