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
inline bool is_subnet(const std::string base_ipv4_address, const unsigned int cidr, const std::string ipv4_address){
	const auto base_bits = get_ipv4_address_bits(base_ipv4_address);
	const auto target_bits = get_ipv4_address_bits(ipv4_address);
	const unsigned int subnet_mask = (~0) << (32 - cidr);

	return (base_bits & subnet_mask) == (target_bits & subnet_mask);
}
inline bool is_subnet(const std::string base_cidr, const std::string ipv4_address){
	// base_cidr : xxx.yyy.zzz.www/cc
	// slash_pos : position of slash
	const auto slash_pos = base_cidr.find_first_of("/");
	// cidr : cc
	const auto cidr = std::stoi(base_cidr.substr(slash_pos+1, base_cidr.length() - slash_pos));

	return is_subnet(base_cidr.substr(0, slash_pos), cidr, ipv4_address);
}
}
}

#endif // __CIDER_FILTER__
