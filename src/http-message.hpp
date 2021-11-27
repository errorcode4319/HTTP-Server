#ifndef _HTTP_MESSAGE_HPP
#define _HTTP_MESSAGE_HPP

#include <iostream>
#include <string>
#include <optional>
#include <functional>
#include <unordered_map>
#include <sstream>
namespace http {


    class HttpMessage {
    public:
        HttpMessage() = default;
        virtual ~HttpMessage() = default;

        std::string to_serialized();

    public:
        void setStartLine(std::string_view startLine) {
            mStartLine = startLine;
        }
        std::string_view getStartLine() const {
            return mStartLine;
        }
        std::optional<std::string_view> getHeader(std::string_view key) const {
            if(auto iter = mHeader.find(key.data()); iter != std::end(mHeader))
                return iter->second;
            return std::nullopt;
        }
        void setHeader(std::string_view key, std::string_view prop) {
            mHeader[key.data()] = prop.data();
        }
        std::string_view getBody() const {
            return mBody;
        }
        void setBody(std::string_view body) {
            mBody = body;
        }

    protected:
        std::string mStartLine = "";
        std::unordered_map<std::string, std::string> mHeader;
        std::string mBody = "";
    };

    class HttpRequest : public HttpMessage {
    public:
        HttpRequest(std::string_view rawMsg) {
            parse(rawMsg);
        }

        std::string_view getURI() const {return mTargetURI;}
        std::string_view getMethod() const {return mMethod;}
    private:
        void parse(std::string_view rawMsg);    

        std::string mTargetURI;
        std::string mMethod;
    };

    class HttpResponse : public HttpMessage {
    public:
        HttpResponse() = default;
        HttpResponse(std::string_view startLine) {
            setStartLine(startLine);
        }
    };

    using REQ = HttpRequest;
    using RES = HttpResponse;

}

#endif 