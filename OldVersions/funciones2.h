
#define NO_EXISTE_BARRA 0 /* Valor para que el panel no muestre la barra en dicho lugar*/
#define EXISTE_BARRA 1 /* Valor para que el panel muestre la barra en dicho lugar*/
#define ELEM_SIN_BARRA_IZQ 0 /* barra de control vacia para mostrar en el panel*/
#define ELEM_SIN_BARRA_DER 4 /* barra de control vacia para mostrar en el panel*/
#define CANT_LINEAS_BARRAS 3 /* cantidad de lineas que ocupa una barra de control formada con ASCII*/
#define VALOR_MAX_DEMANDA 250 /* cantidad de demanda maxima para el aleatorio*/
#define VALOR_MIN_DEMANDA 50 /* cantidad de demanda minima para el aleatorio*/
#define CANTIDAD_TURNOS 3 /* cantidad de turnos que hay en un dia */
#define FILA_INICIO_MENU 17 /* fila en donde empieza el menu */
#define OPCION_SALIR 6 /* opción del menú que finaliza el programa */
#define FILA_BARRAS 3 /* filas de las barras de control*/
#define COLUMNAS_BARRAS 5 /* columnas de las barras de control*/

int obtnrBarrasDeControl() {
	int d;
	scanf(" %d", &d);
	return d;
}
void mostrarMenu(char VecMenu[][50]) {
    int i = 0;
    printf("\033[7m\033[33m %s \033[0m\033[27m\n", VecMenu[i]); //Muestra el primer elemento con el formato de fondo rellenado, con color amarillo
    for (i = 1; i < OPCION_SALIR; i++)
    {
        printf("\033[33m %s \033[0m\n", VecMenu[i]); //muestra el resto de las opciones como letras normales, con color amarillo
    }
}
void mostrarMonitor(int t, int d, int inc) {
	printf("Dias Sin Incidentes | El dia es: %d\n", d);
	switch (t) {
	case 0:
		printf("        %d           | Turno Matutino.\n", inc);
		break;
	case 1:
		printf("        %d           | Turno Vespertino.\n", inc);
		break;
	case 2:
		printf("        %d           | Turno Nocturno.\n", inc);
		break;
	}
}
int crearDemanda() {
	int demanda;
	demanda = rand()%(VALOR_MAX_DEMANDA-(VALOR_MIN_DEMANDA-1))+VALOR_MIN_DEMANDA;
	return demanda;
}
int pasarTurnos(int turno) {
	turno++;
	if (turno >= CANTIDAD_TURNOS) {
		turno = 0;
	}
	return turno;
}
int pasarDia(int turno, int dia) {
	if (turno % CANTIDAD_TURNOS == 0) {
		dia++;
	}
	return dia;
}
void mostrarPanel(int cantfuelactivo) {
	int cont, i, j, k;
	int matriz[FILA_BARRAS][COLUMNAS_BARRAS];
	printf("\n");
	
	cont = 0;
	for (i=0; i<FILA_BARRAS; i++) {
		for (j=0; j<COLUMNAS_BARRAS; j++) {
			if (i==1) {
				matriz[i][j] = EXISTE_BARRA;
			} else {
				if (j==ELEM_SIN_BARRA_IZQ || j==ELEM_SIN_BARRA_DER) {
					matriz[i][j] = NO_EXISTE_BARRA;
				} else {
					matriz[i][j] = EXISTE_BARRA;
				}
			}
		}
	}
	for (i=0; i<FILA_BARRAS; i++) {
		for (k=0; k<CANT_LINEAS_BARRAS; k++) {
			for (j=0; j<COLUMNAS_BARRAS; j++) {
				if (matriz[i][j]==NO_EXISTE_BARRA) {
					printf("         ");
				} else {
					if (cont<cantfuelactivo) {
						switch (k) {
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
					} else {
						switch (k) {
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
int seleccionaItem(char VecMenu[][50])
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
                printf("\033[%d;1H\033[K\033[33m %s \033[0m\n", i+FILA_INICIO_MENU, VecMenu[i]);// el "\033[%d;1H\033[K" es para borrar la línea, más para saber en que línea escribir, \033[33m es para el color amarillo, \033[0m resetea formato de colores

                i++;
                if (i > OPCION_SALIR - 1)
                    i = 0;

                /* pinta el nuevo ítem seleccionado */
                printf("\033[%d;1H\033[K\033[7m\033[33m %s \033[0m\033[27m\n", i+FILA_INICIO_MENU, VecMenu[i]); //[7m\033 es para el fondo rellenado, \033[27m resetea el fondo para que sea normal
                break;

            case 72: /* flecha arriba */
                printf("\033[%d;1H\033[K\033[33m %s \033[0m\n", i+FILA_INICIO_MENU, VecMenu[i]); // el "\033[%d;1H\033[K" es para borrar la línea, más para saber en que línea escribir, \033[33m es para el color amarillo, \033[0m resetea formato de colores

                i--;
                if (i < 0)
                    i = OPCION_SALIR - 1;

                printf("\033[%d;1H\033[K\033[7m\033[33m %s \033[0m\033[27m\n", i+FILA_INICIO_MENU, VecMenu[i]); //[7m\033 es para el fondo rellenado, \033[27m resetea el fondo para que sea normal
                break;
            }
        }

    } while (tecla != 13); /* Enter */

    return i+1; // retorna la opción elegida (1 a 6)
}
float reacividad(int agua, int barras, float xe)
{
	float react;
	react=agua/100*(0.5-barras/11)*(1-xe)
	return react;
}
float temperatura(float react,int agua,float temp)
{	float temp;
	temp=temp+(100*react)-(1-agua/100)
	return temp;
}
float presion (float temp,int agua,float pres, bool vapor)
{
	int vp;
	if (vapor==true)
	{
		vp=-1;
	else
		vp=1;
	} 
	pres=pres+((temp/agua)*vp)
	return presion;
}
int vturb(float pres, bool vapor)
{
	int vturb;
	if (vapor==true)
	{
		if (pres>10 && pres<60)
		{
			if (pres>15)
			{
				if (pres>27)
				{
					if (pres>38)
					{
						if (pres>50)
						{	
							vturb=3000;
						else
							vturb=2250;	
						}
					else 
						vturb=1500;	
					}
				else
					vturb=750;
				}
			else
				vturb=450;
			}
		}
	
	}
	return vturb;
}
int Egenerarada(int vturb)
int Egenerada;
{switch (vturb)
{
case 450:
	Egenerada=86; 
	break;
case 750:
	Egenerada=140; 
	break;
case 1500:
	Egenerada=206;
	break;
case 2250:
	Egenerada=263;
	break;
case 3000:
	Egenerada=326; 
	break;				

default:
	Egenerada=0;
	break;
}
return Egenerarada;
}