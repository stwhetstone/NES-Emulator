# Contents
- [Classes](#classes)
  - [NES](#nes)
  - [CPU](#cpu)
  - [ROM](#rom)
- [Types](#types)
  - [NESTypes](#nestypes)
    - [Bus](#bus)
  - [CPUTypes](#cputypes)
    - [RegList](#reglist)
    - [Flags](#flags)
    - [InstructionDef](#instructiondef)

# Classes
### NES
- public 
   - vars
     - [CPU](#cpu) cpu
     - [ROM](#rom) rom, 
     - [NESTypes](#nestypes)::[Bus](#bus) bus
     - array<uint8_t, 0x800> ram
   - functions
     - NES()
   - static
     - functions
        - hexStringFileToHexFile(string filename)

### CPU
- public
  - vars
    - vector<instructiondef> instructionTable
    - [CPUTypes](#cputypes)::[RegList](#reglist) registers
    - [NESTypes](#nestypes)::[Bus](#bus) &bus
    - uint8_t cyclesRemaining
 - functions
   - CPU([NESTypes](#nestypes)::[Bus](#bus) &b)
   - uint8_t getFlagValue([CPUTypes](#cputypes)::[Flags](#flags) f)
   - void initInstructionTable()
   - void printRegisters()
   - void setFlagValue([CPUtypes](#cputypes)::[Flags](#flags) f, uint8_t b)
   - step()
   - <a href="https://www.nesdev.org/wiki/Instruction_reference">Instructions</a> 
     - form -> void ABC()

### ROM 
- public
  - vars
    - [NESTypes](#nestypes)::[Bus](#bus) &bus
    - vector<int> data
 - functions
   - ROM([NESTypes](#nestypes)::[Bus](#bus) &b)


# Types
## NESTypes
### Bus
struct {\
&nbsp;&nbsp;uint8_t data\
&nbsp;&nbsp;uint16_t address\
}

### CPUTypes
### RegList
struct {\
&nbsp;&nbsp;uint8_t A, X, Y, SP, status\
&nbsp;&nbsp;uint16_t PC\
}
### Flags
enum {\
&nbsp;&nbsp;C = 0\
&nbsp;&nbsp;Z = 1\
&nbsp;&nbsp;I = 2\
&nbsp;&nbsp;D = 3\
&nbsp;&nbsp;B = 4\
&nbsp;&nbsp;ONE = 5\
&nbsp;&nbsp;V = 6\
&nbsp;&nbsp;N = 7\
}

### InstructionDef
struct {\
&nbsp;&nbsp;uint8_t size, cycles\
&nbsp;&nbsp;function<void()> fnc\
}
