#include "crow.h"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

std::string readFile(const fs::path& path) {
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open()) return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    file.close();
    return ss.str();
}

std::string getExecDir(std::string argv0) {
    fs::path execPath = argv0;
    fs::path execDir = execPath.parent_path();
    if(execDir.empty()) {
        execDir = fs::current_path();
    }
    return execDir;
}

int main(int argc, char *argv[]) {
    fs::path execDir = getExecDir(argv[0]);

    // catch the front paths
    fs::path pathIndex = fs::canonical(execDir / "../front/index.html");

    // copy the content of front files
    std::string contentIndex = readFile(pathIndex);

    // start crow app
    crow::SimpleApp app;

    // routes

    // static route
    std::string staticRelativePath = "../front/assets/";
    fs::path staticPath = fs::canonical(execDir / staticRelativePath);
    CROW_ROUTE(app, "/assets/<path>")([execDir, staticPath](const std::string& path) {
        // try to parse the user path
        fs::path requestedPath;
        try {
            requestedPath = fs::canonical(staticPath / path);
        } catch(...) {
            return crow::response(400, "Invalid path");
        }

        // verify if requested path is inside the static directory
        if(requestedPath.string().find(staticPath.string()) != 0) {
            return crow::response(403, "Forbidden");
        }
        
        if(!fs::exists(requestedPath)) {
            return crow::response(404, "File not found");
        }

        std::string content = readFile(requestedPath);
        return crow::response{content};
    });

    // get routes
    CROW_ROUTE(app, "/")([contentIndex](){
        return crow::response(contentIndex);
    });

    // define loglevel and run
    app.loglevel(crow::LogLevel::Warning);
    app.port(8080).multithreaded().run();
}