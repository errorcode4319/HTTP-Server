#include "web-server-commander.hpp"


void WebServerCommander::run(std::string_view cmdline) {
    if(auto iter=mCommandMap.find(cmdline.data()); iter != std::end(mCommandMap)) {
        auto func = iter->second;
        func();
    } else {
        std::cerr << "Invalid Command => \'" << cmdline << '\'' << std::endl;
    }
}

void WebServerCommander::init() {
    if(was == nullptr) {
        std::runtime_error("Was is nullptr");
    }
    
    addCommand("run",       [&](){was->run();});
    addCommand("stop",      [&](){was->stop();});
    addCommand("state",     [&](){std::cout << was->getState() << std::endl;});
    addCommand("restart",   [&](){was->restart();});
    addCommand("reload",    [&](){was->reload();});
    addCommand("quit",      [&](){exit(0);});
}

void WebServerCommander::addCommand(std::string_view cmd, std::function<void()> func) {
    if(auto iter=mCommandMap.find(cmd.data()); iter == std::end(mCommandMap)) {
        mCommandMap.insert({cmd.data(), func});
    } else {
        std::stringstream ss;
        ss << "Add Command Failed: " << cmd << " is already exists" << std::endl;
        throw std::runtime_error(ss.str());
    }
}
