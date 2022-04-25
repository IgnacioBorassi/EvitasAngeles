#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <random>
#include <chrono>
#include <math.h>
#include <sys/mman.h>  
#include <string.h>  
#include <atomic>
#include <tuple>
#include <cassert>

using namespace std;


void* share_mem(int size);
// Los detalles de esta funcion NO son relevantes para el TP!! Pero los aclaro 
//por las dudas
// Esta funcion pide size * sizeof(atomic<float>) bytes dinámicamente
// Este espacio permite reservar suficiendte memoria para un ARREGLO de size 
//enteros atomicos. Nos devuelve un puntero (un tipo similar a las referencias)
// al comienzo de este espacio contiguo de memoria.
// Nota: sizeof es una funcion que nos dice el tamaño de un tipo, en este caso
// atomic float.

tuple<string, int> spawn_children(int max_days,int day, atomic<float> *newborns,
 								  float p, float p2, int max_children,
 								  string my_type);
// Esta funcion simula el proceso de nacimiento de evitas y angeles que se lleva
// a cabo en el dia day. Tener en cuenta que esta función será ejecutada por 
// todos los evitas/angeles que puedan reproducirse en el dia day.
// Ademas del dia de creación, recibe como parametro el arreglo newborns 
// (expresado como puntero, pero eso no les molesta, pueden usar la sintaxis de
// arreglos como siempre) que debe ser actualizado con los nuevos evitas/angeles
// que resulten creados en este dia.
// La funcion debe crear los procesos necesarios y devolver una tupla cuya
// primera componente indique el tipo creado (evita, angel) y el caso especial 
// de "parent".

int main(int argc, char* argv[])
{
	// Pensar el main como proceso "simulacion" que puede controlar todo.
	// La responsabilidad es la de recopilar información de la simualación.

	// Utilizamos los parámetros que recibe main para obtener los valores de la 
	// simulacion (ver orden en pdf)
	int max_days = atoi(argv[1]); 
	int M = atoi(argv[2]); 
	int N = atoi(argv[3]); 
	float ps[3];
	ps[0] = stof(argv[4]);
	ps[1] = stof(argv[5]);
	ps[2] = stof(argv[6]);
	float eps = stof(argv[7]);

 	// Vamos a almacenar la cantidad de evitas/angeles nuevos al termino de cada
 	// dia en un arreglo de floats y el poder acumulado de cada especia por día.
 	// Podemos pensarlo como un arreglo de 2 filas x max_days columnas.
 	// Es probable que necesiten que este arreglo sea compartido por todos los 
 	// procesos.
 	// Notar que en realidad estamos usando un float "atomico". Esto nos da 
 	// propiedades interesantes para evitar algunos problemas en caso de que
 	// ocurran escrituras concurrentes sobre el mismo espacio de memoria.
 	// Pueden asumir que funciona como cualquier otro float. 

 	int size = /*COMPLETAR*/;
	atomic<float> *evitas = (atomic<float> *) share_mem(/*COMPLETAR*/);
	atomic<float> *angeles = (atomic<float> *) share_mem(/*COMPLETAR*/);
	
	// Por las dudas, fijamos a 0 todos los valores
	for(int i = 0;i < 2*size; i++)evitas[i] = 0;
	for(int i = 0;i < 2*size; i++)angeles[i] = 0;

	// Inicializamos el conteo para para el dia 0
	evitas[0] = 1;
	angeles[0] = 1;	
  	
  	// Atributos de cada especie. Sugerencia, setear cada variable según tipo.
  	int pid;
  	atomic<float> *newborns;
  	float max_children,p, p_beserk;
  	string my_type = "";

  	// Crear a Adam y Lilith 
	/*COMPLETAR*/

	// El control de la simulación debería esperar la finalización para comenzar
	// a computar la información necesaria.
		
	// En este punto, solo Adam y Lilith deberian llegar

  	// Variables que representan los dias y la cantidad de hijos de un día.
 	int day, nchildren;
 	
 	// Corremos las simulaciones de nacimiento
	for(day = 1; day < max_days; day++)
	{
		// Simulamos un dia y obtenemos los resultados
		tuple<string,int> result = spawn_children(max_days,day,newborns, p,
												  p_beserk, max_children,my_type);
		string type = get<0>(result);
		nchildren = get<1>(result);
	
		if (type == "parent")
		{	
			
		}

		// Si llego acá, es porque nací en este día.
		// Tengo que avanzar al próximo día de la simulación	
		
		assert(type != "parent");
		continue;
	}

	printf("[%d][dia %d] %s hoja, engendrado de %d \n",getpid(),
			day,my_type.c_str(), getppid());

}


void* share_mem(int size)
{
	// Vamos a pedir size * sizeof(atomic<int>) para reservar suficiendte memoria 
	// para un vector sizeof nos dice el tamaño del tipo atomic int.
    void * mem;
    if( MAP_FAILED == (mem = (atomic<float>*)mmap(NULL, sizeof(atomic<float>)*size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0) ) )
    {
        perror( "mmap failed" );
        exit( EXIT_FAILURE );
    }
    return mem;
}


tuple<string, int> spawn_children(int max_days,int day, atomic<float> *newborns,
		 						  float p, float p2, int max_children, 
		 						  string my_type)
{
	//
	int nchildren = 0;
	int pid = -1;

	// Seteamos los valores de la distribución según cada especie
	float u = (my_type == "Evitas") ? 20 : 25;
	float std = (my_type == "Evitas") ? 5 : 10;

	// Inicializamos un generador con distribucion uniforme en el intervalo (0,1)
	// La semilla es aleatoria, pero puede ser util fijarla para facilitar las pruebas	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator (3);
 	uniform_real_distribution<double> distribution(0.0,1.0);

 	// Inicializamos un generador con distribucion normal
 	normal_distribution<double> ap_distribution(u,std);

	for(int i = 0; i < max_children; i++)
	{
		// Simulamos un experimento de nacimiento. 
		// Ej: Si p = 0.2  y el valor obtenido es <= a 0.2, el evita actual
		// tiene exito en crear un nuevo hijo
		bool isBorn =  /*COMPLETAR*/;

		// Si hay nacimiento tenemos que crear al evita/angel para que luego
		// en day+1 continue con la simulación
		if(isBorn)
		{
			// Actualizar las estructuras necesarias
			/*COMPLETAR*/
		}
	}
	if (pid == -1 || pid > 0)
		my_type = "parent"; 
	
	return make_tuple(my_type, nchildren);
}