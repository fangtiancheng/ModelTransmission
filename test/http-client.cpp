#include <httplib/httplib.h>
#include <iostream>
#include <tuple>
#include <chrono>
#include <filesystem>

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

std::tuple<bool, std::chrono::duration<double>> sendFile(httplib::Client& client,
                                                         const std::string& content,
                                                         const std::string& fileName){
    httplib::Headers headers{{"File-Name", fileName}};
    auto tBegin = std::chrono::steady_clock::now();
    auto result = client.Post(
        "/multipart",
        headers,
        content.size(),
        [&](size_t offset, size_t length, httplib::DataSink &sink) {
            sink.write(content.data() + offset, length);
            return true;
        },
        "text/plain");
    if(result->status != 200){
        return {false, {}};
    } else {
        auto tEnd = std::chrono::steady_clock::now();
        return {true, tEnd - tBegin};
    }
}


int main(){
    using namespace httplib;

    Client clt("127.0.0.1", 12345);

    auto filePath = std::filesystem::path(TEST_FILE_LOC);
    auto fileData = readFile(filePath);
    auto [succ, duration] = sendFile(clt, fileData, filePath.filename());
    if(succ){
        std::cout << "Successfully transmitted, cost " << duration.count() << " seconds\n";
    } else{
        std::cout << "Transmission failed!\n";
    }
    return 0;
}