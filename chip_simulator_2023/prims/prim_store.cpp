#include "prim_store.h"

prims::PrimStore::PrimStore(uint64_t sram_addr, uint64_t dram_addr, uint64_t data_length)
	:sram_addr_(sram_addr), dram_addr_(dram_addr), data_length_(data_length)
{
	assert(sram_addr < (1 << 16));
	assert(dram_addr < (static_cast<uint64_t>(1) << 32));
	assert(data_length < (1 << 16));

	assert(sram_addr + data_length < CORE_MEM_DEPTH);
	assert(uint64_t(dram_addr) < uint64_t(0xFFFFFFFF));
	assert(dram_addr + data_length < DRAM_DEPTH_PER_CORE);

	this->prim_name_ = "Store";

	this->convertParams2PrimIDCode();
}

prims::PrimStore::PrimStore(sc_bv<MEM_PORT_WIDTH> prim_id_code)
	:Primitive(prim_id_code)
{
	assert(prim_id_code.range(3, 0) == sc_bv<4>(2));

	this->sram_addr_ = prim_id_code.range(19, 4).to_uint64();
	this->dram_addr_ = prim_id_code.range(115, 84).to_uint64();
	this->data_length_ = prim_id_code.range(35, 20).to_uint64();

	this->prim_name_ = "Load";
}

void prims::PrimStore::convertParams2PrimIDCode()
{
	this->prim_id_code_.range(3, 0) = sc_bv<4>(2);

	this->prim_id_code_.range(19, 4) = sc_bv<16>(this->sram_addr_);
	this->prim_id_code_.range(115, 84) = sc_bv<32>(this->dram_addr_);
	this->prim_id_code_.range(35, 20) = sc_bv<16>(this->data_length_);
}
