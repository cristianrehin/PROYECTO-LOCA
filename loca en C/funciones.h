#define NO_EXISTE_BARRA 0     /* Valor para que el panel no muestre la barra en dicho lugar*/
#define EXISTE_BARRA 1        /* Valor para que el panel muestre la barra en dicho lugar*/
#define ELEM_SIN_BARRA_IZQ 0  /* barra de control vacia para mostrar en el panel*/
#define ELEM_SIN_BARRA_DER 4  /* barra de control vacia para mostrar en el panel*/
#define CANT_LINEAS_BARRAS 3  /* cantidad de lineas que ocupa una barra de control formada con ASCII*/
#define VALOR_MAX_DEMANDA 250 /* cantidad de demanda maxima para el aleatorio*/
#define VALOR_MIN_DEMANDA 50  /* cantidad de demanda minima para el aleatorio*/
#define CANTIDAD_TURNOS 3     /* cantidad de turnos que hay en un dia */
#define FILA_INICIO_MENU 18   /* fila en donde empieza el menu */
#define ULTIMA_OPCION 8       /* opción del menú que finaliza el programa */
#define FILA_BARRAS 3         /* filas de las barras de control*/
#define COLUMNAS_BARRAS 5     /* columnas de las barras de control*/
#define ESCAPE 1              /* final numero 1, escape*/

int ObtnrBarrasDeControl()
{
    int d;
    scanf(" %d", &d);
    return d;
}
void MostrarMenu(char VecMenu[][50],bool encendido, bool skalaEncendida)
{
	printf("Ingrese una opcion\n");
    int i = 0;
    if(encendido == false)
        printf("\033[7m\033[33m %s \033[0m\033[27m\n", VecMenu[i]); // Muestra el primer elemento con el formato de fondo rellenado, con color amarillo
    for (i = 1; i < ULTIMA_OPCION; i++)
    {
        if(skalaEncendida == false && i == 7){
            i++;
            printf("\033[33m %s \033[0m\n", VecMenu[i]); // muestra el resto de las opciones como letras normales, con color amarillo
        }else
            printf("\033[33m %s \033[0m\n", VecMenu[i]); // muestra el resto de las opciones como letras normales, con color amarillo
    }
}

void MostrarMonitor(int t, int d, int inc, int def, int exc, int dem)
{
	printf("| ------------------- | La demanda generada por la ciudad es: %d MW.\n", dem);
	printf("| Dias Sin Incidentes | El dia es: %d", d);
	if (d<10) {
		/* Este 'si' esta para saber cuanto espacio hay que tener entre que dia es y cuantos dias seguidos de deficit/exceso hay que tener */
		if (exc>=1) {
			printf("                                       Dias seguidos con exceso de energia: %d/3\n", exc);
		} else {
			printf("                                       Dias seguidos con deficit de energia: %d/3\n", def);
		}
	} else {
		if (exc>=1) {
			printf("                                      Dias con exceso de energia: %d/3\n", exc);
		} else {
			printf("                                      Dias con deficit de energia: %d/3\n", def);
		}
	}
	if (inc<10) {
		switch (t) {
		case 0:
			printf("|         %d           | Turno Matutino.\n", inc);
			break;
		case 1:
			printf("|         %d           | Turno Vespertino.\n", inc);
			break;
		case 2:
			printf("|         %d           | Turno Nocturno.\n", inc);
			break;
		}
	} else {
		switch (t) {
		case 0:
			printf("|         %d          | Turno Matutino.\n", inc);
			break;
		case 1:
			printf("|         %d          | Turno Vespertino.\n", inc);
			break;
		case 2:
			printf("|         %d          | Turno Nocturno.\n", inc);
			break;
		}
	}
	printf("| ------------------- | \n");
	printf("\n");
}
int CrearDemanda(float anterior)
{
	int chance;
	int demanda;
	chance = (0 + rand() % 10 - 0 + 1);
	demanda = anterior + (0 + rand() % 30 - 0 + 1) - 15;
	if (demanda < 50)
	{
		demanda = 50;
	}
	if (demanda > 200)
	{
		demanda = 200;
	}
	if (chance > 8)
	{
		switch (chance)
		{
		case 9:
			demanda = trunc(demanda * 1.2);
			/* Agregar lore de por que aumenta un 20% la demanda */
			break;
		case 10:
			demanda = trunc(demanda * 0.8);
			/* Agregar lore de por que se reduce un 20% la demanda */
			break;
		}
	}
	return demanda;
}
int PasarTurnos(int turno)
{
    turno++;
    if (turno >= CANTIDAD_TURNOS)
    {
        turno = 0;
    }
    return turno;
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
	printf("                              Barras de control insertadas (X = insertada, vacia = retirada):\n");
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
                            printf("  /***\\  ");
                            break;
                        case 1:
                            printf(" (  X  ) ");
                            cont++;
                            break;
                        case 2:
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
                        case 0:
                            printf("  /***\\  ");
                            break;
                        case 1:
                            printf(" (     ) ");
                            cont++;
                            break;
                        case 2:
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
    printf("\n");
}
int SeleccionaItem(char VecMenu[][50], bool encendido, bool skalaActiva)
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
                
                if (i > ULTIMA_OPCION - 1 && encendido == true)
                    i++;
                else if (i > ULTIMA_OPCION - 1)
                    i = 0;
                /* pinta el nuevo ítem seleccionado */
                printf("\033[%d;1H\033[K\033[7m\033[33m %s \033[0m\033[27m\n", i + FILA_INICIO_MENU, VecMenu[i]); //[7m\033 es para el fondo rellenado, \033[27m resetea el fondo para que sea normal
                break;

            case 72: /* flecha arriba */
                printf("\033[%d;1H\033[K\033[33m %s \033[0m\n", i + FILA_INICIO_MENU, VecMenu[i]); // el "\033[%d;1H\033[K" es para borrar la línea y para saber en que línea escribir, \033[33m es para el color amarillo, \033[0m resetea formato de colores

                i--;
                if (i < 0 && encendido == true)
                    i--;
                else if (i < 0)
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
	if (def >= 3)
	{
		terminarjuego = 2;
	}
	if (exc >= 3)
	{
		terminarjuego = 3;
	}
	return terminarjuego;
}int CheckeoCatastrofes(float tempactual, float presactual)
{
	int catastrofe;
	catastrofe = 0;
	if (tempactual >= 850)
	{
		catastrofe = 4;
	}
	else
	{
		if (presactual > 125)
		{
			catastrofe = 5;
		}
		else
		{
			if (tempactual >= 650 && tempactual <= 799 && presactual >= 100 && presactual <= 119)
			{
				catastrofe = 6;
			}
		}
	}
	return catastrofe;
}
void CompararGeneracionDemanda(float generacion, float dem)
{
	if (generacion < dem - 30)
	{
		printf(" DEFICIT: generacion %.2f MW; esta %.2f MW por debajo de la demanda.\n", generacion, dem - generacion);
		printf(" \n");
	}
	else
	{
		if (generacion > dem + 30)
		{
			printf(" EXCEDENTE: generacion %.2f MW; esta %.2f MW por encima de la demanda.\n", generacion, generacion - dem);
			printf(" \n");
		}
		else
		{
			printf(" BALANCE ACEPTABLE: generacion %.2f MW para una demanda de %.2f MW (tolerancia +/- 30 MW).\n", generacion, dem);
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
}float PresionActualizada(float tempactual, float presactual, bool valvulaabierta)
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
}float TemperaturaApagado(float tempactual)
{
	float nuevatemp;
	nuevatemp = tempactual - 0.25 * (tempactual - 20);
	return nuevatemp;
}float ConvertirAgua(float pctagua)
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
		printf(" Porcentaje actual: %d %%. Ingrese el nuevo porcentaje (1 a 100):\n", actual);
		scanf("%d", &nuevo);
		if (nuevo < 1 || nuevo > 100)
		{
			printf(" Valor fuera de rango. Debe estar entre 1 y 100.\n");
		}
	} while (!(nuevo >= 1 && nuevo <= 100));
	if (nuevo > actual)
	{
		printf(" La circulacion de agua SUBIO de %d %% a %d %%.\n", actual, nuevo);
	}
	else
	{
		if (nuevo < actual)
		{
			printf(" La circulacion de agua BAJO de %d %% a %d %%.\n", actual, nuevo);
		}
		else
		{
			printf(" La circulacion de agua no cambio (%d %%).\n", nuevo);
		}
	}
	return nuevo;
}
bool MostrarAlertasSkala(bool encendido, float temp, float presion, float react, float pctagua)
{
	bool huboalerta;
	huboalerta = false;
	/* Temperatura y presion se supervisan aunque el reactor este apagado: pueden seguir elevadas. */
	if (temp >= 800)
	{
		printf(" *** SKALA - ALERTA CRITICA DE TEMPERATURA ***\n");
		printf(" Temperatura: %.0f grados C. Zona critica alcanzada.\n", round(temp));
		printf(" Inserte barras de control y aumente la circulacion de agua.\n");
		printf(" El apagado del reactor es el ultimo recurso.\n");
		huboalerta = true;
	}
	else
	{
		if (temp >= 700)
		{
			printf(" *** SKALA - ALERTA DE INCIDENTE TERMICO ***\n");
			printf(" Temperatura: %.0f grados C. Se registra un incidente termico.\n", round(temp));
			printf(" Inserte barras de control y aumente la circulacion de agua.\n");
			printf(" El apagado del reactor es el ultimo recurso.\n");
			huboalerta = true;
		}
		else
		{
			if (temp >= 600)
			{
				printf(" *** SKALA - ADVERTENCIA PREVENTIVA DE TEMPERATURA ***\n");
				printf(" Temperatura: %.0f grados C (rango preventivo: 600-699).\n", round(temp));
				printf(" Revise la reactividad, inserte barras si es elevada y aumente el agua.\n");
				huboalerta = true;
			}
		}
	}
	if (presion >= 120)
	{
		printf(" *** SKALA - ALERTA CRITICA DE PRESION ***\n");
		printf(" Presion: %.2f bar. Umbral critico del simulador alcanzado.\n", round((presion * 10)) / 10);
		printf(" Abra la valvula para aliviar presion si corresponde; el apagado es el ultimo recurso.\n");
		huboalerta = true;
	}
	else
	{
		if (presion > 100)
		{
			printf(" *** SKALA - ADVERTENCIA PREVENTIVA DE PRESION ***\n");
			printf(" Presion: %.2f bar. Revise la valvula de vapor.\n", round((presion * 10)) / 10);
			huboalerta = true;
		}
	}
	if (encendido == true && react > 0.33)
	{
		printf(" *** SKALA - ADVERTENCIA DE REACTIVIDAD ELEVADA ***\n");
		printf(" Reactividad: %.2f (umbral: > 0,25). Inserte barras de control.\n", round((react * 1000)) / 1000);
		huboalerta = true;
	}
	if (pctagua < 40 && temp > 600)
	{
		printf(" *** SKALA - ALERTA DE REFRIGERACION INSUFICIENTE ***\n");
		printf(" Agua: %.2f %% con temperatura superior a 600 grados C.\n", pctagua);
		printf(" Aumente la circulacion de agua inmediatamente.\n");
		printf(" El apagado del reactor es el ultimo recurso.\n");
		huboalerta = true;
	}
	if (huboalerta == false)
	{
		printf(" SKALA: sin alertas en las variables supervisadas.\n");
	}
	return huboalerta;
}

void Finales(int tipofin)
{
	switch (tipofin)
	{
	case ESCAPE:
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
	case 2:
		/* Final si llevas 3 dias seguidos por terminando el dia por debajo de la demanda */
		printf("Debido a su inoperancia ha sido arrestado por el Comit� de Recursos y Energia del Estado.\n");
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
	case 3:
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
	case 4:
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
	case 5:
		/* Final por sobrepresion: ruptura del reactor. */
		printf("\n");
		printf("===============================================\n");
		printf(" RUPTURA DE LA TAPA DEL REACTOR\n");
		printf("===============================================\n");
		printf("\n");
		printf("La presion supero el punto de no retorno.\n");
		printf("El agua que circulaba dentro del reactor se transformo violentamente en vapor.\n");
		printf("La explosion desprendio la tapa del reactor y dejo el nucleo expuesto a la atmosfera.\n");
		printf("Material radiactivo y restos del nucleo fueron liberados a la atmosfera y llevados por el viento a las ciudades y paises vecinos.\n");
		break;
	case 6:
		/* Final combinado: ruptura del reactor y del sistema de refrigeracion. */
		printf("\n");
		printf("===============================================\n");
		printf(" COLAPSO COMBINADO\n");
		printf("===============================================\n");
		printf("\n");
		printf("La temperatura y la presion entraron simultaneamente en zona critica.\n");
		printf("La estructura del reactor y el sistema de refrigeracion sufrieron rupturas graves.\n");
		printf("El agua circulante quedo contaminada.\n");
		printf("La evaporacion libero contaminantes radiactivos al ambiente; nubes toxicas se desplazaron sobre la ciudad y sus alrededores, mientras que el deterioro estructural disperso restos y material contaminado.\n");
		printf("La central quedo inutilizada y la region gravemente contaminada.\n");
		break;
	default:
		printf("Final fuera de rango\n");
	}
}