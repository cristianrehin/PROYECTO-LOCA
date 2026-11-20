/*SKALA fué la computadora diseñada y utilizada por la Unión Sovietica en sus reactores nucleares (cómo el de Chernobyl) hasta octubre de 1995.
  Monitoreaba y documentaba las condiciones del reactor, calibrando miles de sensores analogicos y digitales para asistir a los operadores*/

#include <stdio.h>
#include "funciones.h"

/* Niveles internos de riesgo. */
#define SKALA_NORMAL       0
#define SKALA_ADVERTENCIA  1
#define SKALA_CRITICO      2

/*
   UMBRALES DEL MONITOR
   Temperatura:
    Normal:       < 600
    Advertencia:  >= 600
    Critico:      >= 700
 */
#define SKALA_TEMP_ADVERTENCIA 600.0f
#define SKALA_TEMP_CRITICA     700.0f

/*
  Presion:
    Normal:       < 80
    Advertencia:  >= 80
    Critico:      >= 100

   En el monitor actual la presion pasa de verde a rojo en 80 y a rojo
   con fondo en 100. SKALA conserva esos dos umbrales de gravedad.
 */
#define SKALA_PRES_ADVERTENCIA 80.0f
#define SKALA_PRES_CRITICA     100.0f

/*
  Agua:
    Critico: porcentaje <= 30.
 */
#define SKALA_AGUA_CRITICA 30

/*  Reactividad: */
#define SKALA_REACT_ADVERTENCIA_POS  0.05f
#define SKALA_REACT_ADVERTENCIA_NEG -0.05f
#define SKALA_REACT_CRITICA_POS      0.25f
#define SKALA_REACT_CRITICA_NEG    -0.25f

/* Identificadores de variable para el switch de alarmas especificas. */
#define SKALA_TEMP_ADVERTENCIA_COD  11
#define SKALA_TEMP_CRITICA_COD      12
#define SKALA_PRES_ADVERTENCIA_COD  21
#define SKALA_PRES_CRITICA_COD      22
#define SKALA_AGUA_CRITICA_COD      32
#define SKALA_REACT_ADVERTENCIA_COD 41
#define SKALA_REACT_CRITICA_COD     42

/* Clasificacion individual de cada variable. */
static int SKALANivelTemperatura(float temperatura)
{
    if (temperatura >= SKALA_TEMP_CRITICA)
        return SKALA_CRITICO;

    if (temperatura >= SKALA_TEMP_ADVERTENCIA)
        return SKALA_ADVERTENCIA;

    return SKALA_NORMAL;
}

static int SKALANivelPresion(float presion)
{
    if (presion >= SKALA_PRES_CRITICA)
        return SKALA_CRITICO;

    if (presion >= SKALA_PRES_ADVERTENCIA)
        return SKALA_ADVERTENCIA;

    return SKALA_NORMAL;
}

static int SKALANivelAgua(int porcentajeAgua)
{
    if (porcentajeAgua <= SKALA_AGUA_CRITICA)
        return SKALA_CRITICO;

    return SKALA_NORMAL;
}

static int SKALANivelReactividad(float reactividad)
{
    if (reactividad > SKALA_REACT_CRITICA_POS ||
        reactividad < SKALA_REACT_CRITICA_NEG)
        return SKALA_CRITICO;

    if (reactividad > SKALA_REACT_ADVERTENCIA_POS ||
        reactividad < SKALA_REACT_ADVERTENCIA_NEG)
        return SKALA_ADVERTENCIA;

    return SKALA_NORMAL;
}

/*
  Las advertencias especificas se resuelven dentro de este switch.
  Cada codigo identifica una variable y que tan grave es.
*/
static void SKALAMostrarAlarmaEspecifica(int codigoAlarma)
{
    switch (codigoAlarma)
    {
        case SKALA_TEMP_ADVERTENCIA_COD:
            printf("%sSKALA: TEMPERATURA ELEVADA. REDUZCA LA REACTIVIDAD, "
                   "INSERTE BARRAS DE CONTROL Y AUMENTE LA CIRCULACION DE AGUA.%s\n",
                   AMARILLO, RESET_FORMATO_TEXTO);
            break;

        case SKALA_TEMP_CRITICA_COD:
            printf("%sSKALA: TEMPERATURA CRITICA. INSERTE TODAS LAS BARRAS "
                   "DE CONTROL Y APAGUE EL REACTOR.%s\n",
                   ROJO, RESET_FORMATO_TEXTO);
            break;

        case SKALA_PRES_ADVERTENCIA_COD:
            printf("%sSKALA: PRESION ELEVADA. ABRA LA VALVULA DE VAPOR "
                   "Y CONTROLE LA TEMPERATURA DEL REACTOR.%s\n",
                   AMARILLO, RESET_FORMATO_TEXTO);
            break;

        case SKALA_PRES_CRITICA_COD:
            printf("%sSKALA: PRESION CRITICA. ABRA LA VALVULA DE VAPOR "
                   "INMEDIATAMENTE Y PREPARE EL APAGADO DEL REACTOR.%s\n",
                   ROJO, RESET_FORMATO_TEXTO);
            break;

        case SKALA_AGUA_CRITICA_COD:
            printf("%sSKALA: CIRCULACION DE AGUA INSUFICIENTE. "
                   "AUMENTE EL NIVEL DE AGUA Y CONTROLE LA TEMPERATURA.%s\n",
                   ROJO, RESET_FORMATO_TEXTO);
            break;

        case SKALA_REACT_ADVERTENCIA_COD:
            printf("%sSKALA: REACTIVIDAD FUERA DEL RANGO DE EQUILIBRIO. "
                   "AJUSTE LAS BARRAS DE CONTROL Y REVISE LA CIRCULACION DE AGUA.%s\n",
                   AMARILLO, RESET_FORMATO_TEXTO);
            break;

        case SKALA_REACT_CRITICA_COD:
            printf("%sSKALA: REACTIVIDAD CRITICA. INSERTE BARRAS DE CONTROL "
                   "Y VERIFIQUE EL ESTADO TERMICO DEL REACTOR.%s\n",
                   ROJO, RESET_FORMATO_TEXTO);
            break;

        default:
            /* Codigo fuera de rango: no se emite una alarma. */
            break;
    }
}

/*
  JERARQUIA DE NOTIFICACIONES
 
  1. Dos o mas variables criticas:
     emergencia critica general.
  2. Una variable critica:
     alarma especifica de esa variable.
  3. Sin criticas y dos o mas advertencias:
     aviso general de condiciones inseguras.
  4. Sin criticas y una advertencia:
     alarma especifica de esa variable.
  5. Sin advertencias ni criticas:
     SKALA permanece en silencio.
 
  Una condicion critica siempre prevalece sobre cualquier advertencia.
  Se muestra una sola notificacion por evaluacion.
 */
static void SKALAMostrarJerarquia(int cantidadCriticas,
                                  int codigoCritico,
                                  int cantidadAdvertencias,
                                  int codigoAdvertencia)
{
    switch (cantidadCriticas)
    {
        case 0:
            switch (cantidadAdvertencias)
            {
                case 2:
                case 3:
                    printf("%sSKALA: CONDICIONES INSEGURAS. "
                           "SE RECOMIENDA PRESTAR ATENCION AL MONITOR.%s\n",
                           AMARILLO, RESET_FORMATO_TEXTO);
                    break;

                case 1:
                    SKALAMostrarAlarmaEspecifica(codigoAdvertencia);
                    break;

                case 0:
                default:
                    /* Estado normal: no mostrar mensajes. */
                    break;
            }
            break;
        case 1:
            SKALAMostrarAlarmaEspecifica(codigoCritico);
            break;
        case 2:
        case 3:
        case 4:
            printf("%sSKALA - EMERGENCIA: CONDICIONES CRITICAS. "
                   "APAGUE EL REACTOR E INSERTE LAS BARRAS DE CONTROL, "
                   "NO DISMINUYA EL NIVEL DE AGUA.%s\n",
                   ROJO, RESET_FORMATO_TEXTO);
            break;
        default:
            break;
    }
}


void EvaluarSkala(const Reactor *reactor, int porcentajeAgua) /*Recibe las cuatro variables de riesgo y emite como maximo una notificacion.*/
{
    int nivelTemperatura = SKALANivelTemperatura(reactor->temp);
    int nivelPresion = SKALANivelPresion(reactor->pres);
    int nivelAgua = SKALANivelAgua(porcentajeAgua);
    int nivelReactividad = SKALANivelReactividad(reactor->react);

    int cantidadCriticas = 0;
    int cantidadAdvertencias = 0;
    int codigoCritico = 0;
    int codigoAdvertencia = 0;

    /* Primero se contabilizan las condiciones criticas. */
    if (nivelTemperatura == SKALA_CRITICO)
    {
        cantidadCriticas++;
        codigoCritico = SKALA_TEMP_CRITICA_COD;
    }

    if (nivelPresion == SKALA_CRITICO)
    {
        cantidadCriticas++;
        codigoCritico = SKALA_PRES_CRITICA_COD;
    }

    if (nivelAgua == SKALA_CRITICO)
    {
        cantidadCriticas++;
        codigoCritico = SKALA_AGUA_CRITICA_COD;
    }

    if (nivelReactividad == SKALA_CRITICO)
    {
        cantidadCriticas++;
        codigoCritico = SKALA_REACT_CRITICA_COD;
    }

    /* Las advertencias se cuentan solo si no son condiciones criticas. */
    if (nivelTemperatura == SKALA_ADVERTENCIA)
    {
        cantidadAdvertencias++;
        codigoAdvertencia = SKALA_TEMP_ADVERTENCIA_COD;
    }

    if (nivelPresion == SKALA_ADVERTENCIA)
    {
        cantidadAdvertencias++;
        codigoAdvertencia = SKALA_PRES_ADVERTENCIA_COD;
    }

    if (nivelReactividad == SKALA_ADVERTENCIA)
    {
        cantidadAdvertencias++;
        codigoAdvertencia = SKALA_REACT_ADVERTENCIA_COD;
    }

    SKALAMostrarJerarquia(cantidadCriticas, codigoCritico,
                          cantidadAdvertencias, codigoAdvertencia);
}

