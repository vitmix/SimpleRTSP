#include "SimpleRtspServer.h"
#include "RtspCore.h"
#include "Utils.h"

#ifdef WINDOWS_OS
#include "WinNetwork.h"
#else
#endif

#include <memory>

int main()
{
	//{
	//	auto server = std::make_unique<WinSocket>("127.0.0.1", 27015);
	//	server->init();
	//	server->listenForConnections();
	//	std::cout << "Number of clients connected is:\n" << server->getNumOfConnection() << std::endl;
	//	std::string message;
	//	message = server->read();
	//	if (!message.empty())
	//	{
	//		std::cout << "C->S:\n" << message << std::endl;
	//	}
	//	
	//	rtsp::core::OptionMethod response;
	//	
	//	auto cseqHeader = std::make_unique<rtsp::core::CSeqHeader>();
	//	cseqHeader->setCSeq(2); // need to parse from client's message
	//	response.addHeader(std::move(cseqHeader));

	//	auto publicHeader = std::make_unique<rtsp::core::PublicHeader>();
	//	publicHeader->addSupportedMethod(rtsp::core::DESCRIBE);
	//	publicHeader->addSupportedMethod(rtsp::core::SETUP);
	//	publicHeader->addSupportedMethod(rtsp::core::TEARDOWN);
	//	publicHeader->addSupportedMethod(rtsp::core::PLAY);
	//	publicHeader->addSupportedMethod(rtsp::core::PAUSE);
	//	response.addHeader(std::move(publicHeader));

	//	//const auto& respToWrite = response.getResponse();
	//	//std::cout << "S->C:\n" << respToWrite << std::endl;
	//	//server->write(respToWrite);
	//	
	//	//server->write("RTSP/2.0 505 RTSP Version Not Supported\r\n\r\n");

	//	message.clear();
	//	message = server->read();
	//	if (!message.empty())
	//	{
	//		std::cout << "C->S:\n" << message << std::endl;
	//	}

	//	//server->write("RTSP/2.0 505 RTSP Version Not Supported\r\n\r\n");

	//	message.clear();
	//	message = server->read();
	//	if (!message.empty())
	//	{
	//		std::cout << "C->S:\n" << message << std::endl;
	//	}
	//}

	std::cerr << "Common:" << std::endl;
	GetNowAsNTP();
	std::cerr << "Linux specific:" << std::endl;
	GetNowAsNTPLinux();
	
	return 0;
}
