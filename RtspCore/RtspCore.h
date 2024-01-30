#ifndef RTSP_CORE_H
#define RTSP_CORE_H

#include <vector>
#include <memory>
#include <string>
#include <cstdint>

namespace rtsp
{
    namespace core
    {
        enum MethodType : unsigned
        {
            DESCRIBE,
            GET_PARAMETER,
            OPTIONS,
            PAUSE,
            PLAY,
            PLAY_NOTIFY,
            REDIRECT,
            SETUP,
            SET_PARAMETER,
            TEARDOWN,
            UNKNOWN
        };

        std::string methodTypeToStr(MethodType type);
        MethodType strToMethodType(const std::string& typeStr);

        class IHeader
        {
        public:
            virtual ~IHeader() = default;

            virtual std::string setup() const = 0;
        };

        using IHeaderPtr = std::unique_ptr<IHeader>;
        using Headers = std::vector<IHeaderPtr>;

        class CSeqHeader : public IHeader
        {
        public:
            virtual ~CSeqHeader() = default;

            virtual std::string setup() const override;

            void setCSeq(const unsigned& val) { cseq = val; }

        private:
            unsigned cseq{}; // is unsigned int enough ?
        };

        class PublicHeader : public IHeader
        {
        public:
            virtual ~PublicHeader() = default;

            virtual std::string setup() const override;

            void addSupportedMethod(MethodType method);

        private:
            std::vector<MethodType> supportedMethods;
        };

        class ContentHeader : public IHeader
        {
        public:
            virtual ~ContentHeader() = default;

            explicit ContentHeader(const std::string& type, const int length);

            virtual std::string setup() const override;

        private:
            std::string ctype;
            std::string clength;
        };

        class OptionMethod
        {
        public:
            std::string getResponse() const;
            void addHeader(IHeaderPtr headerPtr);

        private:
            MethodType mtype;
            Headers headers;
        };

        class Sdp
        {
        public:
            enum MediaType : unsigned
            {
                audio,
                video,
                application,
                data,
                control,
                unknown
            };

            // void addO()

            void addS(const std::string& sessionName);

            void addM(MediaType media, const unsigned port, const unsigned char payloadType);

        private:
            std::string body;
        };
    }
}

#endif // !RTSP_CORE_H
