#include <stdio.h>
#include <stdlib.h>
#define NULL 0

typedef struct tag_NODE_S NODE_S;
typedef struct tag_LIST_S LIST_S;
typedef struct tag_OBSERVER_S OBSERVER_S;
typedef struct tag_DISPLAY_ELEMENT_S DISPLAY_ELEMENT_S;
typedef struct tag_SUBJECT_S SUBJECT_S;
typedef struct tag_WEATHER_DATA_S WEATHER_DATA_S;

struct tag_NODE_S{
    struct tag_OBSERVER_S *pstObs;
    struct tag_NODE_S *pNext;
};

struct tag_LIST_S{
    int iNumber;
    NODE_S *pstNode;
};

struct tag_OBSERVER_S{
    void (*pfUpdate)(struct tag_WEATHER_DATA_S);
};

struct tag_DISPLAY_ELEMENT_S{
    void (*pfDisplay)(void);
};

struct tag_SUBJECT_S{
    void (*pfRegisterObserver)(struct tag_OBSERVER_S *);
    void (*pfRemoveObserver)(struct tag_OBSERVER_S *);
    void (*pfNotifyObserver)(struct tag_WEATHER_DATA_S );    
};

struct tag_WEATHER_DATA_S{
    float fTemperature;
    float fHumidity;
    float fPressure;
    SUBJECT_S stSubject;
};

LIST_S *g_pList = NULL;
void RemoveObserver(struct tag_OBSERVER_S *pstObs);
void NotifyObserver(struct tag_WEATHER_DATA_S stData);
void RegisterObserver(struct tag_OBSERVER_S *pstObs);

void ListCreate(void)
{
    if (g_pList != NULL)
    {
        return;
    }
    g_pList = (LIST_S *)malloc(sizeof(LIST_S));
    if (g_pList == NULL)
    {
        while(1);
    }
    g_pList->iNumber = 0;
    g_pList->pstNode = NULL;
}
void InitSubject(struct tag_WEATHER_DATA_S *pstData)
{
    pstData->stSubject.pfNotifyObserver = NotifyObserver;
    pstData->stSubject.pfRegisterObserver = RegisterObserver;
    pstData->stSubject.pfRemoveObserver = RemoveObserver;
}
void ListAdd(struct tag_OBSERVER_S *pstObs)
{
    NODE_S *pNode = g_pList->pstNode;
    NODE_S *pNew = NULL;
    /* 入参检查，上层保证*/
    if (pNode == NULL)
    {
	g_pList->pstNode = (NODE_S*)malloc(sizeof(NODE_S));
        if (g_pList->pstNode == NULL)
	{
	    printf("\r\n 1st malloc fail");
	    return;
	}
	g_pList->iNumber++;
	g_pList->pstNode->pstObs = pstObs;
	g_pList->pstNode->pNext = NULL;
	return;
    }
    
    while(pNode->pNext != NULL)
    {
         pNode = pNode->pNext;
    }

    pNew = (NODE_S *)malloc(sizeof(NODE_S));

    if (pNew == NULL)
    {
        return;
    }

    g_pList->iNumber++;
    pNode->pNext = pNew;
    pNew->pNext = NULL;
    pNew->pstObs = pstObs;
    
    
}

void ListDel(struct tag_OBSERVER_S *pstObs)
{
    NODE_S *pNode = g_pList->pstNode;
    NODE_S *pPrev = NULL;
    /* 入参检查，上层保证*/
    if (NULL == g_pList)
    {
        return;
    }

    if (g_pList->iNumber == 0)
    {
        return;
    }

    while((pNode != NULL)&&(pNode->pstObs != pstObs))
    {
        pPrev = pNode;
        pNode = pNode->pNext;      
    }
    if (pNode == NULL)
    {
        return;
    }
    pPrev->pNext = pNode->pNext;
    g_pList->iNumber--;
    free(pNode);     
}


void RegisterObserver(struct tag_OBSERVER_S *pstObs)
{
    if (NULL == pstObs)
    {
        return;
    }

    ListAdd(pstObs);
}
void RemoveObserver(struct tag_OBSERVER_S *pstObs)
{
    if (NULL == pstObs)
    {
        return;
    }

    ListDel(pstObs);
}

void NotifyObserver(struct tag_WEATHER_DATA_S stData)
{
    int iLoop = 0;
    NODE_S *pNode = g_pList->pstNode;

    for (iLoop = 0;iLoop < g_pList->iNumber;iLoop++)
    {
        if(NULL == pNode)
        {
            printf("pNode is NULL");
            while(1);
        }
        if (NULL == pNode->pstObs->pfUpdate)
	{
	    printf("pfUpdate is NULL");
	    while(1);
	}	
        pNode->pstObs->pfUpdate(stData);
        pNode = pNode->pNext;
    }

}


void CurrCondUpdate(struct tag_WEATHER_DATA_S stData)
{
    printf("\r\nCurrent conditions: %.1fF degrees and %.1f%% humidity",\
        stData.fTemperature,stData.fHumidity);
}
void StatisticsUpdate(struct tag_WEATHER_DATA_S stData)
{
    static float fMax = 0;
    static float fMin = 0;
    float fAvg = 0;
    
    if (stData.fTemperature > fMax)
    {
        fMax = stData.fTemperature;
    }
    if (stData.fTemperature < fMin)
    {
        fMin = stData.fTemperature;
    }
    fAvg = (fMax+fMin)/2;
    printf("\r\nAvg/Max/Min temperature = %.1f/%.1f/%.1f",fAvg,fMax,fMin);
}
void ForecastUpdate(struct tag_WEATHER_DATA_S stData)
{
    static float fLastT = 0;
    static float fLastP = 0;
    static float fLastH = 0;
    do 
    {
        /* 仅测试用，不能准确预报天气 */
        if ((fLastH == 0)&&(fLastT == 0)&&(fLastP == 0))
        {
            printf("\r\nMore of the same");
            break;
        }
        if (fLastP > stData.fPressure)
        {
            printf("\r\nRainy weather");
            break;
        }
        if (fLastT < stData.fTemperature)
        {
            printf("\r\nWatch out for coller");
            break;
        }
        printf("\r\nNo more Forecast");
    }while(0);
    fLastT = stData.fTemperature;
    fLastH = stData.fHumidity;
    fLastP = stData.fPressure;
}
void main(void)
{
    OBSERVER_S stCurrCond;
    OBSERVER_S stStatics;
    OBSERVER_S stForecast;
    WEATHER_DATA_S stWeatherData;

    
    ListCreate();
    InitSubject(&stWeatherData);
    stCurrCond.pfUpdate = CurrCondUpdate;
    
    stWeatherData.stSubject.pfRegisterObserver(&stCurrCond);
    stWeatherData.fHumidity = 65;
    stWeatherData.fPressure = 30.4;
    stWeatherData.fTemperature = 80;
    
    stWeatherData.stSubject.pfNotifyObserver(stWeatherData);
    stStatics.pfUpdate = StatisticsUpdate;    
    stWeatherData.stSubject.pfRegisterObserver(&stStatics);

    stWeatherData.fHumidity = 70;
    stWeatherData.fPressure = 29.2;
    stWeatherData.fTemperature = 82;
    stWeatherData.stSubject.pfNotifyObserver(stWeatherData);

    stForecast.pfUpdate = ForecastUpdate;
    stWeatherData.stSubject.pfRegisterObserver(&stForecast);
    stWeatherData.fHumidity = 90;
    stWeatherData.fPressure = 29.2;
    stWeatherData.fTemperature = 78;
    stWeatherData.stSubject.pfNotifyObserver(stWeatherData);
    stWeatherData.stSubject.pfRemoveObserver(&stForecast);
    stWeatherData.stSubject.pfNotifyObserver(stWeatherData);
    printf("\n");

}


