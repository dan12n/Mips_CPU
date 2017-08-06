#ifndef COMPARCHMIPS_MIPS_CPU_BASIC_DEFINITIONS_HPP
#define COMPARCHMIPS_MIPS_CPU_BASIC_DEFINITIONS_HPP

#define TYPE 0
//Different types
#define R_TYPE 0
#define I_TYPE 1
#define J_TYPE 2
#define OPCODE 1
//R_INFORMATION DEFINTIONS
#define RS 2
#define RD 3
#define RT 4
#define SHIFT 5
#define FUNCTION 6
//I_INFORMATION DEFINTIION
#define IMM_CONSTANT 4
//J_INFORMATION DEFINTIION
#define MEMORY 2

struct mips_inst_def {
    uint8_t name_code;
    uint8_t opcode;

};

mips_error mips_cpu_get_debug_level(mips_cpu_h state, unsigned *level);
mips_error mips_cpu_get_branch(mips_cpu_h state, bool *branch, bool *delay_slot);
mips_error mips_cpu_set_branch(mips_cpu_h state, bool branch, bool delay_slot);
//The definitions are associated with integers to allow easier grouping in execution
//All definitions should not be changed by any means in the program
const mips_inst_def i_type_defs[23] = {
        //name_code, hex_value
        {6, 0x0a},      //slti
        {7, 0x0b},      //sltiu
        {8, 0x0c},      //andi
        {9, 0x0d},      //ori
        {10, 0x0e},      //xori
        {16, 0x08},      //addi
        {17, 0x09},      //addiu
        {26, 0x24},      //lbu
        {27, 0x20},      //lb
        {28, 0x25},      //lhu
        {29, 0x21},      //lh
        {30, 0x0f},      //lui
        {31, 0x23},      //lw
        {32, 0x28},      //sb
        {33, 0x29},      //sh
        {34, 0x2b},      //sw
        {35, 0x04},      //beq
        {36, 0x05},      //bne
        {37, 0x06},      //blez
        {38, 0x07},      //bgtz
        {39, 0x01},      //BLTZ, BGEZ, BLTZAL, BGEZAL
        {42, 0x26},      //lwr
        {43, 0x22}      //lwl
};

const mips_inst_def r_type_defs[26] = {
        //name_code corresponds to funct(5:0) instead of op(31:26) here
        {0, 0x00},        //sll
        {1, 0x04},        //sllv
        {2, 0x02},        //srl
        {3, 0x06},        //srlv
        {4, 0x03},        //sra
        {5, 0x07},        //srav
        {6, 0x2a},          //slt
        {7, 0x2b},          //sltu
        {8, 0x24},          //and
        {9, 0x25},          //or
        {10, 0x26},          //xor
        {11, 0x10},         //mtlo
        {12, 0x18},         //mult
        {13, 0x19},         //multu
        {14, 0x1a},          //div
        {15, 0x1b},         //divu
        {16, 0x20},         //add
        {17, 0x21},         //addu
        {18, 0x22},          //sub
        {19, 0x23},         //subu
        {20, 0x10},        //mfhi
        {21, 0x12},         //mflo
        {22, 0x11},        //mthi
        {23, 0x13},         //mtlo
        {24, 0x08},         //jr
        {25, 0x09}        //jalr
};

const mips_inst_def j_type_defs[2] = {
        //name_code corresponds to opcode
        {40, 0x02},           //j
        {41, 0x03}            //jal
};

#endif //COMPARCHMIPS_MIPS_CPU_BASIC_DEFINITIONS_HPP
