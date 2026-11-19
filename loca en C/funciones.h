#define NO_EXISTE_BARRA 0     		/* Valor para que el panel no muestre la barra en dicho lugar*/
#define EXISTE_BARRA 1        		/* Valor para que el panel muestre la barra en dicho lugar*/
#define ELEM_SIN_BARRA_IZQ 0  		/* barra de control vacia para mostrar en el panel*/
#define ELEM_SIN_BARRA_DER 4  		/* barra de control vacia para mostrar en el panel*/
#define CANT_LINEAS_BARRAS 3  		/* cantidad de lineas que ocupa una barra de control formada con ASCII*/
#define VALOR_MAX_DEMANDA 250 		/* cantidad de demanda maxima para el aleatorio*/
#define VALOR_MIN_DEMANDA 50  		/* cantidad de demanda minima para el aleatorio*/
#define MIN_DEM 50					/* minimo de la demanda*/
#define MAX_DEM 200					/* maximo de la demanda*/
#define CANTIDAD_TURNOS 3     		/* cantidad de turnos que hay en un dia */
#define FILA_INICIO_MENU 25   		/* fila en donde empieza el menu */
#define OPCIONES_MENU 7	  			/* cantidad de opciones del menu */
#define ULTIMA_OPCION 7       		/* opción del menú que finaliza el programa */
#define FILA_BARRAS 3         		/* filas de las barras de control*/
#define COLUMNAS_BARRAS 5     		/* columnas de las barras de control*/
#define TOLERANCIA 30 		  		/* tolerancia de la energia generada y la demandada*/
#define LIMITE_DIAS_SEGUIDOS 5		/* cantidad de dias seguidos que se puede estar en exceso/deficit*/
#define VARIACION_DEMANDA 15		/* variacion de la demanda por cada dia*/
#define FINAL_ESCAPE 1        		/* final 1, escapar*/
#define FINAL_DEFICIT 2				/* final 2, preso por deficit de energia*/
#define FINAL_EXCESO 3				/* final 3, preso por exceso de energia*/
#define FINAL_TEMPERATURA 4			/* final 4, temperatura muy alta*/
#define FINAL_PRESION 5				/* final 5, presion muy alta*/
#define FINAL_MIXTO 6				/* final 6, temperatura y presion en cierto rango*/
#define RANGO_FINAL_TEMPERATURA 850 /* final numero 4, valor de temperatura que lo activa*/
#define RANGO_FINAL_PRESION 125 	/* final numero 5, valor de presion que lo activa*/
#define RANGO_FINAL_MIXTO_TMN 650	/* final numero 6, valor minimo de temperatura*/
#define RANGO_FINAL_MIXTO_TMX 799	/* final numero 6, valor maximo de temperatura*/
#define RANGO_FINAL_MIXTO_PMN 100	/* final numero 6, valor minimo de presion*/
#define RANGO_FINAL_MIXTO_PMX 199	/* final numero 6, valor maximo de presion*/

// Declaraciones adelantadas de las funciones
void MostrarMenu(char VecMenu[OPCIONES_MENU][50]);
void MostrarMonitor(int t, int d, int inc, int def, int exc, int dem);
void MostrarEstado(bool encendido, float pctagua, float aguav, float reac, float tempe, float presi, bool vapr, bool valvula, float velturb, float potenciaagenerar, float potencia, float dem);
void CompararGeneracionDemanda(float generacion, float dem);
void MostrarReactividad(bool encendido, float reac);
void MostrarPanel(int cantfuelactivo);
void NombreEscenario(float reac);
void Finales(int tipofin);
void MostrarReactividad(bool encendido, float reac);
void PasarTurnos(int *turno, int *dia);
int CrearDemanda(float anterior);
int ObtnrBarrasDeControl();
int PasarDia(int turno, int dia);
int SeleccionaItem(char VecMenu[OPCIONES_MENU][50]);
int ObtenerPorcentajeAgua(int actual);
int CheckeoCatastrofes(float tempactual, float presactual);
int VelocidadTurbina(float presactual, bool hay, bool valvulaabierta);
int PotenciaElectrica(float vt);
int CheckeoDiasFinales(float def, float exc);
float ConvertirAgua(float pctagua);
float Reactividad(float aguav, float barrasctl);
float Temperatura(float , int , float );
float TemperaturaApagado(float tempactual);
float PresionActualizada(float tempactual, float presactual, bool valvulaabierta);
bool HayVapor(bool encendido, float presi);