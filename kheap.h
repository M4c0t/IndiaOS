/*
    This file is part of IndiaOS
    Copyright (C) 2017 - 2018 Luis David Julio Macot

    IndiaOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IndiaOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//Escrito por james M.//


/****************************************************************************/
//Administracion de la memoria. definicion de extructuras y funciones.
/****************************************************************************/

typedef void* type_t;

//debe devolver 0 en caso que el primer arg sea menor que el segundo//
typedef char (*lessthan_predicate_t)(type_t,type_t);

typedef struct
{
    type_t *array;
    unsigned int size;
    unsigned int max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;


char standard_lessthan_predicate(type_t a, type_t b);

//crea un array ordenado//
ordered_array_t create_ordered_array(unsigned int max_size, lessthan_predicate_t less_than);
ordered_array_t place_ordered_array(void *addr, unsigned int max_size, lessthan_predicate_t less_than);

//destruye el array//
void destroy_ordered_array(ordered_array_t *array);

//inserta un nuevo elemento a la matriz//
void insert_ordered_array(type_t item, ordered_array_t *array);

//busca el punto en el indice i//
type_t lookup_ordered_array(unsigned int i, ordered_array_t *array);

//Elimina el elemento en la posicion i de la matriz
void remove_ordered_array(unsigned int i, ordered_array_t *array);


#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000

#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0x123890AB
#define HEAP_MIN_SIZE     0x70000

//informacion sobre el bloque
typedef struct
{
    unsigned int magic;   // Numero magico para la identidicacion
    unsigned char is_hole;   // 1 si es un agujero, 0 en caso de bloque
    unsigned int size;    // tamaño del bloque incluyendo pagina final
} header_t;

typedef struct
{
    unsigned int magic;     // Numero magico igual que en header_t.
    header_t *header; // Puntero a la cabeza del bloque
} footer_t;

typedef struct
{
    ordered_array_t index;
    unsigned int start_address; // Comienzo del espacio asignado
    unsigned int end_address;   // final del espacio asignado
    unsigned int max_address;   // direccion maxima
    unsigned char supervisor;     
    unsigned char readonly;       
} heap_t;


//crea un nuevo heap//

heap_t *create_heap(unsigned int start, unsigned int end, unsigned int max, unsigned char supervisor, unsigned char readonly);

//Asigna una region continua de tamaño//
void *alloc(unsigned int size, unsigned char page_align, heap_t *heap);

//Libera un bloque asignado por alloc//
void free(void *p, heap_t *heap);

unsigned int kmalloc_int(unsigned int sz, int align, unsigned int *phys);

//Asigna paginas Alineadas//
unsigned int kmalloc_a(unsigned int sz);

//Asigna paginas phys tiene que ser un unsigned int valido//
unsigned int kmalloc_p(unsigned int sz, unsigned int *phys);

//asigna paginas alineadas
unsigned int kmalloc_ap(unsigned int sz, unsigned int *phys);

//funcion general de asignacion de memoria
unsigned int kmalloc(unsigned int sz);

//libera memoria
void kfree(void *p);

