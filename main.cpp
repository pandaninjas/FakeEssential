#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include "discord/discord.h"
#include "utils.h"
#include "argparse.h"

volatile bool interrupted = false;

int main(int argc, char ** argv) {
    PositionalArg username_arg = get(argv, argc, "--username");
    std::string username;
    if (username_arg.result == ArgParseResult::OK) {
        username = username_arg.value;
    } else {
        std::cout << "Usage: " << split_and_get_last(argv[0], path_separator) << " --username [USERNAME] --inessential [--server [server name]] [--version [version string]]\n";
        return 1;
    }

    PositionalArg server = get(argv, argc, "--server");
    bool different_server = false;
    std::string different_server_name;
    if (server.result == ArgParseResult::OK) {
        different_server_name = server.value;
        different_server = true;
    } else if (server.result == ArgParseResult::NEEDS_ARG) {
        std::cout << "--server requires an argument\n";
        std::exit(1);
    }

    PositionalArg version = get(argv, argc, "--version");
    bool different_version = false;
    std::string different_version_name;
    if (version.result == ArgParseResult::OK) {
        different_version_name = version.value;
        different_version = true;
    } else if (server.result == ArgParseResult::NEEDS_ARG) {
        std::cout << "--version requires an argument\n";
        std::exit(1);
    }

    std::string uuid = get_uuid(username);
    std::string link = "https://crafatar.com/avatars/" + uuid;
    discord::Core* core{};
    discord::Core::Create((has(argv, argc, "--inessential") ? 1049849888600510464 : 1049538781134475324), DiscordCreateFlags_Default, &core);

    if (!core) {
        std::cout << "Failed to instantiate Discord!";
        std::exit(-1);
    }

    discord::Activity activity{};
    activity.SetDetails(("Playing " + ((different_version) ? different_version_name : "Minecraft 1.8.9")).c_str());
    activity.SetState(("Playing on " + ((different_server) ? different_server_name : "mc.hypixel.net")).c_str());
    activity.GetAssets().SetSmallImage(link.c_str());
    activity.GetAssets().SetSmallText(username.c_str());
    activity.GetAssets().SetLargeImage("essential");
    activity.SetType(discord::ActivityType::Playing);
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
                  << " updating activity!\n";
    });


    std::signal(SIGINT, [](int) {
        interrupted = true;
    });
    do {
        core->RunCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } while (!interrupted);
}