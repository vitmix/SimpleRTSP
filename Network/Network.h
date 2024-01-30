#ifndef NETWORK_H
#define NETWORK_H

//#include "RtspCore.h"
#include <string>

class Socket
{
public:
    virtual ~Socket() = default;

    virtual bool init() = 0;
    virtual bool listenForConnections() = 0;
    virtual void close() = 0;

private:

};

//template <typename Type>
//class Response
//{
//public:
//    virtual ~Response() = default;
//
//    void make(Type data) const
//    {
//        makeImpl(data);
//    }
//
//protected:
//    virtual void makeImpl(Type data) = 0;
//};
//
//class RtspOptionResponse : public Response<rtsp::core::OptionMethod>
//{
//public:
//    virtual ~RtspOptionResponse() = default;
//
//protected:
//    virtual void makeImpl(rtsp::core::OptionMethod method) = 0;
//};

#endif // !NETWORK_H
