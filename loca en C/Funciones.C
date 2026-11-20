#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "utiles.h"
#include "funciones.h"

int ObtnrBarrasDeControl()
{
	printf("%s", AMARILLO);
    int d;
	do
	{
		d = leerEntero();
		if(!(d <= CANT_BARRAS_TOTALES && d >= CANT_BARRAS_MINIMA))
			printf("Fuera de rango\n");
	} while (!(d <= CANT_BARRAS_TOTALES && d >= CANT_BARRAS_MINIMA));
	printf("%s",RESET_FORMATO_TEXTO);
    return d;
}
void MostrarMenu(char VecMenu[][50])
{
	printf("%sIngrese una opcion\n", AMARILLO);
    int i = 0;
	printf("%s %s %s\n", AMARILLO_FONDO, VecMenu[i], RESET_FORMATO_TEXTO); // Muestra el primer elemento con el formato de fondo rellenado, con color amarillo
    for (i = 1; i < ULTIMA_OPCION; i++)
    {
		printf("%s %s %s\n", AMARILLO, VecMenu[i], RESET_FORMATO_TEXTO); // muestra el resto de las opciones como letras normales, con color amarillo
    }
}
void MostrarMonitor(int t, int d, int inc, int def, int exc, int dem)
{
	printf("%s+---------------------+ La demanda generada por la ciudad es: %s%d MW.%s\n", AMARILLO, AMARILLO_FONDO, dem, RESET_FORMATO_TEXTO);
	printf("%s| Dias Sin Incidentes | El dia es: %d", AMARILLO, d);
	if (d<MAX_DIAS_SIN_INCIDENTES) {
		/* Este 'si' esta para saber cuanto espacio hay que tener entre que dia es y cuantos dias seguidos de deficit/exceso hay que tener */
		if (exc>=DIA_EXC_DEF) {
			printf("                                       Dias seguidos con exceso de energia: %d/%d\n", exc, LIMITE_DIAS_SEGUIDOS);
		} else {
			printf("                                       Dias seguidos con deficit de energia: %d/%d\n", def, LIMITE_DIAS_SEGUIDOS);
		}
	} else {
		if (exc>=DIA_EXC_DEF) {
			printf("                                      Dias con exceso de energia: %d/%d\n", exc, LIMITE_DIAS_SEGUIDOS);
		} else {
			printf("                                      Dias con deficit de energia: %d/%d\n", def, LIMITE_DIAS_SEGUIDOS);
		}
	}
	if (inc<MAX_DIAS_SIN_INCIDENTES) {
		switch (t) {
		case MATUTINO:
			printf("|         %d           | Turno Matutino.\n", inc);
			break;
		case VESPERTINO:
			printf("|         %d           | Turno Vespertino.\n", inc);
			break;
		case NOCTURNO:
			printf("|         %d           | Turno Nocturno.\n", inc);
			break;
		}
	} else {
		switch (t) {
		case MATUTINO:
			printf("|         %d          | Turno Matutino.\n", inc);
			break;
		case VESPERTINO:
			printf("|         %d          | Turno Vespertino.\n", inc);
			break;
		case NOCTURNO:
			printf("|         %d          | Turno Nocturno.\n", inc);
			break;
		}
	}
	printf("+---------------------+ %s\n", RESET_FORMATO_TEXTO);
	printf("\n");
}
int CrearDemanda(float anterior)
{
	int chance;
	int demanda;
	chance = (rand() % BONIFICADOR_DEMANDA);
	demanda = anterior + (rand() % (VARIACION_DEMANDA*DUPLICADOR)) - VARIACION_DEMANDA;
	if (chance > MAX_CHANCE)
	{
		switch (chance)
		{
		case CHANCE_AUMENTO:
			demanda = trunc(demanda * VEINTE_POR_CIENTO_MAS);
			break;
		case CHANCE_DISMINUYE:
			demanda = trunc(demanda * VEINTE_POR_CIENTO_MENOS);
			break;
		}
	}
	if (demanda < MIN_DEM)
	{
		demanda = MIN_DEM;
	}
	if (demanda > MAX_DEM)
	{
		demanda = MAX_DEM;
	}
	return demanda;
}
void PasarTurnosYDias(int *turno, int *dia)
{
	*turno = *turno + 1;
    if (*turno >= CANTIDAD_TURNOS)
    {
		*dia = *dia + 1;
        *turno = 0;
    }
}
void MostrarPanel(const Reactor *reactor)
{
    int cont, i, j, k;
    int matriz[FILA_BARRAS][COLUMNAS_BARRAS];
	printf("%s                              Barras de control insertadas (X = insertada, vacia = retirada):\n", AMARILLO);
    cont = 0;
    for (i = 0; i < FILA_BARRAS; i++)
    {
        for (j = 0; j < COLUMNAS_BARRAS; j++)
        {
            if (i == 1)
            {
                matriz[i][j] = EXISTE_BARRA;
            }
            else
            {
                if (j == ELEM_SIN_BARRA_IZQ || j == ELEM_SIN_BARRA_DER)
                {
                    matriz[i][j] = NO_EXISTE_BARRA;
                }
                else
                {
                    matriz[i][j] = EXISTE_BARRA;
                }
            }
        }
    }
    for (i = 0; i < FILA_BARRAS; i++)
    {
        for (k = 0; k < CANT_LINEAS_BARRAS; k++)
        {
			printf("                                    ");
            for (j = 0; j < COLUMNAS_BARRAS; j++)
            {
                if (matriz[i][j] == NO_EXISTE_BARRA)
                {
                    printf("         ");
                }
                else
                {
                    if (cont < reactor->barras)
                    {
                        switch (k)
                        {
                        case ARRIBA:
                            printf("  /***\\  ");
                            break;
                        case CENTRO:
                            printf(" (  X  ) ");
                            cont++;
                            break;
                        case ABAJO:
                            printf("  \\___/  ");
                            break;
                        default:
                            break;
                        }
                    }
                    else
                    {
                        switch (k)
                        {
                        case ARRIBA:
                            printf("  /***\\  ");
                            break;
                        case CENTRO:
                            printf(" (     ) ");
                            cont++;
                            break;
                        case ABAJO:
                            printf("  \\___/  ");
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            printf("\n");
        }
    }
    printf("%s\n", RESET_FORMATO_TEXTO);
}
int SeleccionaItem(char VecMenu[][50])
{
    int tecla;
    int i = 0;

    do
    {
        tecla = getch();

        if (tecla == TECLA_ESPECIAL || tecla == TECLA_ESPECIAL_) /* prefijo de tecla especial */
        {
            tecla = getch();

            switch (tecla)
            {
            case FLECHA_ABAJO: /* flecha abajo */
                /* despinta el ítem actual */
                printf("\033[%d;1H\033[K%s %s \033[0m\n", i + FILA_INICIO_MENU, AMARILLO, VecMenu[i]); // el "\033[%d;1H\033[K" es para borrar la línea y para saber en que línea escribir, %s es para el color amarillo, \033[0m resetea formato de colores

                i++;
                if (i > ULTIMA_OPCION - 1)
                    i = 0;
                /* pinta el nuevo ítem seleccionado */
                printf("\033[%d;1H\033[K%s %s \033[0m\033[27m\n", i + FILA_INICIO_MENU, AMARILLO_FONDO, VecMenu[i]); //\033[7m es para el fondo rellenado, \033[27m resetea el fondo para que sea normal
                break;

            case FLECHA_ARRIBA: /* flecha arriba */
                printf("\033[%d;1H\033[K%s %s \033[0m\n", i + FILA_INICIO_MENU, AMARILLO, VecMenu[i]); // el "\033[%d;1H\033[K" es para borrar la línea y para saber en que línea escribir, %s es para el color amarillo, \033[0m resetea formato de colores

                i--;
                if (i < 0)
                    i = ULTIMA_OPCION - 1;

                printf("\033[%d;1H\033[K%s %s \033[0m\033[27m\n", i + FILA_INICIO_MENU, AMARILLO_FONDO, VecMenu[i]); //\033[7m es para el fondo rellenado, \033[27m resetea el fondo para que sea normal
                break;
            }
        }

    } while (tecla != ENTER); /* Enter */

    return i + 1; // retorna la opción elegida (1 a 8)
}
float Temperatura(const Reactor *reactor)
{
    float t;
    t = reactor->temp + (MULTIPLICADOR_100 * DUPLICADOR * DUPLICADOR * reactor->react) - (FRACCIONARIO_TEMP - reactor->agua / PORCENTUADOR);
	if (t < TEMPERATURA_MINIMA)
		t = TEMPERATURA_MINIMA;
    return t;
}

int CheckeoDiasFinales(float def, float exc)
{
	int terminarjuego;
	terminarjuego = 0;
	if (def >= LIMITE_DIAS_SEGUIDOS)
	{
		terminarjuego = FINAL_DEFICIT;
	}
	if (exc >= LIMITE_DIAS_SEGUIDOS)
	{
		terminarjuego = FINAL_EXCESO;
	}
	return terminarjuego;
}
int CheckeoCatastrofes(const Reactor *reactor)
{
	int catastrofe;
	catastrofe = 0;
	if (reactor->temp >= RANGO_FINAL_TEMPERATURA)
	{
		catastrofe = FINAL_TEMPERATURA;
	}
	else
	{
		if (reactor->pres > RANGO_FINAL_PRESION)
		{
			catastrofe = FINAL_PRESION;
		}
		else
		{
			if (reactor->temp >= RANGO_FINAL_MIXTO_TMN && reactor->temp <= RANGO_FINAL_MIXTO_TMX && reactor->pres >= RANGO_FINAL_MIXTO_PMN && reactor->pres <= RANGO_FINAL_MIXTO_PMX)
			{
				catastrofe = FINAL_MIXTO;
			}
		}
	}
	return catastrofe;
}
void CompararGeneracionDemanda(float generacion, float dem)
{
	if (generacion < dem - TOLERANCIA)
	{
		printf("\033[31m DEFICIT: %sgeneracion %.2f MW; esta %.2f MW por debajo de la demanda.\033[0m\n", AMARILLO, generacion, dem - generacion);
		printf(" \n");
	}
	else
	{
		if (generacion > dem + TOLERANCIA)
		{
			printf("\033[31m EXCEDENTE: %sgeneracion %.2f MW; esta %.2f MW por encima de la demanda.\033[0m\n", AMARILLO, generacion, generacion - dem);
			printf(" \n");
		}
		else
		{
			printf("\033[32m BALANCE ACEPTABLE: %sgeneracion %.2f MW para una demanda de %.2f MW (tolerancia +/- %d MW).\033[0m\n", AMARILLO, generacion, dem, TOLERANCIA);
			printf(" \n");
		}
	}
}
int PotenciaElectrica(float vt)
{
	int p;
	p = trunc(COEF_TURBINA * vt / MAX_VEL_TURBINA);
	return p;
}
int VelocidadTurbina(const Reactor *reactor, int hay, int valvulaabierta)
{
	int velocidad;
	velocidad = 0;
	if (hay == TRUE && valvulaabierta == TRUE && reactor->pres >= MIN_PRESION)
	{
		if (reactor->pres >= MAX_PRESION)
		{
			velocidad = MAX_VEL_TURBINA;
		}
		else
		{
			velocidad = trunc(MAX_VEL_TURBINA * (reactor->pres - MIN_PRESION) / 30);
		}
	}
	return velocidad;
}
int HayVapor(int encendido, const Reactor *reactor)
{
	int hay;
	if (encendido == TRUE && reactor->pres >= MIN_PRESION)
	{
		hay = TRUE;
	}
	else
	{
		hay = false;
	}
	return hay;
}
float PresionActualizada(const Reactor *reactor, int valvulaabierta)
{
	float nuevapresion;
	float presobj;
	if (reactor->temp > AGUA_CAL)
	{
		presobj = 1 + (reactor->temp - AGUA_CAL) * COEF_PRES;
	}
	else
	{
		presobj = 1;
	}
	nuevapresion = reactor->pres + COEF_PRES_SUBE * (presobj - reactor->pres);
	if (valvulaabierta == TRUE)
	{
		nuevapresion = nuevapresion - COEF_PRES_BAJA * (nuevapresion - 1);
	}
	if (nuevapresion < 1)
	{
		nuevapresion = 1;
	}
	return nuevapresion;
}
float TemperaturaApagado(const Reactor *reactor)
{
	float nuevatemp;
	nuevatemp = reactor->temp - COEF_TERMICO * (reactor->temp - TEMP_AMBIENTE );
	return nuevatemp;
}
float ConvertirAgua(int pctagua)
{
	float vol;
	vol = CAUDAL * MASA_AGUA / LITROS * pctagua / PORCENTUADOR;
	return vol;
}

/* Reactividad (indice abstracto, escala aprox. -0.5 a +0.5). */
/* - Barras: 0 insertadas = +0.45 ; 11 insertadas = -0.45 antes del ajuste por agua. */
/* - Agua como moderador: menos agua = menos moderacion = MAS reactividad (hasta +0.05); */
/* mas agua = mas moderacion = MENOS reactividad (hasta -0.05). El termino vale 0 al 50 %. */
float Reactividad(const Reactor *reactor)
{
	float fraccionagua;
	float react;
	fraccionagua = reactor->agua / CONVERSOR_AGUA;
	react = (COEF_REAC - COEF_BARRAS * reactor->barras / CANT_BARRAS_TOTALES) + COEF_AGUA * (COEF_FRAC_AGUA - fraccionagua);
	return react;
}
int ObtenerPorcentajeAgua(int actual)
{
	int nuevo;
	do
	{
		printf("%s Porcentaje actual: %d %%. Ingrese el nuevo porcentaje (1 a 100):\n ", AMARILLO, actual);
		nuevo = leerEntero();
		if (nuevo < MIN_PORCEN_AGUA || nuevo > MAX_PORCEN_AGUA)
		{
			printf(" Valor fuera de rango. Debe estar entre 1 y 100.\n ");
		}
	} while (!(nuevo >= MIN_PORCEN_AGUA && nuevo <= MAX_PORCEN_AGUA));
	if (nuevo > actual)
	{
		printf("\n La circulacion de agua SUBIO de %d %% a %d %%.\n", actual, nuevo);
	}
	else
	{
		if (nuevo < actual)
		{
			printf("\n La circulacion de agua BAJO de %d %% a %d %%.\n", actual, nuevo);
		}
		else
		{
			printf("\n La circulacion de agua no cambio (%d %%).\n", nuevo);
		}
	}
	printf("%s",RESET_FORMATO_TEXTO);
	return nuevo;
}
void NombreEscenario(float reac)
{
	if (reac > REAC_POSITIVA_ALTA)
	{
		printf("\033[31mpositiva alta\033[0m\n");
	}
	else
	{
		if (reac > REAC_POSITIVA)
		{
			printf("%spositiva\033[0m\n", AMARILLO);
		}
		else
		{
			if (reac >= REAC_NEGATIVA)
			{
				printf("\033[32mequilibrio\033[0m\n");
			}
			else
			{
				if (reac >= REAC_NEGATIVA_ALTA)
				{
					printf("%snegativa\033[0m\n", AMARILLO);
				}
				else
				{
					printf("\033[31mnegativa alta\033[0m\n");
				}
			}
		}
	}
}
void MostrarReactividad(int encendido, const Reactor *reactor)
{
	if (encendido == TRUE)
	{
		printf("%s Reactividad: %.2f \033[0m", AMARILLO, round((reactor->react * 1000)) / 1000);
		NombreEscenario(reactor->react);
	}
	else
	{
		printf("%s Reactividad: no aplica (reactor apagado)\033[0m\n", AMARILLO);
	}
}
void MostrarEstado(int encendido, float pctagua, const Reactor *reactor, int vapr, int valvula, float velturb, float potenciaagenerar, float dem)
{
	if (encendido == TRUE)
	{
		printf("%s Reactor:\033[32m ENCENDIDO                                                       \033[0m", AMARILLO);
	}
	else
	{
		printf("%s Reactor:\033[31m APAGADO                                                         \033[0m", AMARILLO);
	}
	MostrarReactividad(encendido, reactor);
	/* Verifica la cantidad de unidades que tiene para saber cuantos espacios hay que tener entre la temperatura y la presion */
	if ((trunc(reactor->temp) / DECIMADOR) < 10)
	{
		printf("%s Temperatura: \033[32m%.0f grados C                                                 \033[0m", AMARILLO, round(round((reactor->temp * 10)) / 10));
	}
	else
	{
		if (reactor->temp < TEMP_RIESGO)
			printf("%s Temperatura: \033[32m%.0f grados C                                                \033[0m", AMARILLO, round(round((reactor->temp * DECIMADOR)) / DECIMADOR));
		else if (reactor->temp >=TEMP_RIESGO && reactor->temp <TEMP_CAOS)
			printf("%s Temperatura: %s%.0f grados C\033[0m                                                ", AMARILLO, AMARILLO_FONDO,round(round((reactor->temp * DECIMADOR)) / DECIMADOR));
		else if (reactor->temp>=TEMP_CAOS)
			printf("%s Temperatura: \033[31m%.0f grados C                                                \033[0m", AMARILLO, round(round((reactor->temp * DECIMADOR)) / DECIMADOR));
		else
			printf("%s Temperatura: \033[31;7m%.0f grados C\033[0m                                                ", AMARILLO, round(round((reactor->temp * DECIMADOR)) / DECIMADOR));
	}
	if (reactor->pres <PRES_RIESGO)
		printf("%s Presion: \033[32m%.2f bar\033[0m\n", AMARILLO, round((reactor->pres * DECIMADOR)) / DECIMADOR);
	else if (reactor->pres < PRES_CAOS)
		printf("%s Presion: \033[31m%.2f bar\033[0m\n", AMARILLO, round((reactor->pres * DECIMADOR)) / DECIMADOR);
	else
		printf("%s Presion: \033[31;7m%.2f bar\033[0m\n", AMARILLO, round((reactor->pres * DECIMADOR)) / DECIMADOR);
	if (valvula == TRUE)
	{
		printf("%s Valvula de vapor: \033[32mABIERTA                                                \033[0m", AMARILLO);
		if (vapr == TRUE)
		{
			printf("%s Vapor disponible:\033[32m SI\033[0m\n", AMARILLO);
		}
		else
		{
			printf("%s Vapor disponible:\033[31m NO\033[0m\n", AMARILLO);
		}
	}
	else
	{
		printf("%s Valvula de vapor: \033[31mCERRADA                                                \033[0m", AMARILLO);
		if (vapr == TRUE)
		{
			printf("%s Vapor disponible: \033[32m SI\033[0m\n", AMARILLO);
		}
		else
		{
			printf("%s Vapor disponible:\033[31m NO\033[0m\n", AMARILLO);
		}
	}
	if (velturb / PORCENTUADOR>= 10)
	{
		/* Verifica la cantidad de unidades que tiene para saber cuantos espacios hay que tener entre la velocidad de la turbina y la potencia generada */
		printf("%s Velocidad de la turbina: %.0f RPM                                        \033[0m", AMARILLO, velturb);
	}
	else
	{
		if (velturb == 0)
		{
			/* Verifica que la velocidad de la turbina sea 0 */
			printf("%s Velocidad de la turbina: %.0f RPM                                           \033[0m", AMARILLO, velturb);
		}
		else
		{
			printf("%s Velocidad de la turbina: %.0f RPM                                         \033[0m", AMARILLO, velturb);
		}
	}
	printf("%s Potencia generada al final del turno: %.2f MW\033[0m\n", AMARILLO, potenciaagenerar);

	if (pctagua <=MIN_AGUA)
		printf("%s Circulacion de agua: \033[31m%.2f %%\033[0m %s(\033[0m\033[31m%.2f \033[0m%smillones de litros por turno)\033[0m\n", AMARILLO, pctagua, AMARILLO, round((reactor->agua * 100)) / 100, AMARILLO);
	else
		printf("%s Circulacion de agua: %.2f %% (%.2f millones de litros por turno)\033[0m\n", AMARILLO, pctagua, round((reactor->agua * PORCENTUADOR)) / PORCENTUADOR);
	CompararGeneracionDemanda(reactor->pot, dem);
}
void SubirDatosNuevos(){
	FILE *fp;
	fp = fopen("persistencia.txt", "w");
	if (fp != NULL){
		fprintf(fp,"1\n"); 			// valor para la persistencia
		fprintf(fp,"120\n"); 			// demanda
		fprintf(fp,"0\n"); 				// dia
		fprintf(fp,"0\n"); 				// turno
		fprintf(fp,"0\n"); 				// cantidad de dias en exceso
		fprintf(fp,"0\n"); 				// cantidad de dias en deficit
		fprintf(fp,"0\n"); 				// dias sin accidentes
		fprintf(fp,"50\n"); 			// porcentaje de agua
		fprintf(fp,"0\n"); 				// potencia a generar
		fprintf(fp,"0\n"); 				// determina si hay un final (0 no lo hay)
		fprintf(fp,"0\n"); 				// turnos de generacion en el margen de error
		fprintf(fp,"0\n"); 				// turnos con deficit
		fprintf(fp,"0\n"); 				// turnos con exceso
		fprintf(fp,"0\n"); 				// velocidad de la turbina
		fprintf(fp,"0\n"); 				// reactor apagado
		fprintf(fp,"0\n"); 				// no hay vapor
		fprintf(fp,"0\n"); 				// la valvula de la presion esta cerrada
		fprintf(fp,"0\n"); 				// potencia del reactor en 0
		fprintf(fp,"1\n"); 				// presion del reactor en 1
		fprintf(fp,"0\n"); 				// reactividad del reactor en 0
		fprintf(fp,"20\n"); 			// temperatura del reactor en 20 grados celcius
		fprintf(fp,"11\n"); 			// cantidad de barras del reactor
		fclose(fp);
	}
}
void SubirDatosActualizados(const Reactor *reactorSimulado,int *demanda,int *dia,int *turno,int *cantDiasExc,int *cantDiasDef,int *diasSinAccidentes,int *porcentajeAgua,int *potAGen,int *queFinal,int *turnosAceptables,int *turnosDeficit,int *turnosExcedidos,int *vTurb, int *reactorP, int *vapor, int *valV){
	FILE *fp;
	fp = fopen("persistencia.txt", "w");
	if (fp != NULL)
	{
		fprintf(fp,"1\n"); 			// valor para la persistencia
		fprintf(fp, "%d\n", *demanda);
		fprintf(fp, "%d\n", *dia);
		fprintf(fp, "%d\n", *turno);
		fprintf(fp, "%d\n", *cantDiasExc);
		fprintf(fp, "%d\n", *cantDiasDef);
		fprintf(fp, "%d\n", *diasSinAccidentes);
		fprintf(fp, "%d\n", *porcentajeAgua);
		fprintf(fp, "%d\n", *potAGen);
		fprintf(fp, "%d\n", *queFinal);
		fprintf(fp, "%d\n", *turnosAceptables);
		fprintf(fp, "%d\n", *turnosDeficit);
		fprintf(fp, "%d\n", *turnosExcedidos);
		fprintf(fp, "%d\n", *vTurb);
		fprintf(fp, "%d\n", *reactorP);
		fprintf(fp, "%d\n", *vapor);
		fprintf(fp, "%d\n", *valV);
		fprintf(fp, "%d\n", reactorSimulado->pot);
		fprintf(fp, "%f\n", reactorSimulado->pres);
		fprintf(fp, "%f\n", reactorSimulado->react);
		fprintf(fp, "%f\n", reactorSimulado->temp);
		fprintf(fp, "%d\n", reactorSimulado->barras);
		fclose(fp);
	}
}
void Finales(int tipofin)
{
	FILE *fp;
	fp = fopen("persistencia.txt", "w");
	if (fp != NULL){
		fprintf(fp,"0"); 			// valor para la persistencia
		fclose(fp);
	}
	switch (tipofin)
	{
	case FINAL_ESCAPE:
		/* Final si elegis la opcion 7 */
		printf("Usted decide abandonar su puesto, dejando todo atras, escabulliendose en el bosque, por miedo a lo que haran sus supervisores al enterarse.\n");
		printf("\n");
		printf("\n");
		printf("                                                                                   %%:                                                                           \n");
		printf("                                                                                  -@+                                                                           \n");
		printf("                                                                                 -%%@@*                                                                          \n");
		printf("                                                                                  +@@*                                                                          \n");
		printf("                                                                                *%%@@@*-                                                                         \n");
		printf("                                                                                :-@@@%%:                                                                         \n");
		printf("                                                                               =#@@@@@##+                                                                       \n");
		printf("                                                                               :-%%@@@@%%=                                                                        \n");
		printf("                                                            :@:                 =@@@@@@@@*                      *+                   =@:                        \n");
		printf("                                        +#                  *@#              :#@@@@@@@#-                        %%@:                  #@%%                        \n");
		printf("                                       :@@=               :*@@@%%=            =@%%#@@@@@@*=                      =@@#                =%%@@@@:                      \n");
		printf("                                     :@@@@@%%-              -@@@+             -*=+@@@@@@@@+                    -@@@@#                 @@@%%#:                     \n");
		printf("                                       #@@%%                :@@@@*         -@@@@@@@@@@@@*                    :-=#@@@-              :+#@@@%%                       \n");
		printf("                                   +###@@@@+::           =*%%@@@%%             :=@@@@@@@@@@@@=                -%%@@@@@@*:             -*@@@@@#                     \n");
		printf("                   :#               :-+@@@@@@#:          :+@@@@@@@%%         :*@@@@@@@@@@*-         :*      =+-+@@@@*             ***#@@@@+                      \n");
		printf("                   +@#           :+=: *@@@@:             :+@@@@@%%         :#@@@@@@@@@@@@@:         %%@+     -#@@@@@@@@@@:          -+@@@@@@@@#                   \n");
		printf("                   @@@:           +@@@@@@@@@@@@@:       *@@@@@@@@#=:      +@@@@%%@@@@@@@@@@@%%:     =@@%%     -+@@@@@@%%              -#@@@@@@-                     \n");
		printf("                 -*@@@#+=          :=@@@@@@@*+            -@@@@@@%%=        :: *@@@@@@@@@@@@%%:   =*@@@@#++ *@@@@@@@@#           :@@@@@@@@@@#+++-                 \n");
		printf("                 +%%@@@@%%:       +@@@@@@@@@@@%%+==:       +@@@@@@@@:       -==+@@@@@@@@@@@#--++    %%@@@@@%%-  :--%%@@@@@@@@@*          :@@@@@@@@@*      *           \n");
		printf("                  *@@@@=            -@@@@@@@@@@@@:     :#@@@@@@@@@@@*   +@@@@@@@@@@@@@@@@@@#-  =#@@@@@@+    -@@@@@@@@@@+       =+*@@@@@@@@@#:      +@#          \n");
		printf("              -+#@@@@@@@@@@:     :%%@@@@@@@@@@%%=:     #@@@@@@@@@@@@        -*#@@@@@@@@@@@@@@@@%%::=@@@@@@*+-  =@@@@@@@#=         :+***@@@@%%*%%*:    :+%%@@+:        \n");
		printf("              :#@%%@@@@@@#=     -@@@@@@@@@@@@@@+*@@%%:   :  #@@@@@@@@:     *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%=:-@@@@@@@@@@@@@@@=     :=#@@@@@@@@+::    =@@@=         \n");
		printf("            :--==#@@@@@@@%%#-     +%%@@@@@@@@@@@@@@@- :+@@@@@@@@@@@@@@@@* =****@@@@@@@@@@@@@#++#@@@@@@@@@+:%%@@@@@@@@@@@@@@#-    *@@@@@@@@@@@@@@@@+:+@@@@@%%        \n");
		printf("            =@@@@@@@@@@@@*-   :--=*@@@@@@@@@@@@@%%   :=###*%%@@@@@@@@@@#:  -+@@@@@@@@@@@@@@@%%:  -%%@@@@@@@@##@@@@@@@@@@@@@=       :--+@@@@@@@@@@#:  :@@@@%%         \n");
		printf("       :      -#@@@@@@@@@+--  -@@@@@@@@@@@@@@@*+:     +@=%%@@@@@@@@@=:   %%@@@@@@@@@@@@@@@@@@@#-%%@@@@@@@@@@@@@@@@@@@@@@@@%%*%%#: %%@@@@@@@@@@@@@@@@#-#@@@@@@*        \n");
		printf("      -@+    :--=@@@@@@@@@@-     :@@@@@@@@@@@@@*-:  =@@@@@@@@@@@@@@@@@@%%%%###@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:  +@@@@@@@@@@@@@@@@@@@@@@@@@@:      \n");
		printf("      #@@-  *@@@@@@@@@@@@:    #@@%%@@@@@@@@@@@@@@@%%@=  =@@@@@@@@@@@@@@@=  -%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-  %%@#%%@@@@@@@@@@@@@%%:%%@@@@@@@@@+      \n");
		printf("    =@@@@@@:  #@@@@@@@@@@@#*  -@@@@@@@@@@@@@@@@@@@@%%:#@@@@@@@@@@@@@@%% :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%@@@@@@@@@@@@@@@@@@%%@@@@@@@@@+      \n");
		printf("     -@@@@+: +@@@@@@@@@@@@@*    =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     \n");
		printf("    %%@@@@@#:@@@@@@@@@@@@@@@@%%#@@@@@@@@@@@@@@@@@@@%%*=%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*:=@@@@@@@@@@@@@@@@@@@@@@@@@@@%%      \n");
		printf("    +@@@@@@=+@@@@@@@@@@@@@@@#%%@@@@@@@@@@@@@@@@@@@@##@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%     \n");
		printf(" :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+   \n");
		printf("    =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@:  \n");
		printf(" :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#    \n");
		printf("-*%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
		printf(":@%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@* \n");
		printf(":#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+ \n");
		break;
	case FINAL_DEFICIT:
		/* Final si llevas 5 dias seguidos por terminando el dia por debajo de la demanda */
		printf("Debido a su inoperancia ha sido arrestado por el Comite de Recursos y Energia del Estado.\n");
		printf("\n");
		printf("\n");
		printf("    ::      :::::::::                                                                :::::::::----: \n");
		printf("               :::::-:                                                              ::::::::::----  \n");
		printf("               ::::::::                                                            :::::::::::---:  \n");
		printf("                 :::::::                                                           :  ::::::::::::  \n");
		printf("                    ::::::                                                         ::::::::::----:  \n");
		printf("                       ::::                                              :-=#+--%%#*++**##%%%%%%%%*---:  \n");
		printf("                       :::::                                       :-::--==+======++++++++=---*#%%*: \n");
		printf("                         :::::                     ::::::    :-::---==--==++--::::::::::---=+**=--+:\n");
		printf("              ::          ::::::       :::---- : :--:  --::=-++-=+========-::::::::::::::---==+=---:\n");
		printf("                 :       : ::-=:::-=++-=--=+=+*#=-==-:-=-:         -===--::::::::: :::::-----==-:-::\n");
		printf("                ::::::  :+#*=-==-:-+#*:::::::::                        :::::::::::::::::-------     \n");
		printf("                ::-=+++-:::=+++=-:::-=-:::-:                          :::::::::::::::::-------:     \n");
		printf("                =%%%%+-:-=-:::::::::::::--:                            :::::::::::::::::--------      \n");
		printf("               ==-::-::        :::::::::--:                          :::::::::::::::::-------:      \n");
		printf("               :   ::::::       ::::::::::--                        ::::::::::::::::::-------       \n");
		printf("                      : :      : ::::::::::--:                     ::::::::::::::::::-------        \n");
		printf("                        ::::   ::::::::: ::::-:                   :: ::::::::-::::::----=--:        \n");
		printf("                       ::::::::::::::::::::::---                  ::::::::--+-::::::----=--:        \n");
		printf("                       :::::: ::::::::::::::::---                :::::----=*=::::::----==--         \n");
		printf("                       :::::::::::::::::::::::::--               :::::---++-:::::::----==-          \n");
		printf("                        ::::::::::::::--::::::::-=-             :::::--=+:::::::::---==--:          \n");
		printf("                         :::::::::::::-+-:::::---==:           :::::--==:  :-:::::--====-           \n");
		printf("                           ::::::::::--**-:::-----::          ::::---=+:   ::::::::-====-           \n");
		printf("                           ::::::::::-=++++=-----:::-:       ::-----++-    ::::::::-====-           \n");
		printf("                          ::::::::::--=++    -:::---==-     :::---==+=   :::::::---=====-           \n");
		printf("                          ::::::::::-=+=      :::----==-   ::-:----++::-=*-:::::--=+===-            \n");
		printf("                          :::::::::::--=       :-:----=+=--:::::--=+=--=*#=::::---=++=:             \n");
		printf("                           ::::::::::--=-       :----==+***-:::---++====+=:::::--=+*+-              \n");
		printf("                           ::::::::::---=-      :---===+*#*-:::-==--+**#+:::::--=+**=:              \n");
		printf("                            :::::::::::-:-=+*+===+=---=+=*##+=+++=-==*##-::::--==**+:               \n");
		printf("                              ::::::::::-=+#***#%%%%#=--=--=+##%%%%#*=--====-:::--==+*+:                \n");
		printf("                                ::::::::---==#%%%%%%%%#+===---=#%%##*=-::---------=+**+-                 \n");
		printf("                                  :::::::::--=****+======+**###+-:----:----==++**-                  \n");
		printf("                                    :::::::---+======+======-------------====**+=                   \n");
		printf("                                      :::::---=-=-----=-==--:::-------==+*+=--:                     \n");
		printf("                                         ::----=======-==--------==++++=::                          \n");
		printf("                                          :---------::        :::::                                 \n");
		printf(" \n");
		printf(" ...Presione Enter para continuar...%s\n", RESET_FORMATO_TEXTO);
		getchar();
		break;
	case FINAL_EXCESO:
		/* Final si llevas 5 dias seguidos por terminando el dia por encima de la demanda */
		printf("Debido a su ineficiencia con los recursos previstos ha sido arrestado por el Comite de Recursos y Energia del Estado.\n");
		printf("\n");
		printf("\n");
		printf("    ::      :::::::::                                                                :::::::::----: \n");
		printf("               :::::-:                                                              ::::::::::----  \n");
		printf("               ::::::::                                                            :::::::::::---:  \n");
		printf("                 :::::::                                                           :  ::::::::::::  \n");
		printf("                    ::::::                                                         ::::::::::----:  \n");
		printf("                       ::::                                              :-=#+--%%#*++**##%%%%%%%%*---:  \n");
		printf("                       :::::                                       :-::--==+======++++++++=---*#%%*: \n");
		printf("                         :::::                     ::::::    :-::---==--==++--::::::::::---=+**=--+:\n");
		printf("              ::          ::::::       :::---- : :--:  --::=-++-=+========-::::::::::::::---==+=---:\n");
		printf("                 :       : ::-=:::-=++-=--=+=+*#=-==-:-=-:         -===--::::::::: :::::-----==-:-::\n");
		printf("                ::::::  :+#*=-==-:-+#*:::::::::                        :::::::::::::::::-------     \n");
		printf("                ::-=+++-:::=+++=-:::-=-:::-:                          :::::::::::::::::-------:     \n");
		printf("                =%%%%+-:-=-:::::::::::::--:                            :::::::::::::::::--------      \n");
		printf("               ==-::-::        :::::::::--:                          :::::::::::::::::-------:      \n");
		printf("               :   ::::::       ::::::::::--                        ::::::::::::::::::-------       \n");
		printf("                      : :      : ::::::::::--:                     ::::::::::::::::::-------        \n");
		printf("                        ::::   ::::::::: ::::-:                   :: ::::::::-::::::----=--:        \n");
		printf("                       ::::::::::::::::::::::---                  ::::::::--+-::::::----=--:        \n");
		printf("                       :::::: ::::::::::::::::---                :::::----=*=::::::----==--         \n");
		printf("                       :::::::::::::::::::::::::--               :::::---++-:::::::----==-          \n");
		printf("                        ::::::::::::::--::::::::-=-             :::::--=+:::::::::---==--:          \n");
		printf("                         :::::::::::::-+-:::::---==:           :::::--==:  :-:::::--====-           \n");
		printf("                           ::::::::::--**-:::-----::          ::::---=+:   ::::::::-====-           \n");
		printf("                           ::::::::::-=++++=-----:::-:       ::-----++-    ::::::::-====-           \n");
		printf("                          ::::::::::--=++    -:::---==-     :::---==+=   :::::::---=====-           \n");
		printf("                          ::::::::::-=+=      :::----==-   ::-:----++::-=*-:::::--=+===-            \n");
		printf("                          :::::::::::--=       :-:----=+=--:::::--=+=--=*#=::::---=++=:             \n");
		printf("                           ::::::::::--=-       :----==+***-:::---++====+=:::::--=+*+-              \n");
		printf("                           ::::::::::---=-      :---===+*#*-:::-==--+**#+:::::--=+**=:              \n");
		printf("                            :::::::::::-:-=+*+===+=---=+=*##+=+++=-==*##-::::--==**+:               \n");
		printf("                              ::::::::::-=+#***#%%%%#=--=--=+##%%%%#*=--====-:::--==+*+:                \n");
		printf("                                ::::::::---==#%%%%%%%%#+===---=#%%##*=-::---------=+**+-                 \n");
		printf("                                  :::::::::--=****+======+**###+-:----:----==++**-                  \n");
		printf("                                    :::::::---+======+======-------------====**+=                   \n");
		printf("                                      :::::---=-=-----=-==--:::-------==+*+=--:                     \n");
		printf("                                         ::----=======-==--------==++++=::                          \n");
		printf("                                          :---------::        :::::                                 \n");
		printf("\n");
		printf(" \n");
		printf(" ...Presione Enter para continuar...%s\n", RESET_FORMATO_TEXTO);
		getchar();

		break;
	case FINAL_TEMPERATURA:
		/* Final termico: flash boil y destruccion total de la central. */
		printf("\n");
		printf("===============================================\n");
		printf(" Flash Boil\n");
		printf("===============================================\n");
		printf("\n");
		printf("La temperatura alcanzo el punto critico.\n");
		printf("El hervor fue tan repentino que el agua del sistema se evaporo de forma instantanea.\n");
		printf("La expansion destruyo la central y provoco una explosion devastadora que arraso el emplazamiento y la ciudad.\n");
		printf("No se detectan sistemas operativos ni comunicaciones.\n");
		printf("\n");
		printf("                                   ..:=*+--=====-:.                                         \n");
		printf("                               .+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*                          \n");
		printf("                            :#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-                      \n");
		printf("                          :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-                    \n");
		printf("                     -@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%                   \n");
		printf("                 .%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*#@@-+@@@@-:*-               \n");
		printf("               -@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*+*+:#@@@@=:++:            \n");
		printf("              =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@+-:------:+@=:  +@:        \n");
		printf("           .#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%@@@@@@@@@@@@@@@@*+=-----:+--.==%%-      \n");
		printf("         .@@@@@@@%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%+**@@%%@@@@@@@@@@@@-===:::+@#+   #%%     \n");
		printf("       . -@@@@%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%@@@@@@@@@#*+*#@@@@@@@@@@@@# ------:.    -@+    \n");
		printf("     ....=@%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*=++===+*+=+*+**++=-*@@#:--    ..=#.    \n");
		printf("   ....:@@@@@@@@@@@@@@@@@@@%%*##%%@@%%%%%%@@@@@@@@@@@@@@@@#=++=-%%@@@@@@+:::-+#=:%%@@#-.      .+@- \n");
		printf(" ...: #@@@@@@@@@@@@@@@@@@#@@%%@#%%@%%=:=#+-#@@@@@@@@@@@@@#=-+%%@=-++--+%%@#:=*=     +@+      .=%%:\n");
		printf(" ....:@@@@@@@@@@@@@@@@@@@%%@*--:-:... :*#*#**#+%%@@@@*     .:..:=+++-=#+@+.:=+#+  ...=*:. :@# \n");
		printf("  ... %%@@@%%@@@@@@@@@@@*#%%#*++-=#@@@#+%%+*+-++-.. :+=       :-:::-+**-*. -* ....#:    .. -%%*  \n");
		printf("  ...::+@@@@@*+****#@#@@@@@%%#%%@%%###**++==+***-   ++           ....:...+:.:.. .  :+-::-:+*+  \n");
		printf("  ....::::@@++*#%%#+*+#@@%%@@#%%@*+#*=-:-::::::.   --                  ..:.--        ...  ==   \n");
		printf("    ...:::-@@@@@++++*%%%%@@#=-+%%+-+#-:    ...   .     .+%%+*+--#+-.:::       .        ::::.    \n");
		printf("      ....::::::-=-:+=--:.:++:      . .  ..:%%*++*@@@@@@%%%%+-                                 \n");
		printf("         ...................            .  ::%%@@@@%%#@@@=.                                   \n");
		printf("           ......:::::::::::::::::::::::::-==*@#@@@@@@@*:::::::::::..........               \n");
		printf("       ...::::----===#*++@@@@@@@@@@@@@%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#-=+%%*----:::::.... \n");
		printf("      ....::-+=-+++***%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#@@@##***-....\n");
		printf("          ..........:::::::-------===++=====+**#@@@@@@%%========-+*=-----:::::::........     \n");
		printf("                               :::.  ....:...--=@@@@@@#:. ..       -=.                      \n");
		printf("                            =+++:           ::-*@@@@@@%%.             .#.                    \n");
		printf("                        -=*.                :.+%%@@@@@@+                =+                   \n");
		printf("                    .*=:*                   ..-@@@@@@@#                 =*                  \n");
		printf("     ....::::--:  +*  ==                    ..%%%%*@@@@@%%.                -*     ..           \n");
		printf(" ...::-==*%%%%#***@%%-==#*=+*#%%*-. .:.......::. -@=#@@@@@#.              :+%%@@#*+=+=--::.      \n");
		printf("..:=+=+#@%%###*%%@+===*@%%**+=-*@%%%%+==-....==-:.:@@@@@@@@@:.:...:........-*@@@@@%%%%%%%%%%#**+-..=#*\n");
		printf(":::::=*#*%%@@#@@%%%%#*+##*#@@@@@**+=:-%%@#=.....%%#*@@@@@@@@@@#=---+%%*-=*+--+@@@@@@@%%#**++=-:-=:-\n");
		printf("  -#**##%%#*+@%%+*+###@@@@@@%%@@@@@@@@@=@@@-..%%@@@@@@@@@@@@@@@@@@@@@#*@%%#**#%%#**#%%@@@@%%%%#@*    \n");
		printf("        %%%%-@%%:--=-=#%%@@@@@@@##*%%@@@@@@@@*=@@@@@@@@@@@@@@@@@@@@@*%%%%*****+-:+##%%#*@+          \n");
		printf("        %%@-@-:::..::-++*#%%%%%%#*@*:..*@@@@@@@@@@@@@@@@@@@@@@@@@%%@@%%*=:.-=+==+++++-%%+          \n");
		printf("        #*....:.........:::::#=:-=: .: :@@@@@@-+= .-+#@@@@@@@@%%*=+===--:........*=          \n");
		printf("        +*...............   ++ .   :::..-===+#%%.+@%%++**##%%%%%%%%*++-=:  ... .:.....*-          \n");
		printf("        +=......     .     +%%           :---:-=*=:.  ..:::--:::.     ....       =-          \n");
		printf("        =-                 +#                :::.          .      .             -:          \n");
		printf("        +-          :      =*  .                       ..            -          -:          \n");
		printf("        .-=-.       : ..         :         .   ..   .             .. =        :-=.          \n");
		printf("\n");
		printf("\n");
		break;
	case FINAL_PRESION:
		/* Final por sobrepresion: ruptura del reactor. */
		printf("\n");
		printf("%s===============================================\n", AMARILLO);
		printf(" RUPTURA DE LA TAPA DEL REACTOR\n");
		printf("===============================================\n");
		printf("\n");
		printf("La presion supero el punto de no retorno.\n");
		printf("El agua que circulaba dentro del reactor se transformo violentamente en vapor.\n");
		printf("La explosion desprendio la tapa del reactor y dejo el nucleo expuesto a la atmosfera.\n");
		printf("Material radiactivo y restos del nucleo fueron liberados a la atmosfera y llevados por el viento a las ciudades y paises vecinos.%s\n", RESET_FORMATO_TEXTO);
		break;
	case FINAL_MIXTO:
		/* Final combinado: ruptura del reactor y del sistema de refrigeracion. */
		printf("\n");
		printf("%s===============================================\n", AMARILLO);
		printf(" COLAPSO COMBINADO\n");
		printf("===============================================\n");
		printf("\n");
		printf("La temperatura y la presion entraron simultaneamente en zona critica.\n");
		printf("La estructura del reactor y el sistema de refrigeracion sufrieron rupturas graves.\n");
		printf("El agua circulante quedo contaminada.\n");
		printf("La evaporacion libero contaminantes radiactivos al ambiente; nubes toxicas se desplazaron sobre la ciudad y sus alrededores, mientras que el deterioro estructural disperso restos y material contaminado.\n");
		printf("La central quedo inutilizada y la region gravemente contaminada.%s\n",RESET_FORMATO_TEXTO);
		break;
	default:
		printf("Final fuera de rango\n");
	}
}