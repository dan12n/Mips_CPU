#ifndef COMPARCHMIPS_MIPS_CPU_DECODER_H
#define COMPARCHMIPS_MIPS_CPU_DECODER_H

#include "mips.h"

mips_error instruction_type(const uint32_t& fullword, uint32_t all_information[7]);
void r_extractall(const uint32_t& fullword, uint32_t all_information[7]);
void i_extractall(const uint32_t& fullword, uint32_t all_information[7]);
void j_extractall(const uint32_t& fullword, uint32_t all_information[7]);
void print_all_information(const uint32_t all_information[7]);

#endif //COMPARCHMIPS_MIPS_CPU_DECODER_H
