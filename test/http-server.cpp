#include <httplib/httplib.h>
#include <iostream>
#include <fstream>

int main(){
    using namespace httplib;

    Server svr;

    svr.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
    });
    svr.Post("/multipart", [&](const auto& req, auto& res) {
        std::string fileName = "unknown.file";
        auto fileNamePtr = req.headers.find("File-Name");
        if(fileNamePtr == req.headers.cend()){
            std::cerr << "unknown file name";
        } else {
            fileName = fileNamePtr->second;
        }
        std::ofstream f(fileName, std::ios::out|std::ios::binary);
        if(f.good()){
            f.write(req.body.data(), req.body.size());
            std::cout << "receive " << fileName << '\n';

        } else{
            std::cerr << "file not good\n";
        }
    });

    svr.listen("127.0.0.1", 12345);
}