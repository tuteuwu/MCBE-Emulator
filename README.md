# MCBE-Emulator
Emulador de la maquina conceptual MCBE (Modelo Computacional Básico Elemental)

### **Instalación**
- Clonar el repositorio
	`git clone https://github.com/tuteuwu/MCBE-Emulator.git`
- Entrar al directorio
	`cd MCBE-Emulator`
- Compilar
	`gcc main -o mcbe`
### **Como usarlo?**
- Ejecutarlo con un parámetro de entrada:
- `./mcbe codigo.mcbe`
- Ejecutarlo con logs:
- `./mcbe -l codigo.mcbe`

# **Como Funciona ?**

  La MCBE es una maquina conceptual pensada para aprender como funciona de forma simple una computadora.
  Esta compuesta por los siguientes elementos:
- Memoria
- CPU
	- ALU
	- Program Counter (PC)
	- Instruction Register (IR)
	- Accumulator
	- Unidad de Control (UC)

- Memoria:
	- La memoria esta compuesta por 32 (0-31) casillas de 1 byte.
	- La casilla 30 esta reservada para la entrada (input).
	- La casilla 31 esta reservada para la salida (output).
- CPU:
	- ALU:
		- Unidad aritmetico-logica. Se encarga de hacer las operaciones logicas.
	- PC:
		- Contador de programa. Registra la direccion en memoria de la proxima instruccion.
	- IR:
		- Registro De Instruccion. Registra la instruccion actual a ejecutar.
	- Accumulator
		- Acumulador. registro que guarda un dato para operar.
	- UC
		- Unidad de control. Decodifica las instrucciones y orquesta el hardware.


Instrucciones:

- Las instrucciones se componen de 1 byte. donde los primeros 3 bits representan la instrucción y los ultimo 5 el argumento.

- `    000        00000   `
- `Instrucción   Argumento`

- Esto da lugar a 8 instrucciones y 32 argumentos.