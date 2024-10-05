#include <httplib/httplib.h>
#include <iostream>

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);

    if(!file) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    // Seek to the end to get the file size
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file content into a vector
    std::string fileData; fileData.resize(fileSize);
    file.read(fileData.data(), fileSize);
    return fileData;
}

int main(){
    using namespace httplib;

    Client clt("127.0.0.1", 12345);

    const std::string filePath = TEST_FILE_LOC;
    auto fileData = readFile(filePath);
//    httplib::MultipartFormDataItems items = {
//            { "text1", "text default", "", "" },
//            { "text2", "aωb", "", "" },
//            { "file1", "h\ne\n\nl\nl\no\n", fileData, "image/png" },
//            { "file2", "{\n  \"world\", true\n}\n", "world.json", "application/json" },
//            { "file3", "", "", "application/octet-stream" },
//    };
//    auto res1 = clt.Post("/multipart");
    auto res2 = clt.Post(
            "/multipart", fileData.size(),
            [&](size_t offset, size_t length, DataSink &sink) {
                sink.write(fileData.data() + offset, length);
                return true; // return 'false' if you want to cancel the request.
            },
            "text/plain");
    return 0;
}