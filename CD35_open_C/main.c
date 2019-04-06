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
#define MAX_BUTTER_NUM (100u)
// 投手データ最大人数
#define MAX_PITCHER_NUM (100u)
// 野手データファイル名
#define BUTTER_FILE_NAME ("Batter_Open.dat")
// 投手データファイル名
#define PITCHER_FILE_NAME ("Pitcher_Open.dat")

/*
 * 野手データ
 */
typedef struct _ButterData
{
    unsigned int id;             // ID
    char name[256];              // 選手名
    float daritsu;               // 打率
    // unsigned int shiai;          // 試合
    // unsigned int dasekisuu;      // 打席数
    // unsigned int dasuu;          // 打数
    // unsigned int tokuten;        // 得点
    // unsigned int anda;           // 安打
    // unsigned int ichiruida;      // 一塁打
    // unsigned int niruida;        // 二塁打
    // unsigned int sanruida;       // 三塁打
    // unsigned int honruida;       // 本塁打
    // unsigned int ruida;          // 塁打
    // unsigned int daten;          // 打点
    // unsigned int tourui;         // 盗塁
    // unsigned int touruisi;       // 盗塁死
    // unsigned int gida;           // 犠打
    // unsigned int gihi;           // 犠飛
    // unsigned int sikyu_4;        // 四球
    // unsigned int keien;          // 敬遠
    // unsigned int sikyu_dead;     // 死球
    // unsigned int sansin;         // 三振
    // unsigned int heisatsuda;     // 併殺打
    // float syutsuruiritsu;        // 出塁率
    // float tyoudaritsu;           // 長打率
    float ichiruidaritsu;        // 一塁打率
    float niruidaritsu;          // 二塁打率
    float sanruidaritsu;         // 三塁打率
    float honruidaritsu;         // 本塁打率
    float sikyuuritsu;           // 四球率
    float sanshinritsu;          // 三振率
    unsigned int cost;           // コスト
} ButterData;

/*
 * 投手データ
 */
typedef struct _PitcherData
{
    unsigned int id;           // ID
    char name[256];            // 選手名
    float bougyoritsu;         // 防御率
    // unsigned int shiai;        // 試合
    // unsigned int syouri;       // 勝利
//    unsigned int haiboku;      // 敗北
//    unsigned int se_bu;        // セーブ
//    unsigned int ho_rudo;      // ホールド
//    unsigned int hp;           // ホールドポイント
//    unsigned int kantou;       // 完投
//    unsigned int kanpuu;       // 完封
//    unsigned int musikyuu;     // 無四球
//    float syouritsu;           // 勝率
//    unsigned int dasya;        // 打者
//    unsigned int toukyukai;    // 投球回
//    unsigned int hianda;       // 被安打
//    unsigned int hihonruida;   // 被本塁打
//    unsigned int yosikyu_4;    // 与四球
//    unsigned int keien;        // 敬遠
//    unsigned int yosikyu_dead; // 与死球
//    unsigned int datsusansin;  // 奪三振
//    unsigned int boutou;       // 暴投
//    unsigned int bo_ku;        // ボーク
//    unsigned int sitten;       // 失点
//    unsigned int jisekiten;    // 自責点
    float yoshikyuuritsu;        // 与四球率
    float datsusansinritsu;      // 奪三振率
    float hiandaritsu;           // 被安打率
    float hihonruidaritsu;       // 被本塁打率;
    unsigned int cost;         // コスト
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
 * 文字列->UINT変換
 */
int
analysisDataUInt(char* str, unsigned int* value)
{
    if( NULL == str )
    {
        fprintf(stderr, "analysis error %d\n", __LINE__);
        return -1;
    }

    char *e;
    *value = (unsigned int)strtoul(str, &e, 10);

    if (*e != '\0') {
		fprintf(stderr, "%d:変換不可能部分＝%s\n", __LINE__, e);
    	fprintf(stderr, "%d:%ld文字目の\'%c\',%dが変換不可\n", __LINE__, e-str+1, *e, *e);
    	return -1;
    }

    return 0;
}

/*
 * 文字列->Float変換
 */
int
analysisDataFloat(char* str, float* value)
{
    if( NULL == str )
    {
        fprintf(stderr, "analysis error %d\n", __LINE__);
        return -1;
    }

    char *e;
    *value = (float)strtod(str, &e);

    if (*e != '\0') {
		fprintf(stderr, "%d:変換不可能部分＝%s\n", __LINE__, e);
    	fprintf(stderr, "%d:%ld文字目の\'%c\',%dが変換不可\n", __LINE__, e-str+1, *e, *e);
    	return -1;
    }

    return 0;
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
        if(analysisDataUInt(strtok(buffer, ","), &g_butterData[i].id))
        {
            continue;
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
        if(analysisDataFloat(strtok( NULL, ","), &g_butterData[i].daritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
#if 0
         // 試合
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].shiai))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 打席数
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].dasekisuu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 打数
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].dasuu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 得点
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].tokuten))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 安打
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].anda))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 二塁打
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].niruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 三塁打
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].sanruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 本塁打
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].honruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 塁打
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].ruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 打点
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].daten))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 盗塁
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].tourui))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 盗塁死
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].touruisi))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 犠打
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].gida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 犠飛
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].gihi))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 四球
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].sikyu_4))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 敬遠
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].keien))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 死球
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].sikyu_dead))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 三振
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].sansin))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 併殺打
        if(analysisDataUInt(strtok( NULL, ","), &g_butterData[i].heisatsuda))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 出塁率
        if(analysisDataFloat(strtok( NULL, ","), &g_butterData[i].syutsuruiritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 長打率
        if(analysisDataFloat(strtok( NULL, ","), &g_butterData[i].tyoudaritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
#endif
        // 一塁打率
		if(analysisDataFloat(strtok( NULL, ","), &g_butterData[i].ichiruidaritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 二塁打率
		if(analysisDataFloat(strtok( NULL, ","), &g_butterData[i].niruidaritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 三塁打率
		if(analysisDataFloat(strtok( NULL, ","), &g_butterData[i].sanruidaritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 本塁打率
		if(analysisDataFloat(strtok( NULL, ","), &g_butterData[i].honruidaritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 四球率
		if(analysisDataFloat(strtok( NULL, ","), &g_butterData[i].sikyuuritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // 三振率
		if(analysisDataFloat(strtok( NULL, ","), &g_butterData[i].sanshinritsu))
		{
			fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
			continue;
		}
        // コスト
        if(analysisDataUInt(strtok(NULL, ","), &g_butterData[i].cost))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
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
        if(analysisDataUInt(strtok(buffer, ","), &g_pitcherData[i].id))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
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

        // 防御率
        if(analysisDataFloat(strtok( NULL, ","), &g_pitcherData[i].bougyoritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
#if 0
        // 試合
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].shiai))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 勝利
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].syouri))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 敗北
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].haiboku))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // セーブ
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].se_bu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // ホールド
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].ho_rudo))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // ホールドポイント
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].hp))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 完投
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].kantou))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 完封
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].kanpuu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 無四球
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].musikyuu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 勝率
        if(analysisDataFloat(strtok( NULL, ","), &g_pitcherData[i].syouritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 打者
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].dasya))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 投球回
        float tmp;
        if(analysisDataFloat(strtok( NULL, ","), &tmp))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        g_pitcherData[i].toukyukai = (int)tmp;

        // 被安打
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].hianda))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 被本塁打
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].hihonruida))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 与四球
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].yosikyu_4))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 敬遠
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].keien))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 与死球
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].yosikyu_dead))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 奪三振
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].datsusansin))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 暴投
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].boutou))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // ボーク
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].bo_ku))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 失点
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].sitten))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }

        // 自責点
        if(analysisDataUInt(strtok( NULL, ","), &g_pitcherData[i].jisekiten))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
#endif
        // 与四球率
        if(analysisDataFloat(strtok( NULL, ","), &g_pitcherData[i].yoshikyuuritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        // 奪三振率
        if(analysisDataFloat(strtok( NULL, ","), &g_pitcherData[i].datsusansinritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        // 被安打率
        if(analysisDataFloat(strtok( NULL, ","), &g_pitcherData[i].hiandaritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        // 被本塁打率
        if(analysisDataFloat(strtok( NULL, ","), &g_pitcherData[i].hihonruidaritsu))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
        }
        // コスト
        if(analysisDataUInt(strtok(NULL, ","), &g_pitcherData[i].cost))
        {
            fprintf(stderr, "%d行目解析エラー:%d\n", i+1, __LINE__);
            continue;
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
        printf("%d,%s,%f,%f,%f,%f,%f,%f,%f,%d\n",
            g_butterData[i].id,
            g_butterData[i].name,
            g_butterData[i].daritsu,
            g_butterData[i].ichiruidaritsu,
            g_butterData[i].niruidaritsu,
            g_butterData[i].sanruidaritsu,
            g_butterData[i].honruidaritsu,
            g_butterData[i].sikyuuritsu,
            g_butterData[i].sanshinritsu,
            g_butterData[i].cost);
    }

    printf("#######PITCHER\n");
    for( i = 0u ; i < g_pitcherNum ; i++ )
    {
        printf("%d,%s,%f,%f,%f,%f,%f,%d\n",
            g_pitcherData[i].id,
            g_pitcherData[i].name,
            g_pitcherData[i].bougyoritsu,
            g_pitcherData[i].yoshikyuuritsu,
            g_pitcherData[i].datsusansinritsu,
            g_pitcherData[i].hiandaritsu,
            g_pitcherData[i].hihonruidaritsu,
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