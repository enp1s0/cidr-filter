#ifndef __CIDER_FILTER__
#define __CIDER_FILTER__
#include <string>
#include <stdexcept>

namespace mtk{
namespace network{
inline unsigned int get_ipv4_address_bits(const std::string ipv4_address){
	unsigned int ipv4_address_bits = 0;
	std::size_t block_start = 0;
	for(int i = 0; i < 3; i++){
		const std::size_t block_end = ipv4_address.find_first_of(".", block_start);
		const auto block = std::stoi( ipv4_address.substr(block_start, (block_end - block_start) ));
		if(block > 255) throw(std::runtime_error("invalid ip address : " + ipv4_address));
		ipv4_address_bits = (ipv4_address_bits << 8) | block;
		block_start = block_end + 1;
	}
	const auto block = std::stoi( ipv4_address.substr(block_start, (ipv4_address.length() - block_start) ));
	if(block > 255) throw(std::runtime_error("invalid ip address : " + ipv4_address));
	ipv4_address_bits = (ipv4_address_bits << 8) | block;
	return ipv4_address_bits;
}
inline bool is_included(const std::string base_cidr, const std::string ipv4_address){
	// base_cidr : xxx.yyy.zzz.www/cc
	// slash_pos : position of slash
	const auto slash_pos = base_cidr.find_first_of("/");
	// cidr : cc
	const auto cidr = std::stoi(base_cidr.substr(slash_pos+1, base_cidr.length() - slash_pos));
	// cidr_bits : bits(xxx.yyy.zzz.www)
	auto cidr_bits = get_ipv4_address_bits(base_cidr.substr(0, slash_pos));
	// cidr_bits : bits(ipv4_address)
	auto ipv4_address_bits = get_ipv4_address_bits(ipv4_address);
	// subnetmask
	const unsigned int subnet_mask = 0xffffffff << (32 - cidr);

	return (cidr_bits & subnet_mask) == (ipv4_address_bits & subnet_mask);
}
}
}

#endif // __CIDER_FILTER__
