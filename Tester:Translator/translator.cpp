#include "translator.hpp"

const vector<IS_Vector>	R_Type = {	IS_Vector(100000, "add"), 
								IS_Vector(100001, "addu"), 
								IS_Vector(100010, "sub"), 
								IS_Vector(100011, "subu"), 
							
								IS_Vector(100100, "and"), 
								IS_Vector(100101, "or"), 
								IS_Vector(100110, "xor"), 
							
								IS_Vector(011010, "div"), 
								IS_Vector(011011, "divu"), 
								IS_Vector(011000, "mult"), 
								IS_Vector(011001, "multu"), 
								IS_Vector(010000, "mfhi",), 
								IS_Vector(010010, "mflo"), 
								IS_Vector(010001, "mthi"), 
								IS_Vector(01001, "mtlo"), 
							
								IS_Vector(000000, "sll"), 
								IS_Vector(000010, "srl"), 
								IS_Vector(000100, "sllv"), 
								IS_Vector(000110, "srlv"), 
								IS_Vector(000011, "sra"), 
								IS_Vector(000111, "srav"), 
								IS_Vector(001001, "jalr"), 
								IS_Vector(001000, "jr"), 
								IS_Vector(101010, "slt"), 
								IS_Vector(101011, "sltu") 
						 	 }
// I-type instruction
const vector<IS_Vector> I_Type = { 	IS_Vector(001000, "addi"), 
								IS_Vector(001001, "addiu"),
								IS_Vector(001100, "andi"), 
								IS_Vector(001101, "ori"), 
								IS_Vector(001110, "xori"), 
								// NOTE:ED TO OCESS BLT,BLTZAL, BGEZ, BGEZAL INSIDE THIS ONE AS WELL
								IS_Vector(000001, "b"),
								IS_Vector(000100, "beq"), 
								IS_Vector(000101, "bne"), 								 		
								IS_Vector(000110, "blez"), 
								IS_Vector(000111, "bgtz"), 
								IS_Vector(100011, "lw"), 
								IS_Vector(100001, "lh"), 
								IS_Vector(100000, "lb"), 
								IS_Vector(100010, "lwl"), 
								IS_Vector(100110, "lwr"), 
								IS_Vector(100101, "lhu"), 
								IS_Vector(100100, "lbu"), 
							
								IS_Vector(001111, "lui"), 
							
								IS_Vector(101011, "sw"), 
								IS_Vector(101001, "sh"), 
								IS_Vector(101000, "sb"), 
							
								IS_Vector(001010, "slti"), 
								IS_Vector(001011, "sltiu") 
						 	 }
// J-type instruction
const vector<IS_Vector> J_Type = { 	IS_Vector(000001, "j"), 
								IS_Vector(000011, "jal") 
						 	 };