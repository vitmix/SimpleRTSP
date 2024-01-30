#include "RtspCore.h"
#include <unordered_map>
#include <array>

namespace
{
    static constexpr const auto VERSION_STR = "RTSP/1.0";

    static constexpr const auto METHOD_NUM = static_cast<unsigned>(rtsp::core::UNKNOWN);
    static constexpr const auto MEDIA_TYPE_NUM = static_cast<unsigned>(rtsp::core::Sdp::MediaType::unknown);

    static const std::unordered_map<std::string, rtsp::core::MethodType> strToMethodMap
    {
        { "DESCRIBE", rtsp::core::MethodType::DESCRIBE },
        { "GET_PARAMETER", rtsp::core::MethodType::GET_PARAMETER },
        { "OPTIONS", rtsp::core::MethodType::OPTIONS },
        { "PAUSE", rtsp::core::MethodType::PAUSE },
        { "PLAY", rtsp::core::MethodType::PLAY },
        { "PLAY_NOTIFY", rtsp::core::MethodType::PLAY_NOTIFY },
        { "REDIRECT", rtsp::core::MethodType::REDIRECT },
        { "SETUP", rtsp::core::MethodType::SETUP },
        { "SET_PARAMETER", rtsp::core::MethodType::SET_PARAMETER },
        { "TEARDOWN", rtsp::core::MethodType::TEARDOWN }
    };

    static constexpr const std::array<const char*, METHOD_NUM> methodTypeStrArr
    {
        "DESCRIBE",
        "GET_PARAMETER",
        "OPTIONS",
        "PAUSE",
        "PLAY",
        "PLAY_NOTIFY",
        "REDIRECT",
        "SETUP", 
        "SET_PARAMETER",
        "TEARDOWN"
    };

    static const std::unordered_map<std::string, rtsp::core::Sdp::MediaType> strToMediaTypeMap
    {
        { "audio", rtsp::core::Sdp::MediaType::audio },
        { "video", rtsp::core::Sdp::MediaType::video },
        { "application", rtsp::core::Sdp::MediaType::application },
        { "data", rtsp::core::Sdp::MediaType::data },
        { "control", rtsp::core::Sdp::MediaType::control }
    };

    static constexpr const std::array<const char*, MEDIA_TYPE_NUM> mediaTypeStrArr
    {
        "audio",
        "video",
        "application",
        "data",
        "control"
    };
}

namespace rtsp
{
    namespace core
    {
        std::string methodTypeToStr(MethodType type)
        {
            return methodTypeStrArr.at(static_cast<size_t>(type));
        }

        MethodType strToMethodType(const std::string& typeStr)
        {
            return strToMethodMap.at(typeStr);
        }
        
        std::string CSeqHeader::setup() const
        {
            return "CSeq: " + std::to_string(cseq) + "\r\n";
        }

        void PublicHeader::addSupportedMethod(MethodType method)
        {
            supportedMethods.push_back(method);
        }

        std::string PublicHeader::setup() const
        {
            std::string headerStr("PUBLIC:");
            
            for (const auto& mtype : supportedMethods)
            {
                headerStr.append(" ");
                headerStr.append(methodTypeToStr(mtype));
                headerStr.append(",");
            }
            headerStr.pop_back();
            headerStr.append("\r\n");
            return headerStr;
        }

        ContentHeader::ContentHeader(const std::string& type, const int length)
            : ctype(type), clength(std::to_string(length))
        {}

        std::string ContentHeader::setup() const
        {
            std::string headerStr("Content-Type: ");
            headerStr.append(ctype);
            headerStr.append("\r\n");
            headerStr.append("Content-Length: ");
            headerStr.append(clength);
            headerStr.append("\r\n");
            return headerStr;
        }

        std::string OptionMethod::getResponse() const
        {
            std::string response(VERSION_STR);
            response.append(" 200 OK\r\n");
            
            for (const auto& header : headers)
                response.append(header->setup());

            response.append("\r\n");
            return response;
        }

        void OptionMethod::addHeader(IHeaderPtr headerPtr)
        {
            headers.push_back(std::move(headerPtr));
        }

        void Sdp::addS(const std::string& sessionName)
        {
            body.append("i=" + sessionName + "\r\n");
        }

        void Sdp::addM(MediaType media, const unsigned port, const unsigned char payloadType)
        {
            const auto& mediaStr = mediaTypeStrArr.at(static_cast<size_t>(media));
            body.append("m=");
            body.append(mediaStr);
            body.append(" " + std::to_string(port));
            body.append(" RTP/AVP");
            body.append(" " + std::to_string(payloadType));
            body.append("\r\n");
        }
    }
}
