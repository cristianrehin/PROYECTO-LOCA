#define SIGUE_VIVO 0                    /* verificacion para el while del juego en si*/
/*#define ESCAPE 1 		  			    /* final 1: escape*/
#define NO_EXISTE_BARRA 0     		    /* Valor para que el panel no muestre la barra en dicho lugar*/
#define EXISTE_BARRA 1        		    /* Valor para que el panel muestre la barra en dicho lugar*/
#define ELEM_SIN_BARRA_IZQ 0  		    /* barra de control vacia para mostrar en el panel*/
#define ELEM_SIN_BARRA_DER 4  		    /* barra de control vacia para mostrar en el panel*/
#define CANT_LINEAS_BARRAS 3  		    /* cantidad de lineas que ocupa una barra de control formada con ASCII*/
#define VALOR_MAX_DEMANDA 250 		    /* cantidad de demanda maxima para el aleatorio*/
#define VALOR_MIN_DEMANDA 50  		    /* cantidad de demanda minima para el aleatorio*/
#define MIN_DEM 50					    /* minimo de la demanda*/
#define MAX_DEM 200					    /* maximo de la demanda*/
#define CANTIDAD_TURNOS 3     		    /* cantidad de turnos que hay en un dia */
#define OPCIONES_MENU 7	                /* cantidad de opciones del menu */
#define FILA_INICIO_MENU 25   		    /* fila en donde empieza el menu */
#define ULTIMA_OPCION 7       		    /* opción del menú que finaliza el programa */
#define FILA_BARRAS 3         		    /* filas de las barras de control*/
#define COLUMNAS_BARRAS 5     		    /* columnas de las barras de control*/
#define TOLERANCIA 30 		  		    /* tolerancia de la energia generada y la demandada*/
#define LIMITE_DIAS_SEGUIDOS 5		    /* cantidad de dias seguidos que se puede estar en exceso/deficit*/
#define VARIACION_DEMANDA 15		    /* variacion de la demanda por cada dia*/
#define FINAL_ESCAPE 1        		    /* final 1, escapar*/
#define FINAL_DEFICIT 2				    /* final 2, preso por deficit de energia*/
#define FINAL_EXCESO 3				    /* final 3, preso por exceso de energia*/
#define FINAL_TEMPERATURA 4			    /* final 4, temperatura muy alta*/
#define FINAL_PRESION 5				    /* final 5, presion muy alta*/
#define FINAL_MIXTO 6				    /* final 6, temperatura y presion en cierto rango*/
#define RANGO_FINAL_TEMPERATURA 850     /* final numero 4, valor de temperatura que lo activa*/
#define RANGO_FINAL_PRESION 135 	    /* final numero 5, valor de presion que lo activa*/
#define RANGO_FINAL_MIXTO_TMN 650	    /* final numero 6, valor minimo de temperatura*/
#define RANGO_FINAL_MIXTO_TMX 799	    /* final numero 6, valor maximo de temperatura*/
#define RANGO_FINAL_MIXTO_PMN 100	    /* final numero 6, valor minimo de presion*/
#define RANGO_FINAL_MIXTO_PMX 199	    /* final numero 6, valor maximo de presion*/
#define MAX_DIAS_SIN_INCIDENTES 10		/* Umbral de dias sin incidentes*/
#define DIA_EXC_DEF	1					/*Unidad de dia para exceso/deficit*/
#define BONIFICADOR_DEMANDA	10
#define DUPLICADOR 2
#define MAX_CHANCE 8					/*por debajo de este valor se mantiene la logica del incremento/descenso de demanda*/
#define	CHANCE_AUMENTO 9				/*si chance vale este valor aumenta la demanda*/
#define CHANCE_DISMINUYE 10				/* si chance vale este valor disminuye la demanda*/
#define VEINTE_POR_CIENTO_MAS 1.2		/*sube 20% la demanda*/
#define	VEINTE_POR_CIENTO_MENOS 0.8     /*baja 20% la demanda*/
#define MIN_ALEATORIO	1				/*valor minimo para funcion aleatorio*/
#define FLECHA_ARRIBA 72				/*desliza hacia arriba en el menu*/				
#define FLECHA_ABAJO 80					/*desliza hacia abajo en el menu*/	
#define ENTER 13
#define TECLA_ESPECIAL 0			    /*prefijo de tecla especial*/
#define TECLA_ESPECIAL_ 224			    /*prefijo de tecla especial*/
#define MAX_VEL_TURBINA 3000	        /*velocidad maxima de la turbina*/
#define MIN_PRESION	10			        /*valor minimo de presion para generar energia*/
#define MAX_PRESION	40			        /*valor maximo de presion para generar energia*/
#define REAC_POSITIVA 0.05
#define REAC_POSITIVA_ALTA 0.25
#define REAC_NEGATIVA -0.05
#define REAC_NEGATIVA_ALTA -0.25
#define COEF_TERMICO	0.25
#define TEMP_AMBIENTE	20
#define AGUA_CAL	100			        /*temperatura evaporacion del agua */
#define MIN_AGUA	30                  /*porcentaje minimo de agua para que no de aviso*/
#define TEMP_RIESGO 600
#define TEMP_CAOS	700
#define PRES_RIESGO 80
#define PRES_CAOS	100
#define DECIMADOR 10
#define PORCENTUADOR 100
#define PORCENTUADOR_TEMP 100.f
#define FRACCIONARIO_TEMP 1.f
#define MULTIPLICADOR_100 100
#define LITROS	1000
#define CAUDAL	10.4
#define MASA_AGUA 28800
#define MAX_PORCEN_AGUA	100
#define MIN_PORCEN_AGUA	1         
#define CANT_BARRAS_TOTALES 11
#define CANT_BARRAS_MINIMA 0
#define AMARILLO "\033[33m"     
#define ROJO     "\033[31m"
#define AMARILLO_FONDO "\033[33;7m"     
#define RESET_FORMATO_TEXTO "\033[0m"
#define VALORES_ANTERIORES 1
#define TEMPERATURA_MINIMA 20
#define SKALA_NORMAL       0
#define SKALA_ADVERTENCIA  1
#define SKALA_CRITICO      2
#define COEF_PRES		0.35
#define COEF_PRES_SUBE  0.3
#define COEF_PRES_BAJA  0.2
#define CONVERSOR_AGUA 299.52
#define	COEF_REAC	0.45
#define COEF_BARRAS 0.9
#define COEF_AGUA 0.1
#define COEF_FRAC_AGUA 0.5
#define COEF_TURBINA 250
#define NO_EXISTE_VALORES_ANTERIORES 0

enum TURNOS {MATUTINO,VESPERTINO,NOCTURNO};
enum BARRASPANEL {ARRIBA,CENTRO,ABAJO};

enum Menu{
ENCEN_REACTOR = 1,
INGRESAR_BARRAS,
ABR_VALVULA,
CANT_AGUA,
FIN_TURNO,
APAG_REACTOR,
SALIR
};

// Definir la estructura reactor
typedef struct {
	int agua;
	int pot;
	int barras;
	float pres;
	float temp;
	float react;
}Reactor;


// Declaraciones adelantadas de las funciones
void MostrarMenu(char VecMenu[OPCIONES_MENU][50]);
void MostrarMonitor(int t, int d, int inc, int def, int exc, int dem);
void MostrarEstado(int encendido, float pctagua, const Reactor *reactor, int vapr, int valvula, float velturb, float potenciaagenerar, float dem);
void CompararGeneracionDemanda(float generacion, float dem);
void MostrarReactividad(int encendido, const Reactor *reactor);
void MostrarPanel(const Reactor *reactor);
void NombreEscenario(float reac);
void Finales(int tipofin);
void PasarTurnosYDias(int *turno, int *dia);
void SubirDatosNuevos();
void SubirDatosActualizados(const Reactor *,int *,int *,int *,int *,int *,int *,int *,int *,int *,int *,int *,int *,int *, int *, int *, int *);
int CrearDemanda(float anterior);
int ObtnrBarrasDeControl();
int SeleccionaItem(char VecMenu[OPCIONES_MENU][50]);
int ObtenerPorcentajeAgua(int actual);
int CheckeoCatastrofes(const Reactor *reactor);
int VelocidadTurbina(const Reactor *reactor, int hay, int valvulaabierta);
int PotenciaElectrica(float vt);
int CheckeoDiasFinales(float def, float exc);
int HayVapor(int encendido, const Reactor *reactor);
float ConvertirAgua(int pctagua);
float Reactividad(const Reactor *reactor);
float Temperatura(const Reactor *reactor);
float TemperaturaApagado(const Reactor *reactor);
float PresionActualizada(const Reactor *reactor, int valvulaabierta);