#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

// ファイル一行の最大文字数
#define MAX_LENGTH (256u)
// 1チームの最大コスト
#define MAX_COST (550u)
// 野手データ最大人数
#define MAX_BUTTER_NUM (500u)
// 投手データ最大人数
#define MAX_PITCHER_NUM (500u)
// 野手データファイル名
#define BUTTER_FILE_NAME ("Batter_Beginner.dat")
// 投手データファイル名
#define PITCHER_FILE_NAME ("Pitcher_Beginner.dat")

/*
 * 野手データ
 */
typedef struct _ButterData
{
    unsigned int id;   // ID
    char name[256];    // 氏名
    float daritsu;     // 打率
    unsigned int cost; // コスト
} ButterData;

/*
 * 投手データ
 */
typedef struct _PitcherData
{
    unsigned int id;   // ID
    char name[256];    // 氏名
    float bougyoritsu; // 防御率
    unsigned int cost; // コスト
} PitcherData;

// ファイルから読み込んだ野手人数
unsigned int g_butterNum = 0u;
// ファイルから読み込んだ野手データ
ButterData g_butterData[MAX_BUTTER_NUM];
// ファイルから読み込んだ投手人数
unsigned int g_pitcherNum = 0u;
// ファイルから読み込んだ投手データ
PitcherData g_pitcherData[MAX_PITCHER_NUM];
// 選択した野手データ
ButterData g_selectedButterPlayers[9];
// 選択した投手データ
PitcherData g_seletedPitcherPlayer;

void debugPrint();
void q_sort(ButterData numbers[], int left, int right);
int isCostOver();
/*
 * 選手を選ぶ(改造必須)
 */
int
selectPlayers()
{
    // 例として、コストが低い選手を野手9人、投手１人を選ぶ

    // 野手
    {
        q_sort(g_butterData, 0, g_butterNum-1 );
        memcpy(g_selectedButterPlayers, g_butterData, sizeof(g_selectedButterPlayers));
    }

    // 投手
    {
        g_seletedPitcherPlayer.cost = UINT_MAX;
        unsigned int i;
        for( i = 0 ; i < g_pitcherNum ; i++)
        {
            if( g_seletedPitcherPlayer.cost > g_pitcherData[i].cost )
            {
                memcpy( &g_seletedPitcherPlayer, &g_pitcherData[i], sizeof(g_seletedPitcherPlayer));
            }
        }
    }

    if( isCostOver() )
    {
        fprintf(stderr, "cost over\n");
        return -1;
    }
    
    return 0;
}

/*
 * コストオーバーチェック
 */
int
isCostOver()
{
    unsigned int sum = 0u;
    
    unsigned int i;
    for( i = 0u ; i < 9 ; i++ )
    {
        sum += g_selectedButterPlayers[i].cost;
    }
    
    sum += g_seletedPitcherPlayer.cost;
    
    if( sum > MAX_COST )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * 処理結果出力
 */
void displayTeam()
{
    printf("Example Team\n");
    unsigned int i;
    for( i = 0u ; i < 9 ; i++ )
    {
        printf("%d#%s\n",
            g_selectedButterPlayers[i].id,
            g_selectedButterPlayers[i].name);
    }
    printf("%d#%s\n",
        g_seletedPitcherPlayer.id,
        g_seletedPitcherPlayer.name );
}

/*
 * 野手データ読み込み
 */
int
readButterData()
{
    FILE* fp = fopen(BUTTER_FILE_NAME, "r");
    if( NULL == fp )
    {
        // 読み込み失敗
        fprintf(stderr, "fopen err %s\n", BUTTER_FILE_NAME);
        return 1;
    }

    // 読み込みバッファ
    char buffer[MAX_LENGTH];
    unsigned int i;
    for( i = 0u ; i < MAX_BUTTER_NUM ; i++)
    {
        // 一行読み込み
        memset(buffer, 0, sizeof(buffer));
        if (NULL == fgets(buffer, sizeof(buffer)-1, fp)) {
            break;
        }

        if( '\r' == buffer[strlen(buffer) - 2]  )
        {
            buffer[strlen(buffer) - 2] = '\0';
        }
        else if('\n' == buffer[strlen(buffer) - 1] )
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        // ID
        {
            char* tok = strtok(buffer, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                continue;
            }

            char *e;
            g_butterData[i].id = (unsigned int)strtoul(tok, &e, 10);

    	    if (*e != '\0') {
        		fprintf(stderr, "%d:変換不可能部分＝%s\n", __LINE__, e);
    	    	fprintf(stderr, "%d:%ld文字目の\'%c\'が変換不可\n", __LINE__, e-tok+1, *e);
    	    	continue;
    	    }
        }

        // 名前
        {
            char* tok = strtok( NULL, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                continue;
            }
            g_butterData[i].name[sizeof(g_butterData[i].name)-1] = '\0';
            strncpy(g_butterData[i].name, tok, sizeof(g_butterData[i].name)-1);
        }

        // 打率
        {
            char* tok = strtok( NULL, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                continue;
            }

            char *e;
            g_butterData[i].daritsu = (float)strtof(tok, &e);

    	    if (*e != '\0') {
        		fprintf(stderr, "%d:変換不可能部分＝%s\n", __LINE__, e);
    	    	fprintf(stderr, "%d:%ld文字目の\'%c\'が変換不可\n", __LINE__, e-tok+1, *e);
    	    	continue;
    	    }
        }

        // コスト
        {
            char* tok = strtok( NULL, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                fprintf(stderr, "%d行目", i+1 );
                continue;
            }

            char *e;
            g_butterData[i].cost = (unsigned int)strtoul(tok, &e, 10);

    	    if (*e != '\0') {
        		fprintf(stderr, "%d:%d行目 変換不可能部分＝%s\n", __LINE__, i, e);
    	    	fprintf(stderr, "%d:%ld文字目の\'%c\',%dが変換不可\n", __LINE__, e-tok+1, *e, *e);
    	    	continue;
    	    }

        }
        g_butterNum++;


    }

    fclose(fp);

    return 0;
}

/*
 * 投手データ読み込み
 */
int
readPitcherData()
{
    FILE* fp = fopen(PITCHER_FILE_NAME, "r");
    if( NULL == fp )
    {
        // 読み込み失敗
        fprintf(stderr, "fopen err %s\n", PITCHER_FILE_NAME);
        return 1;
    }

    // 読み込みバッファ
    char buffer[MAX_LENGTH];

    unsigned int i;
    for( i = 0u ; i < MAX_PITCHER_NUM ; i++)
    {
        // 一行読み込み
        memset(buffer, 0, sizeof(buffer));
        if (NULL == fgets(buffer, sizeof(buffer)-1, fp)) {
            break;
        }

        if( '\r' == buffer[strlen(buffer) - 2]  )
        {
            buffer[strlen(buffer) - 2] = '\0';
        }
        else if('\n' == buffer[strlen(buffer) - 1] )
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        // ID
        {
            char* tok = strtok(buffer, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                continue;
            }

            char *e;
            g_pitcherData[i].id = (unsigned int)strtoul(tok, &e, 10);
            /*
             * memo:
             * strtoulにて数値変換が成功した場合、
             * 文字列の最後にeのポインタが指される。
             * 文字列の最後=ヌル文字(\0)のため、*eが\0でない場合、
             * 数値変換に失敗している。
             */
    	    if (*e != '\0') {
        		fprintf(stderr, "%d:変換不可能部分＝%s\n", __LINE__, e);
    	    	fprintf(stderr, "%d:%ld文字目の\'%c\'が変換不可\n", __LINE__, e-tok+1, *e);
    	    	continue;
    	    }
        }

        // 名前
        {
            char* tok = strtok( NULL, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                continue;
            }
            g_pitcherData[i].name[sizeof(g_pitcherData[i].name)-1] = '\0';
            strncpy(g_pitcherData[i].name, tok, sizeof(g_pitcherData[i].name)-1);
        }

        // 打率
        {
            char* tok = strtok( NULL, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                continue;
            }

            char *e;
            g_pitcherData[i].bougyoritsu = (float)strtof(tok, &e);

    	    if (*e != '\0') {
        		fprintf(stderr, "%d:変換不可能部分＝%s\n", __LINE__, e);
    	    	fprintf(stderr, "%d:%ld文字目の\'%c\'が変換不可\n", __LINE__, e-tok+1, *e);
    	    	continue;
    	    }
        }

        // コスト
        {
            char* tok = strtok( NULL, ",");
            if( NULL == tok )
            {
                fprintf(stderr, "analysis error %d\n", __LINE__);
                fprintf(stderr, "%d行目", i+1 );
                continue;
            }

            char *e;
            g_pitcherData[i].cost = (unsigned int)strtoul(tok, &e, 10);

    	    if (*e != '\0') {
        		fprintf(stderr, "%d:%d行目 変換不可能部分＝%s\n", __LINE__, i, e);
    	    	fprintf(stderr, "%d:%ld文字目の\'%c\',%dが変換不可\n", __LINE__, e-tok+1, *e, *e);
    	    	continue;
    	    }

        }
        g_pitcherNum++;


    }

    fclose(fp);

    return 0;
}


int main(void)
{
    // printf("#野手データ読み込み\n");
    if(0 != readButterData())
    {
        return 1;
    }
    // printf("#投手データ読み込み");
    if(0 != readPitcherData())
    {
        return 1;
    }
    // debugPrint();
    if(0 != selectPlayers())
    {
        return 1;
    }

    displayTeam();

    return 0;
}

void debugPrint()
{
    printf("#######BUTTER\n");
    unsigned int i;
    for( i = 0u ; i < g_butterNum ; i++ )
    {
        printf("%d,%s,%f,%d\n",
            g_butterData[i].id,
            g_butterData[i].name,
            g_butterData[i].daritsu,
            g_butterData[i].cost);
    }

    printf("#######PITCHER\n");
    for( i = 0u ; i < g_pitcherNum ; i++ )
    {
        printf("%d,%s,%f,%d\n",
            g_pitcherData[i].id,
            g_pitcherData[i].name,
            g_pitcherData[i].bougyoritsu,
            g_pitcherData[i].cost);
    }
}

/*
 * ソート
 */
void q_sort(ButterData numbers[], int left, int right)
{
    int pivot, l_hold, r_hold;

    l_hold = left;
    r_hold = right;
    pivot = numbers[left].cost;
    ButterData pivotData;
    memcpy(&pivotData, &numbers[left], sizeof(pivotData));
    while (left < right)
    {
        while ((numbers[right].cost >= pivot) && (left < right))
            right--;
        if (left != right)
        {
            memcpy(&numbers[left], &numbers[right], sizeof(numbers[left]));
            left++;
        }
        while ((numbers[left].cost <= pivot) && (left < right))
            left++;
        if (left != right)
        {
            memcpy(&numbers[right], &numbers[left], sizeof(numbers[right]));
            right--;
        }
    }
    memcpy(&numbers[left], &pivotData, sizeof(numbers[left]));
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot)
        q_sort(numbers, left, pivot-1);
    if (right > pivot)
        q_sort(numbers, pivot+1, right);
}