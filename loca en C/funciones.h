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
int CrearDemanda();
int PasarDia(int turno, int dia);
int SeleccionaItem(char VecMenu[OPCIONES_MENU][50]);
int ObtenerPorcentajeAgua(int actual);
int CheckeoCatastrofes(float tempactual, float presactual);
int VelocidadTurbina(float presactual, bool hay, bool valvulaabierta);
int PotenciaElectrica(float vt);
int CheckeoDiasFinales(float def, float exc);
float ConvertirAgua(float pctagua);
float Reactividad(float aguav, float barrasctl);
float Temperatura(float , float , float );
float TemperaturaApagado(float tempactual);
float PresionActualizada(float tempactual, float presactual, bool valvulaabierta);
bool HayVapor(bool encendido, float presi);

int ObtnrBarrasDeControl()
{
    int d;
    scanf(" %d", &d);
    return d;
}
void MostrarMenu(char VecMenu[][50])
{
	printf("\033[33mIngrese una opcion\033[0m\n");
    int i = 0;
	printf("\033[7m\033[33m %s \033[0m\033[27m\n", VecMenu[i]); // Muestra el primer elemento con el formato de fondo rellenado, con color amarillo
    for (i = 1; i < ULTIMA_OPCION; i++)
    {
		printf("\033[33m %s \033[0m\n", VecMenu[i]); // muestra el resto de las opciones como letras normales, con color amarillo
    }
}
void MostrarMonitor(int t, int d, int inc, int def, int exc, int dem)
{
	printf("\033[33m+---------------------+ La demanda generada por la ciudad es: \033[33;7m%d MW.\033[0m\n", dem);
	printf("\033[33m| Dias Sin Incidentes | El dia es: %d\033[0m", d);
	if (d<10) {
		/* Este 'si' esta para saber cuanto espacio hay que tener entre que dia es y cuantos dias seguidos de deficit/exceso hay que tener */
		if (exc>=1) {
			printf("\033[33m                                       Dias seguidos con exceso de energia: %d/%d\033[0m\n", exc, LIMITE_DIAS_SEGUIDOS);
		} else {
			printf("\033[33m                                       Dias seguidos con deficit de energia: %d/%d\033[0m\n", def, LIMITE_DIAS_SEGUIDOS);
		}
	} else {
		if (exc>=1) {
			printf("\033[33m                                      Dias con exceso de energia: %d/%d\033[0m\n", exc, LIMITE_DIAS_SEGUIDOS);
		} else {
			printf("\033[33m                                      Dias con deficit de energia: %d/%d\033[0m\n", def, LIMITE_DIAS_SEGUIDOS);
		}
	}
	if (inc<10) {
		switch (t) {
		case 0:
			printf("\033[33m|         %d           | Turno Matutino.\033[0m\n", inc);
			break;
		case 1:
			printf("\033[33m|         %d           | Turno Vespertino.\033[0m\n", inc);
			break;
		case 2:
			printf("\033[33m|         %d           | Turno Nocturno.\033[0m\n", inc);
			break;
		}
	} else {
		switch (t) {
		case 0:
			printf("\033[33m|         %d          | Turno Matutino.\033[0m\n", inc);
			break;
		case 1:
			printf("\033[33m|         %d          | Turno Vespertino.\033[0m\n", inc);
			break;
		case 2:
			printf("\033[33m|         %d          | Turno Nocturno.\033[0m\n", inc);
			break;
		}
	}
	printf("\033[33m+---------------------+ \033[0m\n");
	printf("\n");
}
int CrearDemanda(float anterior)
{
	int chance;
	int demanda;
	chance = (rand() % 10 - 0 + 1);
	demanda = anterior + (rand() % (VARIACION_DEMANDA*2) - 0 + 1) - VARIACION_DEMANDA;
	if (demanda < MIN_DEM)
	{
		demanda = MIN_DEM;
	}
	if (demanda > MAX_DEM)
	{
		demanda = MAX_DEM;
	}
	if (chance > 8)
	{
		switch (chance)
		{
		case 9:
			demanda = trunc(demanda * 1.2);
			break;
		case 10:
			demanda = trunc(demanda * 0.8);
			break;
		}
	}
	return demanda;
}
void PasarTurnos(int *turno, int *dia)
{
	*turno = *turno + 1;
    if (*turno >= CANTIDAD_TURNOS)
    {
		*dia = *dia + 1;
        *turno = 0;
    }
}
int PasarDia(int turno, int dia)
{
    if (turno % CANTIDAD_TURNOS == 0)
    {
        dia++;
    }
    return dia;
}
void MostrarPanel(int cantfuelactivo)
{
    int cont, i, j, k;
    int matriz[FILA_BARRAS][COLUMNAS_BARRAS];
	printf("\033[33m                              Barras de control insertadas (X = insertada, vacia = retirada):\033[0m\n");
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
                    if (cont < cantfuelactivo)
                    {
                        switch (k)
                        {
                        case 0:
                            printf("\033[33m  /***\\  \033[0m");
                            break;
                        case 1:
                            printf("\033[33m (  X  ) \033[0m");
                            cont++;
                            break;
                        case 2:
                            printf("\033[33m  \\___/  \033[0m");
                            break;
                        default:
                            break;
                        }
                    }
                    else
                    {
                        switch (k)
                        {
                        case 0:
                            printf("\033[33m  /***\\  \033[0m");
                            break;
                        case 1:
                            printf("\033[33m (     ) \033[0m");
                            cont++;
                            break;
                        case 2:
                            printf("\033[33m  \\___/  \033[0m");
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
    printf("\n");
}
int SeleccionaItem(char VecMenu[][50])
{
    int tecla;
    int i = 0;

    do
    {
        tecla = getch();

        if (tecla == 0 || tecla == 224) /* prefijo de tecla especial */
        {
            tecla = getch();

            switch (tecla)
            {
            case 80: /* flecha abajo */
                /* despinta el ítem actual */
                printf("\033[%d;1H\033[K\033[33m %s \033[0m\n", i + FILA_INICIO_MENU, VecMenu[i]); // el "\033[%d;1H\033[K" es para borrar la línea y para saber en que línea escribir, \033[33m es para el color amarillo, \033[0m resetea formato de colores

                i++;
                if (i > ULTIMA_OPCION - 1)
                    i = 0;
                /* pinta el nuevo ítem seleccionado */
                printf("\033[%d;1H\033[K\033[7m\033[33m %s \033[0m\033[27m\n", i + FILA_INICIO_MENU, VecMenu[i]); //[7m\033 es para el fondo rellenado, \033[27m resetea el fondo para que sea normal
                break;

            case 72: /* flecha arriba */
                printf("\033[%d;1H\033[K\033[33m %s \033[0m\n", i + FILA_INICIO_MENU, VecMenu[i]); // el "\033[%d;1H\033[K" es para borrar la línea y para saber en que línea escribir, \033[33m es para el color amarillo, \033[0m resetea formato de colores

                i--;
                if (i < 0)
                    i = ULTIMA_OPCION - 1;

                printf("\033[%d;1H\033[K\033[7m\033[33m %s \033[0m\033[27m\n", i + FILA_INICIO_MENU, VecMenu[i]); //[7m\033 es para el fondo rellenado, \033[27m resetea el fondo para que sea normal
                break;
            }
        }

    } while (tecla != 13); /* Enter */

    return i + 1; // retorna la opción elegida (1 a 8)
}
float Reacividad(int agua, int barras, float xe)
{
    float react;
    react = agua / 100 * (0.5 - barras / 11) * (1 - xe);
    return react;
}
float Temperatura(float react, int agua, float temp)
{
    float t;
    t = temp + (100 * react) - (1 - agua / 100);
    return t;
}
float Presion(float temp, int agua, float pres, bool vapor)
{
    int vp;
    if (vapor == true)
        vp = -1;
    else
        vp = 1;
    pres = pres + ((temp / agua) * vp);
    return pres;
}
int VTurb(float pres, bool vapor)
{
    int vturb = 0;
    if (vapor == true)
    {
        if (pres > 10 && pres < 60)
        {
            if (pres > 15)
            {
                if (pres > 27)
                {
                    if (pres > 38)
                    {
                        if (pres > 50)
                        {
                            vturb = 3000;
                        }
                        else
                            vturb = 2250;
                    }
                    else
                        vturb = 1500;
                }
                else
                    vturb = 750;
            }
            else
                vturb = 450;
        }
    }
    return vturb;
}

int EGenerada(int vturb)
{
    int Egenerada;
    {
        switch (vturb)
        {
        case 450:
            Egenerada = 86;
            break;
        case 750:
            Egenerada = 140;
            break;
        case 1500:
            Egenerada = 206;
            break;
        case 2250:
            Egenerada = 263;
            break;
        case 3000:
            Egenerada = 326;
            break;
        default:
            Egenerada = 0;
            break;
        }
        return Egenerada;
    }
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
int CheckeoCatastrofes(float tempactual, float presactual)
{
	int catastrofe;
	catastrofe = 0;
	if (tempactual >= RANGO_FINAL_TEMPERATURA)
	{
		catastrofe = FINAL_TEMPERATURA;
	}
	else
	{
		if (presactual > RANGO_FINAL_PRESION)
		{
			catastrofe = FINAL_PRESION;
		}
		else
		{
			if (tempactual >= RANGO_FINAL_MIXTO_TMN && tempactual <= RANGO_FINAL_MIXTO_TMX && presactual >= RANGO_FINAL_MIXTO_PMN && presactual <= RANGO_FINAL_MIXTO_PMX)
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
		printf("\033[31m DEFICIT: \033[33mgeneracion %.2f MW; esta %.2f MW por debajo de la demanda.\033[0m\n", generacion, dem - generacion);
		printf(" \n");
	}
	else
	{
		if (generacion > dem + TOLERANCIA)
		{
			printf("\033[31m EXCEDENTE: \033[33mgeneracion %.2f MW; esta %.2f MW por encima de la demanda.\033[0m\n", generacion, generacion - dem);
			printf(" \n");
		}
		else
		{
			printf("\033[32m BALANCE ACEPTABLE: \033[33mgeneracion %.2f MW para una demanda de %.2f MW (tolerancia +/- %d MW).\033[0m\n", generacion, dem, TOLERANCIA);
			printf(" \n");
		}
	}
}
int PotenciaElectrica(float vt)
{
	int p;
	p = trunc(250 * vt / 3000);
	return p;
}
int VelocidadTurbina(float presactual, bool hay, bool valvulaabierta)
{
	int velocidad;
	velocidad = 0;
	if (hay == true && valvulaabierta == true && presactual >= 10)
	{
		if (presactual >= 40)
		{
			velocidad = 3000;
		}
		else
		{
			velocidad = trunc(3000 * (presactual - 10) / 30);
		}
	}
	return velocidad;
}
bool HayVapor(bool encendido, float presi)
{
	bool hay;
	if (encendido == true && presi >= 10)
	{
		hay = true;
	}
	else
	{
		hay = false;
	}
	return hay;
}
float PresionActualizada(float tempactual, float presactual, bool valvulaabierta)
{
	float nuevapresion;
	float presobj;
	if (tempactual > 100)
	{
		presobj = 1 + (tempactual - 100) * 0.35;
	}
	else
	{
		presobj = 1;
	}
	nuevapresion = presactual + 0.5 * (presobj - presactual);
	if (valvulaabierta == true)
	{
		nuevapresion = nuevapresion - 0.2 * (nuevapresion - 1);
	}
	if (nuevapresion < 1)
	{
		nuevapresion = 1;
	}
	return nuevapresion;
}
float TemperaturaApagado(float tempactual)
{
	float nuevatemp;
	nuevatemp = tempactual - 0.25 * (tempactual - 20);
	return nuevatemp;
}
float ConvertirAgua(float pctagua)
{
	float vol;
	vol = 10.4 * 28800 / 1000 * pctagua / 100;
	return vol;
}

/* Reactividad (indice abstracto, escala aprox. -0.5 a +0.5). */
/* - Barras: 0 insertadas = +0.45 ; 11 insertadas = -0.45 antes del ajuste por agua. */
/* - Agua como moderador: menos agua = menos moderacion = MAS reactividad (hasta +0.05); */
/* mas agua = mas moderacion = MENOS reactividad (hasta -0.05). El termino vale 0 al 50 %. */
float Reactividad(float aguav, float barrasctl)
{
	float fraccionagua;
	float react;
	fraccionagua = aguav / 299.52;
	react = (0.45 - 0.9 * barrasctl / 11) + 0.1 * (0.5 - fraccionagua);
	return react;
}
int ObtenerPorcentajeAgua(int actual)
{
	int nuevo;
	do
	{
		printf("\033[33m Porcentaje actual: %d %%. Ingrese el nuevo porcentaje (1 a 100):\033[0m\n", actual);
		scanf("%d", &nuevo);
		if (nuevo < 1 || nuevo > 100)
		{
			printf("\033[33m Valor fuera de rango. Debe estar entre 1 y 100.\033[0m\n");
		}
	} while (!(nuevo >= 1 && nuevo <= 100));
	if (nuevo > actual)
	{
		printf("\033[33m La circulacion de agua SUBIO de %d %% a %d %%.\033[0m\n", actual, nuevo);
	}
	else
	{
		if (nuevo < actual)
		{
			printf("\033[33m La circulacion de agua BAJO de %d %% a %d %%.\033[0m\n", actual, nuevo);
		}
		else
		{
			printf("\033[33m La circulacion de agua no cambio (%d %%).\033[0m\n", nuevo);
		}
	}
	return nuevo;
}
void NombreEscenario(float reac)
{
	if (reac > 0.25)
	{
		printf("\033[31mpositiva alta\033[0m\n");
	}
	else
	{
		if (reac > 0.05)
		{
			printf("\033[33mpositiva\033[0m\n");
		}
		else
		{
			if (reac >= -0.05)
			{
				printf("\033[32mequilibrio\033[0m\n");
			}
			else
			{
				if (reac >= -0.25)
				{
					printf("\033[33mnegativa\033[0m\n");
				}
				else
				{
					printf("\033[31mnegativa alta\033[0m\n");
				}
			}
		}
	}
}
void MostrarReactividad(bool encendido, float reac)
{
	if (encendido == true)
	{
		printf("\033[33m Reactividad: %.2f \033[0m", round((reac * 1000)) / 1000);
		NombreEscenario(reac);
	}
	else
	{
		printf("\033[33m Reactividad: no aplica (reactor apagado)\033[0m\n");
	}
}
void MostrarEstado(bool encendido, float pctagua, float aguav, float reac, float tempe, float presi, bool vapr, bool valvula, float velturb, float potenciaagenerar, float potencia, float dem)
{
	if (encendido == true)
	{
		printf("\033[33m Reactor:\033[32m ENCENDIDO                                                       \033[0m");
	}
	else
	{
		printf("\033[33m Reactor:\033[32m APAGADO                                                         \033[0m");
	}
	MostrarReactividad(encendido, reac);
	/* Verifica la cantidad de unidades que tiene para saber cuantos espacios hay que tener entre la temperatura y la presion */
	if ((trunc(tempe) / 10) < 10)
	{
		printf("\033[33m Temperatura: \033[32m%.0f grados C                                                 \033[0m", round(round((tempe * 10)) / 10));
	}
	else
	{
		if (tempe < 600)
			printf("\033[33m Temperatura: \033[32m%.0f grados C                                                \033[0m", round(round((tempe * 10)) / 10));
		else if (tempe >=600)
			printf("\033[33m Temperatura: \033[33;7m%.0f grados C                                                \033[0m", round(round((tempe * 10)) / 10));
		else if (tempe>=700)
			printf("\033[33m Temperatura: \033[31m%.0f grados C                                                \033[0m", round(round((tempe * 10)) / 10));
		else
			printf("\033[33m Temperatura: \033[31;7m%.0f grados C                                                \033[0m", round(round((tempe * 10)) / 10));
	}
	if (presi <80)
		printf("\033[33m Presion: \033[32m%.2f bar\033[0m\n", round((presi * 10)) / 10);
	else if (presi < 100)
		printf("\033[33m Presion: \033[31m%.2f bar\033[0m\n", round((presi * 10)) / 10);
	else
		printf("\033[33m Presion: \033[31;7m%.2f bar\033[0m\n", round((presi * 10)) / 10);
	if (valvula == true)
	{
		printf("\033[33m Valvula de vapor: \033[32mABIERTA                                                \033[0m");
		if (vapr == true)
		{
			printf("\033[33m Vapor disponible:\033[32m SI\033[0m\n");
		}
		else
		{
			printf("\033[33m Vapor disponible:\033[31m NO\033[0m\n");
		}
	}
	else
	{
		printf("\033[33m Valvula de vapor: \033[31mCERRADA                                                \033[0m");
		if (vapr == true)
		{
			printf("\033[33m Vapor disponible: \033[32m SI\033[0m\n");
		}
		else
		{
			printf("\033[33m Vapor disponible:\033[31m NO\033[0m\n");
		}
	}
	if (velturb / 100 >= 10)
	{
		/* Verifica la cantidad de unidades que tiene para saber cuantos espacios hay que tener entre la velocidad de la turbina y la potencia generada */
		printf("\033[33m Velocidad de la turbina: %.0f RPM                                        \033[0m", velturb);
	}
	else
	{
		if (velturb == 0)
		{
			/* Verifica que la velocidad de la turbina sea 0 */
			printf("\033[33m Velocidad de la turbina: %.0f RPM                                           \033[0m", velturb);
		}
		else
		{
			printf("\033[33m Velocidad de la turbina: %.0f RPM                                         \033[0m", velturb);
		}
	}
	printf("\033[33m Potencia generada al final del turno: %.2f MW\033[0m\n", potenciaagenerar);
	if (pctagua <=30)
		printf("\033[33m Circulacion de agua: \033[31m%.2f %%\033[0m \033[33m(\033[0m\033[31m%.2f \033[0m\033[33mmillones de litros por turno)\033[0m\n", pctagua, round((aguav * 100)) / 100);
	else
		printf("\033[33m Circulacion de agua: %.2f %% (%.2f millones de litros por turno)\033[0m\n", pctagua, round((aguav * 100)) / 100);
	CompararGeneracionDemanda(potencia, dem);
}

void Finales(int tipofin)
{
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
		/* Final si llevas 3 dias seguidos por terminando el dia por debajo de la demanda */
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
		break;
	case FINAL_EXCESO:
		/* Final si llevas 3 dias seguidos por terminando el dia por encima de la demanda */
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
		printf("\033[33m===============================================\033[0m\n");
		printf("\033[33m RUPTURA DE LA TAPA DEL REACTOR\033[0m\n");
		printf("\033[33m===============================================\033[0m\n");
		printf("\n");
		printf("\033[33mLa presion supero el punto de no retorno.\033[0m\n");
		printf("\033[33mEl agua que circulaba dentro del reactor se transformo violentamente en vapor.\033[0m\n");
		printf("\033[33mLa explosion desprendio la tapa del reactor y dejo el nucleo expuesto a la atmosfera.\033[0m\n");
		printf("\033[33mMaterial radiactivo y restos del nucleo fueron liberados a la atmosfera y llevados por el viento a las ciudades y paises vecinos.\033[0m\n");
		break;
	case FINAL_MIXTO:
		/* Final combinado: ruptura del reactor y del sistema de refrigeracion. */
		printf("\n");
		printf("\033[33m===============================================\033[0m\n");
		printf("\033[33m COLAPSO COMBINADO\033[0m\n");
		printf("\033[33m===============================================\033[0m\n");
		printf("\n");
		printf("\033[33mLa temperatura y la presion entraron simultaneamente en zona critica.\033[0m\n");
		printf("\033[33mLa estructura del reactor y el sistema de refrigeracion sufrieron rupturas graves.\033[0m\n");
		printf("\033[33mEl agua circulante quedo contaminada.\033[0m\n");
		printf("\033[33mLa evaporacion libero contaminantes radiactivos al ambiente; nubes toxicas se desplazaron sobre la ciudad y sus alrededores, mientras que el deterioro estructural disperso restos y material contaminado.\033[0m\n");
		printf("\033[33mLa central quedo inutilizada y la region gravemente contaminada.\033[0m\n");
		break;
	default:
		printf("Final fuera de rango\n");
	}
}