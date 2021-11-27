#include "web-server-commander.hpp"


void WebServerCommander::run(std::string_view cmdline) {
    if(cmdline == "help") {
        help();
        return;
    }
    if(auto iter=mCommandMap.find(cmdline.data()); iter != std::end(mCommandMap)) {
        auto&[func, _] = iter->second;
        func();
    } else {
        std::cerr << "Invalid Command => \'" << cmdline << '\'' << std::endl;
    }
}

void WebServerCommander::help() {
    for(auto& iter : mCommandMap) {
        auto& cmd = iter.first;
        auto&[_, desc] = iter.second;
        std::cout << "- " << cmd << " => " << desc << std::endl;
    }
}

void WebServerCommander::init() {
    if(was == nullptr) {
        std::runtime_error("Was is nullptr");
    }
    
    addCommand("run",       [&](){was->run();}, "Start HTTP Server");
    addCommand("stop",      [&](){was->stop();},"Stop HTTP Server");
    addCommand("state",     [&](){std::cout << was->getState() << std::endl;},"Show Service Info");
    addCommand("restart",   [&](){was->restart();}, "Restart HTTP Server");
    addCommand("reload",    [&](){was->reload();}, "Auto reload html pages");
    addCommand("quit",      [&](){exit(0);}, "Quit Application");
}

void WebServerCommander::addCommand(std::string_view cmd, std::function<void()> func, std::string desc) {
    if(auto iter=mCommandMap.find(cmd.data()); iter == std::end(mCommandMap)) {
        mCommandMap.insert({cmd.data(), pack(func, desc)});
    } else {
        std::stringstream ss;
        ss << "Add Command Failed: " << cmd << " is already exists" << std::endl;
        throw std::runtime_error(ss.str());
    }
}
