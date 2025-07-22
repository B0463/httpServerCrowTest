#include "crow.h"
#include <filesystem>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[]) {
    namespace fs = std::filesystem;

    // catch exec path
    fs::path execPath = argv[0];
    fs::path execDir = execPath.parent_path();
    if(execDir.empty()) {
        execDir = fs::current_path();
    }

    // catch the front paths
    fs::path pathIndex = fs::canonical(execDir / "../front/index.html");

    // copy the content of front files
    std::ifstream fileIndex(pathIndex);
    if(!fileIndex.is_open()) {
        return 1;
    }
    std::stringstream buffer;
    buffer << fileIndex.rdbuf();
    std::string contentIndex = buffer.str();
    fileIndex.close();

    // start crow app
    crow::SimpleApp app;

    // routes
    CROW_ROUTE(app, "/")([contentIndex](){
        return crow::response(contentIndex);
    });

    // define loglevel and run
    app.loglevel(crow::LogLevel::Warning);
    app.port(8080).multithreaded().run();
}