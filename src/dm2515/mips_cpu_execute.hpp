//
// Created by Sanjeev Manocha on 19/10/2016.
//

#ifndef COMPARCHMIPS_MIPS_CPU_EXECUTE_HPP
#define COMPARCHMIPS_MIPS_CPU_EXECUTE_HPP

#include "mips.h"

mips_error main_execute(mips_cpu_h state, const uint32_t all_information[7], mips_mem_h mem, uint32_t& LO_register, uint32_t& HI_register, uint32_t& pcN, int& delay_slot);
mips_error name_code_finder (uint32_t& name_code, const uint32_t all_information[7]);
mips_error basic_arithmetic_alu(mips_cpu_h state, const uint32_t *all_information, int operation_type);
mips_error bitwise_operations_alu(mips_cpu_h state, const uint32_t all_information[7], int operation_type);
mips_error shift_operations_alu(mips_cpu_h state, const uint32_t all_information[7], int operation_type);
mips_error set_operations(mips_cpu_h state, const uint32_t all_information[7], int operation_type);
mips_error store_load_operations(mips_cpu_h state, const uint32_t all_information[7], int operation_type);
mips_error load_operations(mips_cpu_h state, const uint32_t all_information[7], int operation_type, mips_mem_h mem);
mips_error store_operations(mips_cpu_h state, const uint32_t all_information[7], int operation_type, mips_mem_h mem);
mips_error read_hi_lo(mips_cpu_h state, const uint32_t all_information[7], const uint32_t& register_HI, const uint32_t& register_LO);
mips_error write_hi_lo(mips_cpu_h state, const uint32_t all_information[7], uint32_t& register_HI, uint32_t& register_LO);
mips_error mul_and_div(mips_cpu_h state, const uint32_t all_information[7], uint32_t& register_HI, uint32_t& register_LO);
mips_error branch_comparisons_delay_slot(mips_cpu_h state, const uint32_t all_information[7], uint32_t& pcN, unsigned branch_type, int& delay_slot);
mips_error execute_jump_and_links(mips_cpu_h state, const uint32_t all_information[7], uint32_t& pcN, unsigned type, int& delay_slot);
mips_error loadword_l_r(mips_cpu_h state, const uint32_t all_information[7], mips_mem_h mem);

#endif //COMPARCHMIPS_MIPS_CPU_EXECUTE_HPP