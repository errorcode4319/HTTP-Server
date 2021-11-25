#include "http-message.hpp"

namespace http {
    
    constexpr std::string_view HEADER_DELIMITER = "\r\n";
    constexpr std::string_view BODY_DELIMITER = "\r\n\r\n";

    std::string HttpMessage::to_serialized() {
        std::stringstream ss;
        for(auto& prop : mHeader) {
            ss << prop.first << ": " << prop.second << HEADER_DELIMITER;
        }
        ss << HEADER_DELIMITER << mBody;
        return ss.str();
    }

    void HttpRequest::parse(std::string_view rawMsg) {
        
        auto startLine = rawMsg.substr(0, rawMsg.find(HEADER_DELIMITER));
        mTargetURI = [&]() {
            std::string ret(startLine.data() + startLine.find(' ') + 1);
            ret = ret.substr(0, ret.find(' '));
            return std::string(ret);
        }();

        mMethod = startLine.substr(0, startLine.find(' '));

        auto rawHeader = rawMsg.substr(
            rawMsg.find(HEADER_DELIMITER) + HEADER_DELIMITER.size(),
            rawMsg.find(BODY_DELIMITER) - rawMsg.find(HEADER_DELIMITER)
        );

        while (rawHeader.find(HEADER_DELIMITER) != std::string::npos) {
            auto header_deli_iter = rawHeader.find(HEADER_DELIMITER);
            auto key_value_deli_iter = rawHeader.find(':');

            auto header = rawHeader.substr(0, header_deli_iter);
            auto key = header.substr(0, key_value_deli_iter);
            auto value = header.substr(key_value_deli_iter + 1, header_deli_iter);
            
            int i=0;
            for(int len=value.size() && value[i] == ' ';i<len;i++)
            value = value.substr(i);

            setHeader(key, value);
            rawHeader = rawHeader.substr(header_deli_iter + HEADER_DELIMITER.size());
        }
        setBody(rawMsg.substr(rawMsg.find(BODY_DELIMITER) + BODY_DELIMITER.size()));

    }

}