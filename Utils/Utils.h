#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <chrono>
#include <cstdint>

#ifdef LINUX_OS

#include <sys/time.h>

struct ntp_ts_t
{
    std::uint32_t seconds;
    std::uint32_t fraction;
};

static void timeval_to_ntp(struct timeval* tv, struct ntp_ts_t* ntp)
{
    ntp->seconds = tv->tv_sec + 2208988800;
    ntp->fraction =
        (std::uint32_t)((double)(tv->tv_usec + 1) * (double)(1LL << 32) * 1.0e-6);
}

static std::uint64_t GetNowAsNTPLinux()
{
	ntp_ts_t ntp;
	timeval currentTime;

	if (gettimeofday(&currentTime, nullptr) != 0)
	{
		std::cerr << "Error getting the currrent time" << std::endl;
		return 0;
	}

	timeval_to_ntp(&currentTime, &ntp);
	const std::uint64_t ntpTimestamp = (static_cast<std::uint64_t>(ntp.seconds) << 32) | ntp.fraction;
	
	// std::cout << "Seconds: " << currentTime.tv_sec << std::endl;
	// std::cout << "Microseconds: " << currentTime.tv_usec << std::endl;
	// std::cout << "NtpSeconds: " << ntp.seconds << std::endl;
	// std::cout << "NtpFraction: " << ntp.fraction << std::endl;
	// std::cout << "NtpTimestamp: " << ntpTimestamp << std::endl;

	return ntpTimestamp;
}

#endif // LINUX_OS

// 25567 days have passed between 1 Jan 1900 and 1 Jan 1970
static constexpr const auto epochOffsetInSeconds = 25567u * 24 * 3600;
static constexpr const auto convFactor = (1ull << 32) * 1.0e-6;

static std::uint64_t GetNowAsNTP()
{
	const auto now = std::chrono::system_clock::now().time_since_epoch();
	const auto sec = std::chrono::duration_cast<std::chrono::seconds>(now).count();
	const auto msc = std::chrono::duration_cast<std::chrono::microseconds>(now).count();
	const auto frac = msc - (sec * 1000000);

	const std::uint64_t ntpSeconds = sec + epochOffsetInSeconds;
	const std::uint32_t ntpFraction = frac * convFactor;
	const std::uint64_t ntpTimestamp = (ntpSeconds << 32) | ntpFraction;

	// std::cout << "Duration: " << now.count() << std::endl;
	// std::cout << "Seconds: " << sec << std::endl;
	// std::cout << "Microseconds: " << msc << " - " << frac << std::endl;
	// std::cout << "NtpSeconds: " << ntpSeconds << std::endl;
	// std::cout << "NtpFraction: " << ntpFraction << std::endl;
	// std::cout << "NtpTimestamp: " << ntpTimestamp << std::endl;

	return ntpTimestamp;
}

#endif // !UTILS_H
