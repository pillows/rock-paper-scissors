#include <iostream>
#include <emscripten.h>
// #include "util/Request.hpp"
#include "rock_paper_scissors.hpp"

int main()
{
    #ifdef __EMSCRIPTEN__
  // clang-format off
  EM_ASM(
      FS.mkdir('/sav');
      FS.mkdir('/img');
      FS.mkdir('/img/Map');
      FS.mkdir('/img/Map/Back');
      FS.mkdir("/img/Mob");
      FS.mkdir("/img/String");
      FS.mkdir("/img/Character");
      FS.mkdir("/img/Effect");
      FS.mkdir("/img/Etc");
    FS.mkdir("/img/Npc");
    FS.mkdir("/img/Morph");
    FS.mkdir("/img/Quest");
    FS.mkdir("/img/Reactor");
    FS.mkdir("/img/Skill");
    FS.mkdir("/img/Sound");
    FS.mkdir("/img/TamingMob");
    FS.mkdir("/img/UI");
    
      FS.mount(IDBFS, {}, '/sav');
      FS.syncfs(true, function(err){console.log("IndexDB synced", err)});
  , 0);
  // clang-format on
#endif
    
    // Initial simulation configuration
    rps::RockPaperScissorsConfig config {
        .screen_width = 1200,
        .screen_height = 800,
        .simulation_rate = 45,
        .piece_size = 28,
        .piece_count = 125,
        .volume = 0.5f,
        .piece_samples = 10,
    };

    // Run game
    try {
        rps::run(config);
    }
    catch (std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
