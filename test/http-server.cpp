#include <httplib/httplib.h>
#include <iostream>
#include <fstream>

int main(){
    using namespace httplib;

    Server svr;

    svr.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    // Match the request path against a regular expression
    // and extract its captures
    svr.Post("/multipart", [&](const auto& req, auto& res) {
//        auto size = req.files.size();
//        auto ret = req.has_file("file1");
//        const auto& file = req.get_file_value("file");
         std::cout << "---------------\n" << req.body << std::endl;
         std::ofstream f("out.png", std::ios::out|std::ios::binary);
         if(f.good()){
             f.write(req.body.data(), req.body.size());
         } else{
             std::cerr << "file not good\n";
         }
    });

    svr.listen("127.0.0.1", 12345);
}