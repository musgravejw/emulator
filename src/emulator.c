#include <stdio.h>
#include <stdlib.h>

/*
* 6502 CPU emulator
* 8 bit word size (bus size)
* 1 thread
* stack - 256 bytes
* 3 general purpose registers
* 
* Designed modified from this project: https://github.com/davepoo/6502Emulator
*/

const static int MAIN_MEMORY_SIZE = 1024 * 64;

// enum for opcodes
// lda_im = 0xa9

typedef struct {
  unsigned char c : 1;  // carry
  unsigned char z : 1;  // zero
  unsigned char i : 1;  // interrupt disable
  unsigned char d : 1;  // decimal mode
  unsigned char b : 1;  // break
  unsigned char v : 1;  // overflow
  unsigned char n : 1;  // negative
} status_flags;

typedef struct {
  unsigned short pc;  // program counter
  unsigned short sp;  // stack pointer

  unsigned char a;  // accumulator
  unsigned char x;
  unsigned char y;

  status_flags *sf;
} cpu;

typedef struct {
  unsigned char ram[MAIN_MEMORY_SIZE];
} memory;

void init(memory *m) {
  for (int i = 0; i < MAIN_MEMORY_SIZE; i++) {
    m->ram[i] = 0;
  }
}

void reset(memory *m, 
           cpu *cpu) {
  cpu->pc = 0xfffc;
  cpu->sp = 0x00ff;
  // clear registers
  // clear status flags

  init(m);
}

unsigned char fetch_byte(int *cycles,
                         memory *m, 
                         cpu *c) {
  unsigned char data = m->ram[c->pc];
  c->pc++;
  cycles--;

  return data;
}

void execute(int *cycles,
             memory *m, 
             cpu *c) {
  // lda: load accumulator, 2 cycles
  // 1 cycle to read opcode
  // 1 cycle to read immediate value

  while (*cycles > 0) {
    unsigned char instruction = fetch_byte(cycles, m, c);

    switch (instruction) {
      case 0xa9: {
        unsigned char value = fetch_byte(cycles, m, c);  // fetch instruction operands from memory
        c->a = value;
        c->sf->z = (c->a == 0);
        c->sf->n = (c->a & 0b10000000) > 0;
        break;
      } case 0: {
        break;
      } default: {
        // throw exception
        printf("Invalid opcode in instruction.");
        break;
      }
    }
  }
}

int main() {
  memory *m;
  cpu *c;

  reset(m, c);

  // load into memory
  //
  // instruction 1
  // opcode
  // m.ram[0xffc] = lda;
  // operand
  // m.ram[0xffd] = 0x42;

  // while (true)
  int *cycles;
  *cycles = 2;
  execute(cycles, m, c);

  return 0;
}